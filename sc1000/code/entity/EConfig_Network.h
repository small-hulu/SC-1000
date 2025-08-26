#ifndef NETWORK_H_1725689977
#define NETWORK_H_1725689977

#include "EInterface_IConfig.h"

namespace ENTITY {

class Network : public IConfig {
public:
    struct LISConfig {
        int autoUpload = 0;
        int direction  = 0;
    } lisConfig;
    struct NetworkConfig {
        QString serverIP;
        QString serverPort;
    } networkConfig;
    struct SerialConfig {
        QString serialPort;
    } serialConfig;

public:
    Network();
    virtual bool Save() override;
};
}  // namespace ENTITY
#endif  // NETWORK_H_1725689977
