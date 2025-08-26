#ifndef FACTORY_H_723273687
#define FACTORY_H_723273687

#include <QString>
#include <QTreeWidget>

#include "WOperationPanel.h"

namespace COM::WFIXTURE {

class FactoryFix {
public:
    static void Init_tree(QTreeWidget* tree);

    static OperationPanel* Create_opeartorWidget(const QString& title);
};
}  // namespace COM::WFIXTURE

#endif  // FACTORY_H_723273687
