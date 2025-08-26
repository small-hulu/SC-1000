#ifndef FRM_H_Encapsulating_Interfaces_By_Cuber_lotus
#define FRM_H_Encapsulating_Interfaces_By_Cuber_lotus

#include <QSharedPointer>

#include "frmmain.h"

class Frm {
public:
    enum Exec_Type : int { Input = 0, Num = 1 };

public:
    /// 初始化一些配置
    static void init();

    /// 启动执行
    /// 在main函数中就加载输入法面板,保证整个应用程序可以使用输入法
    static void exec(Exec_Type type = Input);

public:
    /// 设置不监控这个input
    static void set_noinput(QObject* obj);

public:
    /// 获取原库的示例样例
    static QSharedPointer<frmMain> get_exampleDemoWidget();
};

#endif  // FRM_H_Encapsulating_Interfaces_By_Cuber_lotus
