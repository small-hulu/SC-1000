#ifndef HASHCALCULATOR_H
#define HASHCALCULATOR_H

#include <QCryptographicHash>
#include <QString>
#include <QByteArray>

class HashCalculator
{
public:
    enum class Algorithm {
        Md4 = QCryptographicHash::Md4,
        Md5 = QCryptographicHash::Md5,
        Sha1 = QCryptographicHash::Sha1,
        Sha224 = QCryptographicHash::Sha224,
        Sha256 = QCryptographicHash::Sha256,
        Sha384 = QCryptographicHash::Sha384,
        Sha512 = QCryptographicHash::Sha512,
        Sha3_224 = QCryptographicHash::Sha3_224,
        Sha3_256 = QCryptographicHash::Sha3_256,
        Sha3_384 = QCryptographicHash::Sha3_384,
        Sha3_512 = QCryptographicHash::Sha3_512
    };


    enum class OutputFormat {
        Hex,
        Base64,
        Raw
    };

    static QByteArray hashString(const QString& input,
                                Algorithm algorithm = Algorithm::Md5,
                                OutputFormat format = OutputFormat::Hex);


    static QByteArray hashData(const QByteArray& data,
                              Algorithm algorithm = Algorithm::Md5,
                              OutputFormat format = OutputFormat::Hex);


    static QByteArray hashFile(const QString& filePath,
                              Algorithm algorithm = Algorithm::Md5,
                              OutputFormat format = OutputFormat::Hex);


    static QByteArray md5(const QString& input, OutputFormat format = OutputFormat::Hex);
    static QByteArray sha1(const QString& input, OutputFormat format = OutputFormat::Hex);
    static QByteArray sha256(const QString& input, OutputFormat format = OutputFormat::Hex);
    static QByteArray sha512(const QString& input, OutputFormat format = OutputFormat::Hex);


    static bool verifyHash(const QString& input,
                         const QByteArray& expectedHash,
                         Algorithm algorithm = Algorithm::Md5,
                         OutputFormat format = OutputFormat::Hex);


    static bool verifyFileHash(const QString& filePath,
                             const QByteArray& expectedHash,
                             Algorithm algorithm = Algorithm::Md5,
                             OutputFormat format = OutputFormat::Hex);


    static QString algorithmName(Algorithm algorithm);


    static QString formatName(OutputFormat format);

private:
    static QByteArray computeHash(const QByteArray& data,
                                 Algorithm algorithm,
                                 OutputFormat format);

    static QByteArray formatResult(const QByteArray& rawHash, OutputFormat format);
};

#endif // HASHCALCULATOR_H
