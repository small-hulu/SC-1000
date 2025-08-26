#include "PipetteNeedleReagentBinZ1.h"

#include "communication/device/reagentBin/DReagentBin.h"

namespace COM::WFIXTURE {

template class PipetteNeedleReagentBinZ1<4>;
template class PipetteNeedleReagentBinZ1<5>;
template class PipetteNeedleReagentBinZ1<6>;

template <size_t BinIdx>
PipetteNeedleReagentBinZ1<BinIdx>::PipetteNeedleReagentBinZ1() : PipetteNeedle() {
    /// 该部分需要将移液针和试剂仓同时操作
    OperationPanel::Sudoku matrx = {
        {1, 1, 1},  //
        {0, 0, 0},  //
        {1, 1, 1}   //
    };
    Set_Sudoku(matrx);

    /// 移液针转动
    {
        auto&& needle = Device::instance();
        m_params      = needle.GetInfo_to_ReagentBin(BinIdx).params;
        o_params      = needle.GetInfo_to_ReagentBin(BinIdx).params;
        needle.Set_Config(Device::Mode_YPosition, m_params);
        needle.Exec_sequencedTask();
        m_params = needle.Get_mcuParams();
    }

    /// 试剂仓跟着转动
    {
        auto&& reagentBin = DReagentBin::instance();
        reagent_params    = reagentBin.GetInfo_backHole(BinIdx);
        reagentBin.Set_Config(DReagentBin::Mode_Position, reagent_params);
        reagentBin.Exec_sequencedTask();
    }
}

template <size_t BinIdx>
bool PipetteNeedleReagentBinZ1<BinIdx>::Save_fixtureParams() {
    /// 保存移液针
    /// 注意这里是 Z1
    {
        auto&& needle                                = Device::instance();
        needle.fix.ReagentBinList[BinIdx].yPosition  = m_params.yPosition;
        needle.fix.ReagentBinList[BinIdx].zPosition1 = m_params.zPosition;

        needle.Save_fileFixtureParams();
        needle.Reset_cacheFixtureParams();
        needle.Init_muc();
    }

    /// 保存试剂仓
    {
        auto&& reagentBin               = DReagentBin::instance();
        reagentBin.fix.backList[BinIdx] = reagent_params.position;

        reagentBin.Save_fileFixtureParams();
        reagentBin.Reset_cacheFixtureParams();
        reagentBin.Init_muc();
    }

    return true;
}
template <size_t BinIdx>
bool PipetteNeedleReagentBinZ1<BinIdx>::Save_fixtureParams_Without_Z() {
    /// 保存移液针
    /// 注意这里是 Z1
    {
        auto&& needle                                = Device::instance();
        m_params.zPosition = o_params.zPosition;
        needle.fix.ReagentBinList[BinIdx].yPosition  = m_params.yPosition;
        needle.fix.ReagentBinList[BinIdx].zPosition1 = m_params.zPosition;

        needle.Save_fileFixtureParams();
        needle.Reset_cacheFixtureParams();
        needle.Init_muc();
    }

    /// 保存试剂仓
    {
        auto&& reagentBin               = DReagentBin::instance();
        reagentBin.fix.backList[BinIdx] = reagent_params.position;

        reagentBin.Save_fileFixtureParams();
        reagentBin.Reset_cacheFixtureParams();
        reagentBin.Init_muc();
    }

    return true;
}
template <size_t BinIdx>
void PipetteNeedleReagentBinZ1<BinIdx>::op_clockwise() {
    int num                  = get_inputNum();
    reagent_params.position -= num;
    exec_fixtureMove_ReagentBin();
}

template <size_t BinIdx>
void PipetteNeedleReagentBinZ1<BinIdx>::op_anticlockwise() {
    int num                  = get_inputNum();
    reagent_params.position += num;
    exec_fixtureMove_ReagentBin();
}
}  // namespace COM::WFIXTURE
