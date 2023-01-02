#include "attributeviewer.h"
#include "ui_attributeviewer.h"

attributeViewer::attributeViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::attributeViewer)
{

    ui->setupUi(this);
}

attributeViewer::~attributeViewer()
{
    delete ui;
}

void attributeViewer::init(QString name,QString type,QString attribute,QString block,QString len) {
    this->setWindowTitle("属性");
    ui->nameLabel->setText(name);
    ui->typeLael->setText(type);
    ui->attributeLabel->setText(attribute);
    ui->blockLabel->setText(block);
    ui->lenLabel->setText(len);
}
