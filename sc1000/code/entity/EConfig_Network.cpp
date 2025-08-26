#include "EConfig_Network.h"

namespace ENTITY {

namespace KeyNetwork {
const QString Network       = "Network";
const QString LISConfig     = "LISConfig";
const QString autoUpload    = "autoUpload";
const QString direction     = "direction";
const QString networkConfig = "networkConfig";
const QString serverIP      = "serverIP";
const QString serverPort    = "serverPort";
const QString serialConfig  = "serialConfig";
const QString serialPort    = "serialPort";
}  // namespace KeyNetwork

Network::Network() {
    namespace Key        = KeyNetwork;
    json                 = m_ojson[Key::Network].toObject();
    auto lis             = json[Key::LISConfig].toObject();
    lisConfig.direction  = lis[Key::autoUpload].toInt();
    lisConfig.autoUpload = lis[Key::autoUpload].toInt();

    auto network             = json[Key::networkConfig].toObject();
    networkConfig.serverIP   = network[Key::serverIP].toString();
    networkConfig.serverPort = network[Key::serverPort].toString();

    auto serial             = json[Key::serialConfig].toObject();
    serialConfig.serialPort = serial[Key::serialPort].toString();
}

bool Network::Save() {
    namespace Key = KeyNetwork;

    QJsonObject lis;
    lis[Key::direction]  = lisConfig.direction;
    lis[Key::autoUpload] = lisConfig.autoUpload;

    QJsonObject network;
    network[Key::serverIP]   = networkConfig.serverIP;
    network[Key::serverPort] = networkConfig.serverPort;

    QJsonObject serial;
    serial[Key::serialPort] = serialConfig.serialPort;

    QJsonObject json;
    json[Key::LISConfig]     = lis;
    json[Key::networkConfig] = network;
    json[Key::serialConfig]  = serial;
    m_ojson[Key::Network]    = json;
    return IConfig::Save();
}

}  // namespace ENTITY
