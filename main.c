#include <stdio.h>
#include <raylib.h>

#define VELOCITY 100

int main()
{
	int x = 0;
	int y = 0;

  InitWindow(800, 600, "rayout");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    x += dt * VELOCITY;
    y += dt * VELOCITY;

	BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangle(x, y, 100, 100, RED);
	EndDrawing();
  }

	return 0;
}
