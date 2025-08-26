#include "WFactoryFix.h"

#include <QDebug>

#include "WOperationPanel.h"
#include "gripper/GripperCup.h"
#include "gripper/GripperDilute.h"
#include "gripper/GripperIncubateA.h"
#include "gripper/GripperIncubateB.h"
#include "gripper/GripperIncubateC.h"
#include "gripper/GripperLoad.h"
#include "gripper/GripperMagneticSeparation.h"
#include "gripper/GripperMixing.h"
#include "gripper/GripperPhotoelectric.h"
#include "gripper/GripperRangeMax.h"
#include "gripper/GripperRangeMin.h"
#include "gripper/GripperThrow.h"
#include "magneticSeparation/MagneticSeparationACapacity.h"
#include "magneticSeparation/MagneticSeparationTray.h"
#include "magneticSeparation/MagneticSeparationZDrainDeep.h"
#include "magneticSeparation/MagneticSeparationZMixHeight.h"
#include "mixing/FixCupRangeMax.h"
#include "mixing/FixCupRangeMin.h"
#include "photoelectric/Photoelectric.h"
#include "photoelectric/PhotoelectricBCapacity.h"
#include "photoelectric/PhotoelectricRangeMin.h"
#include "pipetteNeedle/PipetteNeedleDilute.h"
#include "pipetteNeedle/PipetteNeedleLoad.h"
#include "pipetteNeedle/PipetteNeedleReagentBin.h"
#include "pipetteNeedle/PipetteNeedleReagentBinZ1.h"
#include "pipetteNeedle/PipetteNeedleWash.h"
#include "reagentBin/ReagentBinBack.h"
#include "reagentBin/ReagentBinFront.h"
#include "thermostat/FixThermostatTempIncubate.h"
#include "thermostat/FixThermostatTempPhotoelectric.h"
#include "thermostat/FixThermostatTempReagentBinLid.h"

