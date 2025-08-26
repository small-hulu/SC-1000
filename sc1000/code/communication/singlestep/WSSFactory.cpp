#include "WSSFactory.h"

#include "SingleStepGripper.h"
#include "SingleStepMagneticSeparation.h"
#include "SingleStepMixing.h"
#include "SingleStepPhotoelectric.h"
#include "SingleStepPipetteNeedle.h"
#include "SingleStepReagentBin.h"
#include "SingleStepThermostat.h"

namespace COM::SINGLEWSTEP {

QList<QPair<QString, QWidget *>> Create_ssWidgetList(QWidget *parent) {
    QList<QPair<QString, QWidget *>> widgetList;
    widgetList << QPair{"夹爪", new SingleStepGripper(parent)};
    widgetList << QPair{"移液针", new SingleStepPipetteNeedle(parent)};
    widgetList << QPair{"试剂仓", new SingleStepReagentBin(parent)};
    widgetList << QPair{"磁分离", new SingleStepMagneticSeparation(parent)};
    widgetList << QPair{"分杯混匀", new SingleStepMixing(parent)};
    widgetList << QPair{"光电", new SingleStepPhotoelectric(parent)};
    widgetList << QPair{"温控", new SingleStepThermostat(parent)};
    return widgetList;
}

}  // namespace COM::SINGLEWSTEP
