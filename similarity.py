import json
import difflib
from numpy import *
import yara
import binascii
import os

def sim(filePath1,filePath2,fre=0.9,sim=0.95):
    f1 = open(filePath1, "r")
    f2 = open(filePath2, "r")
    l1 = json.load(f1)
    l2 = json.load(f2)
    blockPairs = []
    ruleStrs = []
    k = 0
    l3 = [l for l in l2]
    for i in range(len(l1)):
        for j in range(min(10, len(l3))):
            similarity = difflib.SequenceMatcher(None, l1[i][0], l3[j][0]).ratio()
            if similarity > sim:
                blockPairs.append([i, k + j, l1[i][1]])
                l3.remove(l3[j])
                k += 1
                break
    for blockPair in blockPairs:
        rule=""
        indexs = difflib.SequenceMatcher(None, l1[blockPair[0]][2], l2[blockPair[1]][2]).get_matching_blocks()
        if indexs[0][2]==1:
            indexs.remove(indexs[0])
        for i in range(len(indexs)-1):
            gap1 = indexs[i + 1][0] - indexs[i][0] - indexs[i][2]
            gap2 = indexs[i + 1][1] - indexs[i][1] - indexs[i][2]
            rule += l1[blockPair[0]][2][indexs[i][0]:indexs[i][0] + indexs[i][2]]
            if i!=len(indexs)-2:
                if gap1 == gap2:
                    for k in range(gap1):
                        rule += "?"
                else:
                    rule += "[" + str(min(gap1, gap2)) + "-" + str(max(gap1, gap2)) + "]"
        ruleStrs.append([rule,blockPair[2]])
    return ruleStrs

