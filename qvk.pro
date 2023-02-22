TEMPLATE = lib
#SOURCES += main.cpp
SOURCES += qvk.cpp vk_login_window.cpp vk_response_handler.cpp vk_token_storage.cpp
HEADERS += qvk.h qvk_global.h vk_login_window.h vk_response_handler.h vk_token_storage.h
DEFINES += QVK_LIBRARY
QT += core gui widgets webenginewidgets network quick
CONFIG += c++17 qmltypes
QML_IMPORT_NAME = VkApi
QML_IMPORT_MAJOR_VERSION = 1
