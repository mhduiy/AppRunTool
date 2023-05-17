#include "myconfigdata.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

MyConfigData* MyConfigData::instance = nullptr;

MyConfigData::MyConfigData(QObject *parent)
    : QObject{parent}
{
    init();
}

QVector<APPINFO *> MyConfigData::getAppInfos()
{
    QFile file(CONFIGPATH);
    if(!file.open(QIODevice::ReadOnly)) {
        return {};
    }
    QVector<APPINFO *> result;
    QByteArray jsonInfo = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonInfo);
    QJsonObject dataObj = doc.object();
    QJsonArray appInfoArray = dataObj.value("data").toArray();
    for(QJsonArray::iterator it = appInfoArray.begin(); it!=appInfoArray.end(); it++) {
        if(it->isObject()) {
            QJsonObject jsonObj = it->toObject();
            APPINFO * item = new APPINFO();
            item->appName = jsonObj.value("appName").toString();
            item->appPath = jsonObj.value("appPath").toString();
            item->appOption = jsonObj.value("appOption").toString();
            item->iconPath = jsonObj.value("iconPath").toString();
            result.append(item);
        }
    }
    dataObj.insert("data", appInfoArray);
    doc.setObject(dataObj);
    file.close();
    return result;
}

bool MyConfigData::addAppInfo(const APPINFO &info)
{
    QJsonObject obj;
    obj.insert("appName", info.appName);
    obj.insert("appPath", info.appPath);
    obj.insert("appOption", info.appOption);
    obj.insert("iconPath", info.iconPath);

    QFile file(CONFIGPATH);
    if(!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray jsonInfo = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonInfo);
    QJsonObject dataObj = doc.object();
    QJsonArray appInfoArray = dataObj.value("data").toArray();
    appInfoArray.append(obj);
    dataObj.insert("data", appInfoArray);
    doc.setObject(dataObj);
    file.close();
    file.remove();

    QFile filew(CONFIGPATH);
    if(!filew.open(QIODevice::WriteOnly)) {
        return false;
    }
    filew.write(doc.toJson());
    filew.close();
    return true;
}

bool MyConfigData::modifyAppInfo(const QString appName, const APPINFO &info)
{
    QJsonObject obj;
    obj.insert("appName", info.appName);
    obj.insert("appPath", info.appPath);
    obj.insert("appOption", info.appOption);
    obj.insert("iconPath", info.iconPath);

    QFile file(CONFIGPATH);
    if(!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray jsonInfo = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonInfo);
    QJsonObject dataObj = doc.object();
    QJsonArray appInfoArray = dataObj.value("data").toArray();
    int i = 0;
    for(QJsonArray::iterator it = appInfoArray.begin(); it!=appInfoArray.end(); it++, i++) {
        if(it->toObject().value("appName") == appName) {
            qDebug() << "执行修改";
            appInfoArray.erase(it);
            appInfoArray.insert(i, obj);
            break;
        }
    }
    dataObj.insert("data", appInfoArray);
    doc.setObject(dataObj);
    file.close();
    file.remove();

    QFile filew(CONFIGPATH);
    if(!filew.open(QIODevice::WriteOnly)) {
        return false;
    }
    filew.write(doc.toJson());
    filew.close();
    return true;
}

bool MyConfigData::deleteAppInfo(const QString appName)
{
    QFile file(CONFIGPATH);
    if(!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray jsonInfo = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonInfo);
    QJsonObject dataObj = doc.object();
    QJsonArray appInfoArray = dataObj.value("data").toArray();
    for(QJsonArray::iterator it = appInfoArray.begin(); it!=appInfoArray.end(); it++) {
        if(it->toObject().value("appName") == appName) {
            qDebug() << "执行删除";
            appInfoArray.erase(it);
            break;
        }
    }
    dataObj.insert("data", appInfoArray);
    doc.setObject(dataObj);
    file.close();
    file.remove();

    QFile filew(CONFIGPATH);
    if(!filew.open(QIODevice::WriteOnly)) {
        return false;
    }
    filew.write(doc.toJson());
    filew.close();
    return true;
}

void MyConfigData::init()
{
    qDebug() << CONFIGPATH;
    //创建一个json文件
    if(!QFile::exists(CONFIGPATH)) {
        QFile file(CONFIGPATH);
        file.open(QIODevice::WriteOnly);
        QJsonObject obj;
        QJsonArray arr;
        obj.insert("data", arr);

        QJsonDocument doc;
        doc.setObject(obj);

        file.write(doc.toJson());
        file.close();
    }
}

MyConfigData *MyConfigData::getInstance()
{
    if(MyConfigData::instance == nullptr) {
        MyConfigData::instance = new MyConfigData();
    }
    return MyConfigData::instance;
}
