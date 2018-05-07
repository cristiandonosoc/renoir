/******************************************************************************
 * @file: status.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-05-06
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_UTILS_STATUS_H
#define SRC_UTILS_STATUS_H

#include <cstdarg>
#include <string>

#include "utils/macros.h"

// NOTE(Cristian): Those geniuses at X11 decided that it was a good idea
//                 name their macros Status.... sigh
#ifdef Status
#undef Status
#endif

namespace renoir {
namespace utils {

enum class StatusKind {
  STATUS_OK,
  STATUS_ERROR,
  STATUS_WARNING
};  // enum class StatusKind

struct StatusErrorContext {
  std::string msg;
  std::string filename;
  size_t line;
};  // enum class StatusErrorContext

struct Status {
  StatusKind result = StatusKind::STATUS_OK;
  StatusErrorContext context;
};  // struct Status

template <typename T>
struct StatusOr : public Status {
  T value;  // Must be default-constructible, copyable and movable
            // In general, it should be a POD that's cheap to copy/move

  // Constructors
  StatusOr() = default;
  DEFAULT_COPY(StatusOr);
  DEFAULT_MOVE(StatusOr);

  // Utility constructor
  StatusOr(const T& t) : value(t) {} 

  // Pipe constructors
  StatusOr(const Status& other_status) : Status(other_status) {}
  template <typename U>
  StatusOr(const StatusOr<U>& other_status) : Status(other_status) {}
};

inline bool IsStatusOk(const Status& status) {
  return status.result == StatusKind::STATUS_OK;
}

template <typename T>
inline bool IsStatusOrOk(const StatusOr<T>& status) {
  return status.status.result == StatusKind::STATUS_OK;
}

inline Status CreateStatus(StatusKind result, const char *fmt, ...) {
  va_list arglist;
  va_start(arglist, fmt);
  char buffer[1024];
  vsnprintf(buffer, sizeof(buffer), fmt, arglist);
  va_end(arglist);

  Status status;
  status.result = result;
  status.context.msg = buffer;
  return status;
};

inline Status CreateFilenoStatus(StatusKind result, const char *filename, size_t line, 
                                 const char *fmt, ...) {
  va_list arglist;
  va_start(arglist, fmt);
  char buffer[1024];
  vsnprintf(buffer, sizeof(buffer), fmt, arglist);
  va_end(arglist);

  Status status;
  status.result = result;
  status.context.msg = buffer;
  status.context.filename = filename;
  status.context.line = line;
  return status;
}

#define FILENO_STATUS(status, fmt, ...) \
  ::renoir::utils::CreateFilenoStatus(status, fmt, __VA_ARGS__)


}   // namespace utils
}   // namespace renoir

#endif  // SRC_UTILS_STATUS_H
