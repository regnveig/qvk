#include "vk_token_storage.h"

VkTokenStorage::VkTokenStorage(QString* NewToken, QObject *parent) : QObject{parent} {
    this->SetToken(NewToken);
}

void VkTokenStorage::SetToken(QString* NewToken) {
    this->Token = *NewToken;
}

void VkTokenStorage::GetToken(QString* Buffer) {
    *Buffer = this->Token;
}
