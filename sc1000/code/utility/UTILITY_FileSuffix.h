#ifndef FILESUFFIX_H_1716887274
#define FILESUFFIX_H_1716887274

#include <QJsonObject>
#include <QVector>

namespace UTILITY {
class FileSuffix {
private:
    const QString m_folder;
    const QString m_suffix;

public:
    FileSuffix(QString folder, QString suffix);

public:
    /**
     * 获取项目文件夹中的所有设置的项目配置文件
     * return 为纯文件名，无路径，无后缀
     */
    QList<QString> Get_ProjectList() const;

    /**
     * 生成指向项目文件夹的路径
     * name必须是从 Get_ProjectList 获得的
     * 如果不是，则该路径无效
     */
    QString Get_projectPath(const QString& name) const;

public:
    /**
     * 复制一份外部的文件到项目文件夹中
     * bool cover;表示是否覆盖同名文件。
     */
    bool Copy_newFile(const QString& srcFile, bool cover = true) const;

    /**
     * 删除从 Get_ProjectList 获得的文件
     * 注意：传入的即文件名即可
     * + 不要完整路径
     * + 不要后缀名
     */
    bool Remove_oldFile(const QString& name) const;

    /**
     * 传入一个文件夹路径
     * 处理文件夹路径内的所有json文件
     */
    bool Processing_folder(const QString& srcFile) const;

    /**
     * 根据传入的项目，获取文件的 json
     */
    const QJsonObject& Get_projectJson(const QString& project) const;
};
}  // namespace UTILITY

#endif  // FILESUFFIX_H_1716887274
