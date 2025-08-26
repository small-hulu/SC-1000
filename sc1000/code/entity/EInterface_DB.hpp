#ifndef EINTERFACE_DB_HPP_1720166430
#define EINTERFACE_DB_HPP_1720166430

#include <QHash>
#include <QString>
#include <QVector>
#include <memory>

#include "database/DB.hpp"
#include "utility/UTILITY_Hash.h"

namespace ENTITY {

template <typename DBType>
class EInterface_DB {
public:
    static inline const char* const time_format     = "yyyy-MM-dd hh:mm:ss";
    static inline const char        join_separator  = ',';
    static inline const char        join_separator2 = ';';

public:
    using id_type  = int;
    using db_table = DBType;
    using attr     = typename db_table::table_attribute;

public:
    static QDateTime Generate_stringToDateTime(const QString& s) {
        return QDateTime::fromString(s, time_format);
    }

    static QString Generate_curTime() {
        return QDateTime::currentDateTime().toString(time_format);
    }

    static QList<QString> Get_attrListNames(const QList<attr>& attrList) {
        db_table       dbtable{};
        QList<QString> ret;
        for (auto&& i : attrList) {
            ret << dbtable.Get_chineseEnum(i);
        }
        return ret;
    }

public:
    db_table             cdbtable;
    id_type              id = 0;
    QHash<attr, QString> table{};

public:
    virtual ~EInterface_DB() = default;

public:
    virtual QString to_string() {
        QString str = QString("idx=%1\n").arg(id);
        for (int i = 0; i < attr::Enum_Count; i += 1) {
            QString txt = QString("{%1:%2}\n").arg(cdbtable.Get_chineseEnum(i)).arg(table[(attr)i]);
            str += txt;
        }
        return str;
    }

public:
    int Get_idInt() {
        return id;
    }

    QString Get_idString() {
        return QString::number(id);
    }

    virtual void Set_id(const id_type id) {
        this->id = id;
    }

    virtual void Set_id(const QString& id) {
        Set_id(id.toInt());
    }

public:
    virtual std::size_t Get_hash() {
        return id;
    }

    virtual bool Is_valid() {
        return id != 0;
    }

    virtual QList<QString> Get_allValueList() {
        QList<QString> ret;
        for (int i = 0; i < attr::Enum_Count; i += 1) {
            ret << table[(attr)i];
        }
        return ret;
    }

public:
    virtual QList<QString> Get_attrListValues(const QList<attr>& attrList) {
        QList<QString> msg;
        for (auto&& i : attrList) {
            msg << table[i];
        }
        return msg;
    }

    virtual void Set_attrListValues(const QList<attr>& attrList, const QList<QString>& msgList) {
        for (int i = 0; i < msgList.size(); i += 1) {
            table[attrList[i]] = msgList[i];
        }
    }

    virtual void Set_Values_WithId(const QList<QString>& msgList) {
        assert(msgList.size() - 1 == attr::Enum_Count);
        Set_id(msgList[0]);
        for (int i = 1; i < msgList.size(); i += 1) {
            table[attr(i - 1)] = msgList[i];
        }
    }

    virtual void Set_Values_WithOutId(const QList<QString>& msgList) {
        assert(msgList.size() == attr::Enum_Count);
        for (int i = 0; i < msgList.size(); i += 1) {
            table[attr(i)] = msgList[i];
        }
    }

public:
    int Insert_toDB() {
        return id = DB::SqlExe::Insert_oneLine(cdbtable, Get_allValueList());
    }

    bool Update_toDB() {
        return DB::SqlExe::Update_oneLine_byId(cdbtable, Get_allValueList(), id);
    }

    bool Delete_toDB() {
        return DB::SqlExe::Delete_oneLine_byId(cdbtable, id);
    }

public:
    void Sync_fromDB() {
        auto line = DB::SqlExe::Select_oneLine_byId(cdbtable, id);
        this->Set_Values_WithId(line);
    }

public:
    virtual bool Is_inRangeTime(QDateTime startTime, QDateTime endTime) {
        assert(startTime <= endTime);
        return true;
    }
};
}  // namespace ENTITY
#endif  // EINTERFACE_DB_HPP_1720166430
