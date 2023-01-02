#include "newfiledic.h"
#include "ui_newfiledic.h"

#include <QMessageBox>
NewFileDic::NewFileDic(QWidget *parent,QString fatherPath,int type) :
    QDialog(parent),
    ui(new Ui::NewFileDic),fatherPath(fatherPath),type(type)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(newFD()));
    connect(ui->buttonBox,SIGNAL(rejected()),this,SLOT(cancelFD()));

}

NewFileDic::~NewFileDic()
{
    delete ui;
}

void NewFileDic::newFD() {
    QString lineEditText = ui->lineEdit->text();
    if(lineEditText.length() <= 3 && !lineEditText.isEmpty())
        emit newFileDic(fatherPath,lineEditText,type);
    else {
        QMessageBox::warning(this,"WARNING!","名称不能为空或者长度大于三");
    }
    this->close();
}
void NewFileDic::cancelFD() {
    this->close();
}
