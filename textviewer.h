#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QDialog>
#include <QString>
namespace Ui {
class textViewer;
}

class textViewer : public QDialog
{
    Q_OBJECT

public:
    explicit textViewer(QWidget *parent = nullptr);
    ~textViewer();

private:
    Ui::textViewer *ui;
    QString initText;
public slots:
    void openFile(QString path,QString text);
    void closeFile();

signals:
    void saveFile(QString path,QString text,int st);
};

#endif // TEXTVIEWER_H
