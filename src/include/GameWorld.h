/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld struct and function declarations.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include "Particle.h"
#include "ParticleEmitter.h"
#include "Obstacle.h"

extern const float GRAVITY;

typedef struct GameWorld {

    ParticleEmitter peMoveSin;
    ParticleEmitter peStaticRight;

    int emittersQuantity;
    ParticleEmitter **emitters;

    int newObstaclePos;
    int obstacleQuantity;
    int maxObstacles;
    Obstacle *obstacles;
    
} GameWorld;

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void );

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw );

/**
 * @brief Reads user input and updates the state of the game.
 */
void inputAndUpdateGameWorld( GameWorld *gw );

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw );

void createObstacleGameWorld( GameWorld *gw, float delta, Vector2 pos );
void resolveParticlesObstaclesCollision( GameWorld *gw );
void saveObstacleData( GameWorld *gw, const char *fileName );
void loadObstacleData( GameWorld *gw, const char *fileName );