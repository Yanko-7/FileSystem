#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model_fat.h"
#include "model_readdisc.h"
#include "newfiledic.h"
#include "attributeViewer.h"

#include <QStringList>
#include <QString>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include "model_filedic.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),disc(new Model_ReadDisc("test.txt")),fat(new Model_FAT(disc)),OFT(new Model_OpenedFileTable()),
    fileDic(new Model_FileDic(fat,disc,OFT)),fileOperation(new Model_FileOperation(disc,fat,OFT,fileDic)),tver(nullptr)
{
    ui->setupUi(this);
    setCentralWidget(ui->centralWidget);
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;
    effect->setOffset(-2, 5);
    effect->setColor(QColor("#6a9bd8"));
    effect->setBlurRadius(15);
    QGraphicsDropShadowEffect* effect1 = new QGraphicsDropShadowEffect;
    effect1->setOffset(5, -2);
    effect1->setColor(QColor("#B1B2FF"));
    effect1->setBlurRadius(15);
    QGraphicsDropShadowEffect* effect2 = new QGraphicsDropShadowEffect;
    effect2->setOffset(-2, 5);
    effect2->setColor(QColor("#6a9bd8"));
    effect2->setBlurRadius(15);
    QGraphicsDropShadowEffect* effect3 = new QGraphicsDropShadowEffect;
    effect3->setOffset(-2, 5);
    effect3->setColor(QColor("#fbc2eb"));
    effect3->setBlurRadius(15);
    QGraphicsDropShadowEffect* effect4 = new QGraphicsDropShadowEffect;
    effect4->setOffset(-2, 5);
    effect4->setColor(QColor("#B1B2FF"));
    effect4->setBlurRadius(15);
    //effect2=effect;effect3=effect;effect4=effect;
   //background-color: rgba(247, 251, 255, 0.5);
   ui->label->setGraphicsEffect(effect);
   //ui->listWidget->setGraphicsEffect(effect1);
   //ui->tableWidget->setGraphicsEffect(effect2);
   //ui->tableWidget_2->setGraphicsEffect(effect3);
   ui->treeWidget->setGraphicsEffect(effect4);
    setTableViewFAT();
    setTableViewOFT();
    setTreeViewFileDic(rootItem,fileDic);
    setTableWidgtFileDic(fileDic);
    //signals and slots
}

MainWindow::~MainWindow()
{
    disc->writeBuf2Disc();  //save
    delete ui;
}


void MainWindow::setTableViewFAT() {                   //FAT Table

    ui->tableWidget->setRowCount(128);//设置128行
    ui->tableWidget->setColumnCount(2);//两列
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"BLOCK"<<"NEXT");//table表头
    //ui->tableWidget->setVerticalHeaderLabels(QStringList()<<"BLOCK"<<"NEXT");
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);//可以选择多行？貌似没用
    ui->tableWidget->verticalHeader()->setVisible(false);//关闭垂直的表序
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置均匀列宽
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    for(int i = 0;i < LEN;++i) {//读入保存的文件
        int s = disc->readByte(i/64,i%64);
        //int s=EMPTY;
        QString status;
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString("%1").arg(i)));//block 第一列设置
        ui->tableWidget->item(i,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        if(i <= 2) s=END;
        if(s == END) status = QString("%1").arg(s);
        else if(s == EMPTY) status = QString("%1").arg(s);
        else if(s == DAMAGE) status = QString("%1").arg(s);
        else status = QString("%1").arg(s);
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString("%1").arg(status)));//第二列设置
        ui->tableWidget->item(i,1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
    ui->tableWidget->show();
}

