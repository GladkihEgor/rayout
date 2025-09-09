#ifndef RAYOUT_H_
#define RAYOUT_H_

#include <stdlib.h>
#include <raylib.h>

#define WINDOW_W 800
#define WINDOW_H 600

#define BALL_RADIUS   10
#define BALL_VELOCITY 200

#define BAR_SPEED  250
#define BAR_WIDTH  100
#define BAR_HEIGHT 20

#define TARGET_WIDTH  BAR_WIDTH
#define TARGET_HEIGHT BAR_HEIGHT

#define TARGETS_ROWS    3
#define TARGETS_COLS    5
#define TARGETS_COUNT   (TARGETS_ROWS * TARGETS_COLS)
#define TARGETS_X       (WINDOW_W / 6)
#define TARGETS_Y       (WINDOW_H / 10)
#define TARGETS_PADDING 10


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
