#include "WHomepage_DeviceState.h"

#include "ui_WHomepage_DeviceState.h"
#include "widget/utils/UTILS_Resize.h"

namespace WIDGET {
namespace HOMEPAGE {
DeviceState::DeviceState(QWidget *parent) : QWidget(parent), ui(new Ui::DeviceState) {
    ui->setupUi(this);
}

DeviceState::~DeviceState() {
    delete ui;
}

void DeviceState::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace HOMEPAGE
}  // namespace WIDGET
