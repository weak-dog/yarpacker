import angr
import binascii
import normalizer
import json

def Read(rootdir):
    lines = []
    with open(rootdir, 'r') as f:
        while (True):
            line = f.readline()
            if not line:
                break
            line = line.strip('\n')
            lines.append(line)
    return lines

def bytes_handler(exeFilePath):
    pinOutPath = exeFilePath.replace(".exe", ".pin")
    angrOutPath = exeFilePath.replace(".exe", ".angr")
    res = {}#{bytes:[block,times]}
    lines = Read(pinOutPath)
    proj = angr.Project(exeFilePath)
    for line in lines:
        bbl = line.split(',')
        try:
            b = proj.factory.block(int(bbl[0], 16))
            if b.instructions > 2:
                bts = proj.loader.memory.load(int(bbl[0], 16), int(bbl[1]))
                bts2 = binascii.unhexlify(bytes(bbl[3], encoding="utf8"))
                if bts == bts2:
                    #bb = b.bytes
                    bb=bbl[3]
                    if bb in res.keys():
                        res[bb][1] = res[bb][1] + int(bbl[2])
                    else:
                        res[bb] = [b, int(bbl[2])]
        except:
            pass
    resList = [[i, res[i][0], res[i][1]] for i in res.keys()]#bytes,block,reachtimes
    # 按照地址排序
    sr0 = sorted(resList, key=(lambda x: x[1].addr))
    # 一次遍历去除重复项
    i = 0
    while i != len(sr0) - 1:
        if sr0[i][1].addr + sr0[i][1].size == sr0[i + 1][1].addr + sr0[i + 1][1].size:
            sr0[i][2] += sr0[i + 1][2]
            sr0.remove(sr0[i + 1])
        else:
            i += 1
    #按照执行次数排序
    sr1 = sorted(sr0, key=(lambda x: x[2]),reverse = True)
    fp=open(angrOutPath,'w+')
    for r in sr1:
        fp.write("addr:" + hex(r[1].addr) + "  reachtimes:" + str(r[2]) + "  size:" + str(r[1].size) + "  bytes:" + r[0] + "\n")
        for ins in r[1].capstone.insns:
            fp.write(str(ins)+"\n")
    fp.close()
    return angrOutPath

#汇编代码平滑化
def flat(norFilePath):
    f = open(norFilePath, "r")
    lines = f.readlines()
    f.close()
    res={}#nor:[reachtimes,bytes]
    str=""
    bytes=""
    reachtimes=0
    for line in lines:
        if line.startswith("addr"):
            if str in res.keys():
                res[str][0] = res[str][0] + reachtimes
            else:
                if len(str):
                    res[str] = [reachtimes,bytes]
            if len(line.split())>1:
                reachtimes = int(line.split()[1].split(":")[1])
                bytes = line.split()[3].split(":")[1]
                str=""
        else:
            str+=line.replace('\n', '')+" "
    #字典转化为list
    resList = [[i,res[i][0],res[i][1]] for i in res.keys()]#[nor,reachtimes,bytes]
    #按执行次数排序
    resList = sorted(resList, key=(lambda x: x[1]), reverse=True)
    fp = open(norFilePath.replace(".nor", ".json"), 'w', encoding='utf8')
    json.dump(resList,fp,ensure_ascii=False)
    fp.close()

if __name__ == '__main__':
    exeDir="D://work//yarpacker//examples//upx//"
    configureNum=2
    for i in range(configureNum):
        exeFilePath1 = exeDir + "p" + str(i + 1) + "_1.exe"
        exeFilePath2 = exeDir + "p" + str(i + 1) + "_2.exe"
        norFilePath1=normalizer.normalize(bytes_handler(exeFilePath1))
        flat(norFilePath1)
        print(1)
        norFilePath2 = normalizer.normalize(bytes_handler(exeFilePath2))
        flat(norFilePath2)
        print(2)
