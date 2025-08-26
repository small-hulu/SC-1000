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
#ifndef JSONKEY_H
#define JSONKEY_H

#include <QJsonArray>
#include <QJsonObject>
#include <QString>

/** *****************************************
 * key
 ********************************************/
namespace LTE::KJ {
const QString message      = "message";    /// 备注信息


const QString processSteps = "processSteps";            /// 任务数组
const QString actionType   = "actionType";              /// action类型
const QString operate      = "operate";                 /// operate类型
const QString command      = "command";                 /// 指令码
const QString value        = "value";                   /// 值
const QString hash         = "hash";                    /// hash值


//actionType
const QString ALL          = "ALL";
const QString SC1000       = "SC1000";

//command code : ALL
const QString ALL_1        = "ALL:1";                   /// 通用控制码
const QString ALL_2        = "ALL:2";                   /// 通用控制码
const QString ALL_3        = "ALL:3";                   /// 通用控制码
const QString ALL_4        = "ALL:4";                   /// 通用控制码
const QString ALL_5        = "ALL:5";                   /// 通用控制码
const QString ALL_6        = "ALL:6";                   /// 通用控制码

//command code : SC1000


//actionType
const QString Update      = "U";    /// 修改
const QString Read        = "R";    /// 查询
const QString Create      = "C";    /// 增
const QString Delete      = "D";    /// 删除


//reply_msg
const QString ControlOK        = "ControlOK";
const QString ControlSuccess   = "ControlSuccess";
const QString ControlFail      = "ControlFail";

}  // namespace LTE::KJ

#endif  // JSONKEY_H_1711336323
