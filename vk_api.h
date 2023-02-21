#ifndef VKAPI_H
#define VKAPI_H

#include "vk_token_storage.h"
#include "vk_response_handler.h"

#include <QByteArray>
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

const int C_REQUEST_TIMEOUT = 20000;

class VkApi : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit VkApi(QString* Token, QObject *parent = nullptr);
    int BindQmlEngine(QQmlApplicationEngine* Engine);
    int MakeUrl(QString* Method, QMap<QString, QString>* Parameters, QUrl* Result);
    int Request(QString* Method, QMap<QString, QString>* Parameters, QJsonDocument* Result);
    Q_INVOKABLE QString jsonRequest(QString Command);
private:
    QSslConfiguration ConfigSSL;
    VkTokenStorage* TokenStorage;
};

#endif // VKAPI_H
