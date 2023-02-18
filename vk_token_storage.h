#ifndef VKTOKENSTORAGE_H
#define VKTOKENSTORAGE_H

#include <QObject>
#include <QString>

class VkTokenStorage : public QObject
{
    Q_OBJECT
public:
    explicit VkTokenStorage(QString* NewToken, QObject *parent = nullptr);
    void SetToken(QString* NewToken);
    void GetToken(QString* Buffer);
private:
    QString Token;
};

#endif // VKTOKENSTORAGE_H
