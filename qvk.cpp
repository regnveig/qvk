#include "qvk.h"
#include "vk_response_handler.h"
#include "vk_login_window.h"

VkApi::VkApi(QObject *parent): QNetworkAccessManager{parent} {
    this->ConfigSSL.setDefaultConfiguration(QSslConfiguration::defaultConfiguration());
    this->ConfigSSL.setProtocol(QSsl::TlsV1_2);
    this->TokenStorage = new VkTokenStorage(this);
}

int VkApi::BindQmlEngine(QQmlApplicationEngine* Engine) {
    Engine->rootContext()->setContextProperty("VkApi", this);
    return 0;
}

int VkApi::getWebToken() {
    QUrl TokenUrl = QUrl("https://oauth.vk.com/authorize?client_id=2685278&scope=1073737727&redirect_uri=https://oauth.vk.com/blank.html&display=page&response_type=token&revoke=1");
    VkLoginWindow* View = new VkLoginWindow();
    QEventLoop* Loop = new QEventLoop(this);
    connect(View, SIGNAL(urlChanged(QUrl)), this->TokenStorage, SLOT(TokenFromWeb(QUrl)));
    connect(this->TokenStorage, SIGNAL(TokenAccepted()), View, SLOT(stop()));
    connect(this->TokenStorage, SIGNAL(TokenAccepted()), Loop, SLOT(quit()));
    connect(View, SIGNAL(LoginWindowClosed()), Loop, SLOT(quit()));
    View->setUrl(TokenUrl);
    View->resize(800, 600);
    View->show();
    Loop->exec();
    delete View;
    delete Loop;
    return this->TokenStorage->TokenExists() ? 0 : 1;
}

int VkApi::SetToken(QString* Token) {
    this->TokenStorage->SetToken(Token);
    return 0;
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
    if (!this->TokenStorage->TokenExists()) {
        *Result = QJsonDocument::fromJson(QString(R"({"internal_error":"no_token"})").toUtf8());
        return 1;
    }
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

QString VkApi::account_getBanned(int Offset, int Count) {
    QString Method = QString("account.getBanned");
    QMap<QString, QString> Parameters = {
        { QString("offset"), QString::number(Offset, 'f', 0) },
        { QString("count"), QString::number(Count, 'f', 0) },
    };
    QJsonDocument Result;
    this->Request(&Method, &Parameters, &Result);
    return QString(Result.toJson(QJsonDocument::Compact));
}

QString VkApi::account_getCounters() {
    QString Method = QString("account.getCounters");
    QMap<QString, QString> Parameters;
    QJsonDocument Result;
    this->Request(&Method, &Parameters, &Result);
    return QString(Result.toJson(QJsonDocument::Compact));
}

QString VkApi::account_getInfo() {
    QString Method = QString("account.getInfo");
    QMap<QString, QString> Parameters;
    QJsonDocument Result;
    this->Request(&Method, &Parameters, &Result);
    return QString(Result.toJson(QJsonDocument::Compact));
}

QString VkApi::account_getProfileInfo() {
    QString Method = QString("account.getProfileInfo");
    QMap<QString, QString> Parameters;
    QJsonDocument Result;
    this->Request(&Method, &Parameters, &Result);
    return QString(Result.toJson(QJsonDocument::Compact));
}

QString VkApi::status_get(int UserGroupID) {
    QString Method = QString("status.get");
    QMap<QString, QString> Parameters;
    if (UserGroupID != 0) {
        Parameters = {
            { QString(UserGroupID < 0 ? "group_id" : "user_id"), QString::number(qFabs(UserGroupID), 'f', 0) }
        };
    }
    QJsonDocument Result;
    this->Request(&Method, &Parameters, &Result);
    return QString(Result.toJson(QJsonDocument::Compact));
}

QString VkApi::status_set(QString Text, int GroupID) {
    QString Method = QString("status.set");
    QMap<QString, QString> Parameters = {
        { QString("text"), Text }
    };
    if (GroupID != 0) {
        Parameters.insert(QString("group_id"), QString::number(qFabs(GroupID), 'f', 0));
    }
    QJsonDocument Result;
    this->Request(&Method, &Parameters, &Result);
    return QString(Result.toJson(QJsonDocument::Compact));
}
