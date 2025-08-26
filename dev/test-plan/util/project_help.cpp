#include "project_help.h"

#include "communication/COM_Device.hpp"
#include "communication/controller/WatchDog.h"
#include "communication/utility/IniConfig.h"

void help_stop_reagentBin_loop() {
    using namespace COM;
    WatchDog_Guard guard;
    auto&&         ini = IniConfig::instance();
    ini.setValue(KI::enable_reagentBinSpinLoop, 0);

    auto&& dog = WatchDog::instance();
    dog.Reset();
}
