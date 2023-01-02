

/*
 *  目录记录格式：
 *
 *  1、文件名/目录名            3 byte
 *  2、文件类型名/保留           2 byte
 *  3、文件属性/目录属性        1 byte
 *  4、文件盘块号/目录盘块号     1 byte
 *  5、文件长度/保留                1 byte
 *
 *
 *  3、文件/目录属性(1 byte == 8 bit)：  7      6      5      4      3      2         1       0
 *                                 unused unused unused unused 目录    普通文件   系统文件  只读文件
 *
 *  2、type用一个int变量表示
 *
 *      instance:
 *       3     2    1      1   1
 *       'abc' 'xx' (0x08) 2   0
 *  //
 *
 *  'A' ASC_CODE = 65
 *  'a' ASC_CODE = 97
 *
 *
 *  int isDic();
 *  判断当前实例是否为目录
 *
 *  int isFile();
 *  判断当前实例是否为文件目录
 *
 *  int isReadableOnly();
 *  判断当前实例是否为只读文件
 *
 *  int findEmpty();
 *  寻找空白文件目录
 *
 *  int writeFileDic();
 *  把当前实例写进磁盘
 *
 *  int loadAttribute();
 *  如果在blockId里面存在名为name的dic则载入属性
 *
 *  int static isExist();
 *  在blockId的块里面寻找是否存在有名为name的目录           返回NOT_EXIT 表示不存在  存在返回块内编号(0 ~ 7)
 */
#include "model_filedic.h"
#include <iostream>
using namespace std;
const int RW_MODE_ERROR = 121;



Model_FileDic::Model_FileDic(Model_FAT * f,Model_ReadDisc * d,Model_OpenedFileTable * oft,string parentPath,int * att):fat(f),disc(d),oft(oft),path(parentPath),attribute(att) {
    //初始化自己的attribute 指针
    cout<<attribute<<' '<<path<<endl;
    if(attribute == nullptr) {              //如果是根目录
        attribute = new int[ATT_LEN];
        setThisAtt(parentPath,DIC_ATTRIBUTE,2,0,0);     //置根目录初值
    }else {
        path +=  (path.empty()? "" : "\\") + getName();
    }
    cout<<parentPath<<' '<<attribute<<' '<<path<<endl;
    for(int i = 0;i < ATT_LEN;++i)
        children[i] = nullptr;
}


int Model_FileDic::createDic(string name) {
    if(isExsit(name)) return EXSIT;               //是否已经存在
    Model_StringOperation s(name);
    string fatherPathName = s.getFatherPath();
    string lastName = s.getLast();
    Model_FileDic * father = getPointer(fatherPathName);
    if(father == nullptr || !father->isDic()) return FATHER_PATH_NOT_EXSIT;
    else {
        for(int i = 0;i < ATT_LEN;++i) {
            checkAndAllocate(i,father);
            if(!father->children[i]->isExsit()) {
                int att[ATT_LEN]={0};
                int bId = fat->allocaBlock();
                if(bId != WRONG_RETURN) setAtt(lastName,DIC_ATTRIBUTE,bId,0,0,att);
                else return OUT_OF_SPACE_DISC;
                father->children[i]->writeFileDic(att);
                string tmp=fatherPathName;tmp+=(tmp.empty()? "" : "\\") + lastName;
                father->children[i]->path=tmp;
                //cout<<father<<' '<<father->children[i]<<endl;
                return CREATE_SUCCESS;
            }
        }
        return OUT_OF_SPACE8;
    }
}

