#include "model_presentworkingdic.h"

Model_PresentWorkingDic::Model_PresentWorkingDic():pwp(""){

}


void Model_PresentWorkingDic::toNext(string name) {
    pwp+= (pwp==""?"":"\\")+name;
}

void Model_PresentWorkingDic::setPwp(string pwp) {
    this->pwp = pwp;
}


string Model_PresentWorkingDic::getPwp() {
    return pwp;
}
