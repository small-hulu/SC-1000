#include "WIDGET_HistorySample.h"

#include <QCheckBox>

#include "entity/EConfig_Printer.h"
#include "historySample/WHistory_DHSampleDetail.h"
#include "lis/Lis_SC1000.h"
#include "ui_WIDGET_HistorySample.h"
#include "utility/UTILITY_File.h"
#include "widget/dialog/DIALOG.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"
#include "widget/utils/UTILS.hpp"
#include "xlsx/UsbExport.h"

namespace OM = KeyProject::OtherMessage;

namespace WIDGET::HISTORY {
QList<QString> HistorySample::Get_lineStr(HistorySample::entity_item entity) {
    QList<QString> res;
    res << entity->Get_idString();
    auto idSample     = entity->table[entity_attr::idSample];
    auto sample       = CONTROLLER::Sample::instance().Select_byId_fromDB(idSample);
    using attr_sample = CONTROLLER::Sample::entity_attr;
    if (sample) {
        res << sample->table[attr_sample::number];
        res << sample->table[attr_sample::location];
    } else {
        res << QString("");
        res << QString("数据错误");
    }
    res << entity->table[entity_attr::timeStart];
    res << entity->table[entity_attr::timeEnd];
    QString project = entity->table[entity_attr::project];

    auto          fileCU             = CONTROLLER::Create_ProjectsFolderController();
    QJsonObject   json               = fileCU.Get_projectJson(project);
    QJsonObject   OtherMessage       = json.value(OM::OtherMessage).toObject();
    QJsonObject   projectMessage     = OtherMessage.value(OM::ProjectMessage).toObject();
    const QString Default_Unit       = projectMessage.value(OM::Default_Unit).toString();
    const QString Reference_range    = projectMessage.value(OM::Reference_range).toString();
    const QString Result_Calculation = projectMessage.value(OM::Result_Calculation).toString();

    if (project == "25-OH-VD")  // eg:  10;1(vd2);9(vd3)
    {
        QString     resultData = entity->table[entity_attr::resultData];
        QStringList resultList = resultData.split(";");
        if (Result_Calculation == OM::Triplet) {
            QStringList pro_show;
            pro_show.append(project);
            pro_show.append("25-OH-VD2");
            pro_show.append("25-OH-VD3");
            res << pro_show.join("\n");
            res << resultList.join("\n");
        } else {
            res << project;
            res << resultList.at(0);
        }
    } else {
        res << project;
        auto resData = entity->table[entity_attr::resultData];
        if(project == "hs-cTnI")
        {
            qDebug()<<"resData is"<<resData<<endl;
        }

        if (resData.isEmpty()) {
            res << resData;
        } else {
            bool ok = false;
            double val = entity->table[entity_attr::resultData].toDouble(&ok);
            if (ok) {
                res << QString::asprintf("%.4f", val); // 或 %.4lf 都可以
            } else {
                res << entity->table[entity_attr::resultData]; // 原样输出非数值结果，如 "<0.002"、"计算异常"
            }
            //res << QString::asprintf("%.4lf", entity->table[entity_attr::resultData].toDouble());
        }
    }

    res << Default_Unit;
    res << Reference_range;
    res << entity->table[entity_attr::expData];
    if (entity->table[entity_attr::state].toInt() == (int)Type::ETubeState::UnKnown) {
        res << "*";
    } else {
        res << "";
    }

    return res;
}

HistorySample::HistorySample(QWidget* parent) : QWidget(parent), ui(new Ui::HistorySample) {
    ui->setupUi(this);

    {
        QList<QString> titles;
        titles << QString("选择");
        titles << QString("序号");
        titles << QString("患者编号");
        titles << QString("位置");
        titles << QString("申请时间");
        titles << QString("结束时间");
        titles << QString("项目");
        titles << QString(" 浓度 ");
        titles << QString("单位");
        titles << QString("参考范围");
        titles << QString(" 发光值 ");
        titles << QString(" ");
        table = ui->tableWidget;
        UTILS::init_table_clear(table);
        UTILS::init_table_mode(table);
        UTILS::init_table_titles(table, titles);
        UTILS::init_table_colWidth(table, titles);
        table->verticalHeader()->setDefaultSectionSize(UTILS::table_auto_rowHeight);
    }

    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
        m_ui_btnGroup->Set_refreshFunc(std::bind(&HistorySample::refresh_bottomBtnList, this));
        refresh_bottomBtnList();
    }

