#include "HashCalculator.h"
#include <QFile>
#include <QIODevice>
#include <QDebug>
QByteArray HashCalculator::hashString(const QString& input,
                                    Algorithm algorithm,
                                    OutputFormat format)
{
    QByteArray byteArray = input.toUtf8();
    return computeHash(byteArray, algorithm, format);
}

QByteArray HashCalculator::hashData(const QByteArray& data,
                                  Algorithm algorithm,
                                  OutputFormat format)
{
    return computeHash(data, algorithm, format);
}

QByteArray HashCalculator::hashFile(const QString& filePath,
                                  Algorithm algorithm,
                                  OutputFormat format)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开文件:" << filePath;
        return QByteArray();
    }

    QCryptographicHash hash(static_cast<QCryptographicHash::Algorithm>(algorithm));
    if (hash.addData(&file)) {
        QByteArray result = hash.result();
        return formatResult(result, format);
    }

    qWarning() << "计算文件哈希失败:" << filePath;
    return QByteArray();
}

QByteArray HashCalculator::md5(const QString& input, OutputFormat format)
{
    return hashString(input, Algorithm::Md5, format);
}

QByteArray HashCalculator::sha1(const QString& input, OutputFormat format)
{
    return hashString(input, Algorithm::Sha1, format);
}

QByteArray HashCalculator::sha256(const QString& input, OutputFormat format)
{
    return hashString(input, Algorithm::Sha256, format);
}

QByteArray HashCalculator::sha512(const QString& input, OutputFormat format)
{
    return hashString(input, Algorithm::Sha512, format);
}

bool HashCalculator::verifyHash(const QString& input,
                              const QByteArray& expectedHash,
                              Algorithm algorithm,
                              OutputFormat format)
{
    QByteArray computedHash = hashString(input, algorithm, format);
    return computedHash == expectedHash;
}

bool HashCalculator::verifyFileHash(const QString& filePath,
                                  const QByteArray& expectedHash,
                                  Algorithm algorithm,
                                  OutputFormat format)
{
    QByteArray computedHash = hashFile(filePath, algorithm, format);
    return computedHash == expectedHash;
}

QString HashCalculator::algorithmName(Algorithm algorithm)
{
    switch (algorithm) {
        case Algorithm::Md4: return "MD4";
        case Algorithm::Md5: return "MD5";
        case Algorithm::Sha1: return "SHA1";
        case Algorithm::Sha224: return "SHA224";
        case Algorithm::Sha256: return "SHA256";
        case Algorithm::Sha384: return "SHA384";
        case Algorithm::Sha512: return "SHA512";
        case Algorithm::Sha3_224: return "SHA3-224";
        case Algorithm::Sha3_256: return "SHA3-256";
        case Algorithm::Sha3_384: return "SHA3-384";
        case Algorithm::Sha3_512: return "SHA3-512";
        default: return "Unknown";
    }
}

QString HashCalculator::formatName(OutputFormat format)
{
    switch (format) {
        case OutputFormat::Hex: return "Hex";
        case OutputFormat::Base64: return "Base64";
        case OutputFormat::Raw: return "Raw";
        default: return "Unknown";
    }
}

QByteArray HashCalculator::computeHash(const QByteArray& data,
                                     Algorithm algorithm,
                                     OutputFormat format)
{
    QCryptographicHash hash(static_cast<QCryptographicHash::Algorithm>(algorithm));
    hash.addData(data);
    QByteArray result = hash.result();
    return formatResult(result, format);
}

QByteArray HashCalculator::formatResult(const QByteArray& rawHash, OutputFormat format)
{
    switch (format) {
        case OutputFormat::Hex:
            return rawHash.toHex();
        case OutputFormat::Base64:
            return rawHash.toBase64();
        case OutputFormat::Raw:
            return rawHash;
        default:
            return rawHash.toHex();
    }
}
