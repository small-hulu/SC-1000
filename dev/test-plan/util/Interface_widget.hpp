#ifndef INTERFACE_WIDGET_HPP
#define INTERFACE_WIDGET_HPP

#include <QString>

class Interface_Widget {
protected:
    int m_cnt = 0;

protected:
    virtual void mark_message(const QString &msg) = 0;

protected:
    /// 单次操作
    virtual void operator_once() = 0;
    /// 开始执行
    virtual void start_loop()    = 0;
};

#endif  // INTERFACE_WIDGET_HPP
