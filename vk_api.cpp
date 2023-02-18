#include "vk_api.h"

VkApi::VkApi(QString* Token, QObject *parent): QNetworkAccessManager{parent} {
    this->ConfigSSL.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
    this->ConfigSSL.setProtocol(QSsl::TlsV1_2);
    this->TokenStorage = new VkTokenStorage(Token);
}

int VkApi::MakeUrl(const char* Method, QMap<const char*, QString*>* Parameters, QUrl* Result) {
    QString* Buffer = new QString(C_API_ADDRESS);
    Buffer->append(Method);
    Buffer->append(C_QUESTION_MARK);
    QMapIterator<const char*, QString*> it(*Parameters);
    while (it.hasNext()) {
        it.next();
        Buffer->append(it.key());
        Buffer->append(C_EQUALITY_MARK);
        QByteArray* Value = new QByteArray(QUrl::toPercentEncoding(*(it.value())));
        Buffer->append(*Value);
        delete Value;
        Buffer->append(C_AMPERSAND);
    }
    Buffer->append(C_KEY_API_VERSION);
    Buffer->append(C_EQUALITY_MARK);
    Buffer->append(C_VALUE_API_VERSION);
    Buffer->append(C_AMPERSAND);
    Buffer->append(C_KEY_ACCESS_TOKEN);
    Buffer->append(C_EQUALITY_MARK);
    QString* Token = new QString();
    this->TokenStorage->GetToken(Token);
    Buffer->append(*Token);
    delete Token;
    *Result = QUrl(*Buffer);
    delete Buffer;
    return 0;
}

int VkApi::Request(const char* Method, QMap<const char*, QString*>* Parameters, QJsonDocument* Result) {
    QUrl* Url = new QUrl();
    this->MakeUrl(Method, Parameters, Url);
    QNetworkRequest* Req = new QNetworkRequest();
    Req->setSslConfiguration(this->ConfigSSL);
    Req->setUrl(*Url);

    QTimer* Timer = new QTimer(this);
    Timer->setSingleShot(true);
    QEventLoop* Loop = new QEventLoop(this);
    VkResponseHandler* Handler = new VkResponseHandler(this);
    connect(this, SIGNAL(finished(QNetworkReply*)), Handler, SLOT(HandleReply(QNetworkReply*)));
    connect(Handler, SIGNAL(Ready()), Loop, SLOT(quit()));
    connect(Timer, SIGNAL(timeout()), Loop, SLOT(quit()));
    this->get(*Req);
    delete Req;
    Timer->start(C_REQUEST_TIMEOUT);
    Loop->exec();

    if(Timer->isActive()) {
        Handler->GetData(Result);
    } else {
        QString* ErrorMessage = new QString("\"internal_error\": \"timeout\"");
        *Result = QJsonDocument::fromJson(QByteArray(ErrorMessage->toUtf8()));
        delete ErrorMessage;
    }
    delete Timer;
    delete Loop;
    delete Handler;
    return 0;
}
