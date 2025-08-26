#ifndef PROJECT_H_1739858821
#define PROJECT_H_1739858821

#include <QJsonDocument>

#include "global/GLOBAL_KeyProject.h"
#include "global/GLOBAL_Path.hpp"
#include "utility/UTILITY_FileSuffix.h"

namespace ENTITY {
class Project {
public:
    QJsonObject json;

public:
    Project(const QString project);

    bool Is_valid() {
        return json.isEmpty() == false;
    }

public:
    int Judge_scope(double value);
};
}  // namespace ENTITY

#endif  // PROJECT_H
