#ifndef RAYOUT_H_
#define RAYOUT_H_

#include <stdlib.h>
#include <raylib.h>

#define WINDOW_W 800
#define WINDOW_H (WINDOW_W * 3 / 4)

#define BALL_RADIUS   (WINDOW_W / 80)
#define BALL_VELOCITY (WINDOW_W / 4)

#define BAR_SPEED  (WINDOW_W / 3)
#define BAR_W      (WINDOW_W / 8)
#define BAR_H      (WINDOW_H / 30)

#define TARGET_W BAR_W
#define TARGET_H BAR_H

#define TARGETS_COLS    (WINDOW_W / 160)
#define TARGETS_ROWS    (WINDOW_H / 120)
#define TARGETS_COUNT   (TARGETS_ROWS * TARGETS_COLS)
#define TARGETS_PADDING (BALL_RADIUS * 3)
#define TARGETS_W       (TARGETS_COLS * TARGET_W + (TARGETS_COLS - 1) * TARGETS_PADDING)
#define TARGETS_H       (TARGETS_ROWS * TARGET_H + (TARGETS_ROWS - 1) * TARGETS_PADDING)
#define TARGETS_X       (WINDOW_W/2 - TARGETS_W/2)
#define TARGETS_Y       (WINDOW_H/4 - TARGETS_H/2)


typedef struct {
  Vector2 center;
  float radius;
} Ball;

typedef struct {
  Rectangle rect;
  bool alive;
} Target;

typedef enum {
  NONE,
  TOP,
  BOT,
  LEFT,
  RIGHT,
  BAR,
  TARGET,
} Collision;

typedef enum {
  PAUSE,
  PLAY,
  LOSE,
  WIN,
} GameState;

typedef struct {
  GameState game_state;

  Rectangle bar;
  float bar_speed;
  Ball ball;
  float ball_velocity_x;
  float ball_velocity_y;
  Target *targets;
} State;

State init_state();
void  release_state(State *s);
void  update_state(State *s, float dt);
void  render(const State *s);

#endif //RAYOUT_H_
