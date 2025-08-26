#include "WMainFixture.h"

#include <QDebug>
#include <QTimer>

#include "WFactoryFix.h"
#include "ui_WMainFixture.h"
#include "widget/dialog/DIALOG.h"
#include "widget/modules/WModule_TableHeader.h"

namespace COM::WFIXTURE {
MainFixture::MainFixture(QWidget *parent) : QWidget(parent), ui(new Ui::MainFixture) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_StyledBackground);

    {
        new WIDGET::TableHeader("工装", ui->widget_title_left);
        new WIDGET::TableHeader("数据信息区", ui->widget_title_mid);
    }

    /// 按钮组
    {
        m_ui_btnGroup = WIDGET::ButtonBottomGroup::Get();  // 按钮组
        m_ui_btnGroup->show();
        m_ui_btnGroup->Set_refreshFunc({});
        QList<QString> titles;
        titles << QString("保存全部");
        QList<std::function<void()>> taskList;
        taskList << [this]() {
            if (m_ui_panel) {
                bool          flag = m_ui_panel->Save_fixtureParams();
                const QString msg  = flag ? QString("保存完成") : QString("保存失败");
                WIDGET::DIALOG::Create_DInfoDialog(msg);
            }
        };
        titles << QString("保存XY");
        taskList << [this]() {
            if (m_ui_panel) {
                bool          flag = m_ui_panel->Save_fixtureParams_Without_Z();
                const QString msg  = flag ? QString("保存完成") : QString("保存失败");
                WIDGET::DIALOG::Create_DInfoDialog("保存完成");
            }
        };
        m_ui_btnGroup->Set_BtnList(titles, taskList);
    }

    /// 树状选择
    {
        QTreeWidget *tree = ui->treeWidget;
        FactoryFix::Init_tree(tree);

        connect(tree, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem *item, int column) {
            QString title = item->text(column);
            qDebug()<<"title"<<title<<endl;
            switch_page(title);
        });
    }
}

MainFixture::~MainFixture() {
    if (m_ui_panel) {
        m_ui_panel->deleteLater();
    }
    m_ui_btnGroup->hide();
    delete ui;
}

void MainFixture::switch_page(QString title) {
    assert(title.size());
    if (m_ui_panel) {
        m_ui_panel->close();
        m_ui_panel = nullptr;
    }

    /**
     * 保证让上一轮的先关闭
     * 先 close -> delete -> destruct
     * 在保证该次的 new -> construct -> show
     */
    QTimer::singleShot(0, [this, title]() {
        auto widget = FactoryFix::Create_opeartorWidget(title);
        if (widget == nullptr) {
            return;
        }
        auto parent = ui->widget_op;
        m_ui_panel  = widget;
        widget->setParent(parent);
        widget->setAttribute(Qt::WA_DeleteOnClose);
        widget->show();
    });
}

void MainFixture::showEvent(QShowEvent *event) {
    auto parent = this->parentWidget();
    if (parent) {
        this->setGeometry(parent->rect());
    }
    QWidget::showEvent(event);
}
}  // namespace COM::WFIXTURE
