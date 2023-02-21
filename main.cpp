#include "vk_api.h"
#include <QApplication>
#include <QWebEngineView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWebEngineView view;
    view.setUrl("www.google.com");
    view.resize(1024, 750);
    view.show();
    QString* Token = new QString("<token>");
    VkApi API = VkApi(Token, nullptr);
    delete Token;
    QString Result = API.jsonRequest(R"({"method":"account.getProfileInfo","parameters":{}})");
    qDebug() << Result;
    // QML
    // engine.rootContext()->setContextProperty("VkApi", &API);
    return a.exec();
}
