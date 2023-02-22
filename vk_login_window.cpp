#include "vk_login_window.h"

VkLoginWindow::VkLoginWindow(QWidget *parent) : QWebEngineView{parent} { }

void VkLoginWindow::closeEvent(QCloseEvent *event) {
    QWebEngineView::closeEvent(event);
    emit this->LoginWindowClosed();
}
