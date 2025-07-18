#include "Game.h"
#include <spdlog/spdlog.h>
#include "HandleEvent.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "Texture.h"
#include <imgui_impl_sdl2.h>
#include "ImGUIWindow.h"
#include "ProgramValues.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Game::Game() {}

 bool Game::initSDL() {
    spdlog::info("Initializing SDL...");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        spdlog::error("SDL Failed to initialize: {}", SDL_GetError());
        return false;
    }
    else {
        spdlog::info("Initializing sdl window...");

        window = SDL_CreateWindow(
            "SDL OPEN_GL PRACTICE", 
            SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, 
            ProgramValues::ProgramDimensionX, 
            ProgramValues::ProgramDimensionY, 
            SDL_WINDOW_OPENGL);

        if (!window) {
            spdlog::error("Window creation failed: {}", SDL_GetError());
            spdlog::error("SDL Failed to initialize: {}", SDL_GetError());
            return false;
        }
        else {
            spdlog::info("Window initialized.");
        }

        spdlog::info("SDL initialized.");
        return true;
    }
}

 bool Game::initGLAD() {
    spdlog::info("Initializing GLAD...");

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        spdlog::error("Failed to initialize GLAD.");
        return false;
    }
    else {
        spdlog::info("GLAD initialized.");
        return true;
    }
}

 void Game::initImGUI() {
     imGUIWindow = ImGUIWindow::getInstance();
     imGUIWindow->init(window, &glContext);
 }

 void Game::initHandleEvent() {
     spdlog::info("Initializing event handler...");
     handleEvent = HandleEvent::getInstance();
     spdlog::info("Event handler initialized successfully.");
 }

 void Game::setOpenGLAttributes() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

 bool Game::initOpenGL() {
    spdlog::info("Initializing OpenGL...");

    setOpenGLAttributes();

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        return false;
        spdlog::error("SDL_GL_CreateContext failed: {}", SDL_GetError());
    }

    spdlog::info("OpenGL initialized.");
    return true;
}

 bool Game::initEverything() {
    return initSDL() && initOpenGL() && initGLAD();
}

 void Game::processInput() {
     handleEvent->input(event);
}

 void Game::tutorial() {
     ProgramValues::windowFocused = true;

    glViewport(0, 0, ProgramValues::ProgramDimensionX, ProgramValues::ProgramDimensionY);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shader = std::make_unique<Shader>("source.shader");

    { // Colors
        std::vector<GLfloat> vertices = {
            -0.2f,  0.11f,  0.2f,   0.0f, 1.0f, 
             0.2f,  0.11f,  0.2f,   1.0f, 1.0f,
            -0.2f, -0.11f,  0.2f,   0.0f, 0.0f,
             0.2f, -0.11f,  0.2f,   1.0f, 0.0f,

             -0.2f,  0.11f, -0.2f,   0.0f, 1.0f,
              0.2f,  0.11f, -0.2f,   1.0f, 1.0f,
             -0.2f, -0.11f, -0.2f,   0.0f, 0.0f,
              0.2f, -0.11f, -0.2f,   1.0f, 0.0f,

              -0.2f,  0.11f, -0.2f,   0.0f, 1.0f,
              -0.2f,  0.11f,  0.2f,   1.0f, 1.0f,
              -0.2f, -0.11f, -0.2f,   0.0f, 0.0f,
              -0.2f, -0.11f,  0.2f,   1.0f, 0.0f,

               0.2f,  0.11f,  0.2f,   0.0f, 1.0f,
               0.2f,  0.11f, -0.2f,   1.0f, 1.0f,
               0.2f, -0.11f,  0.2f,   0.0f, 0.0f,
               0.2f, -0.11f, -0.2f,   1.0f, 0.0f,

               -0.2f,  0.11f, -0.2f,   0.0f, 1.0f,
                0.2f,  0.11f, -0.2f,   1.0f, 1.0f,
               -0.2f,  0.11f,  0.2f,   0.0f, 0.0f,
                0.2f,  0.11f,  0.2f,   1.0f, 0.0f,

                -0.2f, -0.11f,  0.2f,   0.0f, 1.0f,
                 0.2f, -0.11f,  0.2f,   1.0f, 1.0f,
                -0.2f, -0.11f, -0.2f,   0.0f, 0.0f,
                 0.2f, -0.11f, -0.2f,   1.0f, 0.0f
        };

        std::vector<GLuint> indices = {
            0, 1, 2, 
            1, 2, 3,
            4, 5, 6, 
            5, 6, 7,
            8, 9, 10, 
            9, 10, 11,
            12, 13, 14, 
            13, 14, 15,
            16, 17, 18, 
            17, 18, 19,
            20, 21, 22, 
            21, 22, 23
        };

        indicesCount = indices.size();

        vertexArray = std::make_unique<VertexArray>();
        vertexBuffer = std::make_unique<VertexBuffer>(vertices.data(), vertices.size() * sizeof(GLfloat));

        vertexArray->AddBuffer(*vertexBuffer, { 3 , 2 });

        elementBuffer = std::make_unique<ElementBuffer>(indices.data(), indicesCount);
    }

    shader->use();
    glUniform3f(glGetUniformLocation(shader->ID, "u_ModifiedCoords"), ProgramValues::x, ProgramValues::x, 0.8f);
    texture = std::make_unique<Texture>("assets/pic.jpg");
    glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 0);


    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)ProgramValues::ProgramDimensionX / (float)ProgramValues::ProgramDimensionY,
        0.1f,
        1000.0f
        );


    glm::mat4 model = glm::mat4(1.0);

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "u_Projection"), 1, GL_FALSE, &projection[0][0]);


    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::vec3(glm::cross(up, cameraDirection));


    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    glm::mat4 view;
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "u_View"), 1, GL_FALSE, &view[0][0]);
}

 void Game::update() {
     SDL_SetRelativeMouseMode(ProgramValues::windowFocused ? SDL_TRUE : SDL_FALSE);

     {
         glm::mat4 projection = glm::perspective(
             glm::radians(ProgramValues::Camera::fov),
             (float)ProgramValues::ProgramDimensionX / (float)ProgramValues::ProgramDimensionY,
             0.1f,
             1000.0f
         );

         glUniformMatrix4fv(glGetUniformLocation(shader->ID, "u_Projection"), 1, GL_FALSE, &projection[0][0]);
     }

     {
         glUniform3f(glGetUniformLocation(shader->ID, "u_ModifiedCoords"), ProgramValues::x, ProgramValues::y, 0.0f);

         glUniform4f(
             glGetUniformLocation(shader->ID, "u_Color"),
             ProgramValues::v1_color.r,
             ProgramValues::v1_color.g,
             ProgramValues::v1_color.b,
             ProgramValues::v1_color.a
         );

         glUniform1f(
             glGetUniformLocation(shader->ID, "u_DimensionScalar"),
             ProgramValues::dimensionScalar
         );
     }

     {
         if (ProgramValues::Movement::FRONT) {
             ProgramValues::Camera::cameraPos += ProgramValues::Camera::cameraSpeed * ProgramValues::Camera::cameraFront;
         }

         if (ProgramValues::Movement::BACK) {
             ProgramValues::Camera::cameraPos -= ProgramValues::Camera::cameraSpeed * ProgramValues::Camera::cameraFront;
         }

         if (ProgramValues::Movement::LEFT) {
             ProgramValues::Camera::cameraPos -= glm::normalize(
                 glm::cross(ProgramValues::Camera::cameraFront, ProgramValues::Camera::cameraUp))
                 * ProgramValues::Camera::cameraSpeed;
         }

         if (ProgramValues::Movement::RIGHT) {
             ProgramValues::Camera::cameraPos += glm::normalize(
                 glm::cross(ProgramValues::Camera::cameraFront, ProgramValues::Camera::cameraUp))
                 * ProgramValues::Camera::cameraSpeed;
         }

         if (ProgramValues::Movement::UP) {
             ProgramValues::Camera::cameraPos += ProgramValues::Camera::cameraUp * ProgramValues::Camera::cameraSpeed;
         }

         if (ProgramValues::Movement::DOWN) {
             ProgramValues::Camera::cameraPos -= ProgramValues::Camera::cameraUp * ProgramValues::Camera::cameraSpeed;
         }
     }

     texture->bind();
}

 void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader->use();
    vertexArray->Bind();
    elementBuffer->Bind();

    {
        auto draw = [this](glm::mat4& model) -> void {
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
            glUniformMatrix4fv(glGetUniformLocation(shader->ID, "u_Model"), 1, GL_FALSE, &model[0][0]);
            glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

        };

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f));
        draw(model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.8f, 0.2f, 0.0f));
        draw(model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.1f, 0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.7f, 0.0f));
        draw(model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.05f, 0.05f, -0.2f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.2f, 1.0f, 0.1f));
        draw(model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.7f, 0.0f, 0.4f));
        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.1f, 0.5f, 1.0f));
        draw(model);

        float time = SDL_GetTicks() / 1000.0f;
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        model = glm::rotate(model, time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
        draw(model);
    }

    {
        glm::mat4 view;
        view = glm::lookAt(
            ProgramValues::Camera::cameraPos, 
            ProgramValues::Camera::cameraPos + ProgramValues::Camera::cameraFront, 
            ProgramValues::Camera::cameraUp
        );

        glUniformMatrix4fv(glGetUniformLocation(shader->ID, "u_View"), 1, GL_FALSE, &view[0][0]);

    }

    vertexArray->Unbind();

    imGUIWindow->render();

    SDL_GL_SwapWindow(window);
}

void Game::initialize() {
    spdlog::info("Initializing program...");

    if (initEverything()) {
        initImGUI();
        initHandleEvent();

        spdlog::info("Program initialized.");
        running = true;
    }
    else {
        spdlog::error("Failed to initialize program.");
    }
}

void Game::reset() {
    vertexArray.reset();
    vertexBuffer.reset();
    elementBuffer.reset();
    shader.reset();
    imGUIWindow->clean();
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Game::isRunning() {
    return running;
}

void Game::setRunning(bool running) {
    this->running = running;
}

Game* Game::getInstance() {
    static Game instance;  // Guaranteed to be created only once
    return &instance;  // Return raw pointer to the instance
}