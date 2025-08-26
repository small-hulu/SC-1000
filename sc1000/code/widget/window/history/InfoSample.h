#ifndef DATASAMPLE_H_1722308943
#define DATASAMPLE_H_1722308943

namespace WIDGET::HISTORY {

/**
 * 在sample模块中，和ui中获取数据的对应的数据结构
 */
struct InfoSample {
    static inline const char* const time_format = "yyyy-MM-dd";

    struct PatientInfo {
        QString name;        // 姓名
        QString ageNum;      // 年龄
        QString ageUnit;     // 年龄的单位
        QString sex;         // 性别
        QString patientId;   // 病人id
        QString department;  // 科室
        QString bedId;       // 床号
        QString inspection;  // 送检者
        QString verifier;    // 检验者
        QString auditor;     // 审核者
        QString remark;      // 备注
    };

    struct ApplyInfo {
        PatientInfo patientInfo;  // 患者信息

        QString        location;     // 申请孔位
        QString        number;       // 申请编号
        QString        detectType;   // 检测类型
        QString        barCode;      // 条码
        QList<QString> projectList;  // 项目列表
    };

    struct HistoryFilterInfo {
        QString timeStart;    // 时间 (年月日)
        QString timeEnd;      // 时间 (年月日)
        QString project;      // 项目
        QString patientName;  // 患者姓名
    };
};
}  // namespace WIDGET::HISTORY
#endif  // DATASAMPLE_H_1722308943
