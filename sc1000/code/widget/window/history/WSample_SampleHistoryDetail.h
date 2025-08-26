/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       WSample_SampleHistoryDetail.h
 * @date       2024-07-17
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 注意：
 * 这个组件应该是可以放多行信息的
 * 但是目前我们量有限，不知道多组的逻辑
 * 因此，先写为单个entity
 * 后期业务逻辑清洗了，再修改
 *****************************************************************************/
#ifndef WSAMPLE_SAMPLEHISTORYDETAIL_H_1721200121
#define WSAMPLE_SAMPLEHISTORYDETAIL_H_1721200121

#include <QTableWidget>
#include <QWidget>

#include "controller/CONTROLLER_ExpSample.h"

namespace Ui {
class SampleHistoryDetail;
}

namespace WIDGET::HISTORY {
class SampleHistoryDetail : public QWidget {
    Q_OBJECT
public:
    using entity_item = CONTROLLER::ExpSample::entity_item;
    using entity_attr = entity_item::element_type::attr;

private:
    Ui::SampleHistoryDetail *ui    = nullptr;
    QTableWidget            *table = nullptr;

private:
    entity_item m_entity;

public:
    explicit SampleHistoryDetail(QWidget *parent = nullptr);
    ~SampleHistoryDetail();

public:
    void Set_entity(entity_item entity);

private:
    void refresh();
    void refresh_message(entity_item entity);

public:
    void Append_tableLine(entity_item entity);

private:
    QList<QString> get_lineStr(entity_item entity);

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace WIDGET::HISTORY

#endif  // WSAMPLE_SAMPLEHISTORYDETAIL_H_1721200121
