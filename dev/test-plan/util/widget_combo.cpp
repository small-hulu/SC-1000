#include "widget_combo.h"

#include <QListView>

namespace UTIL {

void combo_init(QComboBox *combo) {
    combo->clear();
    combo->setView(new QListView());
}

}  // namespace UTIL
