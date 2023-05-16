#include "mainwindow.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QScreen>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QLoggingCategory::defaultCategory()->setEnabled(QtDebugMsg, true);  //允许输出调试信息

    MainWindow w;
    QSize size = QGuiApplication::primaryScreen()->size();
    w.move(100, size.height() - w.height() - 100);
    w.show();
    return a.exec();
}
