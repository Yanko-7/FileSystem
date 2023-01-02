#ifndef MODEL_FAT_H
#define MODEL_FAT_H
#include "model_readdisc.h"
const int LEN = 128;
#define END 0xFF
#define EMPTY 0x00
#define DAMAGE 0xFE
class Model_FAT
{
public:
    Model_FAT(Model_ReadDisc *d);
    int allocaAndInsert(int head);
    int allocaBlock();
    int isEmpty(int blockId);
    int isDamage(int blockId);
    int isLast(int blockId);
    int isAllocated(int blockId);
    int findEmptyBlock();
    int delFromHead(int head);
    int insertBlock(int head);
    int showFATArray();
    int getNext(int blockId);
    void delNexts(int blokcId);
private:
    //int FATArray[LEN];//
    int * FATArray;
    Model_ReadDisc * disc;
};

#endif // MODEL_FAT_H
