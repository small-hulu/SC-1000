#include "EConfig_Project.h"

namespace ENTITY {

Project::Project(const QString project) {
    auto    fileCU = ::UTILITY::FileSuffix(to_absoluteFilePath(Path_ProjectsFolder), "json");
    QString path   = fileCU.Get_projectPath(project);
    QFile   file(path);

    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        qWarning() << QString("File[%1] exist[%2] open[%3] error[%4]")
                          .arg(path)
                          .arg(file.exists())
                          .arg(file.isOpen())
                          .arg(file.errorString());
        return;
    }

    json = QJsonDocument::fromJson(file.readAll()).object();
    if (json.isEmpty()) {
        qWarning() << "JSON 文件解析失败";
        return;
    }
}

int Project::Judge_scope(double value) {
    namespace OM               = KeyProject::OtherMessage;
    QJsonObject OtherMessage   = json.value(OM::OtherMessage).toObject();
    QJsonObject projectMessage = OtherMessage.value(OM::ProjectMessage).toObject();

    const QString scope = projectMessage.value(OM::Reference_range).toString();

    auto pair = scope.split("-");
    if (pair.size() != 2) {
        return 0;
    }

    if (value < pair[0].toDouble()) {
        return -1;
    }
    if (value > pair[1].toDouble()) {
        return 1;
    }

    return 0;
}

}  // namespace ENTITY
