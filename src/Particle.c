#include "Particle.h"
#include "GameWorld.h"
#include "raylib/raylib.h"

static const float MAX_FALL_SPEED = 500.0f;

Particle createParticle( Vector2 pos, Vector2 vel, float radius, Color color ) {

    return (Particle){
        .pos = pos,
        .vel = vel,
        .radius = radius,
        .color = color
    };

}

void updateParticle( Particle *particle, float delta ) {

    particle->pos.x += particle->vel.x * delta;
    particle->pos.y += particle->vel.y * delta;

    particle->vel.y += GRAVITY;

    if ( particle->vel.y >= MAX_FALL_SPEED ) {
        particle->vel.y = MAX_FALL_SPEED;
    }

}

void drawParticle( Particle *particle ) {
    DrawCircleV( particle->pos, particle->radius, particle->color );
}