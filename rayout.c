#include <stdlib.h>
#include <raylib.h>

#include "rayout.h"

void init_targets(Target *targets, int rows, int cols, int w, int h, int x, int y, int padding)
{
  for (int r = 0; r < rows; r += 1)
    for (int c = 0; c < cols; c += 1)
      targets[r*cols + c] = (Target){.rect = {.x = c*(w + padding) + x, .y = r*(h + padding) + y, .width = w, .height = h}, .alive = true};
}

State init_state()
{
  State state = {0};
  Rectangle bar = {.x=0, .y=WINDOW_H-20, .width=100, .height=20};
  state.bar = bar;
  Ball ball = {.center = {.x = WINDOW_W/2, .y = WINDOW_H-bar.height-10}, .radius = 10};
  state.ball = ball;
  state.targets = malloc(TARGETS_COUNT * sizeof(Target));
  init_targets(state.targets, TARGET_ROWS, TARGET_COLS, 100, 20, TARGETS_X, TARGETS_Y, 10);
  state.ball_velocity_x = 200;
  state.ball_velocity_y = 200;
  return state;
}

void release_state(State *s)
{
  if (s->targets != NULL) {
    free(s->targets);
    s->targets = NULL;
  }
}

void update_state(State *s, float dt)
{
  bool ball_hit_bar = CheckCollisionCircleRec(s->ball.center, s->ball.radius, s->bar);
  if (s->ball.center.x + s->ball.radius >= WINDOW_W || s->ball.center.x - s->ball.radius <= 0) s->ball_velocity_x *= -1;
  if (s->ball.center.y + s->ball.radius >= WINDOW_H || s->ball.center.y - s->ball.radius <= 0 || ball_hit_bar) s->ball_velocity_y *= -1;
  s->ball.center.x += dt * s->ball_velocity_x;
  s->ball.center.y += dt * s->ball_velocity_y;

  if (IsKeyDown(KEY_RIGHT) && s->bar.x + s->bar.width < WINDOW_W) s->bar.x += dt * 250;
  else if (IsKeyDown(KEY_LEFT) && s->bar.x > 0) s->bar.x -= dt * 250;

  for (int i = 0; i < TARGETS_COUNT; i += 1)
    if (s->targets[i].alive && CheckCollisionCircleRec(s->ball.center, s->ball.radius, s->targets[i].rect)) {
      s->targets[i].alive = false;
      s->ball_velocity_y *= -1;
    }
}

void render(const State *s)
{
  ClearBackground(BLACK);
  for (int i = 0; i < TARGETS_COUNT; i += 1)
    if (s->targets[i].alive)
      DrawRectangleRec(s->targets[i].rect, WHITE);
  DrawRectangleRec(s->bar, GREEN);
  DrawCircleV(s->ball.center, s->ball.radius, RED);
}
