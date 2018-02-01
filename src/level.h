#ifndef LEVEL_H
#define LEVEL_H

#include "digit.h"

class Level
{
public:
    Level();
    void draw(glm::mat4 VP);
    void update(int score);
    void add(int d = 2);
    void subtract(int d = 1);
private:
    int score, digslen;
    Digit digs[50];
};

#endif // SCORE_H