#ifndef WIDGET_MAIN_TOP_H_1696735355
#define WIDGET_MAIN_TOP_H_1696735355

#include <QLabel>
#include <QPushButton>

namespace Ui {
class MTop;
}

namespace WIDGET::MAIN {

class MTop final : public QWidget {
    Q_OBJECT
private:
    enum class ExpBtnState {
        setStart,        // 允许启动运行
        setPause,        // 允许暂停
        setStopEnable,   // 允许暂停
        setStopDisable,  // 禁止暂停
    };

private:
    Ui::MTop *ui = nullptr;

private:
    QList<QString> m_qss_onIcon;
    QList<QString> m_qss_offIcon;

private:
    QList<QPushButton *> m_titleBtnList;
    QList<QWidget *>     m_titleIconList;
    QList<QLabel *>      m_titleLabelList;

public:
    explicit MTop(QWidget *parent = nullptr);
    virtual ~MTop();

public:
    void Click_indexBtn(int idx);

private:
    void Exp_ModifyExpBtnStyle(ExpBtnState btnState);
    void Exp_StartAndPause();
    void Exp_Stop();

    void EXP_RangeMarginCheck();

signals:
    void signal_switchPage(int);
};

}  // namespace WIDGET::MAIN
#endif  // WIDGET_MAIN_TOP_H_1696735355
