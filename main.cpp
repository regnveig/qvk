#include "vk_api.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString* Token = new QString("<token>");
    VkApi API = VkApi(Token, nullptr);
    delete Token;
    QMap<const char*, QString*>* Parameters = new QMap<const char*, QString*>();
    QJsonDocument Result;
    API.Request("account.getProfileInfo", Parameters, &Result);
    qDebug() << Result;
    for (auto& Value : *Parameters) {
        delete Value;
    }
    delete Parameters;
    return a.exec();
}
