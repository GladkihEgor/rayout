#ifndef RAYOUT_H_
#define RAYOUT_H_

#include <stdlib.h>
#include <raylib.h>

#define WINDOW_W      800
#define WINDOW_H      600
#define TARGET_ROWS   3
#define TARGET_COLS   5
#define TARGETS_X     (WINDOW_W / 6)
#define TARGETS_Y     (WINDOW_H / 10)
#define TARGETS_COUNT (TARGET_ROWS * TARGET_COLS)

typedef struct {
	Vector2 center;
	float radius;
} Ball;

typedef struct {
	Rectangle rect;
	bool alive;
} Target;

typedef struct {
	Rectangle bar;
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
