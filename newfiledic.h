#ifndef NEWFILEDIC_H
#define NEWFILEDIC_H

#include <QDialog>

namespace Ui {
class NewFileDic;
}

class NewFileDic : public QDialog
{
    Q_OBJECT

public:
    explicit NewFileDic(QWidget *parent = nullptr,QString fatherPath = "",int type = 0);
    ~NewFileDic();


signals:
    void newFileDic(QString fatherPath,QString name,int type);
public slots:
    void newFD();
    void cancelFD();

private:
    Ui::NewFileDic *ui;
    QString fatherPath;
    int type;
};

#endif // NEWFILEDIC_H
