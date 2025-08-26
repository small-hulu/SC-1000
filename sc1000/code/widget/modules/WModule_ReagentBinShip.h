#ifndef WMODULE_REAGENTBIN_H
#define WMODULE_REAGENTBIN_H

#include <QAbstractButton>
#include <QWidget>

#include "WModule_ReagentBinSample.h"

namespace Ui {
class ReagentBinShip;
}

namespace WIDGET {

class ReagentBinShip : public QWidget {
    Q_OBJECT
public:
    static constexpr int Dial_Max = 10;

private:
    Ui::ReagentBinShip *ui = nullptr;

private:
    int              m_selectedGroupIdx = 0;
    QVector<QString> m_testTubes;

public:
    explicit ReagentBinShip(QWidget *parent = nullptr);
    ~ReagentBinShip();

private:
    void rotate_reagentBin(int idx);
    void initializeTestTubes();
    void initializeButtonGroups();
    void updateButtonLabels(int m_testTubes_ptr);
    void reset_idle_label_show();

public slots:
    void handleButtonClicked();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    /// 范围[1, 10]
    /// 下标从1开始
    void signal_curGroup(int);
};
}  // namespace WIDGET

#endif  // WMODULE_REAGENTBIN_H
