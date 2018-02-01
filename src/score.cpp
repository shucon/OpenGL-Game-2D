#include "score.h"
#include "main.h"
#include <cstring>

Score::Score()
{
    score = 0;
    digslen = 0;
}

void Score::update(int score) {
    this->score = score;
    char scorestr[50];
    sprintf(scorestr, "%d", score);
    digslen = strlen(scorestr);
    for (int i = digslen - 1; i >= 0; i--) {
        digs[i] = Digit(scorestr[i], 4 - 0.3*(digslen - i)+5, 9);
    }
}

void Score::add(int d) {
    update(score+d);
}

void Score::subtract(int d) {
    update(score-d);
}
void Score::draw(glm::mat4 VP) {
    for (int i = 0; i < digslen; i++) digs[i].draw(VP);
}
