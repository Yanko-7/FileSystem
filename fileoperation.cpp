
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <model_readdisc.h>
#include <model_fat.h>
#include "model_fileoperation.h"
#define de_values "disc.txt"
#define te_values "test.txt"
using namespace std;


/*
int main(void) {
    Model_ReadDisc disc(te_values);
    Model_FAT fat(&disc);
    fat.showFATArray();
    Model_FileDic fd(&fat,&disc);
    Model_OpenedFileTable oft;
    Model_FileOperation s(&disc,&fat,&oft,&fd);
    //string name = "aaa";
    //s.md("aaa");
    //s.create_file("ccc",WRITE_MODE);
    //s.md("aaa\\abc");
    //s.create_file("aaa\\abc\\acc",WRITE_MODE);
    //printf("%s",s.open_file("aca",READ_MODE) == TRUE_RETURN ? "SUCCESS\n" : "FILUE\n");
    disc.showBuf(2);
    fat.showFATArray();
    disc.close();
    return 0;

}
*/


