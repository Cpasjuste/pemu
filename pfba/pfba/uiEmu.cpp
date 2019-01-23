//
// Created by cpasjuste on 01/06/18.
//

#include "burner.h"

#include "c2dui.h"
#include "uiEmu.h"
#include "video.h"

using namespace c2d;
using namespace c2dui;

extern int InpMake(Input::Player *players);

extern unsigned char inputServiceSwitch;
extern unsigned char inputP1P2Switch;

static unsigned int myHighCol16(int r, int g, int b, int /* i */) {
    unsigned int t;
    t = (unsigned int) ((r << 8) & 0xf800); // rrrr r000 0000 0000
    t |= (g << 3) & 0x07e0; // 0000 0ggg ggg0 0000
    t |= (b >> 3) & 0x001f; // 0000 0000 000b bbbb
    return t;
}

PFBAGuiEmu::PFBAGuiEmu(UIMain *ui) : UIEmu(ui) {

    printf("PFBAGuiEmu()\n");
}

int PFBAGuiEmu::load(RomList::Rom *rom) {

    ///////////
    // AUDIO
    //////////
    printf("Init audio device...");
    addAudio(48000);
    if (getAudio()->isAvailable()) {
        // disable interpolation as it produce "cracking" sound
        // on some games (cps1 (SF2), cave ...)
        nInterpolation = 1;
        nFMInterpolation = 0;
        nBurnSoundRate = getAudio()->getSampleRate();
        nBurnSoundLen = getAudio()->getBufferLen();
        pBurnSoundOut = getAudio()->getBuffer();
    }
    printf("done\n");
    ///////////
    // AUDIO
    //////////

    ///////////////
    // FBA DRIVER
    ///////////////
    nBurnDrvActive = rom->drv;
    if (nBurnDrvActive >= nBurnDrvCount) {
        printf("PFBAGui::runRom: driver not found\n");
        return -1;
    }
    bForce60Hz = true;
    EnableHiscores = 1;
    InpInit();
    InpDIP();
    printf("Initialize driver...\n");
    if (DrvInit(rom->drv, false) != 0) {
        printf("\nDriver initialisation failed! Likely causes are:\n"
               "- Corrupt/Missing ROM(s)\n"
               "- I/O Error\n"
               "- Memory error\n\n");
        getUi()->getUiProgressBox()->setVisibility(Visibility::Hidden);
        getUi()->getUiMessageBox()->show("ERROR", "DRIVER INIT FAILED", "OK");
        stop();
        return -1;
    }
    nFramesEmulated = 0;
    nFramesRendered = 0;
    nCurrentFrame = 0;
    setFrameDuration(1.0f / ((float) nBurnFPS / 100.0f));
    //printf("frame_duration: %f\n", getFrameDuration());
    printf("done\n");
    ///////////////
    // FBA DRIVER
    ///////////////

    //////////
    // VIDEO
    //////////
    int w, h;
    BurnDrvGetFullSize(&w, &h);
    nBurnBpp = 2;
    BurnHighCol = myHighCol16;
    BurnRecalcPal();
    PFBAVideo *v = new PFBAVideo(getUi(), (void **) &pBurnDraw, &nBurnPitch, Vector2f(w, h));
    addVideo(v);
    //////////
    // VIDEO
    //////////

    return UIEmu::load(rom);
}

void PFBAGuiEmu::stop() {

    DrvExit();
    InpExit();

    UIEmu::stop();
}

void PFBAGuiEmu::updateFb() {

    if (pBurnDraw == nullptr) {
        nFramesEmulated++;
        nCurrentFrame++;
        nFramesRendered++;
        getVideo()->getTexture()->lock(nullptr, (void **) &pBurnDraw, &nBurnPitch);
        BurnDrvFrame();
        getVideo()->getTexture()->unlock();
    }
}

void PFBAGuiEmu::renderFrame(bool draw) {

    if (!isPaused()) {

        nFramesEmulated++;
        nCurrentFrame++;

        pBurnDraw = nullptr;
        if (draw) {
            nFramesRendered++;
            getVideo()->getTexture()->lock(nullptr, (void **) &pBurnDraw, &nBurnPitch);
        }
        BurnDrvFrame();
        if (draw) {
            getVideo()->getTexture()->unlock();
        }

        if (getAudio() && getAudio()->isAvailable()) {
            getAudio()->play();
        }
    }
}

void PFBAGuiEmu::updateFrame() {

    // TODO
    //int frameSkip = getUi()->getConfig()->getValue(Option::Index::ROM_FRAMESKIP, true);

    renderFrame();
    /*
    timer += getUi()->getDeltaTime().asSeconds();
    if (timer >= 1) {
        timer = 0;
        printf("fps: %.2g/%2d, delta: %f\n", getUi()->getFps(), (nBurnFPS / 100),
               getUi()->getDeltaTime().asSeconds());
    }
    */
}

