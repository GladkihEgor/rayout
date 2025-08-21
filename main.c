#include <stdio.h>
#include <raylib.h>

int main()
{
	Rectangle ball = {.x=0, .y=0, .width=100, .height=100};
	float velocity_x = 100;
	float velocity_y = 100;

  InitWindow(800, 600, "rayout");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
		int w = GetScreenWidth();
		int h = GetScreenHeight();
    float dt = GetFrameTime();
    if (ball.x + ball.width  >= w || ball.x <= 0) velocity_x = -velocity_x;
    if (ball.y + ball.height >= h || ball.y <= 0) velocity_y = -velocity_y;
    ball.x += dt * velocity_x;
    ball.y += dt * velocity_y;

	BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangleRec(ball, RED);
	EndDrawing();
  }

	return 0;
}
