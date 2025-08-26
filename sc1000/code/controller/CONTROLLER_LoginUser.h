#ifndef LOGINUSER_H_1725438023
#define LOGINUSER_H_1725438023

#include "entity/ENTITY_User.h"

namespace CONTROLLER {
class LoginUser {
public:
    using Self = LoginUser;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

public:
    /// 获取总表格总数
    static int Get_allCount();

    /// 根据范围获取条目
    static QList<ENTITY::User> Get_rangeEntity(int start, int end);

public:
    static bool Create_UserToDB(ENTITY::User& user);

public:
    LoginUser();

public:
    ENTITY::User user;

public:
    bool Try_login(const QString& name, const QString& password);
};
}  // namespace CONTROLLER

#endif  // LOGINUSER_H_1725438023
