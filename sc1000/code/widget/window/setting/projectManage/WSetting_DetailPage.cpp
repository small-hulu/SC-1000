#include "WSetting_DetailPage.h"

#include "ui_WSetting_DetailPage.h"
#include "widget/modules/WModule_TableHeader.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SETTING {
DetailPage::DetailPage(QWidget* parent) : QWidget(parent), ui(new Ui::DetailPage) {
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_StyledBackground);
    /// title
    {
        QList<QWidget*> headerList;
        headerList << ui->widget_1;
        headerList << ui->widget_2;
        headerList << ui->widget_3;
        headerList << ui->widget_4;
        headerList << ui->widget_5;
        headerList << ui->widget_6;
        headerList << ui->widget_7;
        QList<QString> names;
        names << QString("基本信息");
        names << QString("固定稀释");
        names << QString("全血预处理");
        names << QString("其他信息");
        names << QString("反应流程");
        names << QString("预处理");
        names << QString("试剂信息");
        assert(headerList.size() == names.size());
        for (int i = 0; i < headerList.size(); i += 1) {
            new TableHeader(names[i], headerList[i]);
        }
    }

    /// init-table style
    {
        QList<QTableWidget*> tables;
        tables << ui->tableWidget_1;
        tables << ui->tableWidget_2;
        tables << ui->tableWidget_3;
        tables << ui->tableWidget_4;
        tables << ui->tableWidget_5;
        tables << ui->tableWidget_6;
        tables << ui->tableWidget_7;
        for (auto table : tables) {
            UTILS::init_table_mode(table);
            UTILS::init_table_no_header(table);
        }
    }

    /// 按钮栏
    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
        m_ui_btnGroup->Set_refreshFunc({});
        QList<QString> titles;
        titles << QString("确认");
        QList<std::function<void()>> taskList;
        taskList << std::bind(&DetailPage::close, this);
        m_ui_btnGroup->Set_BtnList(titles, taskList);
    }
}

DetailPage::DetailPage(QString data, QWidget* parent) : DetailPage(parent) {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data.toUtf8());
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qWarning() << "Invalid JSON data";
        return;
    }
    QList<QTableWidget*> tables;
    tables << ui->tableWidget_1;
    tables << ui->tableWidget_2;
    tables << ui->tableWidget_3;
    tables << ui->tableWidget_4;
    tables << ui->tableWidget_5;
    tables << ui->tableWidget_6;
    tables << ui->tableWidget_7;

    QList<QString> titles;
    titles << QString("BaseMsessage");
    titles << QString("FixedDilution");
    titles << QString("WholeBloodPretreatment");
    titles << QString("OtherMessage");
    titles << QString("ReactionScheme");
    titles << QString("Pretreatment");
    titles << QString("ReagentMessage");

    assert(tables.size() == titles.size());

    QJsonObject jsonObj = jsonDoc.object();
    for (int i = 0; i < tables.size(); i += 1) {
        QTableWidget* table      = tables[i];
        QString       sectionKey = titles[i];
        QJsonObject   sectionObj = jsonObj[sectionKey].toObject();

        QList<QList<QString>> contentList;
        // 获取 ProjectMessage 内容并保持顺序
        QJsonObject projectMessage = sectionObj["ProjectMessage"].toObject();

        // 处理嵌套步骤，按顺序添加内容
        if (sectionKey == "ReactionScheme" || sectionKey == "Pretreatment") {
            QStringList stepKeys = projectMessage.keys();  // 保持原顺序
            for (const QString& stepKey : stepKeys) {
                QJsonObject stepObj = projectMessage[stepKey].toObject();
                QStringList keys    = stepObj.keys();  // 按原顺序处理步骤内容
                for (const QString& key : keys) {
                    contentList.append({key, stepObj[key].toString()});
                }
            }
        }
        // 对于其他部分，保持项目消息的键值对顺序
        else {
            QStringList keys = projectMessage.keys();  // 保持原顺序
            for (const QString& key : keys) {
                contentList.append({key, projectMessage[key].toString()});
            }
        }

        // 设置表格内容
        UTILS::init_table_titles(table, {"", ""});
        UTILS::init_table_fix_content(table, contentList);
    }
}

/**
 * 这里的析构不要`hind()`
 */
DetailPage::~DetailPage() {
    delete ui;
}

/**
 * 基本信息
 */
