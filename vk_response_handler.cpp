#include "vk_response_handler.h"

VkResponseHandler::VkResponseHandler(QObject *parent) : QObject{parent} { }

QJsonDocument VkResponseHandler::GetData() { return this->Result; }

QString VkResponseHandler::EscapeJsonString(QString Value) {
    return Value.replace(R"(\)", R"(\\)").replace(R"(")", R"(\")");
}

void VkResponseHandler::HandleReply(QNetworkReply* Reply) {
    QByteArray Bytes;
    QString ErrorMessage = QString();
    if (Reply->error() == QNetworkReply::NoError) {
        Bytes = Reply->readAll();
        int StatusCode = Reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (StatusCode != 200) {
            ErrorMessage.append(R"({"http_status_code": )");
            ErrorMessage.append(QString::number(StatusCode, 'f', 0));
            ErrorMessage.append(R"(})");
            Bytes = ErrorMessage.toUtf8();
        }
    } else {
        ErrorMessage.append(R"({"network_status_code":)");
        ErrorMessage.append(QString::number(Reply->error(), 'f', 0));
        ErrorMessage.append(R"(,"network_status_desc":")");
        ErrorMessage.append(this->EscapeJsonString(Reply->errorString()));
        ErrorMessage.append(R"("})");
        Bytes = ErrorMessage.toUtf8();
    }
    Reply->deleteLater();
    QJsonDocument Document;
    Document = QJsonDocument::fromJson(Bytes);
    if (Document == QJsonDocument()) {
        ErrorMessage.append(R"({"internal_error":"json_error","api_response":")");
        ErrorMessage.append(this->EscapeJsonString(QString(Bytes)));
        ErrorMessage.append(R"("})");
        Bytes = ErrorMessage.toUtf8();
        Document = QJsonDocument::fromJson(Bytes);
    }
    this->Result = Document;
    emit this->Ready();
}
