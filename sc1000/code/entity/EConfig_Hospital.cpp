#include "EConfig_Hospital.h"

namespace ENTITY {

namespace KeyHospital {
const QString Hospital       = "Hospital";        /// 组名
const QString hospitalName   = "hospitalName";    /// 医院名称
const QString departmentName = "departmentName";  /// 部门名称
const QString departmentList = "departmentList";  /// 部门列表
const QString doctorList     = "doctorList";      /// 医生列表
const QString name           = "name";            /// 医生-姓名
const QString canInspect     = "canInspect";      /// 医生-可送检
const QString canTest        = "canTest";         /// 医生-可测试
const QString canAudit       = "canAudit";        /// 医生-可审核
const QString remark         = "remark";          /// 医生-备注
}  // namespace KeyHospital

Hospital::Hospital() {
    namespace Key = KeyHospital;
    json          = m_ojson[Key::Hospital].toObject();

    hospitalName   = json[Key::hospitalName].toString();
    departmentName = json[Key::departmentName].toString();

    QJsonArray arr = json[Key::departmentList].toArray();
    for (auto item : arr) {
        departmentList << item.toString();
    }

    arr = json[Key::doctorList].toArray();
    for (auto&& item : arr) {
        QJsonObject json = item.toObject();
        doctorList << Doctor::to_Doctor(json);
    }
}

bool Hospital::Save() {
    namespace Key             = KeyHospital;
    json[Key::hospitalName]   = hospitalName;
    json[Key::departmentName] = departmentName;

    QJsonArray arr;
    for (auto item : departmentList) {
        arr << item;
    }
    json[Key::departmentList] = arr;

    arr = QJsonArray();
    for (auto&& item : doctorList) {
        arr << item.to_json();
    }
    json[Key::doctorList] = arr;

    m_ojson[Key::Hospital] = json;
    return IConfig::Save();
}

QString Hospital::Doctor::to_string() const {
    QJsonObject json = to_json();
    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

QJsonObject Hospital::Doctor::to_json() const {
    namespace Key = KeyHospital;
    QJsonObject json;
    json[Key::name]       = name;
    json[Key::canInspect] = canInspect;
    json[Key::canTest]    = canTest;
    json[Key::canAudit]   = canAudit;
    json[Key::remark]     = remark;
    return json;
}

Hospital::Doctor Hospital::Doctor::to_Doctor(const QJsonObject& json) {
    namespace Key = KeyHospital;
    Doctor obj;
    obj.name       = json[Key::name].toString();
    obj.canInspect = json[Key::canInspect].toInt();
    obj.canTest    = json[Key::canTest].toInt();
    obj.canAudit   = json[Key::canAudit].toInt();
    obj.remark     = json[Key::remark].toString();
    return obj;
}

}  // namespace ENTITY
