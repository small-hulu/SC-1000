#ifndef EREAGENT_H_1721877226
#define EREAGENT_H_1721877226

#include <QString>

#include "Qrcode_EBaseAnalysis.h"

namespace QRCODE {
namespace KeyRea {
const QString base              = "base";
const QString base_codetype     = base + QString("/codetype");
const QString base_reagentstype = base + QString("/reagentstype");
const QString base_proname      = base + QString("/proname");
const QString base_udi          = base + QString("/udi");
const QString base_batchnum     = base + QString("/batchnum");
const QString base_startnum     = base + QString("/startnum");
const QString base_endnum       = base + QString("/endnum");
const QString base_bottleno     = base + QString("/bottleno");
const QString base_producdate   = base + QString("/producdate");
const QString base_validity     = base + QString("/validity");
const QString base_tq           = base + QString("/tq");
const QString base_filling      = base + QString("/filling");

const QString project               = "project";
const QString project_rt            = project + QString("/rt");
const QString project_curvestartcon = project + QString("/curvestartcon");
const QString project_curvestartrlu = project + QString("/curvestartrlu");
const QString project_curveendcon   = project + QString("/curveendcon");
const QString project_curveendrlu   = project + QString("/curveendrlu");
const QString project_curvea        = project + QString("/curvea");
const QString project_curveb        = project + QString("/curveb");
const QString project_curvec        = project + QString("/curvec");
const QString project_curved        = project + QString("/curved");
}  // namespace KeyRea

class EReagent : public EBaseAnalysis {
public:
    using EBaseAnalysis::EBaseAnalysis;
};
}  // namespace QRCODE
#endif  // EREAGENT_H_1721877226
