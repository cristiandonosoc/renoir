/******************************************************************************
 * @file: thread.cc
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-05-07
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#include <atomic>

#include "platform/thread.h"

namespace renoir {
namespace platform {

namespace {

std::atomic<size_t> UID_BASE(1);

}   // namespace

ThreadContext::ThreadContext() 
  : thread_id(std::this_thread::get_id()),
    UID(std::atomic_fetch_add(&UID_BASE, 1))  {}

}   // namespace platform
}   // namespace renoir

