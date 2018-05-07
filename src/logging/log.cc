/******************************************************************************
 * @file: log.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-05-06
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/


#include "logging/log.h"

namespace renoir {
namespace logging {

ThreadLocalLogContext::ThreadLocalLogContext() : thread_id(std::this_thread::get_id()) {
  GlobalLogContext *global_context = GetGlobalLogContext();
  size_t size = 0;
  {
    std::lock_guard<std::mutex> guard(global_context->mutex);
    global_context->log_contexts[thread_id] = this;
    size = global_context->log_contexts.size();
  }

  fprintf(stderr, "Created local log context. Size: %zu\n", size);
}

GlobalLogContext *GetGlobalLogContext() {
  static GlobalLogContext global_context;
  return &global_context;
}

void Log(const LogLevel& level, const char *filename, size_t line,
                const char *fmt, ...) {
  PROCESS_VARGLIST(msg_buffer, 1024, fmt);
  LogEntry entry = CreateLogEntry(level, filename, line, msg_buffer);

  ThreadLocalLogContext *context = GetLocalLogContext();

  // We write into the write index
  context->entries[context->write_index] = entry;
  context->write_index = (context->write_index + 1) % sizeof(context->entries);

  if (context->write_index == context->read_index) {
    context->read_index = (context->read_index + 1) % sizeof(context->entries);
  }

  fprintf(stderr, "LOG: %s\n", msg_buffer);
};




}   // namespace logging
}   // namespace renoir
