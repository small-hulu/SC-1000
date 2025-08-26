#ifndef ECONSUMABLE_H_1721876899
#define ECONSUMABLE_H_1721876899

#include <QByteArray>
#include <QString>

#include "Qrcode_EBaseAnalysis.h"

namespace QRCODE {

namespace KeyCon {
const QString base            = "base";
const QString base_codetype   = base + QString("/codetype");
const QString base_consty     = base + QString("/consty");
const QString base_udi        = base + QString("/udi");
const QString base_batchnum   = base + QString("/batchnum");
const QString base_startnum   = base + QString("/startnum");
const QString base_endnum     = base + QString("/endnum");
const QString base_bottleno   = base + QString("/bottleno");
const QString base_producdate = base + QString("/producdate");
const QString base_validity   = base + QString("/validity");
const QString base_tq         = base + QString("/tq");
const QString base_packspec   = base + QString("/packspec");
}  // namespace KeyCon

class EConsumable : public EBaseAnalysis {
public:
    using EBaseAnalysis::EBaseAnalysis;

public:
    int Get_consumableType();
};
}  // namespace QRCODE

#endif  // ECONSUMABLE_H
