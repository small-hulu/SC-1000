#include "UTILITY_FileSuffix.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>

namespace UTILITY {
/**
 * 根据预设的后缀名进行提取文件列表
 * 注意不要给出路径和后缀名，便于直接在前端展示
 * 有专门的另外的函数可以根据单独的name生成完整路径
 */
FileSuffix::FileSuffix(QString folder, QString suffix) : m_folder(folder), m_suffix(suffix) {
    QDir().mkpath(folder);
}

QList<QString> FileSuffix::Get_ProjectList() const {
    QList<QString> m_projectNames;
    const QString  suffix = QString("*.%1").arg(m_suffix);

    QDir dir(m_folder);
    dir.setNameFilters(QStringList() << suffix);
    QFileInfoList infoList = dir.entryInfoList();

    for (auto&& info : infoList) {
        QString name = info.fileName();
        name.chop(suffix.size() - 1);
        m_projectNames << name;
    }
    return m_projectNames;
}

/**
 * 获得完整路径，包含后缀名
 */
QString FileSuffix::Get_projectPath(const QString& name) const {
    return QDir(m_folder).absoluteFilePath(name) + "." + m_suffix;
}

/**
 * 借助QFileInfo完成一些辅助
 * 如检查后缀，是否是文件
 * 注意 QFileInfo::fileName 是带后缀的
 */
bool FileSuffix::Copy_newFile(const QString& srcFile, bool cover) const {
    QFileInfo info(srcFile);
    if (false == info.exists(srcFile) || false == info.isFile()) {
        qWarning() << "Source file does not exist:" << srcFile;
        return false;
    }

    /// 如果有同名文件，则会copy失败
    /// 因此需要判断是否需要覆盖（即删除原先的）
    /// 当然，这里从安全角度是不对的，可能会copy失败导致原先存在的文件被删除
    /// 但是站在用户角度可能会觉得原先是没有这个名字的，因此要拷贝
    /// 所有目前就这么写
    const QString disPath = QDir(m_folder).filePath(info.fileName());
    if (cover) {
        QFile::remove(disPath);
    }
    return QFile::copy(srcFile, disPath);
}

/**
 * 先生成完整路径再删除
 * 为什么：不直接要求传入完整路径？
 * 因为：如果外部已经有了完整路径，那为什么不直接从外部操作remove呢
 */
bool FileSuffix::Remove_oldFile(const QString& name) const {
    QString path = Get_projectPath(name);
    return QFile::remove(path);
}

bool FileSuffix::Processing_folder(const QString& srcFile) const {
    QVector<QString> projectNames;
    const QString    suffix = QString("*.%1").arg(m_suffix);

    QDir dir(srcFile);
    dir.setNameFilters(QStringList() << suffix);
    QFileInfoList infoList = dir.entryInfoList();
    for (auto&& info : infoList) {
        Copy_newFile(info.filePath());
    }
    return true;
}

/**
 * 这里带有缓存，防止大量查询
 */
const QJsonObject& FileSuffix::Get_projectJson(const QString& project) const {
    static const QJsonObject          empty;
    static QMap<QString, QJsonObject> mp;

    if (mp.count(project)) {
        return mp[project];
    }

    QString path = Get_projectPath(project);
    QFile   file(path);

    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        qWarning() << QString("File[%1] exist[%2] open[%3]")
                          .arg(path)
                          .arg(file.exists())
                          .arg(file.isOpen());
        return empty;
    }
    QJsonObject json   = QJsonDocument::fromJson(file.readAll()).object();
    return mp[project] = json;
}
}  // namespace UTILITY
