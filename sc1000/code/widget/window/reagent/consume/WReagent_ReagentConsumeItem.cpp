#include "WReagent_ReagentConsumeItem.h"

#include "ui_WReagent_ReagentConsumeItem.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::REAGENT {
ReagentConsumeItem::ReagentConsumeItem(QWidget *parent)
    : QWidget(parent), ui(new Ui::ReagentConsumeItem) {
    ui->setupUi(this);

    {
        ui->btn_foreground_color->setAttribute(Qt::WA_TransparentForMouseEvents);
        ui->lab_hint->setAttribute(Qt::WA_TransparentForMouseEvents);
        connect(ui->btn_background_color, &QPushButton::clicked, this,
                [this]() { emit signal_clicked(); });
    }

    auto table = ui->tableWidget;
    UTILS::init_table_mode(table);
    UTILS::init_table_no_header(table);
    UTILS::init_table_titles(table, {""});
}

ReagentConsumeItem::~ReagentConsumeItem() {
    delete ui;
}

void ReagentConsumeItem::Set_title(const QString &title) {
    ui->lab_title->setText(title);
}

void ReagentConsumeItem::Set_hint(const QString &msg) {
    ui->lab_hint->setText(msg);
}

void ReagentConsumeItem::Set_ratio(double ratio) {
    auto background = ui->btn_background_color;
    auto foreground = ui->btn_foreground_color;

    QRect rect;
    rect.setX(background->x());
    rect.setY(background->y() + (1.0 - ratio) * background->height());
    rect.setWidth(background->width());
    rect.setHeight(background->height() * ratio);

    foreground->setGeometry(rect);
}

void ReagentConsumeItem::Set_descriptorMessage(const QList<QString> &msgList) {
    auto table = ui->tableWidget;
    UTILS::init_table_titles(table, {""});
    QList<QList<QString>> content;
    for (auto &&s : msgList) {
        content << (QList<QString>() << s);
    }
    UTILS::init_table_fix_content(table, content);
}
}  // namespace WIDGET::REAGENT
