#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <model_fat.h>
#include <model_readdisc.h>
#include <model_fileoperation.h>
#include <model_openedfiletable.h>
#include <model_stringoperation.h>
#include <model_presentworkingdic.h>

#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QAction>
#include <QMenu>
#include "textviewer.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setTableViewFAT();
    void setTableViewOFT();
    void setTreeViewFileDic(QTreeWidgetItem * parent,Model_FileDic * p);
    void setTableWidgtFileDic(Model_FileDic *p);
    void updateOFTTable();
    void updateFATTable();
private:
    Ui::MainWindow *ui;
    Model_ReadDisc * disc;
    Model_FAT * fat;
    Model_OpenedFileTable * OFT;
    Model_FileDic *fileDic;
    Model_FileOperation * fileOperation;
    textViewer * tver;
    QMenu * menu;

    QTreeWidgetItem * rootItem;
public slots:
    void slotListWidget(QListWidgetItem * item);
    void slotTreeWidget(QTreeWidgetItem * item,int col);
    void showListWidgetMenu();
    void saveFile(QString path,QString text,int st);
    void newFileDic2(QString fatherPath,QString name,int type);
    void newFileDic(QAction * action);
    void del();
    void open();
    void viewAttribute();
    void slotListWidgetItemClicked(QListWidgetItem * item);
signals:
    void openFile(QString path,QString text);
    void initAttViewer(QString name,QString type,QString attribute,QString block,QString len);
};

#endif // MAINWINDOW_H