void DetailPage::table_init_1() {
    auto           table = ui->tableWidget_1;
    QList<QString> titles;
    titles << "基本信息";
    titles << "";

    UTILS::init_table_mode(table);
    UTILS::init_table_titles(table, titles);

    /// content 的长度与标题保持一致
    QList<QList<QString>> contentList;
    QList<QString>        content = titles;

    content[0] = QString("项目版本号");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("项目名");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("项目类型");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("反应模式");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("样本量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("多方法学");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("校准样本量(μL)");
    content[1] = QString("/");
    contentList << content;

    UTILS::init_table_fix_content(table, contentList);
}

/**
 * 固定稀释
 */
void DetailPage::table_init_2() {
    auto           table = ui->tableWidget_2;
    QList<QString> titles;
    titles << "固定稀释";
    titles << "";

    UTILS::init_table_mode(table);
    UTILS::init_table_titles(table, titles);

    QList<QList<QString>> contentList;
    QList<QString>        content = titles;

    content[0] = QString("固定稀释开关");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("固定稀释定标");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("固定稀释步数");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("固定稀释倍数");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("稀释后样本量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("固定稀释腔");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("固定稀释回算");
    content[1] = QString("/");
    contentList << content;

    UTILS::init_table_fix_content(table, contentList);
}

/**
 * 全血预处理
 */
void DetailPage::table_init_3() {
    auto           table = ui->tableWidget_3;
    QList<QString> titles;
    titles << "全血预处理";
    titles << "";

    UTILS::init_table_mode(table);
    UTILS::init_table_titles(table, titles);

    QList<QList<QString>> contentList;
    QList<QString>        content = titles;

    content[0] = QString("开关");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("换杯");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("孵育时间(分钟)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("吸试剂量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("预处理后吸液量(μL)");
    content[1] = QString("/");
    contentList << content;

    UTILS::init_table_fix_content(table, contentList);
}

/**
 * 其他信息
 */
void DetailPage::table_init_4() {
    auto           table = ui->tableWidget_4;
    QList<QString> titles;
    titles << "其他信息";
    titles << "";

    UTILS::init_table_mode(table);
    UTILS::init_table_titles(table, titles);

    QList<QList<QString>> contentList;
    QList<QString>        content = titles;

    content[0] = QString("默认单位");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("默认精度");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("厂商编号");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("机型");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("A瓶规格");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("B瓶规格");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("C瓶规格");
    content[1] = QString("/");
    contentList << content;

    UTILS::init_table_fix_content(table, contentList);
}

/**
 * 反应流程
 */
void DetailPage::table_init_5() {
    auto           table = ui->tableWidget_5;
    QList<QString> titles;
    titles << "反应流程";
    titles << "";

    UTILS::init_table_mode(table);
    UTILS::init_table_titles(table, titles);

    QList<QList<QString>> contentList;
    QList<QString>        content = titles;

    content[0] = QString("第一步孵育时间");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R1组分腔体");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R1组分吸液量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R2组分腔体");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R2组分吸液量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R3组分腔体");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R3组分吸液量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("第二步孵育时间");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R1组分腔体");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R1组分吸液量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R2组分腔体");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R2组分吸液量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R3组分腔体");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R3组分吸液量(μL)");
    content[1] = QString("/");
    contentList << content;

    UTILS::init_table_fix_content(table, contentList);
}

/**
 * 预处理
 */
void DetailPage::table_init_6() {
    auto           table = ui->tableWidget_6;
    QList<QString> titles;
    titles << "预处理";
    titles << "";

    UTILS::init_table_mode(table);
    UTILS::init_table_titles(table, titles);

    QList<QList<QString>> contentList;
    QList<QString>        content = titles;

    content[0] = QString("预处理开关");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("预处理步数");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("预处理换杯");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("预处理后吸液量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("第一步孵育时间");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R1组分腔体");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R1组分吸液量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R2组分腔体");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R2组分吸液量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R3组分腔体");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R3组分吸液量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("第二步孵育时间");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R1组分腔体");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R1组分吸液量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R2组分腔体");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R2组分吸液量(μL)");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R3组分腔体");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("R3组分吸液量(μL)");
    content[1] = QString("/");
    contentList << content;

    UTILS::init_table_fix_content(table, contentList);
}

/**
 * 试剂信息
 */
void DetailPage::table_init_7() {
    auto           table = ui->tableWidget_7;
    QList<QString> titles;
    titles << "试剂信息";
    titles << "";

    UTILS::init_table_mode(table);
    UTILS::init_table_titles(table, titles);

    QList<QList<QString>> contentList;
    QList<QString>        content = titles;

    content[0] = QString("A瓶a腔理论罐装量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("A瓶b腔理论罐装量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("A瓶c腔理论罐装量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("A瓶d腔理论罐装量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("B瓶a腔理论罐装量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("B瓶b腔理论罐装量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("B瓶c腔理论罐装量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("B瓶d腔理论罐装量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("C瓶a腔理论罐装量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("C瓶b腔理论罐装量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("C瓶c腔理论罐装量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("C瓶d腔理论罐装量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("A腔试剂残余量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("B腔试剂残余量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("C腔试剂残余量");
    content[1] = QString("/");
    contentList << content;
    content[0] = QString("D腔试剂残余量");
    content[1] = QString("/");
    contentList << content;

    UTILS::init_table_fix_content(table, contentList);
}

/**
 * ！《该表格隐藏》
 */
void DetailPage::table_init_8() {}

void DetailPage::showEvent(QShowEvent* event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}

}  // namespace WIDGET::SETTING
