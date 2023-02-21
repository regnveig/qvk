#ifndef VKRESPONSEHANDLER_H
#define VKRESPONSEHANDLER_H

#include <QJsonDocument>
#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>

class VkResponseHandler : public QObject
{
    Q_OBJECT
public:
    explicit VkResponseHandler(QObject *parent = nullptr);
    QString EscapeJsonString(QString Value);
    QJsonDocument GetData();
public slots:
    void HandleReply(QNetworkReply* Reply);
signals:
    void Ready();
private:
    QJsonDocument Result;
};

#endif // VKRESPONSEHANDLER_H
