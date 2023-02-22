QT += gui widgets webenginewidgets network quick

CONFIG += c++17 console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        vk_api.cpp \
        vk_login_window.cpp \
        vk_response_handler.cpp \
        vk_token_storage.cpp

# CONFIG += qmltypes

# QML_IMPORT_NAME = VkApi
# QML_IMPORT_MAJOR_VERSION = 1

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    vk_api.h \
    vk_login_window.h \
    vk_response_handler.h \
    vk_token_storage.h
