#include "WIDGET_SubPage.h"

#include <QDebug>
#include <QMessageBox>

#include "subpage/WIDGET_SubPageBtnList.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET {
SubPage::SubPage(QWidget *parent) : QWidget(parent), ui(new Ui::SubPage{}) {
    ui->setupUi(this);
}

SubPage::SubPage(const QVector<QString> &nameList, ISwitchPageFactory *factoryPtr, QWidget *parent)
    : SubPage(parent) {
    m_pageFactory.reset(factoryPtr);

    btnList = new SubPageBtnList(nameList.toList(), ui->widge_title);
    connect(btnList, &SubPageBtnList::signal_btnClicked, this, &SubPage::Switch_page);

    auto btnGroup = new ButtonBottomGroup(ui->widget_down);
    ButtonBottomGroup::Set(btnGroup);
    btnGroup->hide();

    QTimer::singleShot(10, this, [this]() {
        /// -1表示第一次进入
        /// 会在工厂函数中缓存上传一次的结果
        this->Switch_page(-1);
    });
}

SubPage::~SubPage() {
    delete ui;
}

/**
 * 注意，这里要做整个app的一个页面index的缓存
 * 因此用static
 * 注意这是一个通用的组件，因此加一个hash表来映射
 */
void SubPage::Switch_page(int index) {
    /// 注意这里用static和&
    static QHash<ISwitchPageFactory::unique_type, int> lastIndexTable;
    auto &lastIndex = lastIndexTable[m_pageFactory->Get_identifyName()];
    /// 重复点击，特判直接走
    if (index == lastIndex) {
        return;
    }
    /// 表示刚创建的时，读取上次的缓存
    if (index < 0) {
        index = lastIndex;
    }
    lastIndex         = index;
    auto parentWidget = ui->widget_main_mid;

    /// 第一次执行是0个
    /// 后续执行都是1个
    for (const QObject *obj : parentWidget->children()) {
        try {
            auto widget = const_cast<QObject *>(obj);
            static_cast<QWidget *>(widget)->close();
        } catch (...) {
            qWarning() << "子界面强转异常";
        }
    }

    QTimer::singleShot(0, this, [this, index]() {
        /// 注意设置setAttribute(Qt::WA_DeleteOnClose)
        auto widget = m_pageFactory->Create_bySwitchEnum(index, ui->widget_main_mid);
        if (nullptr == widget) {
            QMessageBox::warning(nullptr, "警告", "空指针异常 | 该界面还未开发");
            return;
        }
        btnList->Click_indexBtn(index);
        widget->setAttribute(Qt::WA_DeleteOnClose);
        UTILS::resize_widget_fromParent(widget);
        widget->show();
    });
}
}  // namespace WIDGET
