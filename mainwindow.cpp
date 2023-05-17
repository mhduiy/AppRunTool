#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QLayout>
#include "myappitemwidget.h"
#include <QMouseEvent>
#include "addappdialog.h"
#include <QProcess>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("App启动助手");
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(300, 500);

    appLayout = new QGridLayout();
    ui->appWidget->setLayout(appLayout);

    appMenu = new QMenu(this);
    appMenu->addAction("启动");
    appMenu->addAction("修改");
    appMenu->addAction("删除");

    connect(appMenu, &QMenu::triggered, this, &MainWindow::responseAppMenu);

    QMenu *sysTrayMenu = new QMenu();
    QMenu *sysTrayFastOP = new QMenu();
    sysTrayFastOP->setTitle("快捷功能");
    sysTrayFastOP->addAction("打开系统代理");
    sysTrayFastOP->addAction("关闭系统代理");

    sysTrayMenu->addAction("显示主界面");
    sysTrayMenu->addMenu(sysTrayFastOP);
    sysTrayMenu->addAction("关于");
    sysTrayMenu->addAction("退出");

    QIcon icon("/home/mhduiy/Desktop/AndroidTools/res/AndroidTools.svg");

    sysTrayIcon = new QSystemTrayIcon(this);
    sysTrayIcon->setIcon(QIcon("/home/mhduiy/Desktop/AndroidTools/res/AndroidTools.svg"));
    sysTrayIcon->setContextMenu(sysTrayMenu);
    sysTrayIcon->show();

    connect(sysTrayMenu, &QMenu::triggered, this, &MainWindow::responseSysTrayMenu);
    connect(sysTrayFastOP, &QMenu::triggered, this, &MainWindow::responseSysTrayMenu);
    connect(sysTrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::show);

    QTextStream out(stdout);
    QTextStream err(stderr);
    //全局快捷键设置

    const QKeySequence shortcut("Ctrl+Alt+Z");
    globalShortcut = new QxtGlobalShortcut(shortcut);

    if ( !globalShortcut->isValid() ) {
        err << QString("Error: Failed to set shortcut %1")
            .arg(shortcut.toString()) << endl;
    }

    out << QString("Press shortcut %1 (or CTRL+C to exit)").arg(shortcut.toString()) << endl;

    connect(globalShortcut, &QxtGlobalShortcut::activated,[&]{
        if(this->isHidden())
            this->show();
        else
            this->hide();
    });

    updateAppList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateAppList()
{
    auto ret = MyConfigData::getInstance()->getAppInfos();
    for(APPINFO* it : appInfors) {
        delete it;
    }
    QLayoutItem *child;

    while ((child = appLayout->takeAt(0)) != 0) {
        if(child->widget())
            delete child->widget();
        delete child;
    }

    appInfors.resize(0);
    int i = 0;
    for(auto it : ret) {
        MyAppItemWidget *w = new MyAppItemWidget();
        w->setName(it->appName);
        if(!QFile::exists(it->iconPath)) {  //图标文件不存在
            w->setIcon("/home/mhduiy/Desktop/AndroidTools/res/AndroidTools.svg");
        }
        else {
            w->setIcon(it->iconPath);
        }
        w->setProperty("index", i);
        appLayout->addWidget(w, i / 2, i % 2);
        connect(w, &MyAppItemWidget::rightClicked, this, &MainWindow::showAppMenu);
        connect(w, &MyAppItemWidget::clicked, this, [this, i](){this->setCurInfo(i);});
        connect(w, &MyAppItemWidget::doubleClicked, this, [it, this](){
            this->startApp(it);
        });
        i++;
    }
    appInfors = ret;
}

void MainWindow::on_btn_addApp_clicked()
{
    addAppDialog a;
    int ret = a.exec();
    if(ret == 0) {
        return;
    }
    updateAppList();
}


void MainWindow::on_btn_exit_clicked()
{
    this->hide();
}

void MainWindow::showAppMenu(QPoint p)
{
    appMenu->move(p);
    appMenu->show();
}

void MainWindow::startApp(APPINFO *info)
{
    QStringList args;
    for(const QString &arg : info->appOption.split(' ')) {
        args << arg;
    }
    QProcess::startDetached(info->appPath, args, info->appPath.mid(0, info->appPath.lastIndexOf('/') + 1));
}

void MainWindow::responseAppMenu(QAction *action)
{
    if(action->text() == "启动") {
        startApp(appInfors.value(curIndex));
    }
    else if(action->text() == "修改")  {
        modifyCurInfo();
    }
    else if(action->text() == "删除")  {
        MyConfigData::getInstance()->deleteAppInfo(appInfors.value(curIndex)->appName);
        updateAppList();
    }
}

void MainWindow::responseSysTrayMenu(QAction *action)
{
    qDebug() << action->text();
    if(action->text()== "显示主界面") {
        this->show();
    }
    else if(action->text()== "关于") {
        QMessageBox::information(this, "关于", "作者: mhduiy \n 版本: v1.0");
    }
    else if(action->text()== "退出") {
        exit(0);
    }
    else if(action->text()== "打开系统代理") {
        QString cmd = "gsettings set com.deepin.wrap.gnome.system.proxy mode 'manual'";
        system(cmd.toLocal8Bit());
        APPINFO i;
        i.appPath = "notify-send";
        i.appOption = "-a App启动助手 App启动助手--开启系统代理";
        startApp(&i);
    }
    else if(action->text()== "关闭系统代理") {
        QString cmd = "gsettings set com.deepin.wrap.gnome.system.proxy mode 'none'";
        system(cmd.toLocal8Bit());
        APPINFO i;
        i.appPath = "notify-send";
        i.appOption = "-a App启动助手 App启动助手--关闭系统代理";
        startApp(&i);
    }
}

void MainWindow::setCurInfo(int index)
{
    curIndex = index;
}

void MainWindow::modifyCurInfo()
{
    addAppDialog d;
    d.setModify();
    d.setData(appInfors.value(curIndex));
    int ret = d.exec();
    if(ret == 0) {
        return;
    }
    updateAppList();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);

    QPoint mouseToDeskTop = event->globalPos();
    this->move(mouseToDeskTop - mouseToWindow);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    QPoint windowToDesktop = this->geometry().topLeft();
    QPoint mouseToDeskTop = event->globalPos();
    mouseToWindow = mouseToDeskTop - windowToDesktop;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}
