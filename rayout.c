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
  Rectangle bar = {.x=WINDOW_W/2, .y=WINDOW_H-20, .width=100, .height=20};
  state.bar = bar;
  state.bar_speed = 0;
  Ball ball = {.center = {.x = WINDOW_W/2, .y = WINDOW_H-bar.height-20}, .radius = 10};
  state.ball = ball;
  state.targets = malloc(TARGETS_COUNT * sizeof(Target));
  init_targets(state.targets, TARGET_ROWS, TARGET_COLS, 100, 20, TARGETS_X, TARGETS_Y, 10);
  state.ball_velocity_x = 200;
  state.ball_velocity_y = -200;
  return state;
}

void release_state(State *s)
{
  if (s->targets != NULL) {
    free(s->targets);
    s->targets = NULL;
  }
}

void ball_horizontal_collision(State *s, float dt)
{
  int nx = s->ball.center.x + dt * s->ball_velocity_x;
  if (nx - s->ball.radius < 0 || nx + s->ball.radius > WINDOW_W) {
    s->ball_velocity_x *= -1;
    return;
  }

  Vector2 nc = {.x = nx, .y = s->ball.center.y};
  if (CheckCollisionCircleRec(nc, s->ball.radius, s->bar)) {
    s->ball_velocity_x *= -1;
    s->ball_velocity_y *= -1;
    return;
  }

  for (int i = 0; i < TARGETS_COUNT; i += 1) {
    if (s->targets[i].alive && CheckCollisionCircleRec(nc, s->ball.radius, s->targets[i].rect)) {
      s->targets[i].alive = false;
      s->ball_velocity_x *= -1;
      return;
    }
  }

  s->ball.center = nc;
}

void ball_vertical_collision(State *s, float dt)
{
  int ny = s->ball.center.y + dt * s->ball_velocity_y;
  if (ny - s->ball.radius < 0 || ny + s->ball.radius > WINDOW_H) {
    s->ball_velocity_y *= -1;
    return;
  }

  Vector2 nc = {.x = s->ball.center.x, .y = ny};
  if (CheckCollisionCircleRec(nc, s->ball.radius, s->bar)) {
    s->ball_velocity_y *= -1;
    return;
  }

  for (int i = 0; i < TARGETS_COUNT; i += 1) {
    if (s->targets[i].alive && CheckCollisionCircleRec(nc, s->ball.radius, s->targets[i].rect)) {
      s->targets[i].alive = false;
      s->ball_velocity_y *= -1;
      return;
    }
  }

  s->ball.center = nc;
}

void bar_collision(State *s, float dt)
{
  int nx = s->bar.x + dt * s->bar_speed;
  if (nx < 0 || nx + s->bar.width > WINDOW_W) return;
  Rectangle nb = {.x = nx, .y = s->bar.y, .width = s->bar.width, .height = s->bar.height};
  if (CheckCollisionCircleRec(s->ball.center, s->ball.radius, nb)) return;
  s->bar = nb;
}

void update_state(State *s, float dt)
{
  if (IsKeyDown(KEY_RIGHT)) s->bar_speed = BAR_SPEED;
  else if (IsKeyDown(KEY_LEFT)) s->bar_speed = -BAR_SPEED;
  else s->bar_speed = 0;

  ball_horizontal_collision(s, dt);
  ball_vertical_collision(s, dt);
  bar_collision(s, dt);
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
