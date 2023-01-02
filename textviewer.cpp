#include "textviewer.h"
#include "ui_textviewer.h"

#include <QMessageBox>
#include <iostream>
textViewer::textViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::textViewer)
{
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::Dialog);
    connect(this,SIGNAL(rejected()),this,SLOT(closeFile()));
    connect(this,SIGNAL(saveFile(QString,QString,int)),parent,SLOT(saveFile(QString,QString,int)));
    ui->setupUi(this);
}

textViewer::~textViewer()
{
    delete ui;
}

void textViewer::openFile(QString path,QString text) {
    setWindowTitle(path);
    initText = text;
    ui->textEdit->setText(text);
}

void textViewer::closeFile() {
    //std::cout<<ui->textEdit->toPlainText().toStdString()<<std::endl;
    int st = QMessageBox::No;
    if(initText != ui->textEdit->toPlainText()) {
        st = QMessageBox::information(this,"Save", "The context has been changed.Do you want to save?",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    emit saveFile(this->windowTitle(),ui->textEdit->toPlainText(),st);
}
