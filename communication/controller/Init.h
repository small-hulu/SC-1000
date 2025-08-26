#ifndef CONINIT_H_1711329607
#define CONINIT_H_1711329607

#include <QString>
#include <optional>

#include "../utility/ToString.h"

namespace COM {
/**
 * 整体初始化
 * 以模板方法统领整个初始化
 * ===================
 * 1. 手动执行串口号
 * 2. 根据配置的 pid，vid 指定
 */
bool Init(std::optional<QString> portName = std::nullopt);

/**
 * 退出所有状态和线程
 */
bool Exit();

/**
 * 实例化对象
 */
bool Init_instance();

/**
 * 从bootloader进入app
 */
void Init_bootloader();

/**
 * 设置一些基础参数
 */
void Init_mcu();

/**
 * 启动所有部件的loop
 * 也就是QThread::run
 */
void Start_allLoops();

/**
 * 关闭所有部件的loop
 * 也就是QThread::run
 */
void Stop_allLoops();

/**
 * 打印初始化信息
 */
void Log_message();
}  // namespace COM

#endif  // CONINIT_H_1711329607
