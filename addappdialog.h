#ifndef ADDAPPDIALOG_H
#define ADDAPPDIALOG_H

#include <QDialog>
#include "myconfigdata.h"


namespace Ui {
class addAppDialog;
}

class addAppDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addAppDialog(QWidget *parent = nullptr);
    ~addAppDialog();
    void setData(APPINFO *);

    inline void setModify() {
        isModify = true;
    }
    inline void setAdd() {
        isModify = false;
    }

private slots:
    void on_brn_cancel_clicked();

    void on_btn_enter_clicked();

    void on_btn_selectApp_clicked();

    void on_btn_selectIcon_clicked();

private:
    Ui::addAppDialog *ui;
    bool isModify = false;
    QString tName;
};

#endif // ADDAPPDIALOG_H
