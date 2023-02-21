#include "vk_api.h"

VkApi::VkApi(QString* Token, QObject *parent): QNetworkAccessManager{parent} {
    this->ConfigSSL.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
    this->ConfigSSL.setProtocol(QSsl::TlsV1_2);
    this->TokenStorage = new VkTokenStorage(Token);
}

int VkApi::MakeUrl(QString* Method, QMap<QString, QString>* Parameters, QUrl* Result) {
    QString Buffer = QString();
    Buffer.append("https://api.vk.com/method/");
    Buffer.append(*Method);
    Buffer.append("?");
    QMapIterator<QString, QString> it(*Parameters);
    while (it.hasNext()) {
        it.next();
        Buffer.append(it.key());
        Buffer.append("=");
        Buffer.append(QUrl::toPercentEncoding(it.value()));
        Buffer.append("&");
    }
    Buffer.append("v=5.131&access_token=");
    QString Token;
    this->TokenStorage->GetToken(&Token);
    Buffer.append(Token);
    *Result = QUrl(Buffer);
    return 0;
}

int VkApi::Request(QString* Method, QMap<QString, QString>* Parameters, QJsonDocument* Result) {
    QUrl* Url = new QUrl();
    this->MakeUrl(Method, Parameters, Url);
    QNetworkRequest* Req = new QNetworkRequest();
    Req->setSslConfiguration(this->ConfigSSL);
    Req->setUrl(*Url);
    delete Url;
    QTimer* Timer = new QTimer(this);
    QEventLoop* Loop = new QEventLoop(this);
    VkResponseHandler* Handler = new VkResponseHandler(this);
    Timer->setSingleShot(true);
    connect(this, SIGNAL(finished(QNetworkReply*)), Handler, SLOT(HandleReply(QNetworkReply*)));
    connect(Handler, SIGNAL(Ready()), Loop, SLOT(quit()));
    connect(Timer, SIGNAL(timeout()), Loop, SLOT(quit()));
    this->get(*Req);
    delete Req;
    Timer->start(C_REQUEST_TIMEOUT);
    Loop->exec();
    if(Timer->isActive()) {
        *Result = Handler->GetData();
    } else {
        QString ErrorMessage = QString(R"({"internal_error":"timeout"})");
        *Result = QJsonDocument::fromJson(ErrorMessage.toUtf8());
    }
    delete Timer;
    Timer = nullptr;
    delete Loop;
    Loop = nullptr;
    delete Handler;
    Handler = nullptr;
    return 0;
}

QString VkApi::jsonRequest(QString Command) {
    QString KeyMethod = QString("method");
    QString KeyParameters = QString("parameters");
    QJsonDocument CommandDocument = QJsonDocument::fromJson(Command.toLocal8Bit());
    if (!CommandDocument.isObject()) return QString(R"({"internal_error":"command_not_an_object"})");
    QJsonObject CommandObject = CommandDocument.object();
    if (!CommandObject.contains(KeyMethod)) return QString(R"({"internal_error":"no_method_string"})");
    if (!CommandObject.value(KeyMethod).isString()) return QString(R"({"internal_error":"method_is_not_a_string"})");
    QString Method = CommandObject.value(KeyMethod).toString();
    if (!CommandObject.contains(KeyParameters)) return QString(R"({"internal_error":"no_parameters_object"})");
    if (!CommandObject.value(KeyParameters).isObject()) return QString(R"({"internal_error":"parameters_is_not_an_object"})");
    QJsonObject Parameters = CommandObject.value(KeyParameters).toObject();
    QMap<QString, QString> MapParameters;
    QJsonObject::Iterator ParmIt(Parameters.begin());
    QJsonObject::Iterator ParmEnd(Parameters.end());
    while (ParmIt != ParmEnd) {
        QString Key = ParmIt.key();
        QJsonValue Value = ParmIt.value();
        if (!Value.isString()) return QString(R"({"internal_error":"parameter_value_is_not_a_string"})");
        MapParameters.insert(Key, Value.toString());
        ParmIt++;
    }
    QJsonDocument Result;
    this->Request(&Method, &MapParameters, &Result);
    return QString(Result.toJson(QJsonDocument::Compact));
}
