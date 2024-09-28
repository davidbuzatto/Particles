/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld struct and function declarations.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include "Particle.h"
#include "Obstacle.h"

extern const float GRAVITY;

typedef struct GameWorld {

    Vector2 emitterPos;
    Vector2 emitterVel;
    float emitterAngle;
    float emitterAngleVel;

    int particleQuantity;
    int maxParticles;
    Particle *particles;

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

void emittParticle( GameWorld *gw, Vector2 pos, int quantity, float startHue, float endHue );
void resolveParticlesObstaclesCollision( GameWorld *gw );