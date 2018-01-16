#include "main.h"

#ifndef VILLAIN_H
#define VILLAIN_H


class Villain {
public:
    Villain() {}
    Villain(float x, float y, color_t color ,float size);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // VILLAIN_H
