#include "SDL.h"
#include "SDL_image.h"
#include "glad.h"
#include <iostream>
#include <vector>

// vao and vbo
GLuint my_vao = 0;
GLuint my_vbo = 0;
// program object for our shaders
GLuint shader_prg = 0;
SDL_Window *gWindow;
bool isRunning_ = true;

// vertex and fragment shader source
const std::string vss = "#version 410 core\n"
                        "in vec4 position;\n"
                        "void main()\n"
                        "{\n"
                        "   gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
                        "}\0";

const std::string fss = "#version 410 core\n"
                        "out vec4 color;\n"
                        "void main()\n"
                        "{\n"
                        "   color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
                        "}\0";

void VertexSpecification()
{
    // vertices for triangle
    // lives on cpu
    const std::vector<GLfloat> vertexPosition{
        -0.8f, -0.8f, 0.0f,
        0.8f, 0.8f, 0.0f,
        0.0f, 0.8f, 0.0f};

    // setup 1 vao and vbo
    glGenVertexArrays(1, &my_vao);
    glBindVertexArray(my_vao);

    glGenBuffers(1, &my_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, my_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size() * sizeof(GLfloat), vertexPosition.data(), GL_STATIC_DRAW);

    // our first attribute, attribute number 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);

    // unbind and disable
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}

GLuint CompileShader(GLuint type, const std::string &source)
{
    GLuint shaderObj;
    if (type == GL_VERTEX_SHADER)
    {
        shaderObj = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        shaderObj = glCreateShader(GL_FRAGMENT_SHADER);
    }
    const char *src = source.c_str();
    glShaderSource(shaderObj, 1, &src, nullptr);
    glCompileShader(shaderObj);

    return shaderObj;
}

GLuint CreateShaderProgram(const std::string &vertexshadersource, const std::string &fragmentshadersource)
{
    GLuint programObject = glCreateProgram();
    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexshadersource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentshadersource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);
    // glValidateProgram(programObject);
    GLint pls;
    glGetProgramiv(programObject, GL_LINK_STATUS, &pls);
    if (pls != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(programObject, 1024, &log_length, message);
        std::cout << message << "sdcsd";
    }
    else
    {
        std::cout << "all good";
    }

    // glDeleteShader and detach
    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    return programObject;
}

void CreateGraphicsPipeline()
{
    shader_prg = CreateShaderProgram(vss, fss);
}

void handleInput()
{
    SDL_Event event;
    const Uint8 *state = NULL;
    int x, y;
    // poll through events
    while (SDL_PollEvent(&event) != 0)
    {
        // find what events happened
        switch (event.type)
        {
        // the window was closed
        case SDL_QUIT:
            // so we stop running the game
            isRunning_ = false;
            break;
        default:
            break;
        }
    }
}

void draw()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glViewport(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(shader_prg);
    glBindVertexArray(my_vao);
    glBindBuffer(GL_ARRAY_BUFFER, my_vbo);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    SDL_GL_SwapWindow(gWindow);
    std::cout << glGetError();
}

void MainLoop()
{
    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    while (isRunning_)
    {
        frameStart = SDL_GetTicks();
        /**************EVENTS***************/
        handleInput();

        /*************RENDER*************/
        draw();

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime)
        {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
}

void initPrg()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // we create the window here
        gWindow = SDL_CreateWindow("opengl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
        SDL_GLContext context = nullptr;
        context = SDL_GL_CreateContext(gWindow);
        if (context == nullptr)
        {
            std::cout << "failed";
        }
        gladLoadGLLoader(SDL_GL_GetProcAddress);
        if (!gWindow)
            std::cout << "Failed to create window." << std::endl;

        // we made it here so the game is now running
        isRunning_ = true;
    }
    else
    {
        // SDL_Init returned something other than zero, so some error occured
        std::cout << "Failed to initialize SDL." << std::endl;
        // no longer running
        isRunning_ = false;
    }
}

int main(int argc, char *argv[])
{
    initPrg();

    VertexSpecification();

    CreateGraphicsPipeline();

    std::cout << glGetString(GL_VERSION) << "\n";

    MainLoop();

    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    return 0;
}