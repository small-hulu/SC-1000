#include "WModule_SwitchList.h"

#include "ui_WModule_SwitchList.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET {
SwitchList::SwitchList(QWidget *parent) : QWidget(parent), ui(new Ui::SwitchList) {
    ui->setupUi(this);

    ui->edit_msg->setReadOnly(true);

    /// connect
    {
        connect(ui->btn_left, &QPushButton::clicked, this, [this]() {
            const int listLen = m_msgList.size();
            if (m_msgList.size() == 0) {
                return;
            }
            m_listIndex = (m_listIndex - 1 + listLen) % listLen;
            auto &&s    = m_msgList[m_listIndex];
            ui->edit_msg->setText(m_msgList[m_listIndex]);
            emit signal_curMsg(s);
        });

        connect(ui->btn_right, &QPushButton::clicked, this, [this]() {
            const int listLen = m_msgList.size();
            if (m_msgList.size() == 0) {
                return;
            }
            m_listIndex = (m_listIndex + 1 + listLen) % listLen;
            auto &&s    = m_msgList[m_listIndex];
            ui->edit_msg->setText(m_msgList[m_listIndex]);
            emit signal_curMsg(s);
        });
    }

    /// init
    //{ UTILS::resize_show(this); }
}

SwitchList::~SwitchList() {
    delete ui;
}

void SwitchList::Set_msgList(const QList<QString> &msgList) {
    m_msgList = msgList;
    if (m_msgList.size()) {
        ui->edit_msg->setText(m_msgList[0]);
        m_listIndex = 0;
    } else {
        ui->edit_msg->setText("");
        m_listIndex = 0;
    }
}

void SwitchList::Set_curMessage(const QString &msg) {
    for (int i = 0; i < m_msgList.size(); i += 1) {
        if (msg == m_msgList[i]) {
            m_listIndex = i;
            ui->edit_msg->setText(msg);
            break;
        }
    }
}

QString SwitchList::Get_curMessage() {
    return ui->edit_msg->text();
}

void SwitchList::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET
