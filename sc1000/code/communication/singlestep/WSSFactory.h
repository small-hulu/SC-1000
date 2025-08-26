#ifndef FACTORY_H_1723770250
#define FACTORY_H_1723770250

#include <QString>
#include <QWidget>

namespace COM::SINGLEWSTEP {
QList<QPair<QString, QWidget*>> Create_ssWidgetList(QWidget* parent);
}

#endif  // FACTORY_H_1723770250
