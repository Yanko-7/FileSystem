#ifndef TEST_H
#define TEST_H

#include <QWidget>

namespace Ui {
class test;
}

class test : public QWidget
{
    Q_OBJECT

public:
    explicit test(QWidget *parent = nullptr);
    ~test();

private:
    Ui::test *ui;
};

#endif // TEST_H