void MainWindow::setTableViewOFT() {                          //Opened File Table 下方已打开文件块
    ui->tableWidget_2->setRowCount(5);
    ui->tableWidget_2->setColumnCount(9);
    //ui->tableWidget_2->setAlternatingRowColors(true);//隔行颜色
    ui->tableWidget_2->setShowGrid(false);
    //ui->tableWidget_2->setVerticalHeaderLabels(QStringList()<<"path"<<"attribute"<<"head_block"<<"length"<<"mode"<<"块指针_读read_block"<<"字节指针_读read_offset"<<"块指针_写"<<"字节指针_写");
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<"文件路径"<<"文件属性"<<"起始盘块"<<"长度"<<"操作类型"<<"读_块地址"<<"读_块内地址"<<"写_块地址"<<"写_块内地址");
    //ui->tableWidget_2->setHorizontalHeaderLabels(QStringList()<<"path"<<"attribute"<<"head_block"<<"length"<<"mode"<<"read_block"<<"read_offset"<<"write_block"<<"write_offset");
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_2->verticalHeader()->setVisible(false);
    ui->tableWidget_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);//改成与内容相适应
    //ui->tableWidget_2->horizontalHeader()->setStretchLastSection(1);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->show();
}

void MainWindow::setTreeViewFileDic(QTreeWidgetItem * parent,Model_FileDic * p) {  // QTreewidget FileDic
    if(p == fileDic) {
        ui->treeWidget->clear();
        rootItem = new QTreeWidgetItem(ui->treeWidget,QStringList()<<"root");
        rootItem->setData(0,Qt::UserRole,QString(""));
        parent = rootItem;
    }
    if(p->isExsit()) {
        QTreeWidgetItem * item;
        for(int i = 0;i < ATT_LEN;++i) {
            p->checkAndAllocate(i,p);
            if(p->getChild(i)->isDic()) {//是目录项
                QString name = QString().fromStdString(p->getChild(i)->getName());//设置treewidget数据
                item = new QTreeWidgetItem(parent,QStringList()<<name);
                parent->addChild(item);
                item->setIcon(0,QIcon(":/r/folder.png"));
                item->setData(0,Qt::UserRole,QString().fromStdString(p->getChild(i)->getPathName()));
                setTreeViewFileDic(item,p->getChild(i));
            }
            else{//下面可以实现文件也在treewidget中出现

            }
        }
    }
    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)),this,SLOT(slotTreeWidget(QTreeWidgetItem*,int)));//信号与槽绑定，实现双击打开
}


void MainWindow::setTableWidgtFileDic(Model_FileDic *p) {//p为当前指定的块                 //List the File/Dic in ListWidget
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setSpacing(10);
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setIconSize(QSize(70, 70));
    ui->listWidget->setMovement(QListWidget::Static);
    //ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    QListWidgetItem *item;
    for(int i = 0;i < ATT_LEN;++i) { //给中上块添加元素
        p->checkAndAllocate(i,p);
        if(p->getChild(i)->isExsit()) {
            QString name = QString().fromStdString(p->getChild(i)->getName());
            if(p->getChild(i)->isDic()) item = new QListWidgetItem(QIcon(":/r/folder.png"),name);
            else item = new QListWidgetItem(QIcon(":/r/document.png"),name);
            item->setData(Qt::UserRole,QString().fromStdString((p->getChild(i)->getPathName()))); //save data to item
            ui->listWidget->addItem(item);
        }
    }
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //menu->add
    //设置菜单
    menu = new QMenu;
    QAction * openAction = new QAction("&Open...",menu);
    QAction * delAction = new QAction("&Delete...",menu);
    QAction * viewAttributeAction = new QAction("&Attrbute..",menu);
    QAction * newFileAction = new QAction("&New File...",menu);
    QAction * newDicAction = new QAction("&New Dic...",menu);
    openAction->setIcon(QIcon(":/r/open.png"));
    delAction->setIcon(QIcon(":/r/del.png"));
    viewAttributeAction->setIcon(QIcon(":/r/att.png"));
    newFileAction->setIcon(QIcon(":/r/new.png"));
    newDicAction->setIcon(QIcon(":/r/new.png"));
    menu->addAction(openAction);
    menu->addAction(newFileAction);
    menu->addAction(newDicAction);
    menu->addAction(delAction);
    menu->addAction(viewAttributeAction);
    //连接信号函数和槽函数
    connect(openAction,SIGNAL(triggered(bool)),this,SLOT(open()));
    connect(menu,SIGNAL(triggered(QAction *)),this,SLOT(newFileDic(QAction *)));
    connect(delAction,SIGNAL(triggered(bool)),this,SLOT(del()));
    connect(viewAttributeAction,SIGNAL(triggered(bool)),this,SLOT(viewAttribute()));


    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem *)),this,SLOT(slotListWidgetItemClicked(QListWidgetItem *)));//单机
    connect(ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(slotListWidget(QListWidgetItem*)));//双击listwidget里的元素时候
    connect(ui->listWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showListWidgetMenu()));//给右键菜单发信号，设定右键某些开着，某些关着
}


