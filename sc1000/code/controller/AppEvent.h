#ifndef APPEVENT_H_1708247056
#define APPEVENT_H_1708247056

#include <QObject>

namespace CONTROLLER {
class AppEvent : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(AppEvent)

public:
    static AppEvent& instance() {
        static AppEvent obj{};
        return obj;
    }

public:
    explicit AppEvent(QObject* parent = nullptr);

private:
    void receive_comSignal();
    void receive_expSignal();

signals:
    /**
     * @brief signal_switchPage 翻页信号
     * @param level1            一级菜单序号
     * @param level2            二级菜单序号
     */
    void signal_switchPage(int level1, int level2);

    /**
     * @brief signal_expStateHint
     * @param flag  是否展示
     * @param msg
     * 是否展示
     */
    void signal_expStateHint(bool flag, const QString& msg);
};
}  // namespace CONTROLLER

#endif  // APPEVENT_H_1708247056
