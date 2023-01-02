#include "model_fat.h"
#define LEN 128           //FAT块长度
#define startOfTheFat 3
#include <stack>

/*
 * 对FAT的操作集合
 *
    int allocationBlock();
    申请一块  成功返回allocationBlock的id  失败返回0

    int delFromHead(int headBlockId);
    给出头地址从头开始删除到尾  成功1 失败返回0

    int insertBlock(int head);
    在给定的head的链尾部分配并且插入一个Block   成功返回1 失败返回0

    int isTheFirst(int blockId)
    判定给定的blockId是否是一个文件的首部
*/
using namespace std;
Model_FAT::Model_FAT(Model_ReadDisc *d):disc(d){
    FATArray = &disc->buf2[0][0];
}

int Model_FAT::findEmptyBlock() {
    for(int i = startOfTheFat;i < LEN; ++i) {         //从startOfTheFat开始找（磁盘前两个块为FAT本身）
        if(FATArray[i] == EMPTY) {
            return i;
        }
    }
    printf_s("FAT out of space !\n");
    return WRONG_RETURN;
}


int Model_FAT::allocaBlock() {
    int Id = findEmptyBlock();
    if(Id != WRONG_RETURN) {
        FATArray[Id] = END;
        disc->writeByte(Id/64,Id%64,END);
        return Id;
    }
    else {
        return WRONG_RETURN;
    }
}


int Model_FAT::delFromHead(int headBlockId) {
    if(headBlockId >= LEN || headBlockId <= 1) {
        printf_s("headBlockId out of index!\n");
        return WRONG_RETURN;
    }
    while(FATArray[headBlockId] != 0) {
       int tmp = headBlockId;
       headBlockId = FATArray[headBlockId];
       FATArray[tmp] = 0;
       disc->writeByte(tmp/64,tmp%64,0);
       if(headBlockId == END)
           break;
    }
    return TRUE_RETURN;
}


int Model_FAT::allocaAndInsert(int head) {
    while(FATArray[head] != END) {
        head = FATArray[head];
    }
    int Id = allocaBlock();
    if(Id != WRONG_RETURN) {
        FATArray[head] = Id;
        disc->writeByte(head/64,head%64,Id);
        return TRUE_RETURN;
    }
    return WRONG_RETURN;
}

int Model_FAT::isEmpty(int blockId) {
    return FATArray[blockId] == EMPTY ? true : false;
}
int Model_FAT::isDamage(int blockId) {
    return FATArray[blockId] == DAMAGE ? true : false;
}
int Model_FAT::isLast(int blockId){
    return FATArray[blockId] == END ? true :false;
}

int Model_FAT::isAllocated(int blockId) {
    if(blockId > 2 && blockId < 128)
        return FATArray[blockId] != END && FATArray[blockId] != DAMAGE && FATArray[blockId] != EMPTY;
    else return false;
}
int Model_FAT::getNext(int blockId) {
    return FATArray[blockId];
}

void Model_FAT::delNexts(int blockId) {
    stack<int> s;
    int tmp = blockId;
    while(FATArray[blockId] != END) {
       blockId = FATArray[blockId];
       s.push(blockId);
    }
    while(!s.empty()) {
        FATArray[s.top()] = EMPTY;
        s.pop();
    }
    FATArray[tmp] = END;
    cout<<endl<<"tmp"<<tmp<<" "<<FATArray[tmp]<<endl;
}
int Model_FAT::showFATArray() {
    for(int i = 0;i < LEN;++i) {
        if(i%8 == 0)
            printf("\n");
        printf("[%3d] = %3d  ",i,FATArray[i]);
    }
    printf("\n");
    return 1;
}



