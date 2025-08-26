/** ************************************************************************
 * Copyright © 2023 CuberLotus.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       LOG_.hpp
 *
 * @author     CuberLotus
 *
 * @Email      1539349804@qq.com
 *
 * @version    0.1.0
 *
 * @brief
 * 使用"LOG_"宏进行打日志
 * 参数1 日志类型
 * 参数2 日志内容
 *
 * 在"LOG_Config.hpp"
 * 进行定制化的修改
 *
 * 日志效果样例
 * [1.0.0.0] [空指针_错误] [18:03:51] {FILE: main.cpp} {FUNC: main()} {LINE:
 *100} xxxxxxxxxx
 *
 ***************************************************************************/
#ifndef __LOG__HPP__BY__CuberLotus__
#define __LOG__HPP__BY__CuberLotus__

#include <QDebug>

#include "LOG_SLog.h"
#include "LOG_Version.h"

namespace LOG {
void QLog_Init();

void simple_messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
}  // namespace LOG

#endif  // __LOG__HPP__BY__CuberLotus__
