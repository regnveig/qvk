#include "vk_api.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VkApi API = VkApi(nullptr);
    // API.getWebToken();
    QString t = "<token>";
    API.SetToken(&t);
    QString Result = API.status_set("screw the driver");
    qDebug() << Result;
    return a.exec();
}