int Model_FileDic::createFile(string name,int rwMode) {
    if(rwMode == READ_MODE) return RW_MODE_ERROR;            //读写方式检查
    if(isExsit(name)) return EXSIT;               //是否已经存在
    Model_StringOperation s(name);
    string fatherPathName = s.getFatherPath();
    string lastName = s.getLast();
    cout<<lastName<<' '<<fatherPathName<<endl;
    Model_FileDic * father = getPointer(fatherPathName);
    if(father == nullptr || !father->isDic()) return FATHER_PATH_NOT_EXSIT;
    else {
        for(int i = 0;i < ATT_LEN;++i) {
            checkAndAllocate(i,father);
            if(!father->children[i]->isExsit()) {
                int att[ATT_LEN]={0};
                int bId = fat->allocaBlock();
                if(bId != WRONG_RETURN) setAtt(lastName,FILE_ATTRIBUTE | rwMode,bId,0,0,att);
                else return OUT_OF_SPACE_DISC;
                cout<<"ok"<<' '<<lastName<<endl;
                father->children[i]->writeFileDic(att);
                string tmp=fatherPathName;tmp+=(tmp.empty()? "" : "\\") + lastName;
                father->children[i]->path=tmp;
                cout<<father<<' '<<father->children[i]<<endl;
                return CREATE_SUCCESS;
            }
        }
        return OUT_OF_SPACE8;
    }
}

int Model_FileDic::delFileDic(const string & name) {
    Model_StringOperation s(name);
    Model_FileDic * p = getPointer(s);
    if(p != nullptr){
        if(p->isDic() && !p->isLeaf()) return DIC_NOT_EMPTY;
        else if(p->isFile() && oft->isOpen(p->getPathName())) return ALREAD_OPEN;
        fat->delFromHead(p->getBlockId());
        p->clearAtt();
        return TRUE_RETURN;
    }
    return FATHER_PATH_NOT_EXSIT;
}



Model_FileDic * Model_FileDic::getPointer(Model_StringOperation s) {
    string name = s.getNext();
    if(name.empty()) return this;
    for(int i = 0;i < ATT_LEN;++i) {
        checkAndAllocate(i);
        if(children[i]->isExsit() && children[i]->getName() == name) {
            return children[i]->getPointer(s);
        }
    }
    return nullptr;
}


int Model_FileDic::manipulateFunction(string s,bool (Model_FileDic::*Function)(string)) {
    Model_FileDic * fd = getPointer(s);
    if(fd != nullptr) return (fd->*Function)(string(""));
    else return false;
}

bool Model_FileDic::isExsit(string name) {
    if(name.empty()) return isDic() || isFile();
    else return manipulateFunction(name,&Model_FileDic::isExsit);
}

bool Model_FileDic::isDic(string name){ //判断目录
    if(name.empty()) return (attribute[5] & DIC_ATTRIBUTE) > 0 && !fat->isEmpty(attribute[6]) ? true : false;
    else return manipulateFunction(name,&Model_FileDic::isDic);
}

bool Model_FileDic::isFile(string name) {
    //cout<<attribute[5]<<' '<<attribute[6]<<endl;
    if(name.empty()) {
        //cout<<1<<endl;
        return (attribute[5] & FILE_ATTRIBUTE) > 0 && !fat->isEmpty(attribute[6]) ? true : false;
    }
    else return manipulateFunction(name,&Model_FileDic::isFile);
}

bool Model_FileDic::isReadableOnly(string name) {
    if(name.empty()) return (attribute[5] & READ_MODE) == 1;
    else return manipulateFunction(name,&Model_FileDic::isReadableOnly);
}

bool Model_FileDic::isLeaf(string name) {
    if(name.empty()) {
        for(int i = 0;i < ATT_LEN;++i) {
            checkAndAllocate(i);
            if(children[i]->isExsit()) return false;
        }
        return true;
    }else{
        return manipulateFunction(name,&Model_FileDic::isLeaf);
    }
}
/////////////////////////////////////////////////////////
int Model_FileDic::manipulateFunctionINT(string s,int (Model_FileDic::*Function)(string)) {
    Model_FileDic * fd = getPointer(s);
    if(fd != nullptr) return (fd->*Function)(string(""));
    else return false;
}
int Model_FileDic::getLen(string name) {
    if(name.empty()) return attribute[7];
    else return manipulateFunctionINT(name,&Model_FileDic::getLen);
}

int Model_FileDic::getBlockId(string name) {
    if(name.empty()) return attribute[6];
    else return manipulateFunctionINT(name,&Model_FileDic::getBlockId);
}


int Model_FileDic::getType(string name) {
    if(name.empty()) return (attribute[4] | attribute[3]);
    else return manipulateFunctionINT(name,&Model_FileDic::getType);
}

