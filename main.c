#include <raylib.h>

#include "rayout.h"

int main()
{
	State state = init_state();
  InitWindow(WINDOW_W, WINDOW_H, "rayout");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    update_state(&state, dt);
    
	BeginDrawing();
		render(&state);
	EndDrawing();
  }

  release_state(&state);
	return 0;
}

// TODO: bug if ball will collide with the bar on the side
// TODO: make all sizes relative on window size
