#ifndef ATTRIBUTEVIEWER_H
#define ATTRIBUTEVIEWER_H

#include <QDialog>

namespace Ui {
class attributeViewer;
}
//
class attributeViewer : public QDialog
{
    Q_OBJECT

public:
    explicit attributeViewer(QWidget *parent = nullptr);
    ~attributeViewer();

private:
    Ui::attributeViewer *ui;

public slots:
    void init(QString name,QString type,QString attribute,QString block,QString len);
};

#endif // ATTRIBUTEVIEWER_H