bool PFBAGuiEmu::onInput(c2d::Input::Player *players) {

    if (getUi()->getUiMenu()->isVisible()
        || getUi()->getUiStateMenu()->isVisible()) {
        return UIEmu::onInput(players);
    }

    bool combo = false;
    // TODO: control rotation
    int rotation_config =
            getUi()->getConfig()->getValue(Option::Index::ROM_ROTATION, true);
    int rotate_input = 0;
#ifdef __PSP2__
    // TODO: find a way to unify platforms,
    // or allow rotation config from main.cpp
    if (BurnDrvGetFlags() & BDF_ORIENTATION_VERTICAL) {
        if (rotation_config == 0) {
            //rotate controls by 90 degrees
            rotate_input = 1;
        } else if (rotation_config == 2) {
            //rotate controls by 270 degrees
            rotate_input = 3;
        }
    }
#elif __SWITCH__
    if (BurnDrvGetFlags() & BDF_ORIENTATION_VERTICAL) {
        if (rotation_config == 0) {             // OFF
            //rotate controls by 270 degrees
            rotate_input = 3;
        } else if (rotation_config == 1) {      // ON
            //rotate controls by 270 degrees
            rotate_input = 0;
        } else if (rotation_config == 2) {      // FLIP
            //rotate controls by 270 degrees
            rotate_input = 1;
        }
    }
#else
    if (BurnDrvGetFlags() & BDF_ORIENTATION_VERTICAL) {
        rotate_input = rotation_config ? 0 : 3;
    }
#endif

    inputServiceSwitch = 0;
    inputP1P2Switch = 0;

    // look for player 1 menu combo
    if (((players[0].keys & Input::Key::Start) && (players[0].keys & Input::Key::Fire5))
        || ((players[0].keys & Input::Key::Select) && (players[0].keys & Input::Key::Fire5))
        || ((players[0].keys & Input::Key::Start) && (players[0].keys & Input::Key::Fire6))
        || ((players[0].keys & Input::Key::Select) && (players[0].keys & Input::Key::Fire6))) {
        pause();
        getUi()->getConfig()->load(getUi()->getUiRomList()->getSelection());
        getUi()->getUiMenu()->load(true);
        return true;
    }

    // look each players for combos keys
    for (int i = 0; i < PLAYER_MAX; i++) {
        // allow devices with single select/start button to send start/coins (nsw in single joycon mode)
        if (((players[i].keys & Input::Key::Start) && (players[i].keys & Input::Key::Fire1))
            || ((players[i].keys & Input::Key::Select) && (players[i].keys & Input::Key::Fire1))) {
            players[i].keys = Input::Key::Start;
            combo = true;
        } else if (((players[i].keys & Input::Key::Start) && (players[i].keys & Input::Key::Fire2))
                   || ((players[i].keys & Input::Key::Select) && (players[i].keys & Input::Key::Fire2))) {
            players[i].keys = Input::Key::Select;
            combo = true;
        }
    }

    // look for player 1 combos key
    if (!combo) {
        if (((players[0].keys & Input::Key::Start) && (players[0].keys & Input::Key::Fire3))
            || ((players[0].keys & Input::Key::Select) && (players[0].keys & Input::Key::Fire3))) {
            inputServiceSwitch = 1;
        } else if (((players[0].keys & Input::Key::Start) && (players[0].keys & Input::Key::Fire4))
                   || ((players[0].keys & Input::Key::Select) && (players[0].keys & Input::Key::Fire4))) {
            inputP1P2Switch = 1;
        }
    }

    // look for window resize event
    if (players[0].keys & EV_RESIZE) {
        // useful for sdl resize event
        getVideo()->updateScaling();
    }

    return true;
}

void PFBAGuiEmu::onDraw(c2d::Transform &transform) {

    if (!isPaused()) {

        // fps
        int showFps = getUi()->getConfig()->getValue(Option::Index::ROM_SHOW_FPS, true);
        getFpsText()->setVisibility(showFps ? c2d::Visibility::Visible : c2d::Visibility::Hidden);
        if (showFps) {
            sprintf(getFpsString(), "FPS: %.2g/%2d", getUi()->getFps(), nBurnFPS / 100);
            getFpsText()->setString(getFpsString());
        }

        auto players = getUi()->getInput()->getPlayers();
        InpMake(players);
        updateFrame();
    }

    UIEmu::onDraw(transform);
}
