#include "Particle.h"
#include "GameWorld.h"
#include "raylib/raylib.h"

Particle createParticle( Vector2 pos, Vector2 vel, float radius, float angleVel, Color color ) {

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

    /*if ( particle->pos.y + particle->radius >= GetScreenHeight() ) {
        particle->pos.y = GetScreenHeight() - particle->radius;
        particle->vel.y = -200.f;
    }*/

    particle->vel.y += GRAVITY;

}

void drawParticle( Particle *particle ) {
    DrawCircleV( particle->pos, particle->radius, particle->color );
}