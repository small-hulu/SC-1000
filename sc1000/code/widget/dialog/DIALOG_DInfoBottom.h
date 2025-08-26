#ifndef WIDGET_INFODIALOG_H_1695626188
#define WIDGET_INFODIALOG_H_1695626188

#include <QWidget>
#include <set>
#include <unordered_map>

namespace Ui {
class DInfoBottom;
}

namespace WIDGET::DIALOG {

class DInfoDialog final : public QWidget {
    Q_OBJECT

private:  // member data (.)
    static int Gap_y;
    using _Interval = ::std::tuple<int, int>;
    using _Set      = ::std::set<_Interval>;
    using _Map      = ::std::unordered_map<const QWidget *, _Set>;
    static _Map Map_heap;

private:
    Ui::DInfoBottom *ui = nullptr;
    int              y_pos;
    int              y_height;

public:  // member function (.)
    explicit DInfoDialog(QWidget *parent, const QString &text);
    virtual ~DInfoDialog();

    void show();
    bool close();
};

}  // namespace WIDGET::DIALOG

#endif  // WIDGET_INFODIALOG_H_1695626188
