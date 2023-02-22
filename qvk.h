#ifndef QVK_H
#define QVK_H

#include "qvk_global.h"
#include "vk_token_storage.h"

#include <QByteArray>
#include <QDebug>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QMap>
#include <QMapIterator>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include <QSsl>
#include <QSslConfiguration>
#include <QString>
#include <QTimer>
#include <QUrl>
#include <QWebEngineView>
#include <QtMath>

const int C_REQUEST_TIMEOUT = 20000;

class QVK_EXPORT VkApi : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit VkApi(QObject *parent = nullptr);
    int BindQmlEngine(QQmlApplicationEngine* Engine);
    Q_INVOKABLE int getWebToken();
    int SetToken(QString* Token);
    int MakeUrl(QString* Method, QMap<QString, QString>* Parameters, QUrl* Result);
    int Request(QString* Method, QMap<QString, QString>* Parameters, QJsonDocument* Result);
    Q_INVOKABLE QString jsonRequest(QString Command);
    // METHODS
    // account
    Q_INVOKABLE QString account_getBanned(int Offset, int Count);
    Q_INVOKABLE QString account_getCounters();
    Q_INVOKABLE QString account_getInfo();
    Q_INVOKABLE QString account_getProfileInfo();
    // status
    Q_INVOKABLE QString status_get(int UserGroupID = 0);
    Q_INVOKABLE QString status_set(QString Text, int GroupID = 0);
private:
    QSslConfiguration ConfigSSL;
    VkTokenStorage* TokenStorage;
};

#endif // QVK_H
