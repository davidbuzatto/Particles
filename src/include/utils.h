/**
 * @file utils.h
 * @author Prof. Dr. David Buzatto
 * @brief Utilitary function and constant declarations.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include "raylib/raylib.h"

#define min( x, y ) (x) < (y) ? x : y;
#define max( x, y ) (x) > (y) ? x : y;

double toRadians( double degrees );
double toDegrees( double radians );
Vector2 createVel( float velX, float velY, bool randomSignX, bool randomSignY );