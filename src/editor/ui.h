/******************************************************************************
 * @file: ui.h
 * @author: Cristián Donoso C.
 * @email: cristiandonosoc@gmail.com
 * @date: 2018-05-06
 * @license: 2018 Cristián Donoso C. - All Rights Reserved.
 *
 * @description: TODO(Cristian): Add description
 ******************************************************************************/

#ifndef SRC_EDITOR_UI_H
#define SRC_EDITOR_UI_H

#include <imgui/imgui.h>
#include <external/imguidock.h>

#include "logging/log.h"
#include "utils/scope_trigger.h"
#include "utils/string.h"

namespace renoir {
namespace editor {

using ::renoir::utils::ScopeTrigger;
using ::renoir::utils::FormattedString;

using namespace ::renoir::logging;

void LogWindow(ImVec2 start_pos, ImVec2 start_size) {
  /* static size_t static_log_count = 0; */

  ImGui::SetNextWindowPos(start_pos, ImGuiCond_Once);
  ImGui::SetNextWindowSize(start_size, ImGuiCond_Once);


  static bool open = true;
  SCOPED_TRIGGER(ImGui::Begin("log", &open), ImGui::End());


  GlobalLogContext *global_context = GetGlobalLogContext();

  {
    auto window_width = ImGui::GetWindowWidth();
    static float pane_ratio = 0.4f;
    SCOPED_TRIGGER(ImGui::BeginChild("left_pane", {window_width * pane_ratio, 0}, true),
                   ImGui::EndChild());


    for (auto it : global_context->log_contexts) {
      size_t uid = it.second->thread_context->UID;
      const char *name = it.second->thread_context->name.c_str();
      auto label = FormattedString("Thread %zu: %s", uid, name);
      ImGui::Selectable(label.c_str());
    }
  }

}

void TestWindow() {
  SCOPED_TRIGGER(ImGui::Begin("dock_window", NULL, {500, 500}),
                 ImGui::End());
  SCOPED_TRIGGER(ImGui::BeginDockspace(), ImGui::EndDockspace());

  static char tmp[128];
  for (int i = 0; i < 5; i++) {
    sprintf(tmp, "Dock %d", i);
    if (ImGui::BeginDock(tmp)) {
      ImGui::Text("Content of dock %d", i);
    }
    ImGui::EndDock();
  }
}


}   // namespace editor
}   // namespace renoir

#endif  // SRC_EDITOR_UI_H
