#ifndef HOSPITAL_H_1725690463
#define HOSPITAL_H_1725690463

#include <QJsonArray>

#include "EInterface_IConfig.h"

namespace ENTITY {

struct Hospital : public IConfig {
public:
    /**
     * 简单数实体，可组合在类中
     */
    struct Doctor {
        QString name;                /// 姓名
        int     canInspect = false;  /// 可送检
        int     canTest    = false;  /// 可测试
        int     canAudit   = false;  /// 可审核
        QString remark;              /// 备注

        /// 相等，目前只判断姓名
        bool operator==(const Doctor& rhs) const {
            return name == rhs.name;
        }

        QString       to_string() const;
        QJsonObject   to_json() const;
        static Doctor to_Doctor(const QJsonObject& json);
    };

public:  // 不必设计仅做转发作用的get&set
    QString        hospitalName;
    QString        departmentName;
    QList<QString> departmentList;
    QList<Doctor>  doctorList;

public:
    Hospital();
    virtual bool Save() override;

public:
    void Append_department(const QString& name) {
        if (departmentList.contains(name) == false) {
            departmentList << name;
        }
    }
    void Remove_department(const QString& name) {
        if (departmentList.contains(name) == true) {
            departmentList.removeOne(name);
        }
    }

    void Append_doctor(const Doctor& doctor) {
        if (doctorList.contains(doctor) == false) {
            doctorList << doctor;
        }
    }
    void Remove_doctor(const Doctor& doctor) {
        if (doctorList.contains(doctor) == true) {
            doctorList.removeOne(doctor);
        }
    }
    void Remove_doctorByName(const QString& name) {
        for (auto&& doctor : doctorList) {
            if (doctor.name == name) {
                doctorList.removeOne(doctor);
                return;
            }
        }
    }

public:
    QList<Doctor> Get_canInspectDoctorList() {
        QList<Doctor> ret;
        for (auto&& doctor : doctorList) {
            if (doctor.canInspect) {
                ret << doctor;
            }
        }
        return ret;
    }
    QList<Doctor> Get_canTestDoctorList() {
        QList<Doctor> ret;
        for (auto&& doctor : doctorList) {
            if (doctor.canTest) {
                ret << doctor;
            }
        }
        return ret;
    }
    QList<Doctor> Get_canAuditDoctorList() {
        QList<Doctor> ret;
        for (auto&& doctor : doctorList) {
            if (doctor.canAudit) {
                ret << doctor;
            }
        }
        return ret;
    }
};

}  // namespace ENTITY

#endif  // HOSPITAL_H_1725690463
