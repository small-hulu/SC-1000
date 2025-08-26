#include "MagneticSeparationACapacity.h"

namespace COM::WFIXTURE {
MagneticSeparationACapacity::MagneticSeparationACapacity() : MagneticSeparation() {
    OperationPanel::Sudoku matrx = {
        {0, 0, 0},  //
        {0, 0, 0},  //
        {0, 0, 0}   //
    };
    Set_Sudoku(matrx);

    const QString deviceName = KJ::MagneticSeparation;
    QJsonObject   jsonObj    = JsonConfig::instance().Get_json();
    QJsonObject   deviceFix  = jsonObj[deviceName].toObject();
    int           num        = deviceFix[KJ::aSubstrateCapacity].toInt();
    set_inputNum(num);
}

bool MagneticSeparationACapacity::Save_fixtureParams() {
    auto&& device                 = Device::instance();
    device.fix.aSubstrateCapacity = get_inputNum();

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE
