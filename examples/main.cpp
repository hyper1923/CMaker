#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#define WIDTH 800
#define HEIGHT 600


class Camera{
public:
    glm::vec3 pos;
    glm::vec3 rot;
    glm::mat4 matrix = glm::mat4(1.f);
    glm::mat4 projectionMatrix() const {return glm::perspective(45.0f,800.f / 600.f, 0.05f,100.f);}


    void Render(){
        glScissor(0, 0, WIDTH, HEIGHT);
        glViewport(0, 0, WIDTH, HEIGHT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        set_pos(pos.x,pos.y,pos.z);
        glm::mat4 _ftransform = projectionMatrix() * matrix;
        glLoadMatrixf(&_ftransform[0][0]);
        glMatrixMode(GL_MODELVIEW);
    }

private:
    void set_pos(float x, float y, float z) {
        matrix = glm::translate(glm::mat4(1.f), glm::vec3(-x,-y,-z));
    }

    void fps(const float& _sensivity, const glm::vec2& _mouse_pos) {
        //matrix = glm::lookAt()
    };
};

struct Vec3{
    float x;
    float y;
    float z;
};

class Mesh{
public:
    std::vector<float> vertics;
    float angle = 0;
    void Render(){
        glLoadIdentity();
        glRotatef(10,angle,5,5);
        angle++;
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), &vertics[0]);
        glDrawArrays(GL_TRIANGLES, 0, vertics.size()-2);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

};


void LoadObj(const char *path,Mesh* mesh){
    FILE* f = fopen(path,"r");
    if(f == 0){
        printf("An error occured while trying to parse .OBJ file\n\r");
    }
    std::vector<Vec3> tempVec;
    while(1){
        char lineHeader[128];
        int result = fscanf(f,"%s",lineHeader);
        if(result == EOF){
            break;
        }
        if(strcmp(lineHeader,"v") == 0){
            Vec3 vecTemp;
            int matches = fscanf(f,"%f %f %f\n",&vecTemp.x,&vecTemp.y,&vecTemp.z);
            tempVec.push_back(vecTemp);
        }
        if(strcmp(lineHeader,"f") == 0){
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            Vec3 vector1 = tempVec[vertexIndex[0]-1];
            Vec3 vector2 = tempVec[vertexIndex[1]-1];
            Vec3 vector3 = tempVec[vertexIndex[2]-1];

            mesh->vertics.push_back(vector1.x);
            mesh->vertics.push_back(vector1.y);
            mesh->vertics.push_back(vector1.z);

            mesh->vertics.push_back(vector2.x);
            mesh->vertics.push_back(vector2.y);
            mesh->vertics.push_back(vector2.z);

            mesh->vertics.push_back(vector3.x);
            mesh->vertics.push_back(vector3.y);
            mesh->vertics.push_back(vector3.z);
        }


        
    }

    fclose(f);
    
}

Camera cam;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_A){
        cam.pos.x -= 1;
    }
    if(key == GLFW_KEY_D){
        cam.pos.x += 1;
    }
    if(key == GLFW_KEY_W){
        cam.pos.z += 1;
    }
    if(key == GLFW_KEY_S){
        cam.pos.z -= 1;
    }
    if(key == GLFW_KEY_E){
        cam.pos.y += 1;
    }
    if(key == GLFW_KEY_Q){
        cam.pos.y -= 1;
    }
}

int main(){
    glfwInit();
    GLFWwindow *window = glfwCreateWindow(WIDTH,HEIGHT,"GLFW App",0,0);
    Mesh mesh;
    if(!window){
        printf("An error occured while trying to open window\n\r");
    }
    glfwSetKeyCallback(window,key_callback);
    LoadObj("untitled.obj",&mesh);
    glfwMakeContextCurrent(window);
    while(!glfwWindowShouldClose(window)){
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        cam.Render();
        mesh.Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    glfwDestroyWindow(window);
    exit(EXIT_SUCCESS);
    
}