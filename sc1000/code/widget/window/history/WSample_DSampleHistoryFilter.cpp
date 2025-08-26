#include "WSample_DSampleHistoryFilter.h"

#include <QListView>

#include "ui_WSample_DSampleHistoryFilter.h"

namespace WIDGET::HISTORY {

/**
 * @brief ResultDReagentParams::ResultDReagentParams
 * constructor
 */
DSampleHistoryFilter::DSampleHistoryFilter(QWidget *parent)
    : QWidget(parent), ui(new Ui::DSampleHistoryFilter{}) {
    ui->setupUi(this);

    /// attribute & ui
    {
        if (parent) {
            this->setGeometry(parent->rect());
        }
        this->setAttribute(Qt::WA_StyledBackground);
        this->setAttribute(Qt::WA_DeleteOnClose);

        m_uiView_title.clear();
        m_uiView_title << ui->label_paramsName_0;
        m_uiView_title << ui->label_paramsName_1;
        m_uiView_title << ui->label_paramsName_2;
        m_uiView_title << ui->label_paramsName_3;
        m_uiView_title << ui->label_paramsName_4;
        m_uiView_title << ui->label_paramsName_5;
        m_uiView_title << ui->label_paramsName_6;
        m_uiView_title << ui->label_paramsName_7;
        m_uiView_title << ui->label_paramsName_8;
        m_uiView_title << ui->label_paramsName_9;
        m_uiView_title << ui->label_paramsName_10;
        m_uiView_title << ui->label_paramsName_11;

        m_uiView_val.clear();
        m_uiView_val << ui->edit_params_0;
        m_uiView_val << ui->edit_params_1;
        m_uiView_val << ui->edit_params_2;
        m_uiView_val << ui->edit_params_3;
        m_uiView_val << ui->edit_params_4;
        m_uiView_val << ui->edit_params_5;
        m_uiView_val << ui->edit_params_6;
        m_uiView_val << ui->edit_params_7;
        m_uiView_val << ui->edit_params_8;
        m_uiView_val << ui->edit_params_9;
        m_uiView_val << ui->edit_params_10;
        m_uiView_val << ui->edit_params_11;

        assert(m_uiView_val.size() == m_uiView_title.size());
    }

    /// connect 取消 确认
    {
        connect(ui->btn_no, &QPushButton::clicked, this, [this]() { this->close(); });

        connect(ui->btn_yes, &QPushButton::clicked, this, [this]() {
            emit signal_yes();
            this->close();
        });
    }

    /// init
    {
        QDateTime curTime = QDateTime::currentDateTime();
        curTime           = QDateTime(curTime.date());

        auto title = QList<QString>{QString("姓名"), QString("项目"), QString("起始时间"),
                                    QString("结束时间")};
        auto vals =
            QList<QString>{QString(""), QString(""), curTime.toString(InfoSample::time_format),
                           curTime.addDays(1).toString(InfoSample::time_format)};
        assert(title.size() == vals.size());
        assert(title.size() <= m_uiView_val.size());

        for (int i = 0; i < m_uiView_title.size(); i += 1) {
            if (i < title.size()) {
                m_uiView_title[i]->setText(title[i]);
            } else {
                m_uiView_title[i]->hide();
            }
        }
        for (int i = 0; i < m_uiView_val.size(); i += 1) {
            if (i < vals.size()) {
                m_uiView_val[i]->setText(vals[i]);
            } else {
                m_uiView_val[i]->hide();
            }
        }
    }  // init
}

InfoSample::HistoryFilterInfo DSampleHistoryFilter::Get_filterInfo() {
    InfoSample::HistoryFilterInfo info;
    info.patientName = m_uiView_val[0]->text();
    info.project     = m_uiView_val[1]->text();
    info.timeStart   = m_uiView_val[2]->text();
    info.timeEnd     = m_uiView_val[3]->text();

    return info;
}
}  // namespace WIDGET::HISTORY
