#pragma once

#include "Particle.h"
#include "raylib/raylib.h"

typedef struct ParticleEmitter {

    Vector2 pos;
    Vector2 vel;
    float angle;
    float angleVel;

    int newParticlePos;
    int particleQuantity;
    int maxParticles;
    Particle *particles;

} ParticleEmitter;

ParticleEmitter createParticleEmitter( Vector2 pos, Vector2 vel, float angleVel, int maxParticles );
void destroyParticleEmitter( ParticleEmitter *pe );
void updateParticleEmitter( ParticleEmitter *pe, float delta );
void drawParticleEmitter( ParticleEmitter *pe );
void emitParticle( ParticleEmitter *pe, Vector2 pos, Color color );
void emitParticlesColorInterval( ParticleEmitter *pe, int quantity, float startHue, float endHue );
void emitParticlesPositionColorInterval( ParticleEmitter *pe, Vector2 pos, int quantity, float startHue, float endHue );