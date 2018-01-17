#include "main.h"
#include "timer.h"
#include "ball.h"
#include "villain.h"
#include "ground.h"
#include "pool.h"
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
int vill_cnt = 30 , depth = 5 , screen_size = 10;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

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
    for(int i = 0 ; i < vill_cnt ; i++){
        vill[i].draw(VP);
    }
    ground.draw(VP);
    pool.draw(VP);
    ball1.draw(VP);    
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN); //only for testing
    if (left) {
        ball1.tick_left();
    }

    if (right) {
        ball1.tick_right();
    }

    if (up) {
        if(ball1.position.y == -screen_size + depth + 1.0)
            ball1.launch_speed = 0.4;
        if(ball1.position.y < -screen_size + depth + 1.0)
            ball1.launch_speed = 0.3;        
    }

    if (down) {
        ball1.tick_down(); // only for testing
    }
}

void tick_elements() {

    for(int i = 0 ; i < vill_cnt ; i++){
        vill[i].tick();
    }
    ball1.tick_up();
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    float ball_size = 1;
    ball1       = Ball(0.0 , -screen_size + depth + ball_size, COLOR_RED,ball_size);
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
        vill[i].speed = -tmp_speed;
    }
    ground = Ground(COLOR_BLACK,screen_size,depth);
    pool = Pool(COLOR_BLUE,0,-depth);
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
    int width  = 600;
    int height = 600;

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
