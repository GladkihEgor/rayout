#include <stdio.h>
#include <raylib.h>

typedef struct {
	Vector2 center;
	float radius;
} Ball;

typedef struct {
	Rectangle rect;
	bool alive;
} Target;

void fill_targets(Target *targets, int rows, int cols, int w, int h, int x, int y, int padding)
{
	for (int r = 0; r < rows; r += 1)
		for (int c = 0; c < cols; c += 1)	
			targets[r*cols + c] = (Target){.rect = {.x = c*(w + padding) + x, .y = r*(h + padding) + y, .width = w, .height = h}, .alive = true};
}

int main()
{
	int window_w = 800;
	int window_h = 600;
	int target_rows = 3;
	int target_cols = 5;
	int targets_x = window_w / 6;
	int targets_y = window_h / 10;
	int targets_count = target_rows * target_cols;
	Rectangle bar = {.x=0, .y=window_h-20, .width=100, .height=20};
	Ball ball = {.center = {.x = window_w/2, .y = window_h-bar.height-10}, .radius = 10};
	Target targets[targets_count];
	fill_targets((Target *)&targets, target_rows, target_cols, 100, 20, targets_x, targets_y, 10);
	float velocity_x = 200;
	float velocity_y = 200;

  InitWindow(window_w, window_h, "rayout");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
		window_w = GetScreenWidth();
		window_h = GetScreenHeight();
    float dt = GetFrameTime();
		bool ball_hit_bar = CheckCollisionCircleRec(ball.center, ball.radius, bar);
    if (ball.center.x + ball.radius >= window_w || ball.center.x - ball.radius <= 0) velocity_x *= -1;
    if (ball.center.y + ball.radius >= window_h || ball.center.y - ball.radius <= 0 || ball_hit_bar) velocity_y *= -1;
    ball.center.x += dt * velocity_x;
    ball.center.y += dt * velocity_y;

    if (IsKeyDown(KEY_RIGHT) && bar.x + bar.width < window_w) bar.x += dt * 250;
    else if (IsKeyDown(KEY_LEFT) && bar.x > 0) bar.x -= dt * 250;

    for (int i = 0; i < targets_count; i += 1)
    	if (targets[i].alive && CheckCollisionCircleRec(ball.center, ball.radius, targets[i].rect)) {
    		targets[i].alive = false;
				velocity_y *= -1;
    	}
    

	BeginDrawing();
    ClearBackground(BLACK);
    for (int i = 0; i < targets_count; i += 1)
    	if (targets[i].alive)
		    DrawRectangleRec(targets[i].rect, WHITE);
    DrawRectangleRec(bar, GREEN);
		DrawCircleV(ball.center, ball.radius, RED);
	EndDrawing();
  }

	return 0;
}

// TODO: bug if ball will collide with the bar on the side
// TODO: make all sizes relative on window size
