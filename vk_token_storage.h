#ifndef VKTOKENSTORAGE_H
#define VKTOKENSTORAGE_H

#include <QDebug>
#include <QException>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

class VkTokenApiException : public QException { };

class VkTokenStorage : public QObject
{
    Q_OBJECT
public:
    explicit VkTokenStorage(QObject *parent = nullptr);
    void SetToken(QString* NewToken);
    void GetToken(QString* Buffer);
    bool TokenExists();
public slots:
    void TokenFromWeb(const QUrl& Url);
signals:
    void TokenAccepted();
private:
    QString Token;
};

#endif // VKTOKENSTORAGE_H
