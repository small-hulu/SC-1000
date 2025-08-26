#ifndef TESTUI_H
#define TESTUI_H

#include <QWidget>

namespace Ui {
class TestUi;
}

class TestUi : public QWidget {
    Q_OBJECT
private:
    Ui::TestUi *ui;

public:
    explicit TestUi(QWidget *parent = nullptr);
    ~TestUi();
};

#endif  // TESTUI_H
