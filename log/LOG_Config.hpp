/** ************************************************************************
 * Copyright © 2023 luyinfeng.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       LOG_Config.hpp
 *
 * @author     luyinfeng (luyinfeng@smtbio.com)
 * @brief
 * 下方内容可进行定制化的修改
 *
 *
 * @Email      luyinfeng@smtbio.com
 *
 * @date       2023-03-22
 * @version    0.1
 ***************************************************************************/
#ifndef __LOG__CONFIG__HPP__BY__CuberLotus__
#define __LOG__CONFIG__HPP__BY__CuberLotus__

namespace LOG {

/// 保存路径
const char* const _SAVE_PATH_        = "./log";
/// 文件后缀名
const char* const _FILE_SUFFIX_NAME_ = ".log";

/// 根据日期分文件夹
/// 这里是具体日志的内容
const char* const _DATA_FIRMAT_ = "yyyyMMdd";
const char* const _TIME_FORMAT_ = "hh:mm:ss";

}  // namespace LOG

#endif  // __LOG__CONFIG__HPP__BY__CuberLotus__
