# qvk

Qt VK API tiny wrapper.

# Usage

## Qt GUI

```cpp
// Create object
VkApi API = VkApi(nullptr);
// Get VK API token
API.getWebToken();
// Do what you want
QString Result = API.status_set("I set this status with qvk!");
qDebug() << Result;
```

## Qt Quick

`main.cpp`:

```cpp
QQmlApplicationEngine engine;
VkApi API = VkApi(nullptr);
API.BindQmlEngine(&engine);
```

QML:

```qml
VkApi.getWebToken()
var response = status_set("I set this status with qvk!")
console.log(response)
```
