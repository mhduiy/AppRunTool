#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGridLayout>
#include <QMainWindow>
#include <QPoint>
#include <QMenu>
#include "myconfigdata.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum {
    OP_START = 0,
    OP_MODIFY,
    OP_DELETE
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateAppList();

protected:
    virtual void mouseMoveEvent(QMouseEvent *event)override;
    virtual void mousePressEvent(QMouseEvent *event)override;
    virtual void mouseReleaseEvent(QMouseEvent *event)override;

private slots:

    void on_btn_addApp_clicked();

    void on_btn_exit_clicked();
    void showAppMenu(QPoint p);
    void startApp(APPINFO *info);
    void responseAppMenu(QAction *action);
    void setCurInfo(int index);
    void modifyCurInfo();

private:
    Ui::MainWindow *ui;
    QPoint mouseToWindow;
    QGridLayout *appLayout;
    QVector<APPINFO*> appInfors;
    QMenu *appMenu;
    int curIndex;   //当前选择的index
};
#endif // MAINWINDOW_H
