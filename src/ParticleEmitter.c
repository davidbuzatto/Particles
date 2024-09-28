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

void updateParticleEmitter( ParticleEmitter *pe, float delta ) {

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

void emitParticle( ParticleEmitter *pe, Vector2 pos, Color color ) {

    int k = pe->newParticlePos % pe->maxParticles;
    float mult = GetRandomValue( 0, 1 ) == 0 ? 1.0f : -1.0f;

    pe->particles[k] = createParticle( 
        pos, 
        (Vector2) {
            GetRandomValue( 0, 150 ) * mult,
            50.0f
        },
        GetRandomValue( 2, 6 ),
        200.0f * mult, 
        color
    );

    pe->newParticlePos++;

    if ( pe->particleQuantity < pe->maxParticles ) {
        pe->particleQuantity++;
    }

}

void emitParticlesColorInterval( ParticleEmitter *pe, int quantity, float startHue, float endHue ) {
    emitParticlesPositionColorInterval( pe, pe->pos, quantity, startHue, endHue );
}

void emitParticlesPositionColorInterval( ParticleEmitter *pe, Vector2 pos, int quantity, float startHue, float endHue ) {
    for ( int i = 0; i < quantity; i++ ) {
        emitParticle( pe, pos, ColorFromHSV( Lerp( startHue, endHue, pe->angle / 360.0f ), 1.0f, 1.0f ) );
    }
}


