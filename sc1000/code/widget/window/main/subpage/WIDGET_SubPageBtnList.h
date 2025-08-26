#ifndef WIDGET_SUBPAGEBTNLIST_H_1725934670
#define WIDGET_SUBPAGEBTNLIST_H_1725934670

#include <QList>
#include <QPushButton>
#include <QWidget>

namespace Ui {
class SubPageBtnList;
}

namespace WIDGET {
class SubPageBtnList : public QWidget {
    Q_OBJECT
private:
    Ui::SubPageBtnList *ui = nullptr;

private:
    QList<QPushButton *> m_btnList;

public:
    explicit SubPageBtnList(QWidget *parent = nullptr);
    SubPageBtnList(const QList<QString> &names, QWidget *parent = nullptr);
    ~SubPageBtnList();

public:
    void Click_indexBtn(int idx);

signals:
    void signal_btnClicked(int idx);
};
}  // namespace WIDGET

#endif  // WIDGET_SUBPAGEBTNLIST_H_1725934670
