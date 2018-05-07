/******************************************************************************
 * @file: log.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-05-06
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_LOGGING_LOG_H
#define SRC_LOGGING_LOG_H

#include <chrono>
#include <cstdio>
#include <string>
#include <thread>
#include <map>
#include <mutex>

#include "utils/macros.h"
#include "utils/printable_enum.h"

#define RNR_THREAD_LOG_ENTRIES 2048

namespace renoir {
namespace logging {

PRINTABLE_ENUM(LogLevel, LOG_FATAL, LOG_ERROR, LOG_WARN,
                         LOG_INFO, LOG_DEBUG);

struct LogEntry {
  LogLevel level;
  std::string filename;
  std::string msg;
  size_t line;
  time_t time;
  size_t us;
};  

struct ThreadLocalLogContext {
  bool init = false;
  std::thread::id thread_id;
  size_t read_index;
  size_t write_index;
  LogEntry entries[RNR_THREAD_LOG_ENTRIES];

 public:
  // Registers the thread local log context
  ThreadLocalLogContext();
};  

struct GlobalLogContext {
  std::map<std::thread::id, ThreadLocalLogContext*> log_contexts;
  std::mutex mutex;
};  

inline ThreadLocalLogContext* GetLocalLogContext() {
  thread_local ThreadLocalLogContext context = {};
  return &context;
};

GlobalLogContext *GetGlobalLogContext();

inline LogEntry CreateLogEntry(const LogLevel& level, const char *filename, size_t line,
                               const char *msg) {
  LogEntry entry = {};
  entry.level = level;
  entry.filename = filename;
  entry.line = line;
  entry.msg = msg;

  using Clock = std::chrono::system_clock;
  auto now = Clock::now();
  auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
  entry.time = Clock::to_time_t(now);
  entry.us = (size_t)std::chrono::duration_cast<std::chrono::microseconds>(now - seconds).count();

  return entry;
};

void Log(const LogLevel& level, const char *filename, size_t line, const char *fmt, ...);


}   // namespace logging
}   // namespace renoir



#endif  // SRC_LOGGING_LOG_H