namespace COM::WFIXTURE {

namespace Key {
const QString gripper                    = "1 夹爪";
const QString gripper_Cup                = "1. 分杯位";
const QString gripper_Load               = "1. 上料位";
const QString gripper_Dilute             = "1. 稀释位";
const QString gripper_MagneticSeparation = "1. 磁分离位";
const QString gripper_Mixing             = "1. 混匀位";
const QString gripper_Photoelectric      = "1. 光电位";
const QString gripper_Throw              = "1. 抛料位";
const QString gripper_RangeMin           = "1. 关闭量";
const QString gripper_RangeMax           = "1. 张开量";
const QString gripper_IncubateA          = "1. 温裕盘点A";
const QString gripper_IncubateB          = "1. 温裕盘点B";
const QString gripper_IncubateC          = "1. 温裕盘点C";

const QString pipetteNeedle                = "2 移液针";
const QString pipetteNeedle_Wash           = "2. 清洗位";
const QString pipetteNeedle_Load           = "2. 上料位";
const QString pipetteNeedle_Dilute         = "2. 稀释位";
const QString pipetteNeedle_ReagentBin_0   = "2. 试剂位1";
const QString pipetteNeedle_ReagentBin_1   = "2. 试剂位2";
const QString pipetteNeedle_ReagentBin_2   = "2. 试剂位3";
const QString pipetteNeedle_ReagentBin_3   = "2. 试剂位4";
const QString pipetteNeedle_ReagentBin_4   = "2. 样本位1";
const QString pipetteNeedle_ReagentBin_5   = "2. 样本位2";
const QString pipetteNeedle_ReagentBin_6   = "2. 样本位3";
const QString pipetteNeedle_ReagentBinZ1_4 = "2. 样本位1 (备选)";
const QString pipetteNeedle_ReagentBinZ1_5 = "2. 样本位2 (备选)";
const QString pipetteNeedle_ReagentBinZ1_6 = "2. 样本位3 (备选)";

const QString reagentBin         = "3 试剂仓";
const QString reagentBin_Back_0  = "3. (里)试剂位1";
const QString reagentBin_Back_1  = "3. (里)试剂位2";
const QString reagentBin_Back_2  = "3. (里)试剂位3";
const QString reagentBin_Back_3  = "3. (里)试剂位4";
const QString reagentBin_Back_4  = "3. (里)样本位1";
const QString reagentBin_Back_5  = "3. (里)样本位2";
const QString reagentBin_Back_6  = "3. (里)样本位3";
const QString reagentBin_Front_0 = "3. (外)试剂位1";
const QString reagentBin_Front_1 = "3. (外)试剂位2";
const QString reagentBin_Front_2 = "3. (外)试剂位3";
const QString reagentBin_Front_3 = "3. (外)试剂位4";
const QString reagentBin_Front_4 = "3. (外)样本位1";
const QString reagentBin_Front_5 = "3. (外)样本位2";
const QString reagentBin_Front_6 = "3. (外)样本位3";

const QString magneticSeparation            = "4 磁分离";
const QString magneticSeparation_tray       = "4. 转盘孔位";
const QString magneticSeparation_aCapacity  = "4. 底物A容量";
const QString magneticSeparation_ZDrainDeep = "4. 排液深度";
const QString magneticSeparation_ZMixHeight = "4. 混匀深度";

const QString photoelectric                = "5 光电仓";
const QString photoelectric_bPhotoelectric = "5 底物B容量";
const QString photoelectric_rangeMin       = "5 检测位";

const QString mixing_cup          = "6. 分杯";
const QString mixing_cup_rangeMin = "6. 分杯滑道位";
const QString mixing_cup_rangeMax = "6. 分杯推出位";

const QString thermostat                   = "7. 温控模块";
const QString thermostat_tempIncubate      = "7. 温裕盘";
const QString thermostat_tempPhotoelectric = "7. 光电组件";
const QString thermostat_tempReagentBinLid = "7. 试剂仓盖";
}  // namespace Key

void FactoryFix::Init_tree(QTreeWidget *tree) {
    tree->clear();
    QTreeWidgetItem *title = tree->headerItem();
    title->setText(0, "");

    QMap<QString, QList<QString>> mp;
    mp[Key::gripper] << Key::gripper_Cup;
    mp[Key::gripper] << Key::gripper_Load;
    mp[Key::gripper] << Key::gripper_Dilute;
    mp[Key::gripper] << Key::gripper_MagneticSeparation;
    mp[Key::gripper] << Key::gripper_Mixing;
    mp[Key::gripper] << Key::gripper_Photoelectric;
    mp[Key::gripper] << Key::gripper_Throw;
    mp[Key::gripper] << Key::gripper_RangeMin;
    mp[Key::gripper] << Key::gripper_RangeMax;
    mp[Key::gripper] << Key::gripper_IncubateA;
    mp[Key::gripper] << Key::gripper_IncubateB;
    mp[Key::gripper] << Key::gripper_IncubateC;

    mp[Key::pipetteNeedle] << Key::pipetteNeedle_Wash;
    mp[Key::pipetteNeedle] << Key::pipetteNeedle_Load;
    mp[Key::pipetteNeedle] << Key::pipetteNeedle_Dilute;
    mp[Key::pipetteNeedle] << Key::pipetteNeedle_ReagentBin_0;
    mp[Key::pipetteNeedle] << Key::pipetteNeedle_ReagentBin_1;
    mp[Key::pipetteNeedle] << Key::pipetteNeedle_ReagentBin_2;
    mp[Key::pipetteNeedle] << Key::pipetteNeedle_ReagentBin_3;
    mp[Key::pipetteNeedle] << Key::pipetteNeedle_ReagentBin_4;
    mp[Key::pipetteNeedle] << Key::pipetteNeedle_ReagentBin_5;
    mp[Key::pipetteNeedle] << Key::pipetteNeedle_ReagentBin_6;
    mp[Key::pipetteNeedle] << Key::pipetteNeedle_ReagentBinZ1_4;
    mp[Key::pipetteNeedle] << Key::pipetteNeedle_ReagentBinZ1_5;
    mp[Key::pipetteNeedle] << Key::pipetteNeedle_ReagentBinZ1_6;

    mp[Key::reagentBin] << Key::reagentBin_Back_0;
    mp[Key::reagentBin] << Key::reagentBin_Back_1;
    mp[Key::reagentBin] << Key::reagentBin_Back_2;
    mp[Key::reagentBin] << Key::reagentBin_Back_3;
    mp[Key::reagentBin] << Key::reagentBin_Back_4;
    mp[Key::reagentBin] << Key::reagentBin_Back_5;
    mp[Key::reagentBin] << Key::reagentBin_Back_6;
    mp[Key::reagentBin] << Key::reagentBin_Front_0;
    mp[Key::reagentBin] << Key::reagentBin_Front_1;
    mp[Key::reagentBin] << Key::reagentBin_Front_2;
    mp[Key::reagentBin] << Key::reagentBin_Front_3;
    mp[Key::reagentBin] << Key::reagentBin_Front_4;
    mp[Key::reagentBin] << Key::reagentBin_Front_5;
    mp[Key::reagentBin] << Key::reagentBin_Front_6;

    mp[Key::magneticSeparation] << Key::magneticSeparation_tray;
    mp[Key::magneticSeparation] << Key::magneticSeparation_aCapacity;
    mp[Key::magneticSeparation] << Key::magneticSeparation_ZDrainDeep;
    mp[Key::magneticSeparation] << Key::magneticSeparation_ZMixHeight;

    mp[Key::photoelectric] << Key::photoelectric_bPhotoelectric;
    mp[Key::photoelectric] << Key::photoelectric_rangeMin;

    mp[Key::mixing_cup] << Key::mixing_cup_rangeMin;
    mp[Key::mixing_cup] << Key::mixing_cup_rangeMax;

    mp[Key::thermostat] << Key::thermostat_tempIncubate;
    mp[Key::thermostat] << Key::thermostat_tempPhotoelectric;
    mp[Key::thermostat] << Key::thermostat_tempReagentBinLid;

    for (auto it = mp.begin(); it != mp.end(); it += 1) {
        auto             key  = it.key();
        auto             arr  = it.value();
        QTreeWidgetItem *item = new QTreeWidgetItem(tree);
        item->setText(0, key);
        for (int i = 0; i < arr.size(); i += 1) {
            QTreeWidgetItem *sub = new QTreeWidgetItem(item);
            sub->setText(0, arr[i]);
        }
    }

    tree->collapseAll();
    tree->setFocusPolicy(Qt::NoFocus);
}

/**
 * 其实这里制表也没有太大的必要
 * 太多动态数据的缓存不太适合制表
 * 但制表又不能存类型，可以写成模板元编程
 * 但是不利于团队协作开发
 */
OperationPanel *FactoryFix::Create_opeartorWidget(const QString &title) {
    using namespace Key;

    /// 夹爪
    {
        if (title == gripper_Cup) return new GripperCup{};
        if (title == gripper_Load) return new GripperLoad{};
        if (title == gripper_Dilute) return new GripperDilute{};
        if (title == gripper_MagneticSeparation) return new GripperMagneticSeparation{};
        if (title == gripper_Mixing) return new GripperMixing{};
        if (title == gripper_Photoelectric) return new GripperPhotoelectric{};
        if (title == gripper_Throw) return new GripperThrow{};
        if (title == gripper_RangeMin) return new GripperRangeMin{};
        if (title == gripper_RangeMax) return new GripperRangeMax{};
        if (title == gripper_IncubateA) return new GripperIncubateA{};
        if (title == gripper_IncubateB) return new GripperIncubateB{};
        if (title == gripper_IncubateC) return new GripperIncubateC{};
    }

    /// 移液针
    {
        if (title == pipetteNeedle_Wash) return new PipetteNeedleWash{};
        if (title == pipetteNeedle_Load) return new PipetteNeedleLoad{};
        if (title == pipetteNeedle_Dilute) return new PipetteNeedleDilute{};
        if (title == pipetteNeedle_ReagentBin_0) return new PipetteNeedleReagentBin<0>{};
        if (title == pipetteNeedle_ReagentBin_1) return new PipetteNeedleReagentBin<1>{};
        if (title == pipetteNeedle_ReagentBin_2) return new PipetteNeedleReagentBin<2>{};
        if (title == pipetteNeedle_ReagentBin_3) return new PipetteNeedleReagentBin<3>{};
        if (title == pipetteNeedle_ReagentBin_4) return new PipetteNeedleReagentBin<4>{};
        if (title == pipetteNeedle_ReagentBin_5) return new PipetteNeedleReagentBin<5>{};
        if (title == pipetteNeedle_ReagentBin_6) return new PipetteNeedleReagentBin<6>{};
        if (title == pipetteNeedle_ReagentBinZ1_4) return new PipetteNeedleReagentBinZ1<4>{};
        if (title == pipetteNeedle_ReagentBinZ1_5) return new PipetteNeedleReagentBinZ1<5>{};
        if (title == pipetteNeedle_ReagentBinZ1_6) return new PipetteNeedleReagentBinZ1<6>{};
    }

    /// 试剂仓
    {
        if (title == reagentBin_Back_0) return new ReagentBinBack<0>{};
        if (title == reagentBin_Back_1) return new ReagentBinBack<1>{};
        if (title == reagentBin_Back_2) return new ReagentBinBack<2>{};
        if (title == reagentBin_Back_3) return new ReagentBinBack<3>{};
        if (title == reagentBin_Back_4) return new ReagentBinBack<4>{};
        if (title == reagentBin_Back_5) return new ReagentBinBack<5>{};
        if (title == reagentBin_Back_6) return new ReagentBinBack<6>{};
        if (title == reagentBin_Front_0) return new ReagentBinFront<0>{};
        if (title == reagentBin_Front_1) return new ReagentBinFront<1>{};
        if (title == reagentBin_Front_2) return new ReagentBinFront<2>{};
        if (title == reagentBin_Front_3) return new ReagentBinFront<3>{};
        if (title == reagentBin_Front_4) return new ReagentBinFront<4>{};
        if (title == reagentBin_Front_5) return new ReagentBinFront<5>{};
        if (title == reagentBin_Front_6) return new ReagentBinFront<6>{};
    }

    /// 磁分离
    {
        if (title == magneticSeparation_tray) return new MagneticSeparationTray{};
        if (title == magneticSeparation_aCapacity) return new MagneticSeparationACapacity{};
        if (title == magneticSeparation_ZDrainDeep) return new MagneticSeparationZDrainDeep{};
        if (title == magneticSeparation_ZMixHeight) return new MagneticSeparationZMixHeight{};
    }

    /// 光电
    {
        if (title == photoelectric_bPhotoelectric) return new PhotoelectricBCapacity{};
        if (title == photoelectric_rangeMin) return new PhotoelectricRangeMin{};
    }

    /// 分杯
    {
        if (title == mixing_cup_rangeMin) return new FixCupRangeMin{};
        if (title == mixing_cup_rangeMax) return new FixCupRangeMax{};
    }

    /// 温控
    {
        if (title == thermostat_tempIncubate) return new FixThermostatTempIncubate{};
        if (title == thermostat_tempPhotoelectric) return new FixThermostatTempPhotoelectric{};
        if (title == thermostat_tempReagentBinLid) return new FixThermostatTempReagentBinLid{};
    }

    return new OperationPanel();
}

}  // namespace COM::WFIXTURE
