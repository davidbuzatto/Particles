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
#include <string.h>

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
const char* OBSTACLES_FILE = "resources/obstacles/data.txt";

float timeToNextObstacle = 0.1f;
float nextObstacleCounter = 0.0f;
bool showInfo = false;

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    gw->peMoveSin = createParticleEmitter( 
        (Vector2) { 40.0f, 40.0f },
        (Vector2) { 150.0f, 100.0f },
        200.0f,
        1000
    );

    gw->peStaticRight = createParticleEmitter( 
        (Vector2) { 40.0f, GetScreenHeight() / 2 },
        (Vector2) { 0.0f, 0.0f },
        200.0f,
        1000
    );

    gw->emittersQuantity = 2;
    gw->emitters = (ParticleEmitter**) malloc( gw->emittersQuantity * sizeof( ParticleEmitter** ) );
    gw->emitters[0] = &gw->peMoveSin;
    gw->emitters[1] = &gw->peStaticRight;

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
    destroyParticleEmitter( &gw->peMoveSin );
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void inputAndUpdateGameWorld( GameWorld *gw ) {

    float delta = GetFrameTime();

    updateParticleEmitterMoveSin( &gw->peMoveSin, delta );
    emitParticleColorIntervalQuantity( 
        &gw->peMoveSin, 
        0, 150,
        50, 50,
        true, false,
        2, 6,
        180.0f, 285.0f,
        5
    );

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {
        emitParticlePositionColorIntervalQuantity( 
            &gw->peMoveSin, 
            GetMousePosition(), 
            0, 150,
            50, 50,
            true, false,
            2, 6,
            0.0f, 60.0f,
            5
        );
    }

    updateParticleEmitterStaticRight( &gw->peStaticRight, delta );
    emitParticleColorIntervalQuantity( 
        &gw->peStaticRight, 
        300, 500, 
        0, 0,
        false, false,
        2, 6,
        75.0f, 165.0f, 
        5
    );

    if ( IsMouseButtonDown( MOUSE_BUTTON_RIGHT ) ) {
        createObstacleGameWorld( gw, delta, GetMousePosition() );
    }

    if ( IsKeyPressed( KEY_F1 ) ) {
        showInfo = !showInfo;
    }

    if ( IsKeyPressed( KEY_F5 ) ) {
        saveObstacleData( gw, OBSTACLES_FILE );
    }

    if ( IsKeyPressed( KEY_F6 ) ) {
        loadObstacleData( gw, OBSTACLES_FILE );
    }

    resolveParticlesObstaclesCollision( gw );

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( BLACK );

    drawParticleEmitter( &gw->peMoveSin );
    drawParticleEmitter( &gw->peStaticRight );

    for ( int i = 0; i < gw->obstacleQuantity; i++ ) {
        drawObstacle( &gw->obstacles[i] );
    }
    
    if ( showInfo ) {
        DrawFPS( 20, 20 );
        DrawText( TextFormat( "particles: %d", gw->peMoveSin.particleQuantity ), 20, 40, 20, WHITE );
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

    for ( int k = 0; k < gw->emittersQuantity; k++ ) {

        ParticleEmitter *pe = gw->emitters[k];
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

}

void saveObstacleData( GameWorld *gw, const char *fileName ) {
    


}

void loadObstacleData( GameWorld *gw, const char *fileName ) {
    
    char* data = LoadFileText( fileName );

    // obstacles count
    int count = 0;
    char *c = data;
    while ( *c != '\0' ) {
        if ( *c == '\n' ) {
            count++;
        }
        c++;
    }

    free( gw->obstacles );
    gw->newObstaclePos = 0;
    gw->obstacleQuantity = count;
    gw->obstacles = (Obstacle*) malloc( gw->obstacleQuantity * sizeof( Obstacle ) );

    char buffer[100];
    int k = 0;
    c = data;

    char *b;
    int x;
    int y;
    int dimX;
    int dimY;

    while ( *c != '\0' ) {
        if ( *c == '\n' ) {
            buffer[k] = '\0';
            
            b = buffer;
            char b2[10];
            int k2 = 0;
            int cc = 0;
            while ( *b != '\0' ) {
                if ( *b == ',' ) {
                    b2[k2] = '\0';
                    int v = atoi( b2 );
                    switch ( cc ) {
                        case 0: x = v; break;
                        case 1: y = v; break;
                        case 2: dimX = v; break;
                        case 3: dimY = v; break;
                    }
                    cc++;
                    k2 = 0;
                } else {
                    b2[k2++] = *b;
                }
                b++;
            }
            
            int o = gw->newObstaclePos % gw->maxObstacles;

            gw->obstacles[o] = createObstacle( 
                (Vector2) { x, y}, 
                (Vector2) { dimX, dimY },
                RAYWHITE
            );

            gw->newObstaclePos++;
            
            if ( gw->obstacleQuantity < gw->maxObstacles ) {
                gw->obstacleQuantity++;
            }

            buffer[0] = '\0';
            k = 0;
        } else {
            buffer[k++] = *c;
        }
        c++;
    }

}
