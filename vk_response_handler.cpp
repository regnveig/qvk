#include "vk_response_handler.h"

VkResponseHandler::VkResponseHandler(QObject *parent) : QObject{parent} { }

void VkResponseHandler::GetData(QJsonDocument* Document) {
    *Document = this->Result;
}

void VkResponseHandler::HandleReply(QNetworkReply* Reply) {
    QByteArray* Bytes = new QByteArray();
    if (Reply->error() == QNetworkReply::NoError) {
        *Bytes = Reply->readAll();
        int statusCode = Reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode != 200) {
            QString* ErrorMessage = new QString();
            ErrorMessage->append("{\"http_status_code\": ");
            ErrorMessage->append(QString::number(statusCode));
            ErrorMessage->append("}");
            *Bytes = QByteArray(ErrorMessage->toUtf8());
            delete ErrorMessage;
        }
    } else {
        QString* ErrorMessage = new QString();
        ErrorMessage->append("{\"network_status_code\": ");
        ErrorMessage->append(QString::number(Reply->error()));
        ErrorMessage->append(", \"network_status_desc\": \"");
        ErrorMessage->append(Reply->errorString().replace("\\", "\\\\").replace("\"", "\\\""));
        ErrorMessage->append("\"}");
        *Bytes = QByteArray(ErrorMessage->toUtf8());
        delete ErrorMessage;
    }
    Reply->deleteLater();
    QJsonDocument* Document = new QJsonDocument();
    *Document = QJsonDocument::fromJson(*Bytes);
    if (*Document == QJsonDocument()) {
        QString* ErrorMessage = new QString();
        ErrorMessage->append("{\"internal_error\": \"json_error\", \"api_response\": \"");
        ErrorMessage->append(QString(*Bytes).replace("\\", "\\\\").replace("\"", "\\\""));
        ErrorMessage->append("\"}");
        *Bytes = QByteArray(ErrorMessage->toUtf8());
        delete ErrorMessage;
        *Document = QJsonDocument::fromJson(*Bytes);
    }
    delete Bytes;
    this->Result = *Document;
    emit this->Ready();
    delete Document;
}
