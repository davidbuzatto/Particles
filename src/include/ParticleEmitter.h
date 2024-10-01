#pragma once

#include <stdbool.h>

#include "Particle.h"
#include "raylib/raylib.h"

typedef struct ParticleEmitter {

    Vector2 pos;
    Vector2 vel;
    float launchAngle;
    float posAngle;
    float posAngleVel;
    float hueAngle;
    float hueAngleVel;

    float radius;
    bool dragging;
    bool draggable;
    bool mouseOver;

    int newParticlePos;
    int particleQuantity;
    int maxParticles;
    Particle *particles;

} ParticleEmitter;

ParticleEmitter createParticleEmitter( Vector2 pos, Vector2 vel, float launchAngle, float posAngleVel, float hueAngleVel, float radius, bool draggable, int maxParticles );
void destroyParticleEmitter( ParticleEmitter *pe );
void updateParticleEmitterMoveSin( ParticleEmitter *pe, float delta );
void updateParticleEmitterStatic( ParticleEmitter *pe, float delta );
void updateHueAngleBouncing( ParticleEmitter *pe, float delta );
void drawParticleEmitter( ParticleEmitter *pe );
void emitParticle( ParticleEmitter *pe, Vector2 pos, Vector2 vel, float radius, Color color );
void emitParticleColorInterval( ParticleEmitter *pe, Vector2 vel, float minRadius, float maxRadius, float startHue, float endHue );
void emitParticlePositionColorInterval( ParticleEmitter *pe, Vector2 pos, Vector2 vel, float minRadius, float maxRadius, float startHue, float endHue );
bool isMouseOverParticleEmitter( Vector2 pePos, float peRadius, Vector2 mousePos );

void emitParticleColorIntervalQuantity( 
    ParticleEmitter *pe, 
    float minVelX, float maxVelX, 
    float minVelY, float maxVelY, 
    bool randomSignX, bool randomSignY,
    float minRadius, float maxRadius, 
    float startHue, float endHue, 
    int quantity );

void emitParticlePositionColorIntervalQuantity( 
    ParticleEmitter *pe, 
    Vector2 pos,
    float minVelX, float maxVelX, 
    float minVelY, float maxVelY, 
    bool randomSignX, bool randomSignY,
    float minRadius, float maxRadius, 
    float startHue, float endHue, 
    int quantity );

void emitParticlePolarColorIntervalQuantity( 
    ParticleEmitter *pe, 
    float minVel, float maxVel, 
    float minLaunchAngle, float maxLaunchAngle,
    bool randomSignLaunchAnble,
    float minRadius, float maxRadius, 
    float startHue, float endHue, 
    int quantity );

void emitParticlePolarPositionColorIntervalQuantity( 
    ParticleEmitter *pe, 
    Vector2 pos,
    float minVel, float maxVel, 
    float minLaunchAngle, float maxLaunchAngle,
    bool randomSignLaunchAnble,
    float minRadius, float maxRadius, 
    float startHue, float endHue, 
    int quantity );