    {
        auto items = CONTROLLER::ExpSample::instance().Select_all_fromDB();
        std::reverse(items.begin(), items.end());
        Filter_data(items);
    }

    startTimer(Sync_IntervalTime);
}

HistorySample::~HistorySample() {
    m_ui_btnGroup->hide();
    delete ui;
}

void HistorySample::Clear() {
    while (table->rowCount()) {
        table->removeRow(table->rowCount() - 1);
    }
    m_items.clear();
}

void HistorySample::Filter_data(const QList<HistorySample::entity_item>& items) {
    Clear();
    for (auto&& item : items) {
        Append_tableLine(item);
    }
}

void HistorySample::Append_tableLine(HistorySample::entity_item entity) {
    auto line = Get_lineStr(entity);
    assert(line.size() == table->columnCount() - 1);

    const int curRowCnt = table->rowCount();
    table->insertRow(curRowCnt);

    const QString qss_tableCheckBox = R"(
            QCheckBox::indicator {
                width: 30%;
                height: 30%;
            }
            QCheckBox::indicator:unchecked {
                border-image: url(:/static/images/choice.png);
            }
            QCheckBox::indicator:checked {
                border-image: url(:/static/images/choice-full.png);
            }
        )";

    auto [w, h]     = std::tuple(table->columnWidth(0), table->rowHeight(curRowCnt));
    QWidget* widget = new QWidget{};
    widget->resize(w, h);

    QCheckBox* checkBox = new QCheckBox(widget);
    checkBox->resize(qMin(w, h), qMin(w, h));
    checkBox->setStyleSheet(qss_tableCheckBox);

    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(checkBox);

    table->setCellWidget(curRowCnt, 0, widget);
    fill_tableLine(curRowCnt, entity);

    LineEntityNode node;
    node.checkBox = checkBox;
    node.entity   = entity;
    m_items.push_back(node);
}

void HistorySample::fill_tableLine(int idx, HistorySample::entity_item entity) {
    auto line = Get_lineStr(entity);
    for (int j = 1; j < table->columnCount(); j += 1) {
        auto* item = new QTableWidgetItem(line[j - 1]);
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(idx, j, item);
    }

    QString value = line[6];
    if (value.isEmpty()) {
        return;
    }

    ENTITY::Project project(line[5]);
    if (project.Is_valid() == false) {
        return;
    }

    auto [flag, str] = entity->Judge_scope();
    if (flag == 0) {
        // pass
    } else if (flag < 0) {
        auto* item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignCenter);
        item->setForeground(QBrush(Qt::green));
        table->setItem(idx, 7, item);
    } else {
        auto* item = new QTableWidgetItem(str);
        item->setTextAlignment(Qt::AlignCenter);
        item->setForeground(QBrush(Qt::red));
        table->setItem(idx, 7, item);
    }
}

void HistorySample::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;

    if (false) {
    } else if (m_ui_historyDetail) {
        names << QString("返回");
        taskList << std::bind(&HistorySample::show_HistoryTable, this);
    } else if (ui) {
        names << QString("删除");
        taskList << std::bind(&HistorySample::Btn_delete, this);
        names << QString("详情列表");
        taskList << std::bind(&HistorySample::Btn_showDetail, this);
        names << QString("重测");
        taskList << std::bind(&HistorySample::Btn_retest, this);
        names << QString("打印");
        taskList << std::bind(&HistorySample::Btn_print, this);
        names << QString("Lis发送");
        taskList << std::bind(&HistorySample::Brn_lisSend, this);
        names << QString("编辑");
        taskList << std::bind(&HistorySample::Btn_modify, this);
        names << QString("查询");
        taskList << std::bind(&HistorySample::Btn_query, this);
        names << QString("导出");
        taskList << std::bind(&HistorySample::Btn_usbExport, this);
        names << QString("全选/取消");
        taskList << std::bind(&HistorySample::Btn_selectAllOrCancel, this);
    }

    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void HistorySample::show_HistoryTable() {
    UTILS::destroy_widget_refPointer(m_ui_historyDetail);
    ui->tableWidget->show();
}

