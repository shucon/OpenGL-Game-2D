#include "main.h"

#ifndef PLANK_H
#define PLANK_H


class Plank {
public:
    Plank() {}
    Plank(float x, float y, color_t color , float rotation);
    glm::vec3 position;
    float plank_size;
    float plank_width;
    float rotation;
    float size;
    void tick(float pos_x , float size);
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    double speed;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif
