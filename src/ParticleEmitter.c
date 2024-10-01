#include <stdlib.h>
#include <math.h>

#include "Particle.h"
#include "ParticleEmitter.h"
#include "GameWorld.h"
#include "utils.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"

#define PE_RANDOM_MULTIPLIER 1000.0f

ParticleEmitter createParticleEmitter( Vector2 pos, Vector2 vel, float angleVel, float hueAngleVel, int maxParticles ) {

    return (ParticleEmitter) {
        .pos = pos,
        .vel = vel,
        .angle = 0.0f,
        .angleVel = angleVel,
        .hueAngle = 0.0f,
        .hueAngleVel = hueAngleVel,
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

    updateHueAngleBouncing( pe, delta );

    if ( pe->pos.x < 40.0f ) {
        pe->vel.x *= -1.0f;
    } else if ( pe->pos.x >= GetScreenWidth() - 40.0f ) {
        pe->vel.x *= -1.0f;
    }

    for ( int i = 0; i < pe->particleQuantity; i++ ) {
        updateParticle( &pe->particles[i], delta );
    }

}

void updateParticleEmitterMouseDown( ParticleEmitter *pe, float delta ) {

    updateHueAngleBouncing( pe, delta );

    for ( int i = 0; i < pe->particleQuantity; i++ ) {
        updateParticle( &pe->particles[i], delta );
    }

}

void updateParticleEmitterStatic( ParticleEmitter *pe, float delta ) {

    updateHueAngleBouncing( pe, delta );

    for ( int i = 0; i < pe->particleQuantity; i++ ) {
        updateParticle( &pe->particles[i], delta );
    }

}

void updateHueAngleBouncing( ParticleEmitter *pe, float delta ) {

    pe->hueAngle += pe->hueAngleVel * delta;
    if ( pe->hueAngle < 0.0f ) {
        pe->hueAngle = 0.0f;
        pe->hueAngleVel *= -1.0f;
    }
    if ( pe->hueAngle > 360.0f ) {
        pe->hueAngle = 360.0f;
        pe->hueAngleVel *= -1.0f;
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

void emitParticleColorInterval( ParticleEmitter *pe, Vector2 vel, float minRadius, float maxRadius, float startHue, float endHue ) {
    emitParticlePositionColorInterval( pe, pe->pos, vel, minRadius, maxRadius, startHue, endHue );
}

void emitParticlePositionColorInterval( ParticleEmitter *pe, Vector2 pos, Vector2 vel, float minRadius, float maxRadius, float startHue, float endHue ) {
    emitParticle( 
        pe, 
        pos, 
        vel, 
        GetRandomValue( (int) (minRadius * PE_RANDOM_MULTIPLIER), (int) (maxRadius * PE_RANDOM_MULTIPLIER) ) / PE_RANDOM_MULTIPLIER,
        ColorFromHSV( 
            Lerp( startHue, endHue, pe->hueAngle / 360.0f ), 
            1.0f, 
            1.0f
        )
    );
}

void emitParticleColorIntervalQuantity( 
    ParticleEmitter *pe, 
    float minVelX, float maxVelX, 
    float minVelY, float maxVelY, 
    bool randomSignX, bool randomSignY,
    float minRadius, float maxRadius, 
    float startHue, float endHue, 
    int quantity ) {
    
    for ( int i = 0; i < quantity; i++ ) {
        emitParticleColorInterval( 
            pe, 
            createVel( 
                GetRandomValue( (int) (minVelX * PE_RANDOM_MULTIPLIER), (int) (maxVelX * PE_RANDOM_MULTIPLIER) ) / PE_RANDOM_MULTIPLIER, 
                GetRandomValue( (int) (minVelY * PE_RANDOM_MULTIPLIER), (int) (maxVelY * PE_RANDOM_MULTIPLIER) ) / PE_RANDOM_MULTIPLIER, 
                randomSignX, randomSignY
            ),
            minRadius, maxRadius,
            startHue, endHue
        );
    }
    
}

void emitParticlePositionColorIntervalQuantity( 
    ParticleEmitter *pe, 
    Vector2 pos,
    float minVelX, float maxVelX, 
    float minVelY, float maxVelY, 
    bool randomSignX, bool randomSignY,
    float minRadius, float maxRadius, 
    float startHue, float endHue, 
    int quantity ) {
    
    for ( int i = 0; i < quantity; i++ ) {
        emitParticlePositionColorInterval( 
            pe, 
            pos,
            createVel( 
                GetRandomValue( (int) (minVelX * PE_RANDOM_MULTIPLIER), (int) (maxVelX * PE_RANDOM_MULTIPLIER) ) / PE_RANDOM_MULTIPLIER, 
                GetRandomValue( (int) (minVelY * PE_RANDOM_MULTIPLIER), (int) (maxVelY * PE_RANDOM_MULTIPLIER) ) / PE_RANDOM_MULTIPLIER, 
                randomSignX, randomSignY
            ),
            minRadius, maxRadius,
            startHue, endHue
        );
    }

}