void HistorySample::Btn_delete() {
    auto dialog = DIALOG::Create_DIsVerify("确认需要删除？");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this]() {
        for (int i = table->rowCount() - 1; i >= 0; i += -1) {
            if (m_items[i].checkBox->isChecked()) {
                auto item = m_items[i].entity;
                m_items.removeAt(i);
                item->Delete_toDB();
                table->removeRow(i);
            }
        }
    });
}

void HistorySample::Btn_showDetail() {
    const int idx = table->currentRow();
    if (idx < 0) {
        DIALOG::Create_DWarningDialog("未选中有效项");
        return;
    }

    ui->tableWidget->hide();

    assert(m_ui_historyDetail == nullptr);
    m_ui_historyDetail = new SampleHistoryDetail(this);
    m_ui_historyDetail->setAttribute(Qt::WA_DeleteOnClose);
    m_ui_historyDetail->Set_entity(m_items[idx].entity);
    m_ui_historyDetail->show();
}

void HistorySample::Btn_retest() {
    const int idx = table->currentRow();
    if (idx < 0) {
        DIALOG::Create_DWarningDialog("未选中有效项");
        return;
    }
    auto entity = m_items[idx].entity;
    auto&& lte = LTE_LBS::instance();
    lte.Send_Data(entity);

}

