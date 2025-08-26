#ifndef ECALIBRATION_H_1721877308
#define ECALIBRATION_H_1721877308

#include <QString>

#include "Qrcode_EBaseAnalysis.h"

namespace QRCODE {
namespace KeyCal {
const QString base            = "base";
const QString base_codetype   = base + QString("/codetype");
const QString base_proname    = base + QString("/proname");
const QString base_udi        = base + QString("/udi");
const QString base_batchnum   = base + QString("/batchnum");
const QString base_startnum   = base + QString("/startnum");
const QString base_endnum     = base + QString("/endnum");
const QString base_bottleno   = base + QString("/bottleno");
const QString base_producdate = base + QString("/producdate");
const QString base_validity   = base + QString("/validity");
const QString base_rt         = base + QString("/rt");
const QString dilution        = base + QString("/dilution");     // 是否稀释
const QString repetitions     = base + QString("/repetitions");  // 重复次数

const QString calconf                 = "calconf";
const QString calconf_caltype         = calconf + QString("/caltype");
const QString calconf_reagentbatchnum = calconf + QString("/reagentbatchnum");
const QString calconf_calconvlist     = calconf + QString("/calconvlist");

}  // namespace KeyCal

class ECalibration : public EBaseAnalysis {
public:
    using EBaseAnalysis::EBaseAnalysis;
};
}  // namespace QRCODE

#endif  // ECALIBRATION_H_1721877308
