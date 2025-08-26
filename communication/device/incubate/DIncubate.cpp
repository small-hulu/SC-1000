#include "DIncubate.h"

#include <QDebug>

namespace COM {
DIncubate::DIncubate() : QObject() {
    setObjectName("温裕盘");
}

QString DIncubate::to_string() const {
    QString msg;
    QDebug  bug(&msg);
    return msg;
}

void DIncubate::Reset_cacheFixtureParams() {
    m_cu.Reset_cacheFixtureParams();
}

RetCode DIncubate::Init_muc() {
    qCritical() << objectName() << __func__ << "todo";
    return RetCode::TODO;
}
}  // namespace COM
