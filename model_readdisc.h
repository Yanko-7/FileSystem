#ifndef MODEL_READDISC_H
#define MODEL_READDISC_H
#include "iostream"
#include "stdio.h"
const int buf1_LEN = 64;
const int buf2_LEN = 128;
const int WRONG_RETURN = 0xFFFF;
const int TRUE_RETURN = 0xFFFFF;
const int READ_MODE = 0;
const int WRITE_MODE = 1;
const int RESET = -1;
class Model_ReadDisc //模拟的物理磁盘块
{
public:
    Model_ReadDisc(const char * s);
    int readByte(int blockId,int byteId);
    int writeByte(int blockId,int byteId,int byte);
    int * getPoniter(int blockId,int byteId);
    void close();
    void loadBuffer(int * buf,int arrayIndex,int offset,int blockId);
    void showBuf(int a);
    int openFile(int mode,const char *s);
    int writeBuf2Disc();
    int buf2[buf2_LEN][buf1_LEN];
private:
    int * loadBuf1(int blockId);
    int loadBuf2();
    void loadBuf1(int a[]);
    int buf1[buf1_LEN];

    int buf1_block;
    FILE * fp;
    const char * discPath;
};

#endif // MODEL_READDISC_H
