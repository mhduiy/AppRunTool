#include "addappdialog.h"
#include "ui_addappdialog.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>

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
}

void addAppDialog::on_btn_select_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, "选择一个可执行文件");

    QFileInfo fileInfo(path);
    if(ui->ed_appName->text().isEmpty())
        ui->ed_appName->setText(fileInfo.fileName());
    ui->ed_appPath->setText(fileInfo.filePath());
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

    if(!QFile::exists(info.appPath)) {
        QMessageBox::warning(this, "错误", "文件不存在");
        return;
    }
    if(isModify)
        MyConfigData::getInstance()->modifyAppInfo(tName, info);
    else
        MyConfigData::getInstance()->addAppInfo(info);

    accept();
}
