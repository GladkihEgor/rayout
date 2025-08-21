#include <stdio.h>
#include <raylib.h>

int main()
{
	int window_w = 800;
	int window_h = 600;
	Rectangle bar = {.x=0, .y=window_h-20, .width=100, .height=20};
	Rectangle ball = {.x=0, .y=0, .width=20, .height=20};
	float velocity_x = 100;
	float velocity_y = 100;

  InitWindow(window_w, window_h, "rayout");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
		window_w = GetScreenWidth();
		window_h = GetScreenHeight();
    float dt = GetFrameTime();
    bool ball_hit_bar = CheckCollisionRecs(ball, bar);
    if (ball.x + ball.width  >= window_w || ball.x <= 0) velocity_x = -velocity_x;
    if (ball.y + ball.height >= window_h || ball.y <= 0 || ball_hit_bar) velocity_y = -velocity_y;
    ball.x += dt * velocity_x;
    ball.y += dt * velocity_y;

    if (IsKeyDown(KEY_RIGHT) && bar.x + bar.width < window_w) bar.x += dt * 100;
    else if (IsKeyDown(KEY_LEFT)  && bar.x > 0) bar.x -= dt * 100;

	BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangleRec(bar, GREEN);
    DrawRectangleRec(ball, RED);
	EndDrawing();
  }

	return 0;
}
