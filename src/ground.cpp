#include "ground.h"
#include "main.h"

Ground::Ground(color_t color ,float size ,float depth) {
    this->position = glm::vec3(0, 0, 0);
    static GLfloat g_vertex_buffer_data[18];
    g_vertex_buffer_data[0] = -size;
    g_vertex_buffer_data[1] = -size;
    g_vertex_buffer_data[2] = 0;
    g_vertex_buffer_data[3] = size;
    g_vertex_buffer_data[4] = -size;
    g_vertex_buffer_data[5] = 0;
    g_vertex_buffer_data[6] = -size;
    g_vertex_buffer_data[7] = -size+depth;
    g_vertex_buffer_data[8] = 0;
    g_vertex_buffer_data[9] = size;
    g_vertex_buffer_data[10] = -size;
    g_vertex_buffer_data[11] = 0;
    g_vertex_buffer_data[12] = -size;
    g_vertex_buffer_data[13] = -size+depth;
    g_vertex_buffer_data[14] = 0;
    g_vertex_buffer_data[15] = size;
    g_vertex_buffer_data[16] = -size+depth;
    g_vertex_buffer_data[17] = 0;
    this->object = create3DObject(GL_TRIANGLES, 6, g_vertex_buffer_data, color, GL_FILL);
}

void Ground::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ground::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Ground::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
