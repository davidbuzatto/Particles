/**
 * @file utils.c
 * @author Prof. Dr. David Buzatto
 * @brief Utilitary functions implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "raylib/raylib.h"

double toRadians( double degrees ) {
    return degrees * PI / 180.0;
}

double toDegrees( double radians ) {
    return radians * 180.0 / PI;
}

Vector2 createVel( float velX, float velY, bool randomSignX, bool randomSignY ) {
    
    float multX = randomSignX ? ( GetRandomValue( 0, 1 ) == 0 ? 1.0 : -1.0 ) : 1.0;
    float multY = randomSignY ? ( GetRandomValue( 0, 1 ) == 0 ? 1.0 : -1.0 ) : 1.0;

    return (Vector2) {
        velX * multX,
        velY * multY
    };

}