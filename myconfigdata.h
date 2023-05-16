#ifndef MYCONFIGDATA_H
#define MYCONFIGDATA_H

#include <QObject>
#include <QDir>
#include <QStandardPaths>
const QString CONFIGPATH = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + "appData.json";

struct APPINFO {
    QString appName;
    QString appPath;
    QString appOption;
};

class MyConfigData : public QObject
{
    Q_OBJECT
public:
    explicit MyConfigData(QObject *parent = nullptr);
    QVector<APPINFO*> getAppInfos();
    bool addAppInfo(const APPINFO &info);
    bool modifyAppInfo(const QString appName, const APPINFO &info);   //bug
    bool deleteAppInfo(const QString appName);

    void init();

    static MyConfigData *getInstance();

signals:

private:
    static MyConfigData *instance;
};

#endif // MYCONFIGDATA_H
