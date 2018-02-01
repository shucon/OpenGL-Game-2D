#include "main.h"
#include "pool.h"
#include "trampoline.h"
#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color ,float size);
    glm::vec3 position;
    float rotation;
    float size;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick_left();
    void tick_right();
    void tick_up(Pool pool,Trampoline jump);
    void tick_down();
    double speed;
    double speedx;
    double launch_speed;
    double gravity;
    double pond_bot;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