void HistorySample::Btn_print() {
//    const int idx = table->currentRow();
//    if (idx < 0) {
//        DIALOG::Create_DWarningDialog("未选中有效项");
//        return;
//    }
    auto get_data_from_config = [](QString project) -> QMap<QString, QString> {
        using namespace CONTROLLER;
        auto    fileCU = Create_ProjectsFolderController();
        QString path   = fileCU.Get_projectPath(project);
        QFile   file(path);

        if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
            qWarning() << QString("File[%1] exist[%2] open[%3]")
                              .arg(path)
                              .arg(file.exists())
                              .arg(file.isOpen());
            return {};
        }

        QJsonObject json = QJsonDocument::fromJson(file.readAll()).object();
        if (json.isEmpty()) {
            qWarning() << "JSON 文件解析失败";
            return {};
        }

        QJsonObject OtherMessage   = json.value(OM::OtherMessage).toObject();
        QJsonObject projectMessage = OtherMessage.value(OM::ProjectMessage).toObject();

        const QString Default_Unit       = projectMessage.value(OM::Default_Unit).toString();
        const QString Reference_range    = projectMessage.value(OM::Reference_range).toString();
        const QString Result_Explanation = projectMessage.value(OM::Result_Explanation).toString();
        const QString ProjectName_Ch     = projectMessage.value(OM::ProjectName_Ch).toString();
        QMap<QString, QString> result;
        result.insert(OM::Default_Unit, Default_Unit);
        result.insert(OM::Reference_range, Reference_range);
        result.insert(OM::Result_Explanation, Result_Explanation);
        result.insert(OM::ProjectName_Ch, ProjectName_Ch);

        return result;
    };

    auto get_data_from_sample = [](CONTROLLER::Sample::entity_item sample) -> QMap<QString,QString>{
        using attr = CONTROLLER::Sample::entity_attr;
        const QString patient_name  = sample->table[attr::patient_name];
        const QString patient_ageNum = sample->table[attr::patient_ageNum];
        const QString patient_ageUnit = sample->table[attr::patient_ageUnit];
        const QString patient_sex = sample->table[attr::patient_sex];
        const QString patient_patientId = sample->table[attr::number];
        const QString patient_department = sample->table[attr::patient_department];
        const QString patient_bedId = sample->table[attr::patient_bedId];
        const QString patient_inspection = sample->table[attr::patient_inspection];
        const QString patient_verifier = sample->table[attr::patient_verifier];
        const QString patient_auditor = sample->table[attr::patient_auditor];
        const QString patient_remark = sample->table[attr::patient_remark];

        QMap<QString, QString> result;
        result.insert(PRINTER::KeySample_history::patient_name, patient_name);
        result.insert(PRINTER::KeySample_history::patient_age, patient_ageNum);
        result.insert(PRINTER::KeySample_history::patient_ageUnit, patient_ageUnit);
        result.insert(PRINTER::KeySample_history::patient_sex, patient_sex);
        result.insert(PRINTER::KeySample_history::patient_patientId, patient_patientId);
        result.insert(PRINTER::KeySample_history::patient_department, patient_department);
        result.insert(PRINTER::KeySample_history::patient_bedId, patient_bedId);
        result.insert(PRINTER::KeySample_history::patient_inspection, patient_inspection);
        result.insert(PRINTER::KeySample_history::patient_verifier, patient_verifier);
        result.insert(PRINTER::KeySample_history::patient_auditor, patient_auditor);
        result.insert(PRINTER::KeySample_history::patient_remark, patient_remark);
        return result;
    };


    using namespace WIDGET::SETTING;
    ENTITY::Printer         printerConfig;
    ENTITY::PrinterSettings settings = printerConfig.LoadFromJson();
    QJsonObject             json,finalJson;
    QJsonArray printerListArray;
    for (int i = 0; i < table->rowCount() - 1; i += 1) {
        if (m_items[i].checkBox->isChecked()) {
            using namespace PRINTER;
            auto entity       = m_items[i].entity;
            auto idReagent    = entity->table[entity_attr::idReagent];
            auto reagent      = CONTROLLER::Reagent::instance().Select_byId_fromDB(idReagent);
            using attr_sample = CONTROLLER::Reagent::entity_attr;

            auto idSample    = entity->table[entity_attr::idSample];
            auto sample   = CONTROLLER::Sample::instance().Select_byId_fromDB(idSample);

            QMap<QString, QString> res = get_data_from_config(entity->table[entity_attr::project]);
            QMap<QString, QString> res_sample = get_data_from_sample(sample);

            if (settings.printmode == Setting::Thermal_Report) {
                json[KeySample_ts::ProjectName] = entity->table[entity_attr::project];
                if (reagent && reagent->table.contains(attr_sample::batchNum)) {
                    json[KeySample_ts::ReagentBatchNumber] = reagent->table[attr_sample::batchNum];
                } else {
                    json[KeySample_ts::ReagentBatchNumber] = "未设置";
                }
                json[KeySample_ts::Result] = entity->table[entity_attr::resultData] + res[OM::Default_Unit];
                json[KeySample_ts::Reference_range] = res[OM::Reference_range];
                json[KeySample_ts::FinishTime]      = entity->table[entity_attr::timeEnd];
                QJsonArray arr;
                arr.push_back(json);
                json                               = QJsonObject{};
                json[KeySample_ts::PrinterList_ts] = arr;
            }
            else if (settings.printmode == Setting::Sample_Report)  // A4
            {
                json[KeySample::SequenceNumber] = QString("1");
                json[KeySample::ProjectName]    = entity->table[entity_attr::project];
                json[KeySample::Result]         = entity->table[entity_attr::resultData];
                json[KeySample::Unit]           = res[OM::Default_Unit];
                json[KeySample::Luminous]       = entity->table[entity_attr::expData];
                if (reagent && reagent->table.contains(attr_sample::batchNum)) {
                    json[KeySample::ReagentBatchNumber] = reagent->table[attr_sample::batchNum];
                } else {
                    json[KeySample::ReagentBatchNumber] = "未设置";
                }
                json[KeySample::SubstrateBatchNumber]   = QString("002");
                json[KeySample::CalibrationBatchNumber] = QString("003");
                json[KeySample::ApplyTime]              = entity->table[entity_attr::timeStart];
                json[KeySample::FinishTime]             = entity->table[entity_attr::timeEnd];
                json[KeySample::CalibrationTime]        = QString("2024-8-1");
                QJsonArray arr;
                arr.push_back(json);
                json                         = QJsonObject{};
                json[KeySample::PrinterList] = arr;
            }
            else if (settings.printmode == Setting::Inspection_Report)  // A5
            {
                json[KeySample_history::Result]             = entity->table[entity_attr::resultData];
                json[KeySample_history::FinishTime]         = entity->table[entity_attr::timeEnd];
                json[KeySample_history::timeStart]         = entity->table[entity_attr::timeStart];
                json[KeySample_history::Unit]               = res[OM::Default_Unit];
                json[KeySample_history::ProjectName]        = entity->table[entity_attr::project];
                json[KeySample_history::patient_type]       = entity->table[entity_attr::type];

                json[KeySample_history::ProjectName_Ch]     = res[OM::ProjectName_Ch];
                json[KeySample_history::Reference_range]    = res[OM::Reference_range];
                json[KeySample_history::Result_explanation] = res[OM::Result_Explanation];

                json[KeySample_history::patient_name] = res_sample[KeySample_history::patient_name];
                json[KeySample_history::patient_age]  = res_sample[KeySample_history::patient_age];
                json[KeySample_history::patient_ageUnit] = res_sample[KeySample_history::patient_ageUnit];
                json[KeySample_history::patient_patientId] = res_sample[KeySample_history::patient_patientId];
                json[KeySample_history::patient_sex]  = res_sample[KeySample_history::patient_sex];
                json[KeySample_history::patient_department] = res_sample[KeySample_history::patient_department];
                json[KeySample_history::patient_bedId]= res_sample[KeySample_history::patient_bedId];
                json[KeySample_history::patient_inspection] = res_sample[KeySample_history::patient_inspection];
                json[KeySample_history::patient_verifier] = res_sample[KeySample_history::patient_verifier];
                json[KeySample_history::patient_auditor] = res_sample[KeySample_history::patient_auditor];
                json[KeySample_history::patient_remark] = res_sample[KeySample_history::patient_remark];
                printerListArray.push_back(json); // 直接加到总数组
            }
            finalJson[KeySample_history::PrinterList_history] = printerListArray;
        }
    }

    // 打印 json（缩进格式）
    //qDebug().noquote() << QJsonDocument(finalJson).toJson(QJsonDocument::Indented);

    // 根据设置中打印预览是否开启，选择是直接打印还是弹出预览打印界面
    if (settings.preview_mode == "打开") {
        printer = new PRINTER::PrinterSample();
        QPrintPreviewWidget* widget =
            printer->Create_widget(finalJson, settings.res.toDouble(), nullptr);
        widget->setFixedSize(1920, 1000);
        print_preview = new WIDGET_PrintViewSample(this, widget);
        print_preview->setTemp_json(finalJson);
        print_preview->setAttribute(Qt::WA_DeleteOnClose);
        connect(print_preview, &WIDGET_PrintViewSample::send_print_data,
                [this](QJsonObject finalJson) { this->printer->Exec_print_signal(finalJson); });
        print_preview->show();
    } else {
        PRINTER::PrinterSample::Exec_print(finalJson);
    }
}

