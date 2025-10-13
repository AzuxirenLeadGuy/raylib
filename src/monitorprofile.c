#include "raylib.h"
// #include ""
#include "GLFW/glfw3.h"
#include <stdlib.h>

void ClearMonitorProfile(MonitorProfile *profile) {
  free(profile->resolutions);
  free(profile);
}

MonitorProfile *GetSupportedModes(unsigned int monitor_id) {
  int count;
  GLFWmonitor **monitors = glfwGetMonitors(&count);
  if (count < monitor_id) {
    return NULL;
  }
  const GLFWvidmode *modes = glfwGetVideoModes(monitors[count], &count);
  if (count == 0) {
    return NULL;
  }
  MonitorProfile *profile = malloc(sizeof(MonitorProfile));
  profile->monitor_id = monitor_id;
  profile->resolution_count = count;
  unsigned int actual_count = 0;
  int max_rr = modes[0].refreshRate, max_rb = modes[0].redBits,
      max_bb = modes[0].blueBits, max_gb = modes[0].greenBits;
  for (int i = 0; i < count; i++) {
    const GLFWvidmode mode = modes[i];
    if (mode.refreshRate < max_rr || mode.greenBits < max_gb ||
        mode.blueBits < max_bb || mode.redBits < max_rb) {
      continue;
    }
    actual_count++;
  }
  profile->resolutions = calloc(actual_count, sizeof(ScreenResolution));
  actual_count = 0;
  for (int i = 0; i < count; i++) {
    const GLFWvidmode mode = modes[i];
    if (mode.refreshRate < max_rr || mode.greenBits < max_gb ||
        mode.blueBits < max_bb || mode.redBits < max_rb) {
      continue;
    }
    profile->resolutions[actual_count].width = mode.width;
    profile->resolutions[actual_count].height = mode.height;
  }
  return profile;
}
