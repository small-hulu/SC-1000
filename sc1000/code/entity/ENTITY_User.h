#ifndef USER_H_1725437328
#define USER_H_1725437328

#include <QMetaEnum>
#include <QObject>

#include "EInterface_DB.hpp"

namespace ENTITY {
class UserLevelObjcet : public QObject {
    Q_OBJECT
public:
    enum UserLevel : int {
        user = 0,  // 普通用户
        admin,
        super,
        root  // 顶级权限
    };
    Q_ENUM(UserLevel);
};

class User : public EInterface_DB<::DB::TableEntity::user> {
public:
    using UserLevel = UserLevelObjcet::UserLevel;

public:
    static QString get_levelType(int type) {
        QMetaEnum qenum = QMetaEnum::fromType<UserLevel>();
        return qenum.key(type);
    }
    static int get_levelType(QString key) {
        QMetaEnum   qenum  = QMetaEnum::fromType<UserLevel>();
        std::string stdStr = key.toStdString();
        return qenum.keyToValue(stdStr.c_str());
    }

public:
    User() = default;

public:
    UserLevel Get_level() const {
        int level = table[attr::level].toInt();
        return static_cast<UserLevel>(level);
    }
    QString Get_levelString() const {
        int level = table[attr::level].toInt();
        return get_levelType(level);
    }
};
}  // namespace ENTITY

#endif  // USER_H_1725437328
