#ifndef VKLOGINWINDOW_H
#define VKLOGINWINDOW_H

#include <QCloseEvent>
#include <QWebEngineView>
#include <QWidget>

class VkLoginWindow : public QWebEngineView
{
    Q_OBJECT
public:
    explicit VkLoginWindow(QWidget *parent = nullptr);
protected:
    void closeEvent(QCloseEvent *event) override;
signals:
    void LoginWindowClosed();
};

#endif // VKLOGINWINDOW_H
