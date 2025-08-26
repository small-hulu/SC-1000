#ifndef CONFIG_H_1724995586
#define CONFIG_H_1724995586

#include "entity/EConfig_Hospital.h"
#include "entity/EConfig_Network.h"
#include "entity/EConfig_Printer.h"
#include "entity/EConfig_ProjectCombine.h"
#include "entity/EInterface_IConfig.h"

namespace CONTROLLER {

namespace Config {
using IConfig        = ENTITY::IConfig;
using Printer        = ENTITY::Printer;
using Network        = ENTITY::Network;
using ProjectCombine = ENTITY::ProjectCombine;
using Hospital       = ENTITY::Hospital;

};  // namespace Config
}  // namespace CONTROLLER

#endif  // CONFIG_H_1724995586
