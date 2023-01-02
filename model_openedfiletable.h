#ifndef MODEL_OPENEDFILETABLE_H
#define MODEL_OPENEDFILETABLE_H
#include <string>
//#include <map>
#include "model_readdisc.h"

const int ALREAD_OPEN = 1;
const int MAXNUM_OPENEDFILETABLE = 5;

using namespace std;
typedef struct file{ //模拟的已打开文件块
public:
    file(string name,int att,int blockId,int len,int flag,int rp_blockId,int rp_byteId,int wp_blockId,int wp_byteId):
        name(name),att(att),blockId(blockId),len(len),flag(flag),rp_blockId(rp_blockId),rp_byteId(rp_byteId),wp_blockId(wp_blockId),wp_byteId(wp_byteId){}
    int getAtt() {return att;}
    int getBlockId() {return blockId;}
    int getLen() {return len;}
    int getFlag() {return flag;}  //读写模式   READ_MODE  OR  WRITE MODE
    int getRpBlockId() {return rp_blockId;}
    int getRpByteId() {return rp_byteId;}
    int getWpBlockId() {return wp_blockId;}
    int getWpByteId() {return wp_byteId;}
    string getName() {return name;}
    void setAtt(int attribute) {att = attribute;}
    void setLen(int length) {len = length;}
    void setRpBlockId(int blockId) {rp_blockId = blockId;}
    void setRpByteId(int byteId) {rp_byteId = byteId;}
    void setWpBlockId(int blockId) {wp_blockId = blockId;}
    void setWpByteId(int byteId) {wp_byteId = byteId;}
    void setRWP(int set = 0) {rp_blockId = rp_byteId = wp_blockId = wp_byteId = set;}
private:
    string name;
    int att,blockId,len,flag;
    int rp_blockId,rp_byteId;             //读写指针   READ_POINTER_BLOCKID    WRITE_POINTER_BLOCKID
    int wp_blockId,wp_byteId;
}file;


class Model_OpenedFileTable
{
public:
    Model_OpenedFileTable();
    int add(string name,int att,int blockId,int len,int flag,int rp_blockId,int rp_byteId,int wp_blockId,int wp_byteId);
    int findEmpty(int pos = 0);
    int remove(const string & name);
    int isOpen(const string & name);
    int getRpBlockId(const string & name);
    void setOFTItem(const string & name,int setValue,void (file::*setFunction)(int));
    int getOFTItem(const string & name,int (file::*getFunction)());
    int getOFTItem(int pos,int (file::*getFunction)());
    string getName(int pos);
    bool isUsed(int pos);
    void display();
private:
    file * OFT[MAXNUM_OPENEDFILETABLE];
    int used[MAXNUM_OPENEDFILETABLE];
};



#endif // MODEL_OPENEDFILETABLE_H
