#include "WModule_ButtonBottomGroup.h"

#include "ui_WModule_ButtonBottomGroup.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET {
ButtonBottomGroup *ButtonBottomGroup::globalPtr = nullptr;

ButtonBottomGroup::ButtonBottomGroup(QWidget *parent)
    : QWidget(parent), ui(new Ui::ButtonBottomGroup) {
    ui->setupUi(this);

    /// 按钮组在ui上是从右往左的
    m_BtnListView.clear();
    m_BtnListView << ui->btn_0;
    m_BtnListView << ui->btn_1;
    m_BtnListView << ui->btn_2;
    m_BtnListView << ui->btn_3;
    m_BtnListView << ui->btn_4;
    m_BtnListView << ui->btn_5;
    m_BtnListView << ui->btn_6;
    m_BtnListView << ui->btn_7;
    m_BtnListView << ui->btn_8;
    m_BtnListFunc.clear();
    m_BtnListFunc.resize(m_BtnListView.size());

    for (int i = 0; i < m_BtnListView.size(); i += 1) {
        /// 1. 切换界面
        /// 2. 刷新按钮组
        connect(m_BtnListView[i], &QPushButton::clicked, [this, i]() {
            if (m_BtnListFunc[i]) {
                m_BtnListFunc[i]();
                if (m_refreshFunc) {
                    m_refreshFunc();
                }
            } else {
                qWarning() << "btn-fun is empty";
            }
        });
    }
}

ButtonBottomGroup::~ButtonBottomGroup() {
    delete ui;
}

void ButtonBottomGroup::Set_BtnList(const QList<QString>                &btnTextList,
                                    const QList<std::function<void()> > &btnFuncList) {
    assert(btnTextList.size() == btnFuncList.size());
    assert(btnTextList.size() <= m_BtnListView.size());
    for (int i = 0; i < m_BtnListView.size(); i += 1) {
        if (i < btnTextList.size()) {
            m_BtnListFunc[i] = btnFuncList[i];
            m_BtnListView[i]->setText(btnTextList[i]);
            m_BtnListView[i]->show();
        } else {
            m_BtnListView[i]->hide();
        }
    }
}

void ButtonBottomGroup::Set_BtnTextList(const QList<QString> &btnTextList) {
    assert(btnTextList.size() <= m_BtnListView.size());
    for (int i = 0; i < m_BtnListView.size(); i += 1) {
        if (i < btnTextList.size()) {
            m_BtnListView[i]->setText(btnTextList[i]);
            m_BtnListView[i]->show();
        } else {
            m_BtnListView[i]->hide();
        }
    }
}

void ButtonBottomGroup::RemoveAllButtons() {
    for (int i = 0; i < m_BtnListView.size(); ++i) {
        m_BtnListView[i]->hide();
        m_BtnListFunc[i] = nullptr;
    }
}
void ButtonBottomGroup::Set_refreshFunc(const std::function<void()> &func) {
    m_refreshFunc = func;
}

void ButtonBottomGroup::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

    int cnt = 0;
    for (auto btn : m_BtnListView) {
        if (btn->isVisible()) {
            cnt += 1;
        } else {
            break;
        }
    }
    if (cnt == 0) {
        return;
    }

    /// 两个按钮间的间隔
    constexpr int interval = 12;
    /// 整体背景的中间
    const int mid          = this->width() / 2;
    /// 按钮的宽度
    const int width        = m_BtnListView[0]->width();
    /// 按钮y轴（固定）
    const int y            = m_BtnListView[0]->y();
    int       x            = mid;

    /// 根据奇偶个数，算出最右侧的x的位置
    if (cnt & 1) {
        int btnCnt          = cnt;
        int btnIntervalCnt  = btnCnt - 1;
        x                  -= width / 2;
        x                  += (btnIntervalCnt) / 2 * interval;
        x                  += (btnCnt - 1) / 2 * width;
    } else {
        int btnCnt          = cnt;
        int btnIntervalCnt  = btnCnt - 1;
        x                  += interval / 2;
        x                  += (btnIntervalCnt - 1) / 2 * interval;
        x                  += (btnCnt / 2 - 1) * width;
    }

    for (int i = 0; i < cnt; i += 1) {
        m_BtnListView[i]->move(x, y);
        x -= interval;
        x -= width;
    }
}

void ButtonBottomGroup::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET
