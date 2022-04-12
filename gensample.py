import os

def genUpxSample():
    pathDir="D://work//yarpacker//examples//upx"
    # configuration=["-1","-2","-3","-4","-5","-6","-7","-8","-9"]
    configuration = ["-1","-9"]
    os.chdir(pathDir)
    sampleNames=[]
    for i in range(len(configuration)):
        outPath1 = "p" + str(i + 1) + "_1.exe"
        outPath2 = "p" + str(i + 1) + "_2.exe"
        outPath3 = "p" + str(i + 1) + "_3.exe"
        command1 = "upx 1.exe " + configuration[i] + " -o " + outPath1
        command2 = "upx 2.exe " + configuration[i] + " -o " + outPath2
        command3 = "upx test.exe " + configuration[i] + " -o " + outPath3
        os.system(command1)
        os.system(command2)
        os.system(command3)
        sampleNames.append(outPath1)
        sampleNames.append(outPath2)
    return sampleNames,pathDir

def genPinFile(fileNameList,pathDir):
    os.chdir("D://work//yarpacker")
    for sampleName in fileNameList:
        outName=pathDir+"//"+sampleName.replace(".exe", ".pin")
        command="pin32 -t jump6.dll -- "+pathDir+"//"+sampleName+" "+outName
        print(command)
        os.system(command)
if __name__=="__main__":
    sampleNames,pathDir=genUpxSample()
    genPinFile(sampleNames,pathDir)