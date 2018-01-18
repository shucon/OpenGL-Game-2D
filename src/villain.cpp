#include "villain.h"
#include "main.h"

Villain::Villain(float x, float y, color_t color ,float size) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 0.1;
    int n =100 ,i,count=0;
    static GLfloat g_vertex_buffer_data[10000];
    float angle = 360/n;
    for(i=0;i<=2*n;i++){
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

void Villain::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Villain::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Villain::tick() {
    this->position.x -= speed;
    // this->position.y -= speed;
    if (this->position.x > 11) { // 11 = screen_size+max_size

        int i = rand() % 100;   
        float x = (((i+1)*rand()+i*584)%10000)/1000;  
        float temp = -20 - x;
        this->position.x = temp;
    }
}

bounding_box_t Villain::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, this->size, this->size };
    return bbox;
}
