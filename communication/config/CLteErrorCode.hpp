#ifndef CLTEERRORCODE_HPP_1744613670
#define CLTEERRORCODE_HPP_1744613670

#include <QMetaEnum>
#include <QObject>

namespace COM {

class CLteErrorCode : public QObject {
    Q_OBJECT

public:
    enum class ErrorCode : int {
        SCNoError = 0,
        SCModbusError,
        SCForceStop,

        /// [10]
        SC1001,
        SC1009,
        SC1012,
        SC1013,
        /// [10]

        /// [11]
        SC1101,
        SC1109,
        SC1110,
        SC1111,
        SC1112,
        SC1113,
        SC1114,
        SC1115,
        /// [11]

        /// [12]
        SC1201,
        SC1207,
        SC1208,
        SC1209,
        SC1210,
        SC1211,
        SC1212,
        SC1213,
        SC1214,
        SC1215,
        /// [12]

        /// [13]
        SC1301,
        SC1307,
        SC1308,
        SC1309,
        SC1310,
        SC1311,
        SC1312,
        SC1313,
        SC1314,
        SC1315,
        /// [13]

        /// [14]
        SC14100308,
        SC14100313,
        SC14100314,
        SC14100315,
        SC14100508,
        SC14100513,
        SC14100514,
        SC14100515,
        SC14100708,
        SC14100713,
        SC14100714,
        SC14100715,
        /// [14]

        /// [15]
        SC1508,
        SC1512,
        SC1513,
        SC1514,
        SC1515,
        /// [15]

        /// [16]
        SC16100200,
        SC16100202,
        SC16100203,
        SC16100204,
        SC16100700,
        SC16100702,
        SC16100703,
        SC16100704,
        SC16101400,
        SC16101402,
        SC16101403,
        SC16101404,
        SC16102000,
        SC16102002,
        SC16102003,
        SC16102004,
        /// [16]

        SCUnknownError
    };
    Q_ENUM(ErrorCode);

    struct ErrorPkg {
        ErrorCode code = ErrorCode::SCNoError;
        QString   errorStr;

        ErrorPkg() = default;

        ErrorPkg(ErrorCode code, const QString& msg) : code(code), errorStr(msg) {}

        ErrorPkg(const ErrorPkg&)            = default;
        ErrorPkg(ErrorPkg&&)                 = default;
        ErrorPkg& operator=(const ErrorPkg&) = default;
        ErrorPkg& operator=(ErrorPkg&&)      = default;
    };
};
}  // namespace COM
#endif  // CLTEERRORCODE_HPP_1744613670
