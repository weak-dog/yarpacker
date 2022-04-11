#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include<hash_map>
#include "pin.H"

using namespace std;

class bbl {
public:
    ADDRINT header; //bbl头
    int len;        //bbl长度
    int reach;      //bbl执行次数
    unsigned char* memValue;
    bbl(ADDRINT header = 0, int reach = 0) {
        this->header = header;
        this->reach = reach;
        memValue = new unsigned char[100];
    }
    bool operator==(bbl b) const {
        return this->header == b.header;
    }
    bool operator < (bbl b) const {
        return this->header < b.header;
    }
};

FILE* OutFile;
hash_map<ADDRINT, bbl>bbls;
vector<bbl>bbls2;
vector<bbl>bbls3;

VOID recordJump(ADDRINT ins, ADDRINT dst, ADDRINT next, bool taken) {
    PIN_LockClient();
    IMG img1 = IMG_FindByAddress(ins);
    IMG img2 = IMG_FindByAddress(dst);
    PIN_UnlockClient();
    if (IMG_Valid(img1) && IMG_IsMainExecutable(img1)) {
        if (IMG_Valid(img2) && IMG_IsMainExecutable(img2)) {
            hash_map<ADDRINT, bbl>::iterator i1 = bbls.find(dst);
            bool has = false;//dst是否已存在
            if (i1 == bbls.end()) {
                bbl b;  bbls[dst] = b;
            }
            hash_map<ADDRINT, bbl>::iterator i2 = bbls.find(next);
            if (i2 == bbls.end()) {
                bbl b;  bbls[next] = b;
            }
            if (taken) {
                bbls[dst].reach++;
            }
            else {
                bbls[next].reach++;
            }
        }
        else {
            hash_map<ADDRINT, bbl>::iterator i1 = bbls.find(next);
            if (i1 == bbls.end()) {
                bbl b;  bbls[next] = b;
            }
            if (!taken) {
                bbls[next].reach++;
            }
        }
    }
    else {
        if (IMG_Valid(img2) && IMG_IsMainExecutable(img2)) {
            hash_map<ADDRINT, bbl>::iterator i1 = bbls.find(dst);
            if (i1 == bbls.end()) {
                bbl b;  bbls[dst] = b;
            }
            if (taken) {
                bbls[dst].reach++;
            }
        }
    }
}

VOID Instruction(INS ins, VOID* v) {
    if (INS_IsControlFlow(ins)) {
        ADDRINT next = INS_NextAddress(ins);
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)recordJump, IARG_INST_PTR, IARG_BRANCH_TARGET_ADDR, IARG_ADDRINT, next, IARG_BRANCH_TAKEN, IARG_END);
    }
}

VOID Fini(INT32 code, VOID* v)
{
    hash_map<ADDRINT, bbl>::iterator it = bbls.begin();
    for (it = bbls.begin(); it != bbls.end(); ++it) {
        bbls2.push_back(bbl(it->first, (it->second).reach));
    }
    sort(bbls2.begin(), bbls2.end());//地址排序
    for (int i = 0; i < bbls2.size() - 1; i++) {
        int len = bbls2[i + 1].header - bbls2[i].header;
        if (bbls2[i].reach && (len>3)) {
            bbls3.push_back(bbl(bbls2[i].header, bbls2[i].reach));
            bbls3[bbls3.size() - 1].len = len;
            PIN_SafeCopy(bbls3[bbls3.size() - 1].memValue, (void*)bbls3[bbls3.size() - 1].header, len);
        }
    }
    for (int i = 0; i < bbls3.size(); i++) {
        fprintf(OutFile, "%p,%d,%d,", (void*)bbls3[i].header, bbls3[i].len, bbls3[i].reach);
        for (int j = 0; j < bbls3[i].len - 1; j++)fprintf(OutFile, "%02x", bbls3[i].memValue[j]);
        fprintf(OutFile, "%02x\n", bbls3[i].memValue[bbls3[i].len - 1]);
    }
    fclose(OutFile);
}

INT32 Usage()
{
    cerr << "This tool counts the number of dynamic instructions executed" << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

int main(int argc, char* argv[])
{
    if (PIN_Init(argc, argv)) return Usage();
    char* filePath = argv[5];
    OutFile = fopen(filePath, "w");
    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);
    PIN_StartProgram();

    return 0;
}