def processRules(ruleStrs,yaraPath):
    processResult={}
    fullReverse={}#完全匹配的反向字典
    for str in ruleStrs:
        leftBrackets = [i for i in range(len(str[0])) if str[0][i] == '[']
        rightBrackets = [i for i in range(len(str[0])) if str[0][i] == ']']
        Brackets=[(leftBrackets[i],rightBrackets[i]) for i in range(len(leftBrackets))]
        hexStrs=[(rightBrackets[i]+1,leftBrackets[i+1]-1) for i in range(len(Brackets)-1)]
        gapLen=0
        if len(Brackets)>0:
            hexStrs.insert(0,(0,leftBrackets[0]-1))
            hexStrs.append((rightBrackets[-1]+1,len(str[0])-1))
        goodRule=True
        processed = ""
        if len(Brackets)==0:
            for i in range(len(str[0])//2):
                processed += (str[0][2 * i:2 * i + 2] + " ")
            for i in range(len(str[0])):
                if str[0][i]=="?":
                    gapLen+=1
            if len(str[0])>15:
                processResult[processed]=[str[1],len(str[0]),gapLen]
                fullReverse[processed.replace(" ","")]=processed
        else:
            if len(Brackets)>1:
                for hs in range(1,len(Brackets)):
                    if hexStrs[hs][1]-hexStrs[hs][0]==0:
                        goodRule=False
                        break
            if not goodRule:
                continue
            else:
                for hs in range(len(Brackets)):
                    if (hexStrs[hs][1]-hexStrs[hs][0])%2:#偶数
                        # processed += str[0][hexStrs[hs][0]:hexStrs[hs][1] + 1]
                        for i in range((hexStrs[hs][1]-hexStrs[hs][0]+1) // 2):
                            processed+=(str[0][hexStrs[hs][0]+2*i:hexStrs[hs][0]+2*i+2]+" ")
                        lrb = str[0][Brackets[hs][0] + 1:Brackets[hs][1]]
                        processed += (str[0][Brackets[hs][0]:Brackets[hs][1]+1]+" ")
                        gapLen+=int(lrb.split("-")[1])
                    else:#奇数
                        for i in range((hexStrs[hs][1]-hexStrs[hs][0]) // 2):
                            processed+=(str[0][hexStrs[hs][0]+2*i:hexStrs[hs][0]+2*i+2]+" ")
                        lrb=str[0][Brackets[hs][0]+1:Brackets[hs][1]]
                        lrb2="["+'%d'%(int(lrb.split("-")[0])+1)+"-"+'%d'%(int(lrb.split("-")[1])+1)+"] "
                        processed+=lrb2
                        gapLen+=int(lrb.split("-")[1])+1
                if hexStrs[-1][1]-hexStrs[-1][0]>0:
                    if (hexStrs[-1][1]-hexStrs[-1][0])%2:
                        processed+=str[0][hexStrs[-1][0]:hexStrs[-1][1]]
                    else:
                        processed += str[0][hexStrs[-1][0]:hexStrs[-1][1] + 1]
                if processed[-2]=="]" :
                    pos = processed.rfind("[")
                    lrb = processed[pos+1:-2]
                    gapLen -= int(lrb.split("-")[1])
                    processed=processed[0:pos]
                for i in range(len(processed)):
                    if processed[i] == "?":
                        gapLen += 1
                if len(processed) > 20:
                    processResult[processed] = [str[1], len(processed), gapLen]
    return processResult,fullReverse

def genYar(rulePath,ruleName,strList,logic):
    f=open(rulePath,"w")
    f.write("rule " + ruleName + "\n")
    f.write("{\n")
    f.write("\tstrings:\n")
    index = 0
    for i in strList:
        f.write("\t\t$str" + str(index) + "={" + i + "}\n")
        index += 1
    f.write("\tcondition:\n")
    f.write("\t\t$str0")
    for i in range(1,index):
        f.write(" "+logic+" $str"+str(i))
    f.write("\n")
    f.write("}")
    f.write("\n")
    f.close()

def genRules(ruleDir,packerName,ruleDict):
    fullStr=[i for i in ruleDict if not ruleDict[i][2]]
    reStr=[[i] for i in ruleDict if ruleDict[i][2]]
    genYar(ruleDir+"full.yar",packerName,fullStr,"or")
    index=0
    for i in reStr:
        reName="temp"+str(index)
        genYar(ruleDir+reName+".yar",reName,i,"or")
        index+=1
    return index

def check(ruleDir,processRes,fullReverse,index,configure):
    file1=open(ruleDir+"full.yar")
    rules = yara.compile(file=file1)
    file1.close()
    file2=open(ruleDir+"p"+str(configure+1)+"_3.exe","rb")
    matches = rules.match(data=file2.read())
    file2.close()
    res = [binascii.b2a_hex(i[2]).decode() for i in matches[0].strings]
    checkRes={fullReverse[i]:processRes[fullReverse[i]] for i in res}
    for i in range(index):
        rulePath=ruleDir+"temp"+str(i)+".yar"
        file3=open(rulePath)
        rules = yara.compile(file=file3)
        file3.close()
        file4 = open(ruleDir + "p" + str(configure + 1) + "_3.exe", "rb")
        matches = rules.match(data=file4.read())
        file4.close()
        if len(matches):
            f=open(rulePath)
            strLine=f.readlines()[3]
            f.close()
            r=strLine[strLine.find("{")+1:strLine.find("}")]
            checkRes[r] = processRes[r]
    os.remove(ruleDir+"full.yar")
    for i in range(index):
        os.remove(ruleDir+"temp"+str(i)+".yar")
    return checkRes

#规则筛选，规则长度，频率，gap距离，与中位数比较,
def chooseRule(checkRes):
    freq=[checkRes[f][0] for f in checkRes]
    ruleLen = [checkRes[f][1] for f in checkRes]
    gapLen = [checkRes[f][2] for f in checkRes]
    norFreq=normalizeData(freq)
    norRuleLen = normalizeData(ruleLen)
    norGapLen = normalizeData(gapLen)
    rules=[r for r in checkRes]
    score=[norFreq[i]*1+norRuleLen[i]*1-norGapLen[i]*1 for i in range(len(rules))]
    ruleScore=zip(rules,score)
    #排序
    sus = sorted(ruleScore, key=(lambda x: x[1]),reverse = True)
    choosed=[sus[i][0] for i in range(min(10,len(sus)))]
    return choosed

def normalizeData(data):
    dataSet=set(data)
    if len(dataSet)>1:
        theMin=min(data)
        theMax=max(data)
        norData=[(d-theMin)/(theMax-theMin) for d in data]
        return norData
    else:
        return [1]*len(data)

if __name__=="__main__":
    sampleDir="D://work//yarpacker//examples//kkrunchy//"
    configuration=2
    choosed=[]
    for i in range(configuration):
        packerName="kkrunchy"+str(i+1)
        simPart=sim(sampleDir+"p"+str(i+1)+"_1.json", sampleDir+"p"+str(i+1)+"_2.json", 0.9)
        processRes, fullReverse = processRules(simPart, sampleDir)
        index = genRules(sampleDir, packerName, processRes)
        checkRes = check(sampleDir, processRes, fullReverse, index,i)
        if len(choosed):
            for cr in choosed:
                if cr not in choosed:
                    choosed.append(cr)
        else:
            choosed=chooseRule(checkRes)
    genYar(sampleDir + "kkrunchy.yar", "kkrunchy", choosed, "or")