void HistorySample::Brn_lisSend() {
    const int idx = table->currentRow();
    if (idx < 0) {
        DIALOG::Create_DWarningDialog("未选中有效项");
        return;
    }

    LIS::Lis_SC1000::Send_ExpData(m_items[idx].entity); //串口
    LIS::Lis_SC1000::Send_ExpData_By_Net(m_items[idx].entity); //网口


}

void HistorySample::Btn_modify() {
    const int idx = table->currentRow();
    if (idx < 0) {
        DIALOG::Create_DWarningDialog("未选中有效项");
        return;
    }
    auto entity = m_items[idx].entity;

    auto dialog = new DSampleHistoryModify(entity, Background::Get());
    connect(dialog, &DSampleHistoryModify::signal_yes, this, [idx, dialog, this]() {
        auto entity = dialog->Get_entity();
        entity->Update_toDB();
        fill_tableLine(idx, entity);
        auto vals = Get_lineStr(entity);
    });
    dialog->show();
}

void HistorySample::Btn_query() {
    auto get_filterItems =
        [](const InfoSample::HistoryFilterInfo& info) -> QList<HistorySample::entity_item> {
        auto items = CONTROLLER::ExpSample::instance().Select_all_fromDB();
        std::reverse(items.begin(), items.end());
        QList<HistorySample::entity_item> filterItems;

        //! todo 目前的筛选条件有三条
        //! 1. 项目
        //! 2. 患者姓名
        //! 3. 时间
        for (auto item : items) {
            if (info.project.isEmpty() == false) {
                if (info.project != item->table[entity_attr::project]) {
                    continue;
                }
            }
            if (info.patientName.isEmpty() == false) {
                auto idSample = item->table[entity_attr::idSample];
                auto sample   = CONTROLLER::Sample::instance().Select_byId_fromDB(idSample);
                if (sample == nullptr) {
                    continue;
                }
                using attr_sam = CONTROLLER::Sample::entity_attr;
                if (info.patientName != sample->table[attr_sam::patient_name]) {
                    continue;
                }
            }
            if (false == item->Is_inRangeTime(
                             QDateTime::fromString(info.timeStart, InfoSample::time_format),
                             QDateTime::fromString(info.timeEnd, InfoSample::time_format))) {
                continue;
            }

            filterItems << item;
        }

        return filterItems;
    };

    auto dialog = new DSampleHistoryFilter(Background::Get());
    connect(dialog, &DSampleHistoryFilter::signal_yes, this, [this, dialog, get_filterItems]() {
        auto info  = dialog->Get_filterInfo();
        auto items = get_filterItems(info);
        Filter_data(items);
    });
    dialog->show();
}

