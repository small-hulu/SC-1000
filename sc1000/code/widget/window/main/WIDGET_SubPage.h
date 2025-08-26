#ifndef WIDGET_SUBPAGE_H_1696919491
#define WIDGET_SUBPAGE_H_1696919491

#include <QScopedPointer>
#include <QWidget>

#include "subpage/WIDGET_SubPageBtnList.h"
#include "widget/factory/WIDGET_IWidgetFactory.hpp"

/**
 * notes:
 * 一般来说遵循 pimpl 原则是不建议这么写的
 * 但是为了让子类code方便，就写在这里了
 */
#include "ui_WIDGET_SubPage.h"

namespace Ui {
class SubPage;
}

namespace WIDGET {
class SubPage : public QWidget {
    Q_OBJECT
protected:
    Ui::SubPage *ui = nullptr;

private:
    QScopedPointer<ISwitchPageFactory> m_pageFactory;
    SubPageBtnList                    *btnList = nullptr;

public:
    explicit SubPage(QWidget *parent = nullptr);
    SubPage(const QVector<QString> &nameList, ISwitchPageFactory *factoryPtr,
            QWidget *parent = nullptr);
    virtual ~SubPage();

public:
    void Switch_page(int index);

signals:
    void signal_switchPage(int);
};
}  // namespace WIDGET

#endif  // WIDGET_SUBPAGE_H_1696919491
