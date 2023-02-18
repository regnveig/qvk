#ifndef VKAPI_H
#define VKAPI_H

#include "vk_token_storage.h"
#include "vk_response_handler.h"

#include <QByteArray>
#include <QEventLoop>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QMap>
#include <QMapIterator>
#include <QObject>
#include <QSsl>
#include <QSslConfiguration>
#include <QString>
#include <QTimer>
#include <QUrl>

const int C_REQUEST_TIMEOUT = 20000;
const char* const C_API_ADDRESS = "https://api.vk.com/method/";
const char* const C_VALUE_API_VERSION = "5.131";
const char* const C_AMPERSAND = "&";
const char* const C_EQUALITY_MARK = "=";
const char* const C_QUESTION_MARK = "?";
const char* const C_KEY_ACCESS_TOKEN = "access_token";
const char* const C_KEY_API_VERSION = "v";

class VkApi : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit VkApi(QString* Token, QObject *parent = nullptr);
    int MakeUrl(const char* Method, QMap<const char*, QString*>* Parameters, QUrl* Result);
    int Request(const char* Method, QMap<const char*, QString*>* Parameters, QJsonDocument *Result);
private:
    QSslConfiguration ConfigSSL;
    VkTokenStorage* TokenStorage;
};

#endif // VKAPI_H