void HistorySample::Btn_usbExport() {
    QString path = UTILITY::Get_USBDiskPath();
    if (path.isEmpty() == true) {
        DIALOG::Create_DWarningDialog("未检测到U盘");
        return;
    }
    path +=
        QString("sc1000-data/output/") +
        QString("exportdata_%1.csv").arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));

    qDebug() << path;

    QList<QList<QString>> content;
    QList<QString>        line;
    for (int col = 1; col < table->columnCount(); ++col) {
        QTableWidgetItem* headerItem = table->horizontalHeaderItem(col);
        if (headerItem) {
            line << headerItem->text();
        } else {
            line << "";
        }
    }
    content << std::move(line);

    for (int i = 0; i < m_items.size(); i += 1) {
        if (m_items[i].checkBox->isChecked()) {
            line.clear();
            for (int j = 1; j < table->columnCount(); j += 1) {
                line << table->item(i, j)->text();
            }
            content << std::move(line);
        }
    }

    if (UTILITY::Save_toCSV(path, content)) {
        DIALOG::Create_DInfoDialog("保存成功");
    } else {
        DIALOG::Create_DWarningDialog("保存失败");
    }
}

void HistorySample::Btn_selectAllOrCancel() {
    bool isAll = true;
    for (auto&& item : m_items) {
        if (item.checkBox->isChecked() == false) {
            isAll = false;
            break;
        }
    }

    for (auto&& item : m_items) {
        item.checkBox->setChecked(!isAll);
    }
}

void HistorySample::timerEvent(QTimerEvent* event) {
    for (int i = 0; i < m_items.size(); i += 1) {
        auto&& entity = m_items[i].entity;
        if (entity->table[entity_attr::timeEnd].isEmpty() == false) {
            continue;
        }
        entity->Sync_fromDB();
        fill_tableLine(i, entity);
    }
}

void HistorySample::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::HISTORY
