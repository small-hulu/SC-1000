#ifndef TEMPORARY_H
#define TEMPORARY_H

#include <QWidget>

namespace Ui {
class PipetteNeedlePrecision;
}

class PipetteNeedlePrecision : public QWidget {
    Q_OBJECT
private:
    Ui::PipetteNeedlePrecision *ui;

private:
    int m_liquidResidual = 0;

public:
    explicit PipetteNeedlePrecision(QWidget *parent = nullptr);
    ~PipetteNeedlePrecision();

public:
    void op_homing();
    void op_once();
};

#endif  // TEMPORARY_H
