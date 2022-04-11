import json
import re

RegGen32=["eax" , "ebx" , "ecx" , "edx"]
RegGen16=["ax" , "bx" , "cx" , "dx"]
RegGen8=["ah" , "bh" , "ch" , "dh" , "al" , "bl" , "cl" , "dl"]
RegSeg=["cs" , "ds" , "es" , "fs" , "gs" , "ss"]
RegIdx=["edi" , "di" , "esi", "si"]
RegPtr=["ebp" , "bp" , "esp", "sp" , "eip" , "ip"]
RegEFlags=["cf" , "pf" , "af", "zf", "sf" , "tf" , "if" , "df" , "of" , "iopl" , "nt" , "rf" , "vm" , "ac" , "vif" , "vip" , "id"]
RegDict = {1: "reg32", 2: "reg16", 3: "reg8", 4: "regseg", 5: "regidx", 6: "regptr", 7: "regeflags"}
def isConstantValue(str):
    try:
        int(str, 16)
        return True
    except ValueError:
        try:
            int(str, 10)
            return True
        except ValueError:
            return False

def isRegisterReference(str):
    if str in RegGen32:
        return 1
    elif str in RegGen16:
        return 2
    elif str in RegGen8:
        return 3
    elif str in RegSeg:
        return 4
    elif str in RegIdx:
        return 5
    elif str in RegPtr:
        return 6
    elif str in RegEFlags:
        return 7
    else:
        return 0

def normal(str):
    if isConstantValue(str):
        return "val"
    else:
        reg=isRegisterReference(str)
        if reg:
            return RegDict[reg]
        else:
            return str

def normalize(filePath):
    f1=open(filePath,"r")
    norPath=filePath.replace(".angr",".nor")
    f2=open(norPath,"w")
    lines=f1.readlines()
    f1.close()
    for line in lines:
        if line.startswith("addr"):
            f2.write(line)
        else:
            line=line.replace('\t',' ')
            line = re.sub("\[.*\]", "mem", line)
            sp1=line.split(",")
            sp2=sp1[0].split()
            address=sp2[0]
            mnemonic=sp2[1]
            f2.write(mnemonic)  # 地址 助记符
            #f2.write(address + " " + mnemonic)  # 地址 助记符
            #第一个操作数
            for i in range(len(sp2)-2):
                # f2.write("|"+sp2[i+2]+"|")
                f2.write(" "+normal(sp2[i+2]))
            if len(sp1)>1:
                f2.write(",")
                #处理第二个操作数
                sp3 = sp1[1].split()
                for i in range(len(sp3)):
                    # f2.write("|" + sp3[i] + "|")
                    f2.write(" " + normal(sp3[i]))
                f2.write("\n")
            else:
                f2.write("\n")
    f2.write("addr")#结束符
    f2.close()
    return  norPath