void MainWindow::slotListWidget(QListWidgetItem * item) {            // slot function to receive a signal from ui->ListWidget->DoubleClicked(QListWidgetItem *)
    Model_FileDic * p = fileDic->getPointer(item->data(Qt::UserRole).toString().toStdString());//fileDic为全局的当前选定的一个block块, 根据所选对象存的绝对路径去找到一个盘起始块
    if(p != nullptr) {
        if(p->isDic()) {//是文件夹相当于打开，重画list
            ui->listWidget->clear();
            ui->label->setText(QString().fromStdString(p->getPathName()));
            for(int i = 0;i < ATT_LEN;++i) {
                p->checkAndAllocate(i,p);
                if(p->getChild(i)->isExsit()) {
                    QString name = QString().fromStdString(p->getChild(i)->getName());
                    if(p->getChild(i)->isDic()) item = new QListWidgetItem(QIcon(":/r/folder.png"),name);
                    else item = new QListWidgetItem(QIcon(":/r/document.png"),name);
                    item->setData(Qt::UserRole,QString().fromStdString(p->getChild(i)->getPathName()));  //save data to item
                    cout<<"printfp: "<<p<<' '<<"childp"<<' '<<p->getChild(i)<<' '<<i<<' '<<p->getChild(i)->getPathName()<<endl;
                    //for(int i=0;i<8;i++)cout<<p->getChild(i)->
                    //cout<<"debug"<<p->getChild(i)->getPathName()<<endl;
                    ui->listWidget->addItem(item);
                }
            }
        }
        else {
            //open file
            int st = fileOperation->open_file(p->getPathName(),READ_MODE);
            if(st == ALREAD_OPEN) {
                QMessageBox::information(this,QString("info"),QString("File already open"));//如果打开，提示
                return;
            }
            fileOperation->read_file(p->getPathName(),MAX_BUFFER_LEN);//将内容读入到对象里
            string text = fileOperation->toString();
            updateOFTTable();
            if(tver != nullptr) disconnect(this,SIGNAL(openFile(QString,QString)),tver,SLOT(openFile(QString,QString)));
            tver = new textViewer(this);
            connect(this,SIGNAL(openFile(QString,QString)),tver,SLOT(openFile(QString,QString)));           //不要放在textViewer
            emit openFile(QString().fromStdString(p->getPathName()),QString().fromStdString(text));//这里不是很懂
            tver->show();
        }
    }
}


void MainWindow::slotTreeWidget(QTreeWidgetItem * item,int col) {//slot function to receive a signal from ui->TreeWidget->DoubleClicked(QTreeWidgetItem *)
    Model_FileDic * p = fileDic->getPointer(item->data(0,Qt::UserRole).toString().toStdString());
    QString name = item->text(col);
    if(name == "root") {      //根目录做特殊处理
        QListWidgetItem * i = new QListWidgetItem(name);
        i->setData(Qt::UserRole,QString(""));
        slotListWidget(i);
    }
    //cout<<name.toStdString()<<"  --- "<<endl;
    if(p!=nullptr) {
        if(p->isDic()) {
            QListWidgetItem * i = new QListWidgetItem(name);
            i->setData(Qt::UserRole,item->data(0,Qt::UserRole));
            slotListWidget(i);//顺便更新list
        }
    }

    //cout<<"slotTreeWidget: "<<item->text(col).toStdString()<<"  colCount "<<item->columnCount()<<endl;
}

