#include "vk_api.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString* Token = new QString("<token>");
    VkApi API = VkApi(Token, nullptr);
    delete Token;
    QString Result = API.jsonRequest(R"({"method":"account.getProfileInfo","parameters":{}})");
    qDebug() << Result;
    // QML
    // engine.rootContext()->setContextProperty("VkApi", &API);
    return a.exec();
}
