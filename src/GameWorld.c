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
bool showInfo = true;
float currentZoom = 1.0f;

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    gw->peMoveSin = createParticleEmitter( 
        (Vector2) { 40.0f, 40.0f },
        (Vector2) { 150.0f, 100.0f },
        200.0f,
        200.0f,
        1000
    );

    gw->peMouseDown = createParticleEmitter( 
        (Vector2) { 0 },
        (Vector2) { 0 },
        0.0f,
        200.0f,
        1000
    );

    gw->peStaticRight = createParticleEmitter( 
        (Vector2) { 40.0f, GetScreenHeight() / 2 },
        (Vector2) { 0.0f, 0.0f },
        0.0f,
        200.0f,
        1000
    );

    gw->peStaticTop = createParticleEmitter( 
        (Vector2) { GetScreenWidth() * 0.75f, GetScreenHeight() - 10 },
        (Vector2) { 0.0f, 0.0f },
        0.0f,
        200.0f,
        1000
    );

    gw->emittersQuantity = 4;
    gw->emitters = (ParticleEmitter**) malloc( gw->emittersQuantity * sizeof( ParticleEmitter** ) );
    gw->emitters[0] = &gw->peMoveSin;
    gw->emitters[1] = &gw->peMouseDown;
    gw->emitters[2] = &gw->peStaticRight;
    gw->emitters[3] = &gw->peStaticTop;

    gw->newObstaclePos = 0;
    gw->obstacleQuantity = 0;
    gw->maxObstacles = 400;
    gw->obstacles = (Obstacle*) malloc( gw->maxObstacles * sizeof( Obstacle ) );

    gw->camera = (Camera2D) {
        .target = { GetScreenWidth() / 2, GetScreenHeight() / 2 },
        .offset = { GetScreenWidth() / 2, GetScreenHeight() / 2 },
        .rotation = 0.0f,
        .zoom = 1.0f
    };

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

    emitParticleColorIntervalQuantity( 
        &gw->peMoveSin, 
        0, 150,
        50, 50,
        true, false,
        2, 6,
        180.0f, 240.0f,
        5
    );

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {
        emitParticlePositionColorIntervalQuantity( 
            &gw->peMouseDown, 
            GetScreenToWorld2D( GetMousePosition(), gw->camera ), 
            0, 150,
            50, 50,
            true, false,
            2, 6,
            0.0f, 60.0f,
            5
        );
    }

    emitParticleColorIntervalQuantity( 
        &gw->peStaticRight, 
        300, 500, 
        0, 0,
        false, false,
        2, 6,
        75.0f, 165.0f, 
        5
    );

    emitParticleColorIntervalQuantity( 
        &gw->peStaticTop, 
        0, 40, 
        -400, -600,
        true, false,
        1, 3,
        270.0f, 330.0f, 
        5
    );

    updateParticleEmitterMoveSin( &gw->peMoveSin, delta );
    updateParticleEmitterMouseDown( &gw->peMouseDown, delta );
    updateParticleEmitterStatic( &gw->peStaticRight, delta );
    updateParticleEmitterStatic( &gw->peStaticTop, delta );

    if ( IsMouseButtonDown( MOUSE_BUTTON_RIGHT ) ) {
        createObstacleGameWorld( gw, delta, GetScreenToWorld2D( GetMousePosition(), gw->camera ) );
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

    if ( IsKeyPressed( KEY_F7 ) ) {
        resetObstacles( gw );
    }

    resolveParticlesObstaclesCollision( gw );

    if ( IsKeyPressed( KEY_UP ) ) {
        currentZoom += 0.1f;
    } else if ( IsKeyPressed( KEY_DOWN ) ) {
        currentZoom -= 0.1f;
        if ( currentZoom <= 0.0f ) {
            currentZoom = 0.1f;
        }
    }

    updateCamera( &gw->camera );

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( BLACK );

    BeginMode2D( gw->camera );

    drawParticleEmitter( &gw->peMoveSin );
    drawParticleEmitter( &gw->peMouseDown );
    drawParticleEmitter( &gw->peStaticRight );
    drawParticleEmitter( &gw->peStaticTop );

    for ( int i = 0; i < gw->obstacleQuantity; i++ ) {
        drawObstacle( &gw->obstacles[i] );
    }
    
    if ( showInfo ) {
        DrawFPS( 20, 20 );
        int y = 20;
        DrawText( TextFormat( "particles (moving): %d", gw->peMoveSin.particleQuantity ), 20, y += 20, 20, WHITE );
        DrawText( TextFormat( "particles (mouse): %d", gw->peMouseDown.particleQuantity ), 20, y += 20, 20, WHITE );
        DrawText( TextFormat( "particles (static left): %d", gw->peStaticRight.particleQuantity ), 20, y += 20, 20, WHITE );
        DrawText( TextFormat( "particles (static right): %d", gw->peStaticTop.particleQuantity ), 20, y += 20, 20, WHITE );
        DrawText( TextFormat( "obstacles: %d", gw->obstacleQuantity ), 20, (y += 20), 20, WHITE );
        DrawText( "<F5>: save obstacles", 20, (y += 20), 20, WHITE );
        DrawText( "<F6>: load obstacles", 20, (y += 20), 20, WHITE );
        DrawText( "<F7>: reset obstacles", 20, (y += 20), 20, WHITE );
    }

    EndMode2D();
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
                    p->pos.y = o->rect.y + o->rect.height + p->radius;
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
    
    FILE *file = fopen( fileName, "w" );

    if ( file != NULL ) {

        fprintf( file, "%d\n", gw->maxObstacles );
        fprintf( file, "%d\n", gw->obstacleQuantity );

        for ( int i = 0; i < gw->obstacleQuantity; i++ ) {
            Obstacle *o = &gw->obstacles[i];
            fprintf( file, "%.2f %.2f %.2f %.2f\n", o->rect.x, o->rect.y, o->rect.width, o->rect.height );
        }

        fclose( file );

    }

}

void loadObstacleData( GameWorld *gw, const char *fileName ) {
    
    FILE *file = fopen( fileName, "r" );
    
    if ( file != NULL ) {

        free( gw->obstacles );

        fscanf( file, "%d", &gw->maxObstacles );
        fscanf( file, "%d", &gw->obstacleQuantity );
        gw->newObstaclePos = 0;
        gw->obstacles = (Obstacle*) malloc( gw->maxObstacles * sizeof( Obstacle ) );

        int k = 0;

        while ( !feof( file ) ) {

            float x;
            float y;
            float width;
            float height;

            int read = fscanf( file, "%f %f %f %f", &x, &y, &width, &height );
            gw->obstacles[k++] = createObstacle( (Vector2){ x, y }, (Vector2){ width, height }, RAYWHITE );

            if ( read != 4 ) {
                break;
            }

        }

        fclose( file );

    }

}

void resetObstacles( GameWorld *gw ) {
    gw->newObstaclePos = 0;
    gw->obstacleQuantity = 0;
}

void updateCamera( Camera2D *camera ) {

    float hWidth = GetScreenWidth() / 2;
    float hHeight = GetScreenHeight() / 2;

    camera->zoom = currentZoom;

    camera->target.x = hWidth;
    camera->target.y = hHeight;

    camera->offset.x = hWidth + ( hWidth * ( camera->zoom - 1.0f ) ); 
    camera->offset.y = hHeight + ( hHeight * ( camera->zoom - 1.0f ) );

}