void MainWindow::showListWidgetMenu() {         //展示 Right Click Menu
    menu->hideTearOffMenu();
    bool condition = ui->listWidget->itemAt(ui->listWidget->mapFromGlobal(QCursor::pos())) != nullptr;
    QList<QAction *> list =  menu->actions();
    for(QAction* action:list) {// 这里是让某些键亮着，某些黑着
        if(action->text() == QString("&New File...") || action->text() == "&New Dic...") action->setEnabled(!condition);
        else action->setEnabled(condition);
    }
    menu->exec(QCursor::pos());
}


void MainWindow::updateOFTTable() {//更新已打开表
    ui->tableWidget_2->clearContents();
    for(int i = 0;i < MAXNUM_OPENEDFILETABLE;++i) {
        if(!OFT->isUsed(i)) continue;
        string name = OFT->getName(i);
        ui->tableWidget_2->setItem(i,0,new QTableWidgetItem(QString().fromStdString(name)));
        ui->tableWidget_2->setItem(i,1,new QTableWidgetItem(QString("%1").arg(OFT->getOFTItem(i,&file::getAtt))));
        ui->tableWidget_2->setItem(i,2,new QTableWidgetItem(QString("%1").arg(OFT->getOFTItem(i,&file::getBlockId))));
        ui->tableWidget_2->setItem(i,3,new QTableWidgetItem(QString("%1").arg(OFT->getOFTItem(i,&file::getLen))));
        QString Flag = OFT->getOFTItem(i,&file::getFlag) == READ_MODE ? QString("0") : QString("1");
        ui->tableWidget_2->setItem(i,4,new QTableWidgetItem(Flag));
        ui->tableWidget_2->setItem(i,5,new QTableWidgetItem(QString("%1").arg(OFT->getOFTItem(i,&file::getRpBlockId))));
        ui->tableWidget_2->setItem(i,6,new QTableWidgetItem(QString("%1").arg(OFT->getOFTItem(i,&file::getRpByteId))));
        ui->tableWidget_2->setItem(i,7,new QTableWidgetItem(QString("%1").arg(OFT->getOFTItem(i,&file::getWpBlockId))));
        ui->tableWidget_2->setItem(i,8,new QTableWidgetItem(QString("%1").arg(OFT->getOFTItem(i,&file::getWpByteId))));
        for(int j=0;j<=8;j++)ui->tableWidget_2->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}

void MainWindow::updateFATTable() {
    for(int i = 0;i < LEN;++i) {
        int s = disc->readByte(i/64,i%64);
        QString status;
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString("%1").arg(i)));
        ui->tableWidget->item(i,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        if(i <= 2) s=END;
        if(s == END) status = QString("%1").arg(s);
        else if(s == EMPTY) status = QString("%1").arg(s);
        else if(s == DAMAGE) status = QString("%1").arg(s);
        else status = QString("%1").arg(s);
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString("%1").arg(status)));
        ui->tableWidget->item(i,1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}

void MainWindow::saveFile(QString path,QString text,int st) {
    string sPath = path.toStdString();
    string sText = text.toStdString();
    //cout<<sPath<<"  "<<sText<<endl;
    fileOperation->close_file(sPath);
    updateOFTTable();
    if(st == QMessageBox::Yes) {
        int len = static_cast<int>(sText.length());
        for(int i = 0;i < len;++i)
            fileOperation->writeBuffer[i] = static_cast<int>(sText.at(static_cast<unsigned>(i)));
        int st = fileOperation->write_file(sPath,fileOperation->writeBuffer,len);
        updateOFTTable();
        if(st == TRUE_RETURN)
            QMessageBox::information(this,QString("info"),QString("Successfully Save!"));
        fileOperation->close_file(sPath);
        updateOFTTable();
        updateFATTable();
    }
    //
}


