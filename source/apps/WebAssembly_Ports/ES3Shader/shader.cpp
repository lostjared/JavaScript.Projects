#include"mx.hpp"
#include"argz.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <GLES3/gl3.h>
#endif

#include"gl.hpp"
#include"loadpng.hpp"
#include<iostream>
#include<string_view>
#include<fstream>
#include<memory>
#define CHECK_GL_ERROR() \
{ GLenum err = glGetError(); \
if (err != GL_NO_ERROR) \
printf("OpenGL Error: %d at %s:%d\n", err, __FILE__, __LINE__); }

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifdef __EMSCRIPTEN__
#include "SDL_image.h"
#endif
static const char *vSource = R"(#version 300 es
precision highp float;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0); 
    TexCoord = aTexCoord;         
}
)";
static const char *testSource = R"(#version 300 es
precision highp float;
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D textTexture;
uniform float time_f;
uniform vec2 iResolution;

void main(void) {
    vec2 centeredCoord = TexCoord - 0.5;
    float angle = time_f;
    
    float zoom = abs(sin(time_f)) * 2.0 + 0.5; // Ping-pong zoom effect
    centeredCoord *= zoom;
    
    vec2 rotatedCoord;
    rotatedCoord.x = centeredCoord.x * cos(angle) - centeredCoord.y * sin(angle);
    rotatedCoord.y = centeredCoord.x * sin(angle) + centeredCoord.y * cos(angle);
    rotatedCoord += 0.5;
    
    FragColor = texture(textTexture, rotatedCoord);
}
)";
static const char *testSource2 = R"(#version 300 es
precision highp float;
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D textTexture;
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

)";

std::vector<gl::ShaderProgram *> shader_programs;
gl::GLSprite sprite;
size_t shader_index = 0;

class CVShader : public gl::GLObject {
    static CVShader* active_instance;
    std::string prev_text;
    float animation_time = 0.0f; 
public:
    static CVShader* getInstance() { return active_instance; }
    
    CVShader() {
        active_instance = this;
        std::cout << "Created CVShader instance: " << this << "\n";
    }

    GLuint texture = 0;
    float w = 960, h = 720;
    virtual ~CVShader() override {
        for(auto &shader : shader_programs) {
            delete shader;
        }
    }

    void load(gl::GLWindow *win) override {
        w = static_cast<float>(win->w);
        h = static_cast<float>(win->h);
       if(!loadCode(testSource)) {
            std::cerr << "Error loading source...\n";
       }
       texture = gl::loadTexture("/data/bg.png");
       sprite.initSize(win->w,  win->h);
       sprite.initWithTexture(shader_programs[shader_index], texture, 0.0f, 0.0f, win->w, win->h);
       CHECK_GL_ERROR();
       std::cout << "Initial load - Vector size: " << shader_programs.size() << "\n";
       CHECK_GL_ERROR();    
    }

    void draw(gl::GLWindow *win) override {
        glDisable(GL_DEPTH_TEST);
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastUpdateTime) / 1000.0f; 
        lastUpdateTime = currentTime;
        animation_time += deltaTime; 
        sprite.setShader(shader_programs[shader_index]);
        shader_programs[shader_index]->useProgram();
        GLint location1 = glGetUniformLocation(shader_programs[shader_index]->id(), "time_f");
        GLint location2 = glGetUniformLocation(shader_programs[shader_index]->id(), "iResolution");
        GLint location3 = glGetUniformLocation(shader_programs[shader_index]->id(), "alpha");
        if(location2 != -1) {
            glUniform2f(location2, w, h);
        }
        if(location1 != -1) {
            glUniform1f(location1, animation_time);
        }
        if(location3 != -1) {
            glUniform1f(location3, 1.0f);
        }
        sprite.draw();
        update(deltaTime);
    }

    bool loadCode(const std::string &code) {
        std::cout << "LoadCode called on CVShader instance: " << this << "\n";
        std::cout << "Vector address in loadCode: " << &shader_programs << "\n";
        if(code.empty()) return false;
        std::cout << "Loading code; " << code << "\n";
        std::cout << "Before new shader - Vector size: " << shader_programs.size() << "\n";
        gl::ShaderProgram *shader_program = new gl::ShaderProgram();
        try {
            if(!shader_program->loadProgramFromText(vSource, code)) {
                mx::system_err << "Error loading shader program\n";
                delete shader_program;
                return false;
            }
            animation_time = 0.0f; 
            prev_text = code;
            mx::system_out << "Loaded Succesfully...\n";
        } catch(mx::Exception &e) {
            mx::system_err << e.text() << "\n";
            delete shader_program;
            return false;
        }
        shader_programs.push_back(shader_program);
        shader_index = shader_programs.size() - 1; 
        std::cout << "After push - Vector size: " << shader_programs.size() 
                  << " Index: " << shader_index 
                  << " Program ID: " << shader_program->id() << "\n";
        return true;
    }
    
    void event(gl::GLWindow *win, SDL_Event &e) override {
        /*
        switch(e.type) {
            case SDL_KEYDOWN:
            
            switch(e.key.keysym.sym) {
               case SDLK_UP:
                std::cout << "Event called on CVShader instance: " << this << "\n";
                std::cout << "Vector address: " << &shader_programs << "\n";
                std::cout << "Event handler vector size: " << shader_programs.size() << "\n";
                if(index < shader_programs.size()-1) {
                    index++;
                    sprite.setShader(shader_programs[index]);
                    std::cout << "Index increased to: " << index << "\n";  
                }
                break;
                case SDLK_DOWN:
                if(index > 0) {
                    index--;
                    sprite.setShader(shader_programs[index]);
                    std::cout << "Index Changed to: " << index << "\n";
                }
                break;
            }
            break;
        }*/
    }
    void update(float deltaTime) {}
    
