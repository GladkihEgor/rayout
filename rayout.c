#include <assert.h>
#include <stdlib.h>
#include <raylib.h>

#include "rayout.h"

Target *init_targets()
{
  Target *targets = malloc(TARGETS_COUNT * sizeof(Target));
  for (int r = 0; r < TARGETS_ROWS; r += 1) {
    for (int c = 0; c < TARGETS_COLS; c += 1) {
      int i = r*TARGETS_COLS + c;
      assert(i < TARGETS_COUNT && "Ups something wrong");
      int x = c*(TARGET_WIDTH + TARGETS_PADDING) + TARGETS_X;
      int y = r*(TARGET_HEIGHT + TARGETS_PADDING) + TARGETS_Y;
      targets[i] = (Target){.rect = {.x = x, .y = y, .width = TARGET_WIDTH, .height = TARGET_HEIGHT}, .alive = true};
    }
  }
  return targets;
}

State init_state()
{
  State state = {0};
  state.game_state = PAUSE;

  Rectangle bar = {.x=WINDOW_W/2 - BAR_WIDTH/2, .y=WINDOW_H-BAR_HEIGHT, .width=BAR_WIDTH, .height=BAR_HEIGHT};
  state.bar = bar;
  state.bar_speed = 0;
  Ball ball = {.center = {.x = WINDOW_W/2, .y = WINDOW_H - bar.height - BALL_RADIUS - 1}, .radius = BALL_RADIUS};
  state.ball = ball;
  state.targets = init_targets();
  state.ball_velocity_x = BALL_VELOCITY;
  state.ball_velocity_y = -BALL_VELOCITY;
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

Collision ball_vertical_collision(State *s, float dt)
{
  int ny = s->ball.center.y + dt * s->ball_velocity_y;
  if (ny + s->ball.radius > WINDOW_H) {
    s->ball_velocity_y *= -1;
    return BOT;
  }
  if (ny - s->ball.radius < 0) {
    s->ball_velocity_y *= -1;
    return TOP;
  }

  Vector2 nc = {.x = s->ball.center.x, .y = ny};
  if (CheckCollisionCircleRec(nc, s->ball.radius, s->bar)) {
    s->ball_velocity_y *= -1;
    return BAR;
  }

  for (int i = 0; i < TARGETS_COUNT; i += 1) {
    if (s->targets[i].alive && CheckCollisionCircleRec(nc, s->ball.radius, s->targets[i].rect)) {
      s->targets[i].alive = false;
      s->ball_velocity_y *= -1;
      return TARGET;
    }
  }

  s->ball.center = nc;
  return NONE;
}

void bar_collision(State *s, float dt)
{
  int nx = s->bar.x + dt * s->bar_speed;
  if (nx < 0 || nx + s->bar.width > WINDOW_W) return;
  Rectangle nb = {.x = nx, .y = s->bar.y, .width = s->bar.width, .height = s->bar.height};
  if (CheckCollisionCircleRec(s->ball.center, s->ball.radius, nb)) return;
  s->bar = nb;
}

bool all_targets_died(State *s)
{
  for (size_t i = 0; i < TARGETS_COUNT; i+=1) {
    if (s->targets[i].alive) return false;
  }
  return true;
}

void update_state(State *s, float dt)
{
  if (IsKeyPressed(KEY_SPACE)) {
    if (s->game_state == PLAY) s->game_state = PAUSE;
    else s->game_state = PLAY;
  };

  switch (s->game_state) {
  case PLAY: {
    if (IsKeyDown(KEY_RIGHT)) s->bar_speed = BAR_SPEED;
    else if (IsKeyDown(KEY_LEFT)) s->bar_speed = -BAR_SPEED;
    else s->bar_speed = 0;

    ball_horizontal_collision(s, dt);
    switch (ball_vertical_collision(s, dt)) {
    case BOT:{
      release_state(s);
      *s = init_state();
      s->game_state = LOSE;
    } return;
    default: break;
    }
    bar_collision(s, dt);

    if (all_targets_died(s)) {
      release_state(s);
      *s = init_state();
      s->game_state = WIN;
    }
  } break;
  default: break;
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
  switch (s->game_state) {
  case LOSE: {
    DrawText("WASTED", 0, 0, 32, WHITE);
  } break;
  case WIN: {
    DrawText("CONGRATULATIONS", 0, 0, 32, WHITE);
  } break;
  case PAUSE: {
    DrawText("PAUSE", 0, 0, 32, WHITE);
  } break;
  default: break;
  }
}
