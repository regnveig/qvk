#include "vk_token_storage.h"

VkTokenStorage::VkTokenStorage(QObject *parent) : QObject{parent} { }

void VkTokenStorage::SetToken(QString* NewToken) {
    this->Token = *NewToken;
}

void VkTokenStorage::GetToken(QString* Buffer) {
    *Buffer = this->Token;
}

bool VkTokenStorage::TokenExists() {
    return (this->Token != QString());
}

void VkTokenStorage::TokenFromWeb(const QUrl& Url) {
    if (Url.fileName() == "blank.html") {
        QString StrUrl = Url.url();
        StrUrl = StrUrl.replace(R"(#)", R"(?)");
        QUrl NewUrl = QUrl(StrUrl);
        QUrlQuery Query = QUrlQuery(NewUrl.query());
        QList<QPair<QString, QString>> QueryItems = Query.queryItems();
        QList<QPair<QString, QString>>::iterator it;
        for (it = QueryItems.begin(); it != QueryItems.end(); ++it) {
            qDebug() << it->first << it->second;
            if (it->first == QString("access_token")) {
                this->Token = it->second;
                emit this->TokenAccepted();
                return;
            }
        }
        throw VkTokenApiException();
    }
}
