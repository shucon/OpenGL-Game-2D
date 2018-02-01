#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color ,float size) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->size = size;
    speed = 0.1;
    gravity = 0.01;
    launch_speed = 0;
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


void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick_left() {
    this->position.x -= speed;
    // this->position.y -= speed;
}

void Ball::tick_right() {
    this->position.x += speed;
    // this->position.y -= speed;
}

void Ball::tick_up(Pool pool,Trampoline jump) {
    this->position.y += launch_speed;
    launch_speed -= gravity;
    if (position.y < -10 + 5 + 1.0 + jump.size + jump.board && position.x>jump.position.x-jump.size-jump.board && position.x<jump.position.x+jump.size+jump.board)
        launch_speed = -1.1*launch_speed;
    if (position.y < -4.0 && (position.x < pool.position.x - (double)pool.size) && (position.x > pool.position.x + (double)pool.size) )
        position.y = -4.0;
    else if (position.y < -4.0 && (position.x > pool.position.x - (double)pool.size) && (position.x < pool.position.x + (double)pool.size) ){
        if(position.x < pool.position.x)
            position.x += speed/2;        
        else
            position.x -= speed/2;        
        pond_bot = -5.0-(((double)pool.size-1.0)*sin(acos(fabs((double)pool.position.x-(double)position.x)/((double)pool.size-1.0))));
        if (position.y < pond_bot && pond_bot == pond_bot){
            position.y = pond_bot;
        }
        if (pond_bot != pond_bot)
            position.y = -4.2;
    }
    else if (position.y < -4.0)
        position.y = -4.0;    
}

void Ball::tick_down() {
     this->position.y -= speed;
}

bounding_box_t Ball::bounding_box() {
    // printf("%lf",this->size);
    float x = this->position.x, y = this->position.y;
    // 1 is the ball size
    bounding_box_t bbox = { x, y, 1, 1 };
    return bbox;
}
