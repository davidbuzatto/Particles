#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "Particle.h"
#include "ParticleEmitter.h"
#include "GameWorld.h"
#include "utils.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"

#define PE_RANDOM_MULTIPLIER 1000.0f

ParticleEmitter createParticleEmitter( Vector2 pos, Vector2 vel, float launchAngle, float posAngleVel, float hueAngleVel, float radius, bool draggable, int maxParticles ) {

    return (ParticleEmitter) {
        .pos = pos,
        .vel = vel,
        .launchAngle = launchAngle,
        .posAngle = 0.0f,
        .posAngleVel = posAngleVel,
        .hueAngle = 0.0f,
        .hueAngleVel = hueAngleVel,
        .radius = radius,
        .draggable = draggable,
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

    if ( pe->draggable && pe->mouseOver ) {
        DrawCircleV( pe->pos, pe->radius, Fade( RAYWHITE, 0.5f ) );    
        DrawCircleLinesV( pe->pos, pe->radius, RAYWHITE );
    }

    for ( int i = 0; i < pe->particleQuantity; i++ ) {
        drawParticle( &pe->particles[i] );
    }

}

void updateParticleEmitterMoveSin( ParticleEmitter *pe, float delta ) {

    pe->pos.x += pe->vel.x * delta;
    pe->pos.y += pe->vel.y * sin( DEG2RAD * pe->posAngle ) * delta;

    pe->posAngle += pe->posAngleVel * delta;
    if ( pe->posAngle > 360.0f ) {
        pe->posAngle = 0.0f;
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

bool isMouseOverParticleEmitter( Vector2 pePos, float peRadius, Vector2 mousePos ) {

    float c1 = mousePos.x - pePos.x;
    float c2 = mousePos.y - pePos.y;

    return c1 * c1 + c2 * c2 <= peRadius * peRadius;

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

void emitParticlePolarColorIntervalQuantity( 
    ParticleEmitter *pe, 
    float minVel, float maxVel, 
    float minLaunchAngle, float maxLaunchAngle,
    bool randomSignLaunchAnble,
    float minRadius, float maxRadius, 
    float startHue, float endHue, 
    int quantity ) {

    for ( int i = 0; i < quantity; i++ ) {

        float rVel = GetRandomValue( (int) (minVel * PE_RANDOM_MULTIPLIER), (int) (maxVel * PE_RANDOM_MULTIPLIER) ) / PE_RANDOM_MULTIPLIER;
        float launAngleOffset = GetRandomValue( (int) (minLaunchAngle * PE_RANDOM_MULTIPLIER), (int) (maxLaunchAngle * PE_RANDOM_MULTIPLIER) ) / PE_RANDOM_MULTIPLIER;

        if ( randomSignLaunchAnble ) {
            if ( GetRandomValue( 0, 1 ) == 0 ) {
                launAngleOffset *= -1.0f;
            }
        }

        emitParticleColorInterval( 
            pe, 
            createVel( 
                rVel * sin( DEG2RAD * ( pe->launchAngle + launAngleOffset ) ), 
                rVel * cos( DEG2RAD * ( pe->launchAngle + launAngleOffset ) ), 
                false, false
            ),
            minRadius, maxRadius,
            startHue, endHue
        );
    }
    
}

void emitParticlePolarPositionColorIntervalQuantity( 
    ParticleEmitter *pe, 
    Vector2 pos,
    float minVel, float maxVel, 
    float minLaunchAngle, float maxLaunchAngle,
    bool randomSignLaunchAnble,
    float minRadius, float maxRadius, 
    float startHue, float endHue, 
    int quantity ) {

    for ( int i = 0; i < quantity; i++ ) {

        float rVel = GetRandomValue( (int) (minVel * PE_RANDOM_MULTIPLIER), (int) (maxVel * PE_RANDOM_MULTIPLIER) ) / PE_RANDOM_MULTIPLIER;
        float launAngleOffset = GetRandomValue( (int) (minLaunchAngle * PE_RANDOM_MULTIPLIER), (int) (maxLaunchAngle * PE_RANDOM_MULTIPLIER) ) / PE_RANDOM_MULTIPLIER;

        if ( randomSignLaunchAnble ) {
            if ( GetRandomValue( 0, 1 ) == 0 ) {
                launAngleOffset *= -1.0f;
            }
        }

        emitParticlePositionColorInterval( 
            pe, 
            pos,
            createVel( 
                rVel * sin( DEG2RAD * ( pe->launchAngle + launAngleOffset ) ), 
                rVel * cos( DEG2RAD * ( pe->launchAngle + launAngleOffset ) ), 
                false, false
            ),
            minRadius, maxRadius,
            startHue, endHue
        );
    }

}
