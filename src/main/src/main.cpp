#include <VBE/VBE.hpp>
#include <VBE/graphics/Image.hpp>
#include <iostream>
#include <memory>
#include <Box2D/Box2D.h>
#include "assets.h"
#include "gamescene.h"
#include "webserver.h"
#include "websocketinput.h"

using namespace std;
Window* window;

int main() {
    string assetPath = findAssetPath();
    map<string, string> webassets;
    webassets["/"] = assetPath +"index.html";
    for (const string& s : facePaths) {
        webassets["/"+s] = assetPath+s;
    }
    WebServer web(webassets);
    WebSocketInput input;

    // Create screen
    ContextSettings settings;
    settings.versionMajor = 3;
    settings.versionMinor = 3;
    window = new Window(Window::DisplayMode::createWindowedMode(800, 600), settings);

    loadAssets();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Scene* s = new GameScene(&input);

    while(true) {
        window->update();
        if(Keyboard::pressed(Keyboard::Escape) || window->isClosing())
            break;
        if(Keyboard::pressed(Keyboard::Q))
            window->setDisplayMode(Window::getFullscreenModes()[0]);
        if(Keyboard::pressed(Keyboard::W))
            window->setDisplayMode(Window::DisplayMode::createWindowedMode(800, 600));

        s->update();

        // Set viewport
        glViewport(0, 0, window->getSize().x, window->getSize().y);

        glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw ALL the things!!!!
        s->draw();

        if(s->nextScene) {
            Scene* next = s->nextScene;
            delete s;
            s = next;
        }
        window->swapBuffers();
    }
}
