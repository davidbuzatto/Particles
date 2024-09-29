#pragma once

#include "raylib/raylib.h"

typedef struct Particle {
    Vector2 pos;
    Vector2 vel;
    float radius;
    Color color;
} Particle;

Particle createParticle( Vector2 pos, Vector2 vel, float radius, Color color );
void updateParticle( Particle *particle, float delta );
void drawParticle( Particle *particle );