#ifndef LTE_SC_H_1739951617
#define LTE_SC_H_1739951617

#include "communication/config/CLteErrorCode.hpp"
#include "entity/ENTITY_EExpSample.h"
#include "lte.h"

class LTE_SC {
public:
    using ErrorCode = COM::CLteErrorCode::ErrorCode;

    static void Send_SC_ErrorCode(ErrorCode error);

    static void Send_SC_Data(std::shared_ptr<ENTITY::EExpSample> expItem);

    static void Send_SC_Reply(QString data,QString MD5);

    static bool Init(const QString &vid, const QString &pid);
};

#endif  // LTE_SC_H_1739951617
