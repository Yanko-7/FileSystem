#ifndef MODEL_STRINGOPERATION_H
#define MODEL_STRINGOPERATION_H
#include "string"
using namespace std;
class Model_StringOperation
{

public:
    Model_StringOperation(string name);
    string getNext(int type = 0);
    string getFatherPath();
    string getLast();
    void goBack();
    /*
     * string getNext();
     * 不断给出下一个目录名或者文件名  没有就返回空字符串
     *
     *
     */
    //static unsigned int pointer;
private:
    string name;
    unsigned int pointer,pp;
};

#endif // MODEL_STRINGOPERATION_H
