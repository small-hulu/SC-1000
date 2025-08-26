#ifndef WSAMPLE_SAMPLEAPPLY_H_1716270808_1720081654
#define WSAMPLE_SAMPLEAPPLY_H_1716270808_1720081654

#include <QAbstractButton>
#include <QPushButton>
#include <QWidget>

namespace Ui {
class ReagentBinSample;
}

struct ReagentBinCache {
    static int GroupIdx_Cache;
};

namespace WIDGET {
class ReagentBinSample : public QWidget {
    Q_OBJECT
public:
    static constexpr int Dial_Max = 10;

private:
    Ui::ReagentBinSample *ui = nullptr;

private:
    /// [0, 9]
    int m_selectedGroupIdx                        = -1;
    /// [1, 30]
    int                         m_selectedCellIdx = -1;
    QVector<QList<QString>>     m_btnTxtTable;
    QList<QList<QPushButton *>> m_btnList;

public:
    explicit ReagentBinSample(QWidget *parent = nullptr);
    ~ReagentBinSample();

public:
    void Update_dial();
    void Select_onebtn(QString btnTxt);

private:
    void handle_btnClicked();
    void rotate_reagentBin(int idx);

protected:
    void showEvent(QShowEvent *event) override;

signals:
    /// 范围[1, 10]
    void signal_curGroup(int);
};
}  // namespace WIDGET

#endif  // WSAMPLE_SAMPLEAPPLY_H_1716270808
