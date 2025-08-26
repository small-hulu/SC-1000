#include "LOG_WidgetDebug.h"

#include <QTextEdit>
#include <QThread>
#include <iostream>
#include <mutex>

namespace LOG {

/**
 * @brief DebugHandler::m_displayBoard
 * static data for static ballback
 */
QWidget         *WidgetDebug::m_displayBoard      = nullptr;
QtMessageHandler WidgetDebug::m_oldMessageHandler = nullptr;

/**
 * @brief DebugHandler::newDebugHandlerFunc
 * @param type
 * @param context
 * @param msg
 * callback fun
 * thread safe
 */
void WidgetDebug::newDebugHandlerFunc(QtMsgType type, const QMessageLogContext &context,
                                      const QString &msg) {
    static std::mutex           mtx;
    std::lock_guard<std::mutex> lock(mtx);

    if (m_displayBoard) {
        dynamic_cast<QTextEdit *>(m_displayBoard)->append(msg);
    } else {
        m_oldMessageHandler(type, context, msg);
    }
}

/**
 * @brief DebugHandler::DebugHandler
 * clear ptr data
 */
WidgetDebug::WidgetDebug() {
    m_oldMessageHandler = nullptr;
    m_displayBoard      = nullptr;
}

/**
 * @brief DebugHandler::~DebugHandler
 * set before state
 */
WidgetDebug::~WidgetDebug() {
    if (nullptr != m_oldMessageHandler) {
        qInstallMessageHandler(m_oldMessageHandler);
        m_oldMessageHandler = nullptr;
    }
    m_displayBoard = nullptr;
}

/**
 * @brief DebugHandler::Set_Widget
 * @param widget
 * set debug widget
 * e.g. QTextEdit; QPlainTextEdit;
 */
void WidgetDebug::Set_Widget(QWidget *widget) {
    if (WidgetDebug::newDebugHandlerFunc != m_oldMessageHandler) {
        m_displayBoard = widget;
    }
}

/**
 * @brief DebugHandler::Start
 * 保证显示的widget已经获得了，否则这个操作无效
 * install
 */
void WidgetDebug::Start() {
    assert(m_displayBoard);
    m_oldMessageHandler = qInstallMessageHandler(WidgetDebug::newDebugHandlerFunc);
}
}  // namespace LOG
