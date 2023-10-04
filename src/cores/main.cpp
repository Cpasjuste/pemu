//
// Created by cpasjuste on 08/12/2021.
//

#include "app.h"

using namespace c2d;

int main(int argc, char *argv[]) {
    // create main app/renderer
    auto app = new RRLauncher::App({1280, 720});

    while (!app->quit) {
        // renderer everything
        app->flip();
    }

    // will delete all child's (textures, shapes, text..)
    delete (app);

    return 0;
}
