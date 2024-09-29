#include <stdlib.h>
#include <math.h>

#include "Particle.h"
#include "ParticleEmitter.h"
#include "GameWorld.h"
#include "utils.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"

ParticleEmitter createParticleEmitter( Vector2 pos, Vector2 vel, float angleVel, int maxParticles ) {

    return (ParticleEmitter) {
        .pos = pos,
        .vel = vel,
        .angle = 0.0f,
        .angleVel = angleVel,
        .newParticlePos = 0,
        .particleQuantity = 0,
        .maxParticles = maxParticles,
        .particles = (Particle*) malloc( maxParticles * sizeof( Particle ) )
    };

}

void destroyParticleEmitter( ParticleEmitter *pe ) {
    free( pe->particles );
}

void drawParticleEmitter( ParticleEmitter *pe ) {

    for ( int i = 0; i < pe->particleQuantity; i++ ) {
        drawParticle( &pe->particles[i] );
    }

}

void updateParticleEmitterMoveSin( ParticleEmitter *pe, float delta ) {

    pe->pos.x += pe->vel.x * delta;
    pe->pos.y += pe->vel.y * sin( DEG2RAD * pe->angle ) * delta;

    pe->angle += pe->angleVel * delta;
    if ( pe->angle > 360.0f ) {
        pe->angle = 0.0f;
    }

    if ( pe->pos.x < 40.0f ) {
        pe->vel.x *= -1.0f;
    } else if ( pe->pos.x >= GetScreenWidth() - 40.0f ) {
        pe->vel.x *= -1.0f;
    }

    for ( int i = 0; i < pe->particleQuantity; i++ ) {
        updateParticle( &pe->particles[i], delta );
    }

}

void updateParticleEmitterStaticRight( ParticleEmitter *pe, float delta ) {

    pe->angle += pe->angleVel * delta;
    if ( pe->angle > 360.0f ) {
        pe->angle = 0.0f;
    }

    for ( int i = 0; i < pe->particleQuantity; i++ ) {
        updateParticle( &pe->particles[i], delta );
    }

}

void emitParticle( ParticleEmitter *pe, Vector2 pos, Vector2 vel, float radius, Color color ) {

    int k = pe->newParticlePos % pe->maxParticles;

    pe->particles[k] = createParticle( 
        pos, 
        vel,
        radius,
        color
    );

    pe->newParticlePos++;

    if ( pe->particleQuantity < pe->maxParticles ) {
        pe->particleQuantity++;
    }

}

void emitParticleColorInterval( ParticleEmitter *pe, Vector2 vel, int minRadius, int maxRadius, float startHue, float endHue ) {
    emitParticlePositionColorInterval( pe, pe->pos, vel, minRadius, maxRadius, startHue, endHue );
}

void emitParticlePositionColorInterval( ParticleEmitter *pe, Vector2 pos, Vector2 vel, int minRadius, int maxRadius, float startHue, float endHue ) {
    emitParticle( 
        pe, 
        pos, 
        vel, 
        (float) GetRandomValue( minRadius, maxRadius ),
        ColorFromHSV( 
            Lerp( startHue, endHue, pe->angle / 360.0f ), 
            1.0f, 
            1.0f
        )
    );
}

void emitParticleColorIntervalQuantity( 
    ParticleEmitter *pe, 
    int minVelX, int maxVelX, 
    int minVelY, int maxVelY, 
    bool randomSignX, bool randomSignY,
    int minRadius, int maxRadius, 
    float startHue, float endHue, 
    int quantity ) {
    
    for ( int i = 0; i < quantity; i++ ) {
        emitParticleColorInterval( 
            pe, 
            createVel( (float) GetRandomValue( minVelX, maxVelX ), (float) GetRandomValue( minVelX, maxVelX ), randomSignX, randomSignY ),
            minRadius, maxRadius,
            startHue, endHue
        );
    }
    
}

void emitParticlePositionColorIntervalQuantity( 
    ParticleEmitter *pe, 
    Vector2 pos,
    int minVelX, int maxVelX, 
    int minVelY, int maxVelY, 
    bool randomSignX, bool randomSignY,
    int minRadius, int maxRadius, 
    float startHue, float endHue, 
    int quantity ) {
    
    for ( int i = 0; i < quantity; i++ ) {
        emitParticlePositionColorInterval( 
            pe, 
            pos,
            createVel( (float) GetRandomValue( minVelX, maxVelX ), (float) GetRandomValue( minVelX, maxVelX ), randomSignX, randomSignY ),
            minRadius, maxRadius,
            startHue, endHue
        );
    }

}


