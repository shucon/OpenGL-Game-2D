#include "main.h"
#include "timer.h"
#include "ball.h"
#include "villain.h"
#include "ground.h"
#include "pool.h"
#include "plank.h"
#include "trampoline.h"
#include "magnet.h"
#include "porcupine.h"
#include "score.h"
#include "level.h"
#include <stdlib.h>

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1;
Villain vill[1000];
Ground ground;
Pool pool;
Plank plank[10];
Trampoline jump;
Magnet magnet[2];
Porcupine porc;
Score score_dis;
Level level;
int vill_cnt = 15 , depth = 5 , screen_size = 10,plank_cnt = 0,scr_cnt=0,lvl_cnt=1;
unsigned int time_stamp = 0 , time_cnt = 0;
int porc_flag = 0 , magnet_flag = 0;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0 , pi = 3.141;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);
    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render

    magnet[0].draw(VP);
    magnet[1].draw(VP);
    for(int i = 0 ; i < vill_cnt ; i++){
        vill[i].draw(VP);
    }
    ground.draw(VP);
    pool.draw(VP);
    jump.draw(VP);
    ball1.draw(VP);    
    for(int i = 0 ; i < plank_cnt ; i++){
        plank[i].draw(VP);
    }
    porc.draw(VP);
    score_dis.draw(VP);
    level.draw (VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    // int down = glfwGetKey(window, GLFW_KEY_DOWN); //only for testing
    if (left && time_stamp==0) {
        ball1.tick_left();
    }

    if (right && time_stamp==0) {
        if (ball1.position.y == -4.0 && ball1.position.x>=jump.position.x-jump.size-jump.board-1 && ball1.position.x<=jump.position.x+jump.size+jump.board+1)
        ball1.position.x=jump.position.x-jump.size-jump.board-1;
        else
        ball1.tick_right();
    }

    if (up) {
        // printf("%lf  %lf\n",ball1.position.y,-screen_size + depth + 1.0 + jump.size + jump.board - 0.3 );
        if(ball1.position.y == -screen_size + depth + 1.0 && (ball1.position.x < -3 || ball1.position.x > 3))
            ball1.launch_speed = 0.4;
        if((float)ball1.position.y == (float)ball1.pond_bot) //POOL
            ball1.launch_speed = 0.25;        
        // if((float)ball1.position.y == (float)(-screen_size + depth + 1.0 + jump.size + jump.board - 0.3) && ball1.position.x > jump.position.x-jump.size-jump.board ) //TRAMPOLINE        
        //     ball1.launch_speed = 0.5;        
    }

    // if (down) {
    //     ball1.tick_down(); // only for testing
    // }
}
void span_screen(){
    reset_screen();
}

void zoom_screen(int type){
    if(type == 1 && screen_zoom > 0.4)
        screen_zoom -= 0.03;
    else if(type == -1 && screen_zoom < 1.6){
        screen_zoom += 0.03;
    }
}

void touch_plank(){
    float x1, x2, y1, y2, slope, m1, b, d;
    for(int i=0;i<plank_cnt;i++){
        float plank_x = plank[i].position.x;
        float plank_y = plank[i].position.y;
        float plank_l = plank[i].plank_size;
        float plank_w = plank[i].plank_width;
        slope = plank[i].rotation;
        m1 = tan(slope*pi/180);
        x1 = ball1.position.x;
        y1 = ball1.position.y;
        y2 = (m1*x1 - y1 - m1*plank_x + plank_y)/(1+m1*m1) + y1;
        x2 = (-1*m1)*(m1*x1 - y1 - m1*plank_x + plank_y)/(1+m1*m1) + x1;
        d = abs(m1*x1 - y1 - m1*plank_x + plank_y)/sqrt(1+m1*m1);
        b = (x2-plank_x)*(x2-plank_x)+(y2-plank_y)*(y2-plank_y);
        if(b<= plank_l*plank_l){
            if(d<=ball1.size && ball1.launch_speed<=0){
                ball1.launch_speed = -1*ball1.launch_speed;
                if(slope<=90 && slope>0)
                    ball1.speedx = -0.15;
                else
                    ball1.speedx = 0.15;
                time_stamp = time_cnt;
            }
        }
    }
}

