#include "addappdialog.h"
#include "ui_addappdialog.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QTextCodec>

addAppDialog::addAppDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addAppDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("添加App");
}

addAppDialog::~addAppDialog()
{
    delete ui;
}

void addAppDialog::setData(APPINFO *info)
{
    ui->ed_appName->setText(info->appName);
    tName = info->appName;
    ui->ed_appPath->setText(info->appPath);
    ui->ed_appOption->setText(info->appOption);
    ui->ed_iconPath->setText(info->iconPath);
}

void addAppDialog::on_brn_cancel_clicked()
{
    reject();
}


void addAppDialog::on_btn_enter_clicked()
{
    APPINFO info;
    info.appName = ui->ed_appName->text();
    info.appPath = ui->ed_appPath->text();
    info.appOption = ui->ed_appOption->text();
    info.iconPath = ui->ed_iconPath->text();

    if(!QFile::exists(info.appPath)) {
        QMessageBox::warning(this, "错误", "文件不存在");
        return;
    }

    if(!QFile::exists(info.iconPath)) {
        int ret = QMessageBox::warning(this, "错误", "图标文件不存在，将使用默认图标", QMessageBox::Ok|QMessageBox::Cancel);
        qDebug() << ret;
        if(ret != QMessageBox::Ok) {
            return;
        }
    }
    if(isModify)
        MyConfigData::getInstance()->modifyAppInfo(tName, info);
    else
        MyConfigData::getInstance()->addAppInfo(info);

    accept();
}

void addAppDialog::on_btn_selectApp_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "选择一个可执行文件");
    if(path.isEmpty()) {
        return;
    }
    QFileInfo fileInfo(path);
    if(fileInfo.suffix() == "desktop") { //desktop文件
        QSettings set(path, QSettings::IniFormat);
        set.setIniCodec(QTextCodec::codecForName("utf-8"));

        QString name = set.value("Desktop Entry/Name[zh_CN]").toString();
        if(name.isEmpty()) {
            name =  set.value("Desktop Entry/Name").toString();
        }
        QString exec = set.value("Desktop Entry/Exec").toString();
        QString iconPath =  set.value("Desktop Entry/Icon").toString();
        if(!QFile::exists(iconPath)) {
            QString t = iconPath;
            iconPath = "/usr/share/icons/hicolor/scalable/apps/" + iconPath;
        }
        if(!QFile::exists(iconPath)) {
            iconPath = "";
        }

        ui->ed_appName->setText(name);
        ui->ed_appPath->setText(exec.split(' ').value(0));
        ui->ed_iconPath->setText(iconPath);
    }
    else {
        if(ui->ed_appName->text().isEmpty())
            ui->ed_appName->setText(fileInfo.fileName());
        ui->ed_appPath->setText(fileInfo.filePath());
    }
}

void addAppDialog::on_btn_selectIcon_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "选择一个icon");
    if(path.isEmpty()) {
        return;
    }
    QFileInfo fileInfo(path);
    ui->ed_iconPath->setText(fileInfo.filePath());
}

