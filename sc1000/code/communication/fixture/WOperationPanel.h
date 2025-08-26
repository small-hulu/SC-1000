#ifndef WOPERATIONPANEL_H_1723440372
#define WOPERATIONPANEL_H_1723440372

#include <QWidget>
#include <array>

namespace Ui {
class OperationPanel;
}

namespace COM::WFIXTURE {
class OperationPanel : public QWidget {
    Q_OBJECT
public:
    //! 注意：这里必须保证 3x3
    using Sudoku = QList<QList<int>>;

private:
    Ui::OperationPanel *ui = nullptr;

public:
    explicit OperationPanel(QWidget *parent = nullptr);
    ~OperationPanel();

public:
    void Set_Sudoku(const Sudoku &matrx);

public:
    virtual bool Save_fixtureParams() {
        return false;
    }
    virtual bool Save_fixtureParams_Without_Z() {
        return false;
    }
protected:
    double get_inputNum();
    void   set_inputNum(double num);

protected:
    virtual void exec_fixtureMove() {
        assert(false);
    }
    virtual void exec_fixtureMove_Z() {
        assert(false);
    }
protected:
    virtual void op_xadd() {}
    virtual void op_xsub() {}
    virtual void op_yadd() {}
    virtual void op_ysub() {}
    virtual void op_zadd() {}
    virtual void op_zsub() {}
    virtual void op_clockwise() {}
    virtual void op_anticlockwise() {}

protected:
    virtual void showEvent(QShowEvent *event) override;

signals:
    void signal_message(const QString &msg);
};
}  // namespace COM::WFIXTURE

#endif  // WOPERATIONPANEL_H_1723440372
