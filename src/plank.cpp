#include "plank.h"
#include "main.h"

Plank::Plank(float x, float y, color_t color , float rotation) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = rotation;
    speed = 0.1;
    plank_size = 2.0;
    plank_width = 0.5;
    static GLfloat g_vertex_buffer_data[18];
    g_vertex_buffer_data[0] = -plank_size;
    g_vertex_buffer_data[1] = 0;
    g_vertex_buffer_data[2] = 0;
    g_vertex_buffer_data[3] = plank_size;
    g_vertex_buffer_data[4] = 0;
    g_vertex_buffer_data[5] = 0;
    g_vertex_buffer_data[6] = -plank_size;
    g_vertex_buffer_data[7] = plank_width;
    g_vertex_buffer_data[8] = 0;
    g_vertex_buffer_data[9] = -plank_size;
    g_vertex_buffer_data[10] = plank_width;
    g_vertex_buffer_data[11] = 0;
    g_vertex_buffer_data[12] = plank_size;
    g_vertex_buffer_data[13] = 0;
    g_vertex_buffer_data[14] = 0;
    g_vertex_buffer_data[15] = plank_size;
    g_vertex_buffer_data[16] = plank_width;
    g_vertex_buffer_data[17] = 0;
    this->object = create3DObject(GL_TRIANGLES, 6, g_vertex_buffer_data, color, GL_FILL);
}

void Plank::tick(float pos , float size) {
    if (this->rotation == 60)
        this->position.x = pos - size*cos(30*3.141/180);
    else
        this->position.x = pos + size*cos(30*3.141/180);    
    // this->position.y -= speed;
}


void Plank::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Plank::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Plank::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 1, 1 };
    return bbox;
}
