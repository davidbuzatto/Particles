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
void updateParticleEmitterMoveSin( ParticleEmitter *pe, float delta );
void updateParticleEmitterStaticRight( ParticleEmitter *pe, float delta );
void drawParticleEmitter( ParticleEmitter *pe );
void emitParticle( ParticleEmitter *pe, Vector2 pos, Vector2 vel, float radius, Color color );
void emitParticleColorInterval( ParticleEmitter *pe, Vector2 vel, int minRadius, int maxRadius, float startHue, float endHue );
void emitParticlePositionColorInterval( ParticleEmitter *pe, Vector2 pos, Vector2 vel, int minRadius, int maxRadius, float startHue, float endHue );
void emitParticleColorIntervalQuantity( 
    ParticleEmitter *pe, 
    int minVelX, int maxVelX, 
    int minVelY, int maxVelY, 
    bool randomSignX, bool randomSignY,
    int minRadius, int maxRadius, 
    float startHue, float endHue, 
    int quantity );
void emitParticlePositionColorIntervalQuantity( 
    ParticleEmitter *pe, 
    Vector2 pos,
    int minVelX, int maxVelX, 
    int minVelY, int maxVelY, 
    bool randomSignX, bool randomSignY,
    int minRadius, int maxRadius, 
    float startHue, float endHue, 
    int quantity );