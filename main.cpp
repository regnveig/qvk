#include <QApplication>
#include "qvk.h"

int main(int argc, char* argv[]) {
    QApplication App(argc, argv);
    VkApi API;
    API.getWebToken();
    qDebug() << API.account_getProfileInfo();
    return App.exec();
}
