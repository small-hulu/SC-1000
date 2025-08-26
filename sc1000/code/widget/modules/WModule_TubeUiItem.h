#ifndef WMODULE_SAMPLETUBE_H_1719798365
#define WMODULE_SAMPLETUBE_H_1719798365

#include <QGridLayout>
#include <QWidget>

#include "global/GLOBAL_Type.h"

namespace Ui {
class TubeUiItem;
}

namespace WIDGET {

/// [type]
class TubeUiItem;
using entity_ui_tube = TubeUiItem *;
/// [type]

/// [class]
class TubeUiItem : public QWidget {
    Q_OBJECT
private:
    Ui::TubeUiItem *ui = nullptr;

private:
    Type::ETubeState m_StateType = {};
    Type::ETubeExp   m_expType   = {};

public:
    explicit TubeUiItem(QWidget *parent = nullptr);
    ~TubeUiItem();

public:
    /// 设置右侧文字信息
    void Set_lableRight(const QString &txt);
    /// 设置底部文字信息
    void Set_labelBottom(const QString &txt);
    /// 获取文字信息
    QString Get_labelRightStr();
    QString Get_labelBottomStr();

public:
    void Set_clicked(int flag);
    void Set_StateType(int type);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;

signals:
    void signal_clicked(TubeUiItem *);
};
/// [class]

}  // namespace WIDGET
#endif  // WMODULE_SAMPLETUBE_H_1719798365
