/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       Qrcode_EQuality.h
 * @date       2024-11-28
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief      File Description
[base]
codetype=4
qcname=PGII
udi=89789489
batchnum=20241128005
bottleno=1
producdate=2024-11-28
validity=50
dilution=0

[qci]
proname=PGII
targetlist=5,50
sdlist=0.5,5
 *****************************************************************************/
#ifndef EQUALITY_H_1721877388
#define EQUALITY_H_1721877388

#include <QString>

#include "Qrcode_EBaseAnalysis.h"

namespace QRCODE {

namespace KeyQua {
const QString base            = "base";
const QString base_codetype   = base + QString("/codetype");
const QString base_qcname     = base + QString("/qcname");
const QString base_udi        = base + QString("/udi");
const QString base_batchnum   = base + QString("/batchnum");
const QString base_bottleno   = base + QString("/bottleno");
const QString base_producdate = base + QString("/producdate");
const QString base_validity   = base + QString("/validity");

const QString qci            = "qci";
const QString qci_proname    = qci + QString("/proname");
const QString qci_targetlist = qci + QString("/targetlist");  // list
const QString qci_sdlist     = qci + QString("/sdlist");      // list
}  // namespace KeyQua

class EQuality : public EBaseAnalysis {
public:
    using EBaseAnalysis::EBaseAnalysis;
};
}  // namespace QRCODE

#endif  // EQUALITY_H_1721877388
