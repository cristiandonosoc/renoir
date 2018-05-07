/******************************************************************************
 * @file: thread.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-05-06
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_PLATFORM_THREAD_H
#define SRC_PLATFORM_THREAD_H

#include <thread>

namespace renoir {
namespace platform {

struct ThreadContext {
  std::thread::id thread_id;
  std::string name;

 public:
  ThreadContext() : thread_id(std::this_thread::get_id()) {}
};

inline ThreadContext *GetThreadContext() {
  thread_local ThreadContext context = {};
  return &context;
}


}   // namespace platform
}   // namespace renoir

#endif  // SRC_PLATFORM_THREAD_H
