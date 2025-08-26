#ifndef ECALIBRATION_H_1719971670
#define ECALIBRATION_H_1719971670

#include "EInterface_DB.hpp"
#include "qrcode/QRCODE.hpp"

namespace ENTITY {
class ECalibration : public EInterface_DB<::DB::TableEntity::calibration> {
public:
    ECalibration();

    virtual std::size_t Get_hash() override {
        return UTILITY::get_hash(id, qHash(table[attr::qrcode]));
    }

public:
    QRCODE::ECalibration Get_qrcodeEntity() {
        QRCODE::ECalibration qr(table[attr::qrcode].toUtf8());
        return qr;
    }

public:
    QString Get_Project() {
        return table[attr::project];
    }
    QString Get_bottleNumber();

public:
    QList<double> Get_c0n_ConcentrationList() {
        QString msg = table[attr::C0nConcentrationList];
        if (msg.isEmpty()) {
            return {};
        }
        auto          liststr = msg.split(join_separator);
        QList<double> res;
        for (auto s : liststr) {
            res << s.toDouble();
        }
        return res;
    }
    void Set_c0n_ConcentrationList(const QList<double>& list) {
        QList<QString> res;
        for (auto d : list) {
            res << QString::number(d);
        }
        table[attr::C0nConcentrationList] = res.join(join_separator);
    }
};
}  // namespace ENTITY

#endif  // ECALIBRATION_H_1719971670
