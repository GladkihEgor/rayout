#include <dlfcn.h>
#include <raylib.h>

#include "rayout.h"

int main()
{
  void *rayout = dlopen("librayout.dylib", 0);
  if (rayout == NULL) TraceLog(LOG_FATAL, "Can't open rayout dll:%s\n", dlerror());

  const char* init_state_name = "init_state";
  State (*init_state)() = dlsym(rayout, init_state_name);
  if (init_state == NULL) TraceLog(LOG_FATAL, "Can't find %s symbol in rayout dll:%s\n", init_state_name, dlerror());

  const char* release_state_name = "release_state";
  void (*release_state)(State*) = dlsym(rayout, release_state_name);
  if (release_state == NULL) TraceLog(LOG_FATAL, "Can't find %s symbol in rayout dll:%s\n", release_state_name, dlerror());

  const char* update_state_name = "update_state";
  void (*update_state)(State*, float) = dlsym(rayout, update_state_name);
  if (update_state == NULL) TraceLog(LOG_FATAL, "Can't find %s symbol in rayout dll:%s\n", update_state_name, dlerror());

  const char* render_name = "render";
  void (*render)(State*) = dlsym(rayout, render_name);
  if (render == NULL) TraceLog(LOG_FATAL, "Can't find %s symbol in rayout dll:%s\n", render_name, dlerror());

  State state = init_state();
  InitWindow(WINDOW_W, WINDOW_H, "rayout");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    if (IsKeyDown(KEY_R)) {
      if (dlclose(rayout) == -1) TraceLog(LOG_FATAL, "Can't close rayout dll:%s\n", dlerror());
      rayout = dlopen("librayout.dylib", 0);
      if (rayout == NULL) TraceLog(LOG_FATAL, "Can't open rayout dll:%s\n", dlerror());
      init_state = dlsym(rayout, init_state_name);
      release_state = dlsym(rayout, release_state_name);
      update_state = dlsym(rayout, update_state_name);
      render = dlsym(rayout, render_name);
      if (init_state == NULL) TraceLog(LOG_FATAL, "Can't find %s symbol in rayout dll:%s\n", init_state_name, dlerror());
      if (release_state == NULL) TraceLog(LOG_FATAL, "Can't find %s symbol in rayout dll:%s\n", release_state_name, dlerror());
      if (update_state == NULL) TraceLog(LOG_FATAL, "Can't find %s symbol in rayout dll:%s\n", update_state_name, dlerror());
      if (render == NULL) TraceLog(LOG_FATAL, "Can't find %s symbol in rayout dll:%s\n", render_name, dlerror());
    }

    float dt = GetFrameTime();
    update_state(&state, dt);

    BeginDrawing();
    render(&state);
    EndDrawing();
  }

  release_state(&state);
  if (dlclose(rayout) == -1) TraceLog(LOG_FATAL, "Can't close rayout dll:%s\n", dlerror());
  return 0;
}

// TODO: bug if ball will collide with the bar on the side
// TODO: make all sizes relative on window size
