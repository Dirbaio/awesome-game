#include <VBE/VBE.hpp>
#include <VBE/graphics/Image.hpp>
#include <iostream>
#include <memory>
#include <Box2D/Box2D.h>
#include "assets.h"
#include "gamescene.h"

using namespace std;


int main() {
    // Create screen
    ContextSettings settings;
    settings.versionMajor = 3;
    settings.versionMinor = 3;
    Window window (Window::DisplayMode::createWindowedMode(800, 600), settings);

    loadAssets();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Scene* s = new GameScene();

    while(true) {
        window.update();
        if(Keyboard::pressed(Keyboard::Escape) || window.isClosing())
            break;
        if(Keyboard::pressed(Keyboard::Q))
            window.setDisplayMode(Window::getFullscreenModes()[0]);
        if(Keyboard::pressed(Keyboard::W))
            window.setDisplayMode(Window::DisplayMode::createWindowedMode(800, 600));

        s->update();

        // Set viewport
        glViewport(0, 0, window.getSize().x, window.getSize().y);

        glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Projection matrix.
        float aspect = float(window.getSize().x)/window.getSize().y;
        float zoom = 20.0f;
        projection = glm::ortho(-zoom*aspect, zoom*aspect, -zoom, zoom);

        // Draw ALL the things!!!!
        s->draw();

        if(s->nextScene) {
            Scene* next = s->nextScene;
            delete s;
            s = next;
        }
        window.swapBuffers();
    }
}