void tick_elements() {
    
    span_screen();
    if (ball1.position.y <= -4){
        ball1.speedx = 0;
        time_stamp = 0 ;
    }
    ball1.position.x += ball1.speedx;
    touch_plank();
    if (ball1.position.x > porc.position.x && ball1.position.x < porc.position.x+3 && ball1.position.y <= -3){
        ball1.position.x = -8;
        ball1.position.y = -5;
        ball1.launch_speed = 0;
        porc.position.x = -100;
    }

    if(time_cnt % 300 == 0 && porc_flag %2 ==0 && lvl_cnt > 1){
        porc.position.x = -rand()%3 - 6;
        if(porc_flag % 8 == 0)
            porc.position.x = 3;
        porc_flag++;        
    }

    else if(time_cnt % 300 == 0 && porc_flag%2==1 && lvl_cnt > 1){
        porc.position.x = -100;
        porc_flag++;
        if (time_cnt > 10000)
            time_cnt = 0;
    }

    if (magnet[0].position.x < 0 && magnet[0].position.x > -10 && lvl_cnt > 2)
        if(ball1.position.x > -10)
        ball1.position.x -= 0.01*lvl_cnt;        
    if (magnet[0].position.x > 0 && lvl_cnt > 2)
        if(ball1.position.x < 10)    
        ball1.position.x += 0.01*lvl_cnt;
    
    if(time_cnt % 700 == 0 && magnet_flag %2 ==0 && lvl_cnt > 2){
        magnet[0].position.x = -8;
        magnet[1].position.x = magnet[0].position.x;
        magnet[0].rotation = -90;
        magnet[1].rotation = -90;
        if(magnet_flag % 7 == 0){
            magnet[0].position.x = 8;
            magnet[1].position.x = magnet[0].position.x;
            magnet[0].rotation = 90;
            magnet[1].rotation = 90;
            ball1.position.x += 0.05;
        }
        magnet_flag++;        
    }

    else if(time_cnt % 700 == 0 && magnet_flag%2==1 && lvl_cnt > 2){
        magnet[0].position.x = -100;
        magnet[1].position.x = -100;
        magnet_flag++;
    }

    ball1.tick_up(pool,jump);
    for(int i = 0 ; i < vill_cnt ; i++){
        vill[i].tick();     
        if (detect_collision(ball1.bounding_box(), vill[i].bounding_box()) && ball1.launch_speed < 0) {
            vill[i].position.x = 11;
            ball1.launch_speed = -ball1.launch_speed;
            ball1.speedx = 0;
            time_stamp = 0 ;        
            score_dis.add(1);
            scr_cnt++;
            if (scr_cnt%5 == 0){
                lvl_cnt++;
                level.add(1);
            }
        }
    }

    for(int i = 0 ; i < plank_cnt ; i++){
        plank[i].tick(vill[i*5].position.x , 0.9);
    }
    time_cnt++;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    float ball_size = 1;
    ball1       = Ball(-9.0 , -screen_size + depth + ball_size, COLOR_RED,ball_size);
    for(int i = 0 ; i < vill_cnt ; i++){  
        float x = (((i+1)*rand()+i*584)%10000)/1000;  
        float y = (((i+1)*rand()+i*784)%10000)/1000;
        int temp = (i+1)%5;
        ball_size = 1 - (0.1*(float)temp);
        if (y < -(float)(screen_size - depth - 4)){
            y = -y ;
        }
        double tmp_speed = (double)(i%10+1)/100;
        vill[i]       = Villain(x, y, COLOR_GREEN,ball_size);
        if (i%5 == 0){
            // printf("%d",plank_cnt);
            if(i%2 == 0)
                plank[plank_cnt]       = Plank(x - ball_size*cos(30*3.141/180), y + ball_size*sin(30*3.141/180), COLOR_BROWN , 60);
            else
                plank[plank_cnt]       = Plank(x + ball_size*cos(30*3.141/180), y + ball_size*sin(30*3.141/180), COLOR_BROWN , -60);                
            plank[plank_cnt].speed = tmp_speed;
            plank_cnt++;
        }
        vill[i].speed = -tmp_speed;
    }
    ground = Ground(COLOR_BLACK,screen_size,depth);
    pool = Pool(COLOR_BLUE,0,-depth);
    jump = Trampoline(COLOR_RED,8,-3.8);
    magnet[0] = Magnet(COLOR_MAGNET,2,-6,4);
    magnet[1] = Magnet(COLOR_BACKGROUND,0.5,-6,4);
    magnet[0].position.x = -100;
    magnet[1].position.x = -100;
    porc = Porcupine(COLOR_MAGNET,3,0,-(screen_size-depth));
    porc.position.x = -100;
    score_dis.update(0);
    level.update(1);
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 800;
    int height = 800;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + screen_size / screen_zoom;
    float bottom = screen_center_y - screen_size / screen_zoom;
    float left   = screen_center_x - screen_size / screen_zoom;
    float right  = screen_center_x + screen_size / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