private:
    Uint32 lastUpdateTime = SDL_GetTicks();
};

int new_width = 966, new_height = 720;

CVShader* CVShader::active_instance = nullptr;


class MainWindow : public gl::GLWindow {
public:

    CVShader *shader_obj = nullptr;

    MainWindow(std::string path, int tw, int th) : gl::GLWindow("ES 3 Shader", tw, th) {
        setPath(path);
        shader_obj = CVShader::getInstance();
        if (!shader_obj) {
            shader_obj = new CVShader();
        }
        setObject(shader_obj);
        shader_obj->load(this);
    }
    
    ~MainWindow() override {}
    
    virtual void event(SDL_Event &e) override {}
    
    virtual void draw() override {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, new_width, new_height);
        shader_obj->draw(this);
        swap();
        delay();
    }
    
    void loadCode(const std::string &code) {
        std::cout << "MainWindow loadCode called on shader_obj: " << shader_obj << "\n";
        if(shader_obj->loadCode(code)) {
            std::cout << "Code loaded successfully...\n";
        } else {
            std::cerr << "Error loading code...\n";
        }
    }
};

MainWindow *main_w = nullptr;

void eventProc() {
    main_w->proc();
}

int main(int argc, char **argv) {
#ifdef __EMSCRIPTEN__
    try {
        MainWindow main_window("/", 960, 720);
        main_w = &main_window;
        emscripten_set_main_loop(eventProc, 0, 1);
    } catch (mx::Exception &e) {
        std::cerr << e.text() << "\n";
        return EXIT_FAILURE;
    }
#endif
    return 0;
}

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
using namespace emscripten;


EMSCRIPTEN_BINDINGS(shader_bindings) {
    class_<MainWindow>("MainWindow")
        .constructor<std::string, int, int>()
        .function("loadCode", &MainWindow::loadCode);
}
 
    void loadImage(const std::vector<uint8_t>& imageData) {
        std::cout << "Received image data of size: " << imageData.size() << " bytes\n";
        std::ofstream outFile("image.png", std::ios::binary);
        outFile.write(reinterpret_cast<const char*>(imageData.data()), imageData.size());
        outFile.close();
        std::cout << "Saved image as 'image.png'.\n";
        SDL_Surface *surface = png::LoadPNG("image.png");
        if(surface) {
            int imgWidth = surface->w;
            int imgHeight = surface->h;
            main_w->setWindowSize(imgWidth, imgHeight);
            emscripten_set_canvas_element_size("#canvas", imgWidth, imgHeight);
            int canvasWidth = imgWidth, canvasHeight = imgHeight;
            emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight);
            new_width = canvasWidth;
            new_height = canvasHeight;
            sprite.initSize(canvasWidth, canvasHeight);
            sprite.loadTexture(shader_programs[shader_index], "image.png", 0, 0, canvasWidth, canvasHeight);
            SDL_FreeSurface(surface);
        }
    }

    void loadImageJPG(const std::vector<uint8_t>& imageData) {
        std::ofstream outFile("image.jpg", std::ios::binary);
        outFile.write(reinterpret_cast<const char*>(imageData.data()), imageData.size());
        outFile.close();
        SDL_Surface *surface = IMG_Load("image.jpg");
        if(surface) {
            int imgWidth = surface->w;
            int imgHeight = surface->h;
            main_w->setWindowSize(imgWidth, imgHeight);
            emscripten_set_canvas_element_size("#canvas", imgWidth, imgHeight);
            int canvasWidth = imgWidth, canvasHeight = imgHeight;
            emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight);
            new_width = canvasWidth;
            new_height = canvasHeight;
            sprite.initSize(canvasWidth, canvasHeight);
            GLuint text = gl::createTexture(surface, true);
            sprite.initWithTexture(shader_programs[shader_index], text, 0, 0, canvasWidth, canvasHeight);
            SDL_FreeSurface(surface);
        }
    }

EMSCRIPTEN_BINDINGS(image_loader) {
    emscripten::function("loadImage", &loadImage);
    emscripten::function("loadImageJPG",  &loadImageJPG);
    emscripten::register_vector<uint8_t>("VectorU8");
}

#endif