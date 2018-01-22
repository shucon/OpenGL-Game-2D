#include "porcupine.h"
#include "main.h"

Porcupine::Porcupine(color_t color , int count_tri,float x ,float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    static GLfloat g_vertex_buffer_data[10000];
    int size = 1;
    float temp_x = 0,temp_y = 0;
    int count = 0;
    for(int i = 0;i<count_tri;i++){
        g_vertex_buffer_data[count]=temp_x;
        g_vertex_buffer_data[count+1]=temp_y;
        g_vertex_buffer_data[count+2]=0.0f;
        //2nd vertex
        g_vertex_buffer_data[count+3]=temp_x+size*0.5;
        g_vertex_buffer_data[count+4]=temp_y+size*0.866;
        g_vertex_buffer_data[count+5]=0.0f;
        //3rd vertex
        count+=6;
        g_vertex_buffer_data[count]=temp_x+size;
        g_vertex_buffer_data[count+1]=temp_y;
        g_vertex_buffer_data[count+2]=0.0f;
        count += 3;
        temp_x = temp_x + size;
    }
    this->object = create3DObject(GL_TRIANGLES, 9*count_tri, g_vertex_buffer_data, color, GL_FILL);
}

void Porcupine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Porcupine::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Porcupine::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
