#ifndef CRETTYPE_H_1711617693
#define CRETTYPE_H_1711617693

#include <QDebug>
#include <QString>

namespace COM {

/**
 * @brief The ConRetCode struct
 * 这个使用的非常频繁，请不要继承QObject
 * 本质就是包装一个enum
 * operator enum()
 */
struct RetCode final {
public:
    /**
     * @brief The ComRet enum
     * 大于0：均需要强制结束
     */
    enum RetEnum : int {
        Running = 0,       /// 正在运行中
        Success = 1,       /// 操作成功
        ForceStop,         /// 强制停止
        TextParsingError,  /// 文本解析失败
        WriteError,        /// 单纯读这个操作失败
        ReadError,         /// 单纯写这个操作失败
        TimeOut,           /// 超时
        MCUError,          /// 下位机错误
        RefuseError,       /// 拒接执行
        TryAgain,          /// 再次尝试
        CrashError,        /// 碰撞
        ItemExistError,    /// 物体存在异常
        Insufficient_in,   /// 试剂不足-进液
        Insufficient_out,  /// 试剂不足-出液
        DetectError,       /// 检测失败
        CupError,          /// 分杯异常
        ModbusError,       /// modbus错误

        TODO,
        UnknownError = 0x3f3f3f3f,
    };

public:
    RetEnum result = UnknownError;

public:
    RetCode() = default;
    RetCode(RetEnum num) : result(static_cast<RetEnum>(num)) {}
    RetCode(const RetCode&)            = default;
    RetCode& operator=(const RetCode&) = default;

public:
    QString to_string() const {
        switch (result) {
        case Running         : return "Running";
        case Success         : return "Success";
        case ForceStop       : return "ForceStop";
        case WriteError      : return "WriteError";
        case TextParsingError: return "TextParsingError";
        case ReadError       : return "ReadError";
        case TimeOut         : return "TimeOut";
        case MCUError        : return "MCUError";
        case RefuseError     : return "RefuseError";
        case TryAgain        : return "TryAgain";
        case CrashError      : return "CrashError";
        case ItemExistError  : return "ItemExistError";
        case Insufficient_in : return "Insufficient_in";
        case Insufficient_out: return "Insufficient_out";
        case DetectError     : return "DetectError";
        case CupError        : return "CupError";
        case ModbusError     : return "ModbusError";
        case TODO            : return "TODO";
        case UnknownError    : return "UnknownError";
        }
        return "Enum Out of Range";
    }

public:
    friend inline QDebug operator<<(QDebug os, const RetCode& flag) {
        return os << flag.to_string();
    }

    friend inline bool operator==(const RetCode& lhs, const RetCode& rhs) {
        return lhs.result == rhs.result;
    }

    friend inline bool operator!=(const RetCode& lhs, const RetCode& rhs) {
        return lhs.result != rhs.result;
    }
};

inline QString to_string(const RetCode& flag) {
    return flag.to_string();
}

}  // namespace COM
#endif  // CRETTYPE_H_1711617693
