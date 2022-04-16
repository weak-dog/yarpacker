import os
import shutil

def CreateDir(path):
    isExists=os.path.exists(path)
    # 判断结果
    if not isExists:
        # 如果不存在则创建目录
        os.makedirs(path)
        print(path+' 目录创建成功')
    else:
        # 如果目录存在则不创建，并提示目录已存在
        print(path+' 目录已存在')

def CopyFile(filepath, newPath,num):
    try:
        # 不能确定正确执行的代码
        fileNames = os.listdir(filepath)
    except:
        return num
    for file in fileNames:
        if file[0]=="." or file[0]=="$":
            continue
        newDir=filepath+"//"+file
        if os.path.isfile(newDir):
            index = file.rfind(".")
            if file[index:] == ".exe":
                if num>99:
                    break
                num += 1
                print(str(num),newDir)
                newFile = newPath + str(num)+".exe"
                shutil.copyfile(newDir, newFile)
        else:
            print("dir")
            if num<=99:
                num=CopyFile(newDir, newPath,num)
    return num

#
def genUpxSample():
    pathDir="C://work//yarpacker//samples"
    os.chdir(pathDir)
    fileNames = os.listdir(pathDir)
    for file in fileNames:
        print(file)
        if file!="upx.exe":
            command="upx "+file+" -o u"+file
            print(command)
            os.system(command)

if __name__ == "__main__":
    path = 'D:/'
    mkPath = "C://work/yarpacker//samples//"
    # CreateDir(mkPath)
    # CopyFile(path,mkPath,0)
    genUpxSample()
