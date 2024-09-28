/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "GameWorld.h"
#include "ResourceManager.h"

#include "raylib/raylib.h"
#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

#define min( x, y ) (x) < (y) ? x : y;

const float GRAVITY = 20.0f;
float timeToNextObstacle = 0.1f;
float nextObstacleCounter = 0.0f;

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    gw->emitterPos = (Vector2) { 40.0f, 40.0f };
    gw->emitterVel = (Vector2) { 150.0f, 100.0f };
    gw->emitterAngle = 0.0f;
    gw->emitterAngleVel = 200.0f;

    gw->particleQuantity = 0;
    gw->maxParticles = 2000;
    gw->particles = (Particle*) malloc( gw->maxParticles * sizeof( Particle ) );

    gw->obstacleQuantity = 0;
    gw->maxObstacles = 200;
    gw->obstacles = (Obstacle*) malloc( gw->maxObstacles * sizeof( Obstacle ) );

    return gw;

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw ) {
    free( gw->particles );
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void inputAndUpdateGameWorld( GameWorld *gw ) {

    float delta = GetFrameTime();

    gw->emitterPos.x += gw->emitterVel.x * delta;
    gw->emitterPos.y += gw->emitterVel.y * sin( DEG2RAD * gw->emitterAngle ) * delta;
    gw->emitterAngle += gw->emitterAngleVel * delta;
    if ( gw->emitterAngle > 360.0f ) {
        gw->emitterAngle = 0.0f;
    }

    if ( gw->emitterPos.x < 40.0f ) {
        gw->emitterVel.x *= -1.0f;
    } else if ( gw->emitterPos.x >= GetScreenWidth() - 40.0f ) {
        gw->emitterVel.x *= -1.0f;
    }

    emittParticle( gw, gw->emitterPos, 5, 180.0f, 285.0f );

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {
        emittParticle( gw, GetMousePosition(), 5, 0.0f, 60.0f );
    }

    if ( IsMouseButtonDown( MOUSE_BUTTON_RIGHT ) ) {

        nextObstacleCounter += delta;

        if ( nextObstacleCounter >= timeToNextObstacle ) {

            nextObstacleCounter = 0;

            int pos = gw->obstacleQuantity % gw->maxObstacles;

            gw->obstacles[pos] = createObstacle( 
                GetMousePosition(), 
                (Vector2) {
                    20.0f, 20.0f
                },
                RAYWHITE
            );

            gw->obstacleQuantity++;

        }

    }

    int q = min( gw->particleQuantity, gw->maxParticles );
    for ( int i = 0; i < q; i++ ) {
        updateParticle( &gw->particles[i], delta );
    }

    resolveParticlesObstaclesCollision( gw );

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( BLACK );

    int q = min( gw->particleQuantity, gw->maxParticles );
    for ( int i = 0; i < q; i++ ) {
        drawParticle( &gw->particles[i] );
    }

    q = min( gw->obstacleQuantity, gw->maxObstacles );
    for ( int i = 0; i < q; i++ ) {
        drawObstacle( &gw->obstacles[i] );
    }

    EndDrawing();

}

void emittParticle( GameWorld *gw, Vector2 pos, int quantity, float startHue, float endHue ) {

    for ( int i = 0; i < quantity; i++ ) {

        int k = gw->particleQuantity % gw->maxParticles;
        float mult = GetRandomValue( 0, 1 ) == 0 ? 1.0f : -1.0f;

        gw->particles[k] = createParticle( 
            pos, 
            (Vector2) {
                GetRandomValue( 0, 150 ) * mult,
                50.0f
            },
            GetRandomValue( 2, 6 ),
            200.0f * mult, 
            ColorFromHSV( Lerp( startHue, endHue, gw->emitterAngle / 360.0f ), 1.0f, 1.0f )
        );

        gw->particleQuantity++;

    }

}

void resolveParticlesObstaclesCollision( GameWorld *gw ) {

    int pq = min( gw->particleQuantity, gw->maxParticles );
    int oq = min( gw->obstacleQuantity, gw->maxObstacles );

    for ( int i = 0; i < pq; i++ ) {
        Particle *p = &gw->particles[i];
        for ( int j = 0; j < oq; j++ ) {
            Obstacle *o = &gw->obstacles[j];
            if ( CheckCollisionCircleRec( p->pos, p->radius, o->topCP ) ) {
                p->vel.y = -200.f;
            } else if ( CheckCollisionCircleRec( p->pos, p->radius, o->bottomCP ) ) {
                //
            } else if ( CheckCollisionCircleRec( p->pos, p->radius, o->leftCP ) ) {
                p->pos.x = o->rect.x - p->radius;
                p->vel.x = -fabs( p->vel.x );
            } else if ( CheckCollisionCircleRec( p->pos, p->radius, o->rightCP ) ) {
                p->pos.x = o->rect.x + o->rect.width + p->radius;
                p->vel.x = fabs( p->vel.x );
            }
        }
    }

}
