/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       CJsonKey.h
 * @date       2024-03-25
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief      File Description
 * https://www.bejson.com/jsoneditoronline/
 * 为了开发协调方便
 * 请让变量名和内容保持一致！！！
 *
 *****************************************************************************/
#ifndef JSONKEY_H_1711336323
#define JSONKEY_H_1711336323

#include <QJsonArray>
#include <QJsonObject>
#include <QString>

/** *****************************************
 * key
 ********************************************/
namespace COM::KJ {
const QString message      = "message";    /// 备注信息
const QString emergency    = "emergency";  /// 急诊标志
/**
 * 步骤列表
 * =====================
 * 到时候以list链式方式连接
 */
const QString processSteps = "processSteps";            /// 任务数组
const QString deviceName   = "deviceName";              /// 设备名称
const QString actionType   = "actionType";              /// action类型
const QString deviceMode   = "deviceMode";              /// 模式（int）
const QString actionTypeCombine = "actionTypeCombine";  /// 动作力度，对于主体部件的再分组

/**
 * 更加细化的组合属性
 * 《变量名和内容直接保持对应相等》
 */
const QString op            = "op";             /// 单一设备的操作
const QString ops           = "ops";            /// 系列操作
const QString from          = "from";           /// 具有转移性质的设备的操作
const QString to            = "to";             /// 具有转移设备性质的操作
const QString target        = "target";         /// 目标器件
const QString processActual = "processActual";  /// 实际处理
const QString processPre    = "processPre";     /// 预处理

/**
 * 移液针的吸液模式分为两种
 * 这两个都与 `门限 threshold(数值类型)` 有关
 * 客户端使用 `suctionMode(数值类型)` 做区分
 * 1. 液面检测 (suctionMode=0, 默认为0，即不设置 sutionType 则为液面检测)
 * 2. 随量跟踪 (suctionMode=1)
 */
const QString x         = "x";          /// 坐标和定位相关
const QString y         = "y";          /// 坐标和定位相关
const QString z         = "z";          /// 坐标和定位相关
const QString i         = "i";          /// 坐标和定位相关
const QString j         = "j";          /// 坐标和定位相关
const QString idx       = "idx";        /// 坐标和定位相关
const QString position  = "position";   /// 位置值
const QString capacity  = "capacity";   /// 容量值
const QString velocity  = "velocity";   /// 速度
const QString threshold = "threshold";  /// 门限
const QString z1        = "z1";         /// z轴深度的备选位置
const QString suctionMode =
    "suctionMode";  /// 吸液模式（液面检测=0， 试剂的随量跟踪=1， 样本的随量跟踪=2）
enum class Enum_suctionMode {
    liquidLevelDetector           = 0,  /// 液面检测
    trackingAndMeasurementReagent = 1,  /// 试剂的随量跟踪
    trackingAndMeasurementSample  = 2,  /// 样本的随量跟踪
};

/**
 * 专为试剂仓提供
 * 同时应用与移液针上
 */
const QString idxBinBack  = "idxBinBack";
const QString idxBinFront = "idxBinFront";
const QString idxBinGroup = "idxBinGroup";

/**
 * help data
 */
const QString offset    = "offset";     /// 偏移量
const QString count     = "count";      /// 计数相关的
const QString sleepTime = "sleepTime";  /// 等待时间
const QString delayTime = "delayTime";  /// 延迟时间
const QString temp      = "temp";       /// 温度值
}  // namespace COM::KJ

/** *****************************************
 * Value
 ********************************************/
namespace COM::KJ {
const QString ActionMajor = "ActionMajor";  /// 大动作
const QString ActionMinor = "ActionMinor";  /// 小动作
const QString ActionPatch = "ActionPatch";  /// 原子动作

/**
 * 核心设备《主》
 */
const QString Gripper            = "Gripper";             /// 夹爪
const QString MagneticSeparation = "MagneticSeparation";  /// 磁分离
const QString PipettingNeedle    = "PipettingNeedle";     /// 移液针
const QString ReagentBin         = "ReagentBin";          /// 试剂仓
const QString ReagentBinLid      = "ReagentBinLid";       /// 试剂仓盖
const QString Mixing             = "Mixing";              /// 混匀分杯
const QString Photoelectric      = "Photoelectric";       /// 光电
const QString Incubate           = "Incubate";            /// 温裕盘
const QString Thermostat         = "Thermostat";          /// 温控

/**
 * 核心设备《副》
 */
const QString Throw         = "Throw";   /// 抛料位
const QString Cup           = "Cup";     /// 分杯位
const QString Wash          = "Wash";    /// 洗针 & 加清洗液
const QString Load          = "Load";    /// 上料位（温裕盘前端的一个特殊位置）
const QString Dilute        = "Dilute";  /// 稀释位（温裕盘前端的一个特殊位置）
const QString DoubleSuction = "DoubleSuction";  /// 移液针，同时吸取两组液体

}  // namespace COM::KJ

/**
 * 专为磁分离服务
 */
namespace COM::KJ {
const QString Needle0            = "Needle0";
const QString Needle1            = "Needle1";
const QString Needle2            = "Needle2";
const QString ASubstrate         = "ASubstrate";
const QString washCapacity       = "washCapacity";
const QString aSubstrateCapacity = "aSubstrateCapacity";
const QString drainDeep          = "drainDeep";
const QString mixHeight          = "mixHeight";
}  // namespace COM::KJ

namespace COM::KJ {
const QString fix             = "fix";
const QString rangeMin        = "rangeMin";
const QString rangeMax        = "rangeMax";
const QString zPositionHeight = "zPositionHeight";
}  // namespace COM::KJ

#endif  // JSONKEY_H_1711336323
