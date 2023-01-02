#include "mainwindow.h"
#include <QApplication>
#include <control_main.h>
#include <QFile>
using namespace std;
QByteArray readTextFile(const QString &file_path) {
  QFile input_file(file_path);
  QByteArray input_data;

  if (input_file.open(QIODevice::Text | QIODevice::Unbuffered | QIODevice::ReadOnly)) {
    input_data = input_file.readAll();
    input_file.close();
    return input_data;
  }
  else {
    return QByteArray();
  }
}
int main(int argc, char *argv[])
{
    //QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough); //第二个参数用来控制缩放策略。详细解释可以按F1看帮助文档。
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
