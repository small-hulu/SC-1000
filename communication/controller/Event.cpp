#include "Event.h"

#include <QDebug>

#include "../device/photoelectric/DPhotoelectric.h"
#include "log/LOG.h"
#include "../utility/IniConfig.h"

namespace COM {
Event::Event(QObject *parent) : QObject(parent) {
    connect(&DPhotoelectric::instance(), &DPhotoelectric::signal_DetectValue, this,
            [](int value) {
                double rate = IniConfig::instance()[KI::photoelectric_valRate].toDouble();
                qDebug()<<"系数："<<rate<<"  "<<"原始值："<<value<<endl;
                qDebug()<<value * rate<<endl;
                //sLog_Photoelectric() << value * rate;
            });
}
}  // namespace COM
