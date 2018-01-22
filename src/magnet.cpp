#include "magnet.h"
#include "main.h"

Magnet::Magnet(color_t color , float size,float x ,float y) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 270;
    int n =200 ,i,count=0;
    static GLfloat g_vertex_buffer_data[10000];
    float angle = 360/n;
    for(i=n-20;i<=2*n-41;i++){
        float angle_t = angle*i;
        g_vertex_buffer_data[count]=size*cos(angle_t*3.141/180);
        g_vertex_buffer_data[count+1]=size*sin(angle_t*3.141/180);
        g_vertex_buffer_data[count+2]=0.0f;
        //2nd vertex
        g_vertex_buffer_data[count+3]=0.0f;
        g_vertex_buffer_data[count+4]=0.0f;
        g_vertex_buffer_data[count+5]=0.0f;
        //3rd vertex
        count+=6;
        angle_t = angle*(i+1);
        g_vertex_buffer_data[count]=size*cos(angle_t*3.141/180);
        g_vertex_buffer_data[count+1]=size*sin(angle_t*3.141/180);
        g_vertex_buffer_data[count+2]=0.0f;
        count += 3;
    }
    this->object = create3DObject(GL_TRIANGLES, 9*n, g_vertex_buffer_data, color, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Magnet::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
