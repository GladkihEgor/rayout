#include <stdio.h>
#include <raylib.h>

typedef struct {
	Vector2 center;
	float radius;
} Ball;

int main()
{
	int window_w = 800;
	int window_h = 600;
	Rectangle bar = {.x=0, .y=window_h-20, .width=100, .height=20};
	Ball ball = {.center = {.x = 10, .y = 10}, .radius = 10};
	float velocity_x = 100;
	float velocity_y = 100;

  InitWindow(window_w, window_h, "rayout");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
		window_w = GetScreenWidth();
		window_h = GetScreenHeight();
    float dt = GetFrameTime();
		bool ball_hit_bar = CheckCollisionCircleRec(ball.center, ball.radius, bar);
    if (ball.center.x + ball.radius >= window_w || ball.center.x - ball.radius <= 0) velocity_x = -velocity_x;
    if (ball.center.y + ball.radius >= window_h || ball.center.y - ball.radius <= 0 || ball_hit_bar) velocity_y = -velocity_y;
    ball.center.x += dt * velocity_x;
    ball.center.y += dt * velocity_y;

    if (IsKeyDown(KEY_RIGHT) && bar.x + bar.width < window_w) bar.x += dt * 100;
    else if (IsKeyDown(KEY_LEFT) && bar.x > 0) bar.x -= dt * 100;

	BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangleRec(bar, GREEN);
		DrawCircleV(ball.center, ball.radius, RED);
	EndDrawing();
  }

	return 0;
}

// TODO: bug if ball will collide with the bar on the side
// TODO: make all sizes relative on window size
