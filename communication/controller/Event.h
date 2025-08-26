#ifndef EVENT_H_1712478275
#define EVENT_H_1712478275

#include <QObject>

#include "../interface/ISingleton.hpp"

namespace COM {

enum DeviceConsumeType : int {
    ASubstrate,  // 底物A
    BSubstrate,  // 底物B
    WasteTank,   // 废料箱
};

class Event
    : public QObject
    , public ISingleton<Event> {
    Q_OBJECT
public:
    explicit Event(QObject* parent = nullptr);

signals:
    /**
     * @brief signal_waring
     * 简单的警告
     * @param msg
     * @param is_serious 是否严重
     */
    void signal_waring(const QString& msg, int is_serious = false);

    /**
     * @param type      物品种类
     * @param consume   消耗量
     */
    void signal_consume(int type, double consume);

    /**
     * @brief signal_consumeWaring
     * 耗材相关异常
     * @param msg
     * 如：
     * 1.检测到废液桶灌满
     * 2.检测到清洗液（缓冲瓶）空
     */
    void signal_consumeWaring(QString msg = "");

    /**
     * mcu异常
     */
    void signal_mcuError(int code, const QString& msg);
};
}  // namespace COM
#endif  // EVENT_H_1712478275
