#include "Obstacle.h"
#include "raylib/raylib.h"

Obstacle createObstacle( Vector2 pos, Vector2 dim, Color color ) {

    float marginP = 0.1f;
    float thicknessP = 0.3f; 

    return (Obstacle) {
        .rect = {
            .x = pos.x,
            .y = pos.y,
            .width = dim.x,
            .height = dim.y
        },
        .topCP = {
            .x = pos.x + dim.x * marginP,
            .y = pos.y,
            .width = dim.x * ( 1.0f - marginP * 2 ),
            .height = dim.y * thicknessP
        },
        .bottomCP = {
            .x = pos.x + dim.x * marginP,
            .y = pos.y + dim.y - dim.y * thicknessP,
            .width = dim.x * ( 1.0f - marginP * 2 ),
            .height = dim.y * thicknessP
        },
        .leftCP = {
            .x = pos.x,
            .y = pos.y + dim.y * marginP,
            .width = dim.x * thicknessP,
            .height = dim.y * ( 1.0f - marginP * 2 )
        },
        .rightCP = {
            .x = pos.x + dim.x - dim.x * thicknessP,
            .y = pos.y + dim.y * marginP,
            .width = dim.x * thicknessP,
            .height = dim.y * ( 1.0f - marginP * 2 )
        },
        .color = color
    };
}

void drawObstacle( Obstacle *obstacle ) {
    DrawRectangleRec( obstacle->rect, obstacle->color );
    /*DrawRectangleRec( obstacle->topCP, GREEN );
    DrawRectangleRec( obstacle->bottomCP, RED );
    DrawRectangleRec( obstacle->leftCP, BLUE );
    DrawRectangleRec( obstacle->rightCP, YELLOW );*/
}