#include "WIDGET_SampleStateItem.h"

#include "global/GLOBAL_Type.h"
#include "ui_WIDGET_SampleStateItem.h"

namespace WIDGET::SAMPLE {
SampleStateItem::SampleStateItem(QWidget* parent) : QWidget(parent), ui(new Ui::SampleStateItem) {
    ui->setupUi(this);

    ui->lab_position->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->widget_image->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->widget_state->setAttribute(Qt::WA_TransparentForMouseEvents);

    connect(ui->pushButton, &QPushButton::clicked, this,
            [this]() { emit this->signal_clicked(this); });
}

SampleStateItem::~SampleStateItem() {
    delete ui;
}

QString SampleStateItem::to_string() const {
    QString msg;
    msg += ui->lab_position->text();
    return msg;
}

void SampleStateItem::Set_labelBottom(const QString& type) {
    ui->lab_position->setText(type);
}

void SampleStateItem::Set_labelBottom(int type) {
    Set_labelBottom(Type::Get_TubeSpaceMark((Type::ETubeSpaceMark)type));
}

void SampleStateItem::Set_StateType(int type) {
    const QString imagePath = Type::Get_TubeStateInfo((Type::ETubeState)type).pathImage;
    QString       qss       = QString("image: url(%1);");
    ui->widget_image->setStyleSheet(qss.arg(imagePath));
}

void SampleStateItem::Set_ExpType(int type) {
    if (type < 0) {
        ui->widget_state->hide();
        return;
    }
    const QString imagePath = Type::Get_TubeExpInfo((Type::ETubeExp)type).pathImage;
    QString       qss       = QString("image: url(%1);");
    ui->widget_state->setStyleSheet(qss.arg(imagePath));
    ui->widget_state->show();
}

QString SampleStateItem::Get_labelBottom() {
    return ui->lab_position->text();
}
}  // namespace WIDGET::SAMPLE
