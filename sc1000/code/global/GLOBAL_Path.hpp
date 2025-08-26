/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       GLOBAL_PATH.hpp
 * @date       2024-05-27
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      1539349804@qq.com
 *
 * @brief
 * 将文件夹名和文件名单独分开
 * 不要以为api会自动解析是都是文件夹还是文件
 * 有的文件夹中也是可以有个点的就是说可以创建一个和具体文件名一样的文件夹
 * ===================================
 * 约束：
 * > 文件夹名：
 * >>> 不要以(/)开头
 * >>> 会直接定位到盘或者系统根目录
 * > 文件名：
 * >>> 直接写裸的
 * >>> 不要加 (/) (./)等形式
 *
 *****************************************************************************/
#ifndef GLOBAL_PATH_HPP_1698301218
#define GLOBAL_PATH_HPP_1698301218

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

const char* const Path_DBFolder = "./sc1000-data/database";
const char* const Path_DBFile   = "sc1000.db";

const char* const Path_ProjectsFolder = "./sc1000-data/projects";

const char* const Path_ConfigFolder = "./sc1000-data/config";
const char* const Path_ConfigFile   = "config.json";

const char* const Path_IniFolder = "./sc1000-data";
const char* const Path_IniFile   = "sc1000.ini";

const char* const Path_Projects = "sc1000-data/projects";

inline QString to_absoluteFilePath(const QString& path) {
    return QFileInfo(QCoreApplication::applicationDirPath(), path).absoluteFilePath();
}

#endif  // GLOBAL_PATH_HPP_1698301218
