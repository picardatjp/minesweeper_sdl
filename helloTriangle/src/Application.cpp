#include <iostream>
#include <vector>
#include <string>
#include "Application.hpp"
#include "SDL.h"
#include "glad.h"

GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;

GLuint gGraphicsPipelineShaderProgram = 0;

const std::string gVertexShaderSource = "#version 460 core\n"
                                        "in vec4 position;\n"
                                        "void main()\n"
                                        "{\n"
                                        "   gl_Position = vec4(position.x, position.y, position.z, position.w);"
                                        "}\n";
const std::string gFragmentShaderSource = "#version 460 core\n"
                                          "out vec4 color;\n"
                                          "void main()\n"
                                          "{\n"
                                          "   color = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
                                          "}\n";

void VertexSpecification()
{
    const std::vector<GLfloat> vertexPosition{
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size() * sizeof(GLfloat), vertexPosition.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}

GLuint CompileShader(GLuint type, const std::string &source)
{
    GLuint shaderObject;
    if (type == GL_VERTEX_SHADER)
    {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
        // std::cout << "v ";
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
        // std::cout << "f ";
    }
    const GLchar *src = (const GLchar *)source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);
    GLint success;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, (GLint *)&success);
    // std::cout << glGetError();
    if (success != GL_TRUE)
    {
        std::cout << "shader not compiled!";
        GLint len;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &len);
        std::vector<GLchar> errorLog(len);
        glGetShaderInfoLog(shaderObject, len, &len, &errorLog[0]);
        for (GLchar c : errorLog)
            std::cout << c;
        glDeleteShader(shaderObject);
    }
    return shaderObject;
}

GLuint CreateShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
{
    GLuint programObject = glCreateProgram();
    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);
    GLint isLinked;
    glGetProgramiv(programObject, GL_LINK_STATUS, (GLint *)&isLinked);
    if (isLinked != GL_TRUE)
    {
        std::cout << "program not linked!\n";
        GLint len;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &len);
        std::vector<GLchar> errorLog(len);
        glGetProgramInfoLog(programObject, len, &len, &errorLog[0]);
        for (GLchar c : errorLog)
            std::cout << c;
        glDeleteProgram(programObject);
        glDeleteShader(myVertexShader);
        glDeleteShader(myFragmentShader);
    }
    // std::cout << glGetError();

    return programObject;
}

void CreateGraphicsPipeline()
{
    gGraphicsPipelineShaderProgram = CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);
}

void getOpenGLInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}

// Application Constructor
Application::Application()
{
}

// Application Destructor
Application::~Application()
{
}

// Application initializer function
void Application::initSDL(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    // flags
    // int flags = 0;
    // flags |= SDL_WINDOW_OPENGL;
    // if (fullscreen)
    // {
    //     // add fullscreen to our flags
    //     flags |= SDL_WINDOW_FULLSCREEN;
    // }

    // SDL_Init returns 0 if everything went well
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        // we create the window here
        window_ = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_OPENGL);
        if (window_ == nullptr)
            std::cout << "Failed to create window." << std::endl;

        context_ = SDL_GL_CreateContext(window_);
        std::cout << SDL_GetError();
        if (context_ == nullptr)
            std::cout << "Failed to create opengl context" << std::endl;

        if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
            std::cout << "glad was not initialized";

        // we made it here so the game is now running
        is_running_ = true;
    }
    else
    {
        // SDL_Init returned something other than zero, so some error occured
        std::cout << "Failed to initialize SDL." << std::endl;
        // no longer running
        is_running_ = false;
    }
}

void Application::initOpenGL()
{
    getOpenGLInfo();
    VertexSpecification();
    CreateGraphicsPipeline();
}

// this is where events like input are handled
void Application::handleEvents()
{
    // create an event
    SDL_Event event;
    const Uint8 *state = NULL;
    // poll the events
    while (SDL_PollEvent(&event) != 0)
    {
        // find what events happened
        switch (event.type)
        {
        // the window was closed
        case SDL_QUIT:
            // so we stop running the game
            is_running_ = false;
            break;
        default:
            break;
        }
    }
}

// update game objects here eg. sprite locations adn whatnot
void Application::update()
{
    // update positions and states here
}

// our render function which draws to the screen
void Application::render()
{
    /**********PRE DRAW*************/
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, WINDOW_WIDTH_, WINDOW_HEIGHT_);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gGraphicsPipelineShaderProgram);
    // std::cout << glGetError() << "\n";

    /*************DRAW***************/
    glBindVertexArray(gVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(window_);
}

// clean up memory and close things
void Application::clean()
{
    // destroy window, renderer and quit all SDL processes
    SDL_DestroyWindow(window_);
    SDL_Quit();
}