int Model_FileDic::getAtt(string name) {
    if(name.empty()) return attribute[5];
    else return manipulateFunctionINT(name,&Model_FileDic::getAtt);
}


//////////////////////////////////////
void Model_FileDic::manipulateFunctionVOID(int setValue,string s, void (Model_FileDic::*Function)(int, string)) {
    Model_FileDic * fd = getPointer(s);
    if(fd != nullptr) return (fd->*Function)(setValue,string(""));
    else return;
}

void Model_FileDic::setLen(int len, string name) {
    if(name.empty()) attribute[7] = len;
    else manipulateFunctionVOID(len,name,&Model_FileDic::setLen);
}






void Model_FileDic::setAtt(int att[]) {
    for(int i = 0;i < ATT_LEN;++i)
        attribute[i] = att[i];
}

void Model_FileDic::clearAtt() {
    for(int i = 0;i < ATT_LEN;++i)
        attribute[i] = 0;
}

void Model_FileDic::setAtt(string name,int att,int blockId,int len,int type,int * attr) {
    int i;
    cout<<name+"+"<<endl;
    for(i = 0;i <  static_cast<int>(name.size()) ;++i) {
        attr[i] = name.at(static_cast<unsigned int>(i));
    }
    if(i < 2) attr[i] = '\0';
    attr[3] = type & 0x00FF;
    attr[4] = type & 0xFF00;
    attr[5] = att;
    attr[6] = blockId;
    attr[7] = len;
    cout<<"begin:";
    for(int i=0;i<8;i++)cout<<attr[i]<<' ';
    cout<<endl;
}

void Model_FileDic::setThisAtt(string name,int att,int blockId,int len,int type) {
    int i;
    for(i = 0;i <  static_cast<int>(name.size()) ;++i) {
        attribute[i] = name.at(static_cast<unsigned int>(i));
    }
    if(i < 2) attribute[i] = '\0';
    attribute[3] = type & 0x00FF;
    attribute[4] = type & 0xFF00;
    attribute[5] = att;
    attribute[6] = blockId;
    attribute[7] = len;
}


void Model_FileDic::writeFileDic(int * att) {
    cout<<"end:";
    for(int i = 0;i < ATT_LEN;++i) {
        attribute[i] = att[i];
        cout<<attribute[i]<<' ';
    }
    cout<<endl;
}

string Model_FileDic::getName() {
    char ts[4];
    int i;
    for(i = 0;i < 3;++i)
        ts[i] = static_cast<char>(attribute[i]);
    ts[i] = '\0';
    string s(ts);
    return s;
}




string Model_FileDic::getPathName() {
    return path;
}

int Model_FileDic::findEmpty() { //寻找空白文件目录
    // 2 + 4 + 8 = 14
    //int blockId = getBlockId();
    for(int i = 0;i < ATT_LEN;++i) {
        if(isExsit())
            return i;
    }
    return WRONG_RETURN;
}



void Model_FileDic::print(int h,string blankSpace) {
    if(isExsit()) cout<<blankSpace<<getName() + (isFile()?" f":" d")<<"  sb:"<<getBlockId()<<endl;
    blankSpace += "--";
    for(int i = 0;i < ATT_LEN;++i) {
        checkAndAllocate(i);
        if(isDic()) {
            children[i]->print(h+1,blankSpace);
        }
    }
}
void Model_FileDic::checkAndAllocate(int i,Model_FileDic * obj) {
    Model_FileDic * p = nullptr == obj ? this : obj;
    int blockId = p->getBlockId();
    if(p->children[i] == nullptr) {
        p->children[i] = new Model_FileDic(fat,disc,oft,p->path,disc->getPoniter(blockId,i*8));
    }else {
        if(p->children[i]->isExsit() &&  (p->path == p->children[i]->path || p->path + "\\" == p->children[i]->path)) {
            delete  p->children[i];
            p->children[i] = new Model_FileDic(fat,disc,oft,p->path,disc->getPoniter(blockId,i*8));
        }
        //cout<<p->children[i]->getPathName()<<endl;
    }
}


Model_FileDic * Model_FileDic::getChild(int i) {
    return children[i];
}

Model_FileDic::~Model_FileDic() {}
