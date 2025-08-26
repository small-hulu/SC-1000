#ifndef DATASAMPLE_H_1722308943
#define DATASAMPLE_H_1722308943

#include <QList>
#include <QString>

namespace WIDGET::SAMPLE {

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

        void Reset() {
            *this = PatientInfo{};
        }
    };

    struct ApplyInfo {
        bool           lis_sign = false;        // lis标志位
        QString        emergency_sign;          // 急诊
        int            applyCount;              // 申请个数
        PatientInfo    patientInfo;             // 患者信息
        QString        lab_barcode;             // 判断是否为批量申请
        QString        location;                // 申请孔位
        QString        userEditNumber;          // 申请编号
        QString        detectType;              // 检测类型
        QString        sampleTube;              // 样本管类型
        QString        barCode;                 // 条码
        QList<QString> projectList;             // 项目列表
        QString        projectList_MerageName;  // 项目合并的合并名字
        QString        project_Merage;          // 合并项目的单独项目
        QString        project;                 // LIS项目

        void Reset() {
            *this = ApplyInfo{};
        }
    };

    struct HistoryFilterInfo {
        QString timeStart;    // 时间 (年月日)
        QString timeEnd;      // 时间 (年月日)
        QString project;      // 项目
        QString patientName;  // 患者姓名

        void Reset() {
            *this = HistoryFilterInfo{};
        }
    };
};
}  // namespace WIDGET::SAMPLE
#endif  // DATASAMPLE_H_1722308943
