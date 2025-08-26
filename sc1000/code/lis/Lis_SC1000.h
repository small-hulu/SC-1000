#ifndef LIS_SC1000_H_1742283811
#define LIS_SC1000_H_1742283811

#include "Lis.h"
#include "controller/CONTROLLER_ExpSample.h"
#include "widget/window/setting/lisManage/Lis_net_con.h"
#include <QTextCodec>
namespace LIS {

struct Lis_SC1000 {
private:
    static void send_agent(const QString& data, bool isForce);

public:
    static void Set_defaultInfo(Lis::ComInfo& comInfo);

    static void Send_ExpData(CONTROLLER::ExpSample::entity_item entity, bool isForce = true);
    static void Send_ExpData_By_Net(CONTROLLER::ExpSample::entity_item entity);
    static void Send_singleMessage(const QString& data, bool isForce = true);
};
}  // namespace LIS

#endif  // LIS_SC1000_H_1742283811
