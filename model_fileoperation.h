#ifndef MODEL_FILEOPERATION_H
#define MODEL_FILEOPERATION_H
#include <string>
#include <cstdio>
#include "model_readdisc.h"
#include "model_stringoperation.h"
#include "model_filedic.h"
#include "model_fat.h"
#include "model_openedfiletable.h"

const int OPEN_MODE_ERROR = 118;
const int FILE_NAME_NOT_EXSIT_ERROR = 119;
const int END_OF_FILE = 120;
const int RW_MODE_ERROR = 121;
const int FILE_NOT_OPEN = 123;
const int FILE_ALREADY_OPEN = 124;
const int FILE_NOT_EXIT = 125;

//const int EXSIT = -2;


const int MAX_BUFFER_LEN = 10*64;

using namespace std;
class Model_FileOperation //文件操作类
{
public:
    Model_FileOperation(Model_ReadDisc * d,Model_FAT * f,Model_OpenedFileTable * o,Model_FileDic * fd);
    string getNext(const string & name);
    int create_file(string name,int att);
    int open_file(string name,int mode);
    int read_file(const string & name,int len,int offset = 0);
    int write_file(const string &name,int* buf,int len);
    int close_file(const string & name);
    int delete_file(const string & name);
    int chang(const string &name,int *att);
    int md(string dname);
    int dir(const string & dname);
    int rd(const string & name);
    string toString();

    int writeBuffer[MAX_BUFFER_LEN];
private:
    int buf1[ATT_LEN];
    Model_ReadDisc * disc;
    Model_FAT * fat;
    Model_OpenedFileTable * oft;
    Model_FileDic * fileDic;
    char buffer[MAX_BUFFER_LEN];
    int bufferLen;
    void toBuffer();

};

#endif // MODEL_FILEOPERATION_H
