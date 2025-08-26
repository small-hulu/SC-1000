#ifndef WIDGET_OutputConfig_H_1724311324
#define WIDGET_OutputConfig_H_1724311324

#include <QWidget>

#include "widget/modules/WModule_ButtonBottomGroup.h"
#include "Lis_net_con.h"
#include "controller/CONTROLLER_Config.h"
namespace Ui {
class LISConfig;
}

namespace WIDGET::SETTING {

class LISConfig : public QWidget {
    Q_OBJECT

private:
    Ui::LISConfig* ui = nullptr;

private:
    ButtonBottomGroup* m_ui_btnGroup = nullptr;

private:
    /// todo
    /// 目前紧急使用，后期要作为全局的相关配置
    QMap<int, QString> mpLISDirection;
    QMap<int, QString> mpLISUpload;

public:
    LISConfig(QWidget* parent = nullptr);
    ~LISConfig();

private:
    void save();
    void lis_net_connect();
protected:
private slots:
    void on_pushButton_clicked();
    void showEvent(QShowEvent* event) override;
};

}  // namespace WIDGET::SETTING

#endif  // WIDGET_OutputConfig_H_1724311324
