#pragma once

#include "raylib/raylib.h"

typedef struct Obstacle {
    Rectangle rect;
    Rectangle topCP;
    Rectangle bottomCP;
    Rectangle leftCP;
    Rectangle rightCP;
    Color color;
} Obstacle;

Obstacle createObstacle( Vector2 pos, Vector2 dim, Color color );
void drawObstacle( Obstacle *obstacle );