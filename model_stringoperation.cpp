#include "model_stringoperation.h"

Model_StringOperation::Model_StringOperation(string name):name(name),pointer(0){}

string  Model_StringOperation::getNext(int type) {
    unsigned int i = 0;
    char tmp[10];
    if(pointer < name.size()) {
        if(name.at(pointer) == '\\')
            pointer++;
        while(pointer < name.size() && name.at(pointer)!= '\\') {
            tmp[i++] = name.at(pointer++);
        }
        tmp[i] = '\0';
        if(type) goBack();
        return string(tmp);
    }else {
        return string();
    }
}

string Model_StringOperation::getFatherPath() {
    pp = pointer;
    while(pointer < name.size()) {
        pp = pointer;
        getNext();
    }
    return name.substr(0,pp);
}


string Model_StringOperation::getLast() {
    return name.substr(pp==0?0:pp+1,name.size());
}
void Model_StringOperation::goBack() {
    while(pointer > 0) {
        --pointer;
        if(name.at(pointer) == '\\')
            break;
    }
}


