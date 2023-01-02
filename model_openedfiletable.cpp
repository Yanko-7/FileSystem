#include "model_openedfiletable.h"


/*
 *
 *
 *
 *
 *
 *
 *
 */


Model_OpenedFileTable::Model_OpenedFileTable()
{
    for(int i = 0;i < MAXNUM_OPENEDFILETABLE;++i) {
        used[i] = 0;
        OFT[i] = nullptr;
    }
}


int Model_OpenedFileTable::remove(const string & name) {
    for(int i = 0;i < MAXNUM_OPENEDFILETABLE;++i) {
        if(used[i] == 1 && OFT[i]->getName() == name) {
            used[i] = 0;
            delete OFT[i];
            OFT[i] = nullptr;
            return TRUE_RETURN;
        }
    }
    return WRONG_RETURN;
}

int Model_OpenedFileTable::isOpen(const string & name) {
    for(int i = 0;i < MAXNUM_OPENEDFILETABLE;++i) {
        if(used[i] == 1 && OFT[i]->getName() == name) {
            return true;
        }
    }
    return false;
}

int Model_OpenedFileTable::findEmpty(int pos) {
    for(int i = pos;i < MAXNUM_OPENEDFILETABLE;++i)
        if(used[i] == 0) return i;
    return WRONG_RETURN;
}


int Model_OpenedFileTable::add(string name,int att,int blockId,int len,int flag,int rp_blockId,int rp_byteId,int wp_blockId,int wp_byteId) {
    int index = findEmpty();
    if(index == WRONG_RETURN)
       return WRONG_RETURN;
    used[index] = 1;
    OFT[index] = new file(name,att,blockId,len,flag,rp_blockId,rp_byteId,wp_blockId,wp_byteId);
    return TRUE_RETURN;
}

int Model_OpenedFileTable::getOFTItem(const string &name, int (file::*getFunction)()) {       //利用函数指针和file结构体内的getxxx()方法   只用一个接口就可以get到OFT[]数组里的很多属性
    for(int i = 0;i < MAXNUM_OPENEDFILETABLE;++i) {
        if(used[i] == 1 && OFT[i]->getName() == name) {
            return (OFT[i]->*getFunction)();
        }
    }
    return WRONG_RETURN;
}
int Model_OpenedFileTable::getOFTItem(int pos, int (file::*getFunction)()) {
    return  (OFT[pos]->*getFunction)();
}
void Model_OpenedFileTable::setOFTItem(const string & name,int setValue,void (file::*setFunction)(int)) {
    for(int i = 0;i < MAXNUM_OPENEDFILETABLE;++i) {
        if(used[i] == 1 && OFT[i]->getName() == name) {
            (OFT[i]->*setFunction)(setValue);
        }
    }
}

void Model_OpenedFileTable::display() {
    for(int i = 0;i < MAXNUM_OPENEDFILETABLE;++i) {
        if(used[i]) {
            file * p = OFT[i];
            cout<<"name:"<<p->getName()<<" att:"<<p->getAtt()<<" len:"<<p->getLen()<<" rp_blockId:"<<p->getRpBlockId()<<" rp_byteId:"<<p->getRpByteId()<<endl;
        }
    }
}


string Model_OpenedFileTable::getName(int pos) {
    return OFT[pos]->getName();
}


bool Model_OpenedFileTable::isUsed(int pos) {
    return used[pos] == 1;
}








