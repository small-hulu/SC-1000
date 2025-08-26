#ifndef PIPETTENEEDLE_2_3_7_H
#define PIPETTENEEDLE_2_3_7_H

#include <QJsonObject>
#include <QWidget>

#include "util/Interface_widget.hpp"

namespace Ui {
class PipetteNeedle_3_7;
}

class PipetteNeedle_3_7
    : public QWidget
    , public Interface_Widget {
    Q_OBJECT
    using Self = PipetteNeedle_3_7;

private:
    Ui::PipetteNeedle_3_7 *ui = nullptr;

private:
    QJsonObject m_json;

public:
    explicit PipetteNeedle_3_7(QWidget *parent = nullptr);
    ~PipetteNeedle_3_7();

private:
    void mark_message(const QString &msg) override;

private:
    /// 单次操作
    void operator_once() override;
    /// 开始执行
    void start_loop() override;
};

#endif  // PIPETTENEEDLE_2_3_7_H
