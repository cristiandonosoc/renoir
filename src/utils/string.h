/******************************************************************************
 * @file: string.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-05-06
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UTILS_STRING_H
#define SRC_UTILS_STRING_H

#include <cstdarg>
#include <string>

#include "utils/macros.h"

namespace renoir {
namespace utils {

template <size_t BUFFER_SIZE = 1024>
std::string PRINTF_FORMAT_ATTRIBUTE(1, 2) FormattedString(const char *fmt, ...) {
  PROCESS_VARGLIST(tmp_buffer, BUFFER_SIZE, fmt);
  return tmp_buffer;
}


}   // namespace utils
}   // namespace renoir



#endif  // SRC_UTILS_STRING_H
