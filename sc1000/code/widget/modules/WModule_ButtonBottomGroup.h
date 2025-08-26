#ifndef WMODULE_BUTTONBOTTOMGROUP_H_1721011010
#define WMODULE_BUTTONBOTTOMGROUP_H_1721011010

#include <QPushButton>
#include <QWidget>

namespace Ui {
class ButtonBottomGroup;
}

namespace WIDGET {
class ButtonBottomGroup : public QWidget {
    Q_OBJECT
public:
    static ButtonBottomGroup* globalPtr;
    static void               Set(ButtonBottomGroup* ptr) {
        assert(ptr);
        globalPtr = ptr;
    }

    static ButtonBottomGroup* Get() {
        assert(globalPtr);
        return globalPtr;
    }

private:
    Ui::ButtonBottomGroup* ui = nullptr;

private:
    QVector<QPushButton*>              m_BtnListView;  /// 底部的按钮组
    QVector<std::function<void(void)>> m_BtnListFunc;  /// 底部的按钮组

    std::function<void(void)> m_refreshFunc;

public:
    explicit ButtonBottomGroup(QWidget* parent = nullptr);
    ~ButtonBottomGroup();

public:
    /// 设置
    /// + 按钮名称
    /// + 按钮功能
    /// =========
    /// 两个列表的长度一致
    void Set_BtnList(const QList<QString>&                   btnTextList,
                     const QList<std::function<void(void)>>& btnFuncList);
    void Set_BtnTextList(const QList<QString>& btnTextList);
    /// 设置刷新函数
    void Set_refreshFunc(const std::function<void(void)>& func);
    void RemoveAllButtons();

protected:
    void paintEvent(QPaintEvent* event) override;

    void showEvent(QShowEvent* event) override;
};
}  // namespace WIDGET

#endif  // WMODULE_BUTTONBOTTOMGROUP_H_1721011010
