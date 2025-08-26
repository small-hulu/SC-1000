#ifndef DEBUGHANDLER_H_1685083081
#define DEBUGHANDLER_H_1685083081

#include <QDebug>
#include <QWidget>

namespace LOG {

class WidgetDebug {
private:
    static QWidget         *m_displayBoard;
    static QtMessageHandler m_oldMessageHandler;

private:
    static void newDebugHandlerFunc(QtMsgType, const QMessageLogContext &, const QString &);

public:
    explicit WidgetDebug();
    ~WidgetDebug();

public:
    void Set_Widget(QWidget *widget);
    void Start();
};

}  // namespace LOG

#endif  // DEBUGHANDLER_H_1685083081
