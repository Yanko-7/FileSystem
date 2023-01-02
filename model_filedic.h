 #ifndef MODEL_FILEDIC_H
#define MODEL_FILEDIC_H
#include "model_fat.h"
#define ATT_LEN 8


#define NOT_EXSIT -1
#define EXSIT -2
#define DIC_ATTRIBUTE 8
#define FILE_ATTRIBUTE 4
#define FILE_DIC_ATTRIBUTE 4
#define READABLE_ONLY 1
#define OUT_OF_SPACE8 -3
#define FATHER_PATH_NOT_EXSIT 9
#define CREATE_SUCCESS 10
#define OUT_OF_SPACE_DISC 11
#define DIC_ALREADY_EXSIT 12
#define DIC_NOT_EMPTY 13

//const int RW_MODE_ERROR = 121;
#include <string>
#include "model_openedfiletable.h"
#include "model_readdisc.h"
#include "model_stringoperation.h"
//using namespace std;
//const int EXSIT = -2;
class Model_FileDic
{
public:
    Model_FileDic(Model_FAT * f,Model_ReadDisc * d,Model_OpenedFileTable * oft,string parentPath = "",int * att = nullptr);
    ~Model_FileDic();
    //isBean()
    bool isDic(string name = "");
    bool isFile(string name = "");
    bool isReadableOnly(string name = "");
    bool isExsit(string name = "");               //是否存在
    bool isLeaf(string name = "");
    //getBean()
    int getLen(string name = "");
    int getBlockId(string name = "");
    int getType(string name = "");
    int getAtt(string name = "");
    string getName();                                   //文件名
    string getPathName();                               //路径名
    //setBean();
    void setLen(int len,string name = "");
    void setAtt(int att[]);
    void setAtt(string name,int att,int blockId,int len,int type,int *attr);
    void setThisAtt(string name,int att,int blockId,int len = 0,int type = 0);
    void clearAtt();
    void setPath();
    //
    int findEmpty();
    void writeFileDic(int *att);
    //unity function
    int delFileDic(const string & name);         //删除目录
    int createDic(string name);          //创建目录
    int createFile(string name,int rwMode); //创建文件
    void print(int h = 0,string blankSpace = "");                              //从当前目录开始打印
    Model_FileDic * getPointer(Model_StringOperation s);                       //得到路径为name的Pointer
    void checkAndAllocate(int i,Model_FileDic * obj = nullptr);                //检查第i个子目录是否为空
    Model_FileDic * getChild(int i );
private:
    //base pointer
    Model_FAT * fat;//FAT表
    Model_ReadDisc * disc;//模拟的物理磁盘块
    Model_OpenedFileTable * oft;//模拟的打开块
    //composite:
    Model_FileDic * children[ATT_LEN];  //孩子指针数组
    string path;      //路径名
    int * attribute;  //属性数组
    //Functions Declared:
    int manipulateFunction(string s,bool (Model_FileDic::*Function)(string name));
    int manipulateFunctionINT(string s,int (Model_FileDic::*Function)(string name));
    void manipulateFunctionVOID(int setV,string s,void (Model_FileDic::*Function)(int setValue,string name));
};

#endif // MODEL_FILEDIC_H
