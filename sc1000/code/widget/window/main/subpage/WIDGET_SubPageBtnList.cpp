#include "WIDGET_SubPageBtnList.h"

#include "ui_WIDGET_SubPageBtnList.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET {
SubPageBtnList::SubPageBtnList(QWidget *parent) : QWidget(parent), ui(new Ui::SubPageBtnList) {
    ui->setupUi(this);

    const QString qss   = R"(
QPushButton[state="on"] {
  font-family: PingFang SC;
  font-weight: bold;
  font-size: 32px;
  color: #FFFFFF;
  line-height: 42px;

  background: #22A4B3;
  %1
}

QPushButton[state="off"] {
  font-family: PingFang SC;
  font-weight: 500;
  font-size: 32px;
  color: #333333;
  line-height: 42px;

  background: #FFFFFF;
  %1
}
)";
    const QString first = QString(R"(
border-top-left-radius : 10px;
border-top-right-radius : 10px;
border-bottom-left-radius : 0px;
border-bottom-right-radius : 0px;
)");
    const QString mid   = QString("border-radius: 0px;");
    const QString last  = QString(R"(
border-top-left-radius : 0px;
border-top-right-radius : 0px;
border-bottom-left-radius : 10px;
border-bottom-right-radius : 10px;
)");

    m_btnList.clear();
    m_btnList << ui->pushButton;
    m_btnList << ui->pushButton_2;
    m_btnList << ui->pushButton_3;
    m_btnList << ui->pushButton_4;
    m_btnList << ui->pushButton_5;
    m_btnList << ui->pushButton_6;
    m_btnList << ui->pushButton_7;
    for (int i = 0; i < m_btnList.size(); i += 1) {
        auto btn = m_btnList[i];
        if (i == 0) {
            btn->setStyleSheet(qss.arg(first));
        } else if (i + 1 == m_btnList.size()) {
            btn->setStyleSheet(qss.arg(last));
        } else {
            btn->setStyleSheet(qss.arg(mid));
        }
        btn->setProperty("state", "off");

        connect(m_btnList[i], &QPushButton::clicked, this, [this, i, btn = m_btnList[i]]() {
            sLog_test() << "2nd LevelPage switch to page" << btn->text();
            Click_indexBtn(i);
            emit signal_btnClicked(i);
        });
    }
}

SubPageBtnList::SubPageBtnList(const QList<QString> &names, QWidget *parent)
    : SubPageBtnList(parent) {
    assert(m_btnList.size() >= names.size());
    for (int i = 0; i < m_btnList.size(); i += 1) {
        if (i < names.size()) {
            m_btnList[i]->setText(names[i]);
            m_btnList[i]->show();
        } else {
            m_btnList[i]->hide();
        }
    }
}

SubPageBtnList::~SubPageBtnList() {
    delete ui;
}

void SubPageBtnList::Click_indexBtn(int idx) {
    assert(idx < m_btnList.size());
    for (auto &&btn : m_btnList) {
        btn->setProperty("state", "off");
        UTILS::style_polish(btn);
    }
    m_btnList[idx]->setProperty("state", "on");
    UTILS::style_polish(m_btnList[idx]);
}
}  // namespace WIDGET
