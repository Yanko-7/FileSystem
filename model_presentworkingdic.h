#ifndef MODEL_PRESENTWORKINGDIC_H
#define MODEL_PRESENTWORKINGDIC_H



#include <string>



using namespace std;
class Model_PresentWorkingDic
{
public:
    Model_PresentWorkingDic();
    void toNext(string name);
    void setPwp(string pwp);
    string getPwp();
private:
    string pwp;
};

#endif // MODEL_PRESENTWORKINGDIC_H