void MainWindow::newFileDic(QAction * action) {
    NewFileDic * nfd;
    if(action->text() == "&New File...") nfd = new NewFileDic(this,ui->label->text(),1);
    else if(action->text() == "&New Dic...") nfd = new NewFileDic(this,ui->label->text(),0);
    else return;
    connect(nfd,SIGNAL(newFileDic(QString,QString,int)),this,SLOT(newFileDic2(QString,QString,int)));
    nfd->show();
}
void MainWindow::newFileDic2(QString fatherPath,QString name,int type) {
    QString path = fatherPath +  (fatherPath.isEmpty() ? "":"\\") + name;
    string spath = path.toStdString();
    cout<<spath<<" "<<type<<endl;
    if(type) fileOperation->create_file(spath,WRITE_MODE);
    else fileOperation->md(spath);
    updateFATTable();
    setTreeViewFileDic(nullptr,fileDic);
    QListWidgetItem * item =  new QListWidgetItem();
    item->setData(Qt::UserRole,ui->label->text());
    slotListWidget(item);
}

void MainWindow::del() {
    QList<QListWidgetItem *> list = ui->listWidget->selectedItems();
    for(QListWidgetItem * item:list) {
        string path = item->data(Qt::UserRole).toString().toStdString();
        cout<<path<<endl;
    }
    cout<<"yes"<<endl;
    for(QListWidgetItem * item:list) {
        string path = item->data(Qt::UserRole).toString().toStdString();
        if(fileDic->isFile(path)) {
            cout<<"isFile"<<endl;
            if(OFT->isOpen(path)) {
                QMessageBox::warning(this,"Warnig","文件已经打开,操作失败");
                return;
            }else {
                fileOperation->delete_file(path);
                //if()
            }
        }else{
            if(!fileDic->isLeaf(path)) {
                QMessageBox::warning(this,"Waring","文件夹非空,操作失败");
            }
            else{
                fileOperation->rd(path);
            }
        }
    }
    updateFATTable();
    setTreeViewFileDic(nullptr,fileDic);
    QListWidgetItem * item = new QListWidgetItem();
    cout<<ui->label->text().toStdString()<<endl;
    item->setData(Qt::UserRole,ui->label->text());
    slotListWidget(item);
}

void MainWindow::open() {
    cout<<"open"<<endl;
    QList<QListWidgetItem *> list = ui->listWidget->selectedItems();
    for(QListWidgetItem * item:list) {
        slotListWidget(item);
    }
}

void MainWindow::viewAttribute() {
    QList<QListWidgetItem *> list = ui->listWidget->selectedItems();
    for(QListWidgetItem * item:list) {
        QString path = item->data(Qt::UserRole).toString();
        attributeViewer * aver = new attributeViewer(this);
        int isDic = fileDic->isDic(path.toStdString());
        QString name = item->text();
        QString type = isDic ? QString("文件夹") : QString("文本文件");
        QString attribute = isDic ? QString("保留"):QString("读写文件");
        QString block = QString::number(fileDic->getBlockId(path.toStdString()));
        QString len = QString::number(fileDic->getLen(path.toStdString()));
        connect(this,SIGNAL(initAttViewer(QString,QString,QString,QString,QString)),aver,SLOT(init(QString,QString,QString,QString,QString)));
        emit initAttViewer(name,type,attribute,block,len);
        aver->show();
    }
}

void MainWindow::slotListWidgetItemClicked(QListWidgetItem * item) {
    /*
    string path = item->data(Qt::UserRole).toString().toStdString();
    int Id = fileDic->getBlockId(path);
    cout<<Id<<endl;
    ui->tableWidget->itemDelegate(new QModelIndex());
    ui->tableWidget->at(Id)->setSelected(true);
    */
}
