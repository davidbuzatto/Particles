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
#include "ParticleEmitter.h"
#include "ResourceManager.h"
#include "utils.h"

#include "raylib/raylib.h"
#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

const float GRAVITY = 20.0f;

float timeToNextObstacle = 0.1f;
float nextObstacleCounter = 0.0f;
bool showInfo = false;

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    gw->pe = createParticleEmitter( 
        (Vector2) { 40.0f, 40.0f },
        (Vector2) { 150.0f, 100.0f },
        200.0f,
        4000
    );

    gw->newObstaclePos = 0;
    gw->obstacleQuantity = 0;
    gw->maxObstacles = 400;
    gw->obstacles = (Obstacle*) malloc( gw->maxObstacles * sizeof( Obstacle ) );

    return gw;

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw ) {
    destroyParticleEmitter( &gw->pe );
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void inputAndUpdateGameWorld( GameWorld *gw ) {

    float delta = GetFrameTime();

    updateParticleEmitter( &gw->pe, delta );
    emitParticlesColorInterval( &gw->pe, 5, 180.0f, 285.0f );

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {
        emitParticlesPositionColorInterval( &gw->pe, GetMousePosition(), 5, 0.0f, 60.0f );
    }

    if ( IsMouseButtonDown( MOUSE_BUTTON_RIGHT ) ) {
        createObstacleGameWorld( gw, delta, GetMousePosition() );
    }

    if ( IsKeyPressed( KEY_F1 ) ) {
        showInfo = !showInfo;
    }

    resolveParticlesObstaclesCollision( gw );

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( BLACK );

    drawParticleEmitter( &gw->pe );

    for ( int i = 0; i < gw->obstacleQuantity; i++ ) {
        drawObstacle( &gw->obstacles[i] );
    }
    
    if ( showInfo ) {
        DrawFPS( 20, 20 );
        DrawText( TextFormat( "particles: %d", gw->pe.particleQuantity ), 20, 40, 20, WHITE );
        DrawText( TextFormat( "obstacles: %d", gw->obstacleQuantity ), 20, 60, 20, WHITE );
    }

    EndDrawing();

}

void createObstacleGameWorld( GameWorld *gw, float delta, Vector2 pos ) {

    nextObstacleCounter += delta;

    if ( nextObstacleCounter >= timeToNextObstacle ) {

        nextObstacleCounter = 0;

        int k = gw->newObstaclePos % gw->maxObstacles;

        gw->obstacles[k] = createObstacle( 
            pos, 
            (Vector2) {
                20.0f, 20.0f
            },
            RAYWHITE
        );

        gw->newObstaclePos++;
        
        if ( gw->obstacleQuantity < gw->maxObstacles ) {
            gw->obstacleQuantity++;
        }

    }

}

void resolveParticlesObstaclesCollision( GameWorld *gw ) {

    ParticleEmitter *pe = &gw->pe;
    Particle *particles = pe->particles;

    for ( int i = 0; i < pe->particleQuantity; i++ ) {

        Particle *p = &particles[i];

        for ( int j = 0; j < gw->obstacleQuantity; j++ ) {
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
