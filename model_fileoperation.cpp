#include "model_fileoperation.h"




/*
 *
 *
 *
 *
 */

#include <algorithm>

Model_FileOperation::Model_FileOperation(Model_ReadDisc * d,Model_FAT * f,Model_OpenedFileTable * o,Model_FileDic * fd) : disc(d),fat(f),oft(o),fileDic(fd) {
    //fileDic->setThisAtt(fileDic->getName());
    //fileDic->setThisAtt();
}


int Model_FileOperation::create_file(string name,int rwMode) { //文件名 文件属性（只读或者写）
    return fileDic->createFile(name,rwMode);
}

int Model_FileOperation::md(string name) {
    return fileDic->createDic(name);
}

int Model_FileOperation::open_file(string name,int rwMode) { //文件名 操作类型
    if(fileDic->isExsit(name)) {        //是否存在
        if(!oft->isOpen(name)) {        //是否已经打开
            if(rwMode == WRITE_MODE && fileDic->isReadableOnly(name))  //读写模式是否正确
                return OPEN_MODE_ERROR;
            Model_FileDic * p = fileDic->getPointer(name);
            //cout<<"OFT ADD:" + name<<endl;
            int st = oft->add(name,p->getAtt(),p->getBlockId(),p->getLen(),rwMode,p->getBlockId(),0,p->getBlockId(),0);
            if(st == WRONG_RETURN) return WRONG_RETURN;
            else return TRUE_RETURN;
        }
        else return ALREAD_OPEN;
    }
    else return FILE_NAME_NOT_EXSIT_ERROR;
}


int Model_FileOperation::read_file(const string & name,int len,int offset) {   //文件名  长度为字节   读到buffer   offset = 0 代表从头开始读 1代表从读指针的位置开始读
    if(!oft->isOpen(name)) {                                     //检查是否打开
        int status = open_file(name,READ_MODE);                  //没打开就打开
        if(status != TRUE_RETURN) return status;
    }
    if(oft->getOFTItem(name,&file::getFlag) != READ_MODE) return RW_MODE_ERROR;    //检查打开方式
    int oftLen = oft->getOFTItem(name,&file::getLen);
    int oftRpBlockId = (offset == 0 ? fileDic->getBlockId(name) : oft->getOFTItem(name,&file::getRpBlockId));
    int oftRpByteId = (offset == 0 ? 0 : oft->getOFTItem(name,&file::getRpByteId));
    bufferLen = min(len,min(oftLen,MAX_BUFFER_LEN));             //bufferLen取三者最小
    //循环开始:
    for(int i = 0;i < bufferLen;++i,++oftRpByteId) {
        if(i != 0 && oftRpByteId%buf1_LEN == 0 && !fat->isLast(oftRpBlockId)) {
            oftRpBlockId = fat->getNext(oftRpBlockId);
            oftRpByteId = 0;
        }
        buffer[i] = static_cast<char>(disc->readByte(oftRpBlockId,oftRpByteId));
    }
    //更新读写指针:
    oft->setOFTItem(name,oftRpBlockId,&file::setRpBlockId);
    oft->setOFTItem(name,bufferLen%buf1_LEN,&file::setRpByteId);
    return TRUE_RETURN;
}

int Model_FileOperation::write_file(const string & name,int * buf,int len) {
    if(!oft->isOpen(name)) {                                     //检查是否打开
        int status = open_file(name,WRITE_MODE);                  //没打开就打开
        if(status != TRUE_RETURN) return status;
    }
    if(oft->getOFTItem(name,&file::getFlag) != WRITE_MODE) return RW_MODE_ERROR;    //检查打开方式
    int oftWpBlockId = oft->getOFTItem(name,&file::getWpBlockId);
    int oftWpByteId = oft->getOFTItem(name,&file::getWpByteId);
    //循环开始:
    int i;
    for(i = 0;i < len;++i,++oftWpByteId) {
        cout<<static_cast<char>(buf[i]);
        if(i!=0 && oftWpByteId%buf1_LEN == 0) {
            if(fat->allocaAndInsert(oftWpBlockId) != TRUE_RETURN)
               return OUT_OF_SPACE_DISC;
            oftWpBlockId = fat->getNext(oftWpBlockId);
            oftWpByteId = 0;
        }
        disc->writeByte(oftWpBlockId,oftWpByteId,buf[i]);
    }
    fat->delNexts(oftWpBlockId);
    //更新读写指针:
    cout<<"WRITE:len"<<len<<endl;
    oft->setOFTItem(name,oftWpBlockId,&file::setWpBlockId);
    oft->setOFTItem(name,i%buf1_LEN,&file::setWpByteId);
    oft->setOFTItem(name,i,&file::setLen);
    fileDic->setLen(len,name);    //记得把长度写回去
    return TRUE_RETURN;
}
int Model_FileOperation::close_file(const string & name) {
    if(oft->isOpen(name)) {
        oft->remove(name);
    }
    return FILE_NOT_OPEN;
}

int Model_FileOperation::delete_file(const string & name) {
    return fileDic->delFileDic(name);
}
int Model_FileOperation::rd(const string & name) {
    return fileDic->delFileDic(name);
}

int Model_FileOperation::chang(const string & name,int *att) {
    if(!fileDic->isExsit(name)) return FILE_NOT_EXIT;
    else {
        if(oft->isOpen(name)) return ALREAD_OPEN;
        else {
            Model_FileDic * p = fileDic->getPointer(name);
            p->setAtt(att);
            return TRUE_RETURN;
        }
    }
}

int Model_FileOperation::dir(const string & name) {
    if(!fileDic->isExsit(name)) return FILE_NOT_EXIT;
    else {
        Model_FileDic * p = fileDic->getPointer(name);
        p->print();
        return TRUE_RETURN;
    }
}


string Model_FileOperation::toString(){
    buffer[bufferLen] = '\0';
    return string(buffer);
}
