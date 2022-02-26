//
// Created by cpasjuste on 01/06/18.
//

#include "burner.h"
#include "burnint.h"

#include "c2dui.h"
#include "uiEmu.h"
#include "video.h"
#include "retro_input_wrapper.h"

using namespace c2d;
using namespace c2dui;

int nVidFullscreen = 0;
INT32 bVidUseHardwareGamma = 1;

UINT32 (__cdecl *VidHighCol)(INT32 r, INT32 g, INT32 b, INT32 i);

INT32 VidRecalcPal() { return BurnRecalcPal(); }

#ifdef __PFBA_ARM__
extern int nSekCpuCore;

static bool isHardware(int hardware, int type) {
    return (((hardware | HARDWARE_PREFIX_CARTRIDGE) ^ HARDWARE_PREFIX_CARTRIDGE)
            & 0xff000000) == (unsigned int) type;
}
#endif

static UINT32 myHighCol16(int r, int g, int b, int /* i */) {
    UINT32 t;
    t = (r << 8) & 0xf800;
    t |= (g << 3) & 0x07e0;
    t |= (b >> 3) & 0x001f;
    return t;
}

static UiMain *uiInstance;
static FloatRect *textureRectInstance;

PFBAGuiEmu::PFBAGuiEmu(UiMain *ui) : UIEmu(ui) {

    printf("PFBAGuiEmu()\n");

    uiInstance = ui;
    textureRectInstance = &textureRect;
}

#ifdef __PFBA_ARM__

int PFBAGuiEmu::getSekCpuCore() {

    int sekCpuCore = 0; // SEK_CORE_C68K: USE CYCLONE ARM ASM M68K CORE

    std::vector<std::string> zipList;
    int hardware = BurnDrvGetHardwareCode();

    std::string bios = ui->getConfig()->get(Option::Id::ROM_NEOBIOS, true)->getValueString();
    if (isHardware(hardware, HARDWARE_PREFIX_SNK) && Utility::contains(bios, "UNIBIOS")) {
        sekCpuCore = 1; // SEK_CORE_M68K: USE C M68K CORE
        ui->getUiMessageBox()->show(
                "WARNING", "UNIBIOS DOESNT SUPPORT THE M68K ASM CORE\n"
                           "CYCLONE ASM CORE DISABLED", "OK");
    }

    if (isHardware(hardware, HARDWARE_PREFIX_SEGA_MEGADRIVE)) {
        sekCpuCore = 1; // SEK_CORE_M68K: USE C M68K CORE
    } else if (isHardware(hardware, HARDWARE_PREFIX_SEGA)) {
        if (hardware & HARDWARE_SEGA_FD1089A_ENC
            || hardware & HARDWARE_SEGA_FD1089B_ENC
            || hardware & HARDWARE_SEGA_MC8123_ENC
            || hardware & HARDWARE_SEGA_FD1094_ENC
            || hardware & HARDWARE_SEGA_FD1094_ENC_CPU2) {
            sekCpuCore = 1; // SEK_CORE_M68K: USE C M68K CORE
            ui->getUiMessageBox()->show(
                    "WARNING", "ROM IS CRYPTED, USE DECRYPTED ROM (CLONE)\n"
                               "TO ENABLE CYCLONE ASM CORE (FASTER)", "OK");
        }
    } else if (isHardware(hardware, HARDWARE_PREFIX_TOAPLAN)) {
        zipList.push_back("batrider");
        zipList.push_back("bbakraid");
        zipList.push_back("bgaregga");
    } else if (isHardware(hardware, HARDWARE_PREFIX_SNK)) {
        zipList.push_back("kof97");
        zipList.push_back("kof98");
        zipList.push_back("kof99");
        zipList.push_back("kof2000");
        zipList.push_back("kof2001");
        zipList.push_back("kof2002");
        zipList.push_back("kf2k3pcb");
        //zipList.push_back("kof2003"); // WORKS
    }

    std::string zip = BurnDrvGetTextA(DRV_NAME);
    for (unsigned int i = 0; i < zipList.size(); i++) {
        if (zipList[i].compare(0, zip.length(), zip) == 0) {
            ui->getUiMessageBox()->show(
                    "WARNING", "THIS GAME DOES NOT SUPPORT THE M68K ASM CORE\n"
                               "CYCLONE ASM CORE DISABLED", "OK");
            sekCpuCore = 1; // SEK_CORE_M68K: USE C M68K CORE
            break;
        }
    }

    zipList.clear();

    return sekCpuCore;
}

#endif

int PFBAGuiEmu::load(const ss_api::Game &game) {

    currentGame = game;
    ui->getUiStatusBox()->show("STARTING EMULATION: " + game.names.at(0).text,
                               "TIPS: PRESS (COIN + START) BUTTONS FOR IN GAME MENU");

    std::string zipName = Utility::removeExt(game.path);
    if (game.system.id == SYSTEM_ID_COLECO) {
        zipName = "cv_" + zipName;
    } else if (game.system.id == SYSTEM_ID_GAMEGEAR) {
        zipName = "gg_" + zipName;
    } else if (game.system.id == SYSTEM_ID_MEGADRIVE || game.system.id == SYSTEM_ID_MEGADRIVE_HACK) {
        zipName = "md_" + zipName;
    } else if (game.system.id == SYSTEM_ID_MSX || game.system.id == SYSTEM_ID_MSX2) {
        zipName = "msx_" + zipName;
    } else if (game.system.id == SYSTEM_ID_PCE) {
        zipName = "pce_" + zipName;
    } else if (game.system.id == SYSTEM_ID_SG1000) {
        zipName = "sg1k_" + zipName;
    } else if (game.system.id == SYSTEM_ID_SGX) {
        zipName = "sgx_" + zipName;
    } else if (game.system.id == SYSTEM_ID_SMS) {
        zipName = "sms_" + zipName;
    } else if (game.system.id == SYSTEM_ID_TG16) {
        zipName = "tg_" + zipName;
    } else if (game.system.id == SYSTEM_ID_ZX3) {
        zipName = "spec_" + zipName;
    } else if (game.system.id == SYSTEM_ID_NES) {
        zipName = "nes_" + zipName;
    } else if (game.system.id == SYSTEM_ID_NES_FDS) {
        zipName = "fds_" + zipName;
    } else if (game.system.id == SYSTEM_ID_CHANNELF) {
        zipName = "chf_" + zipName;
    } else if (game.system.id == SYSTEM_ID_NGP || game.system.id == SYSTEM_ID_NGPC) {
        zipName = "ngp_" + zipName;
    }

    for (unsigned int i = 0; i < nBurnDrvCount; i++) {
        nBurnDrvActive = i;
        if (zipName == BurnDrvGetTextA(DRV_NAME)) {
            break;
        }
    }

    if (nBurnDrvActive >= nBurnDrvCount) {
        printf("PFBAGui::runRom: driver not found\n");
        return -1;
    }

#ifdef __PFBA_ARM__
    nSekCpuCore = getSekCpuCore();
    printf("nSekCpuCore: %s\n", nSekCpuCore > 0 ? "M68K" : "C68K (ASM)");
#endif

    int audio_freq = ui->getConfig()->get(Option::Id::ROM_AUDIO_FREQ, true)->getValueInt(44100);
    nInterpolation = ui->getConfig()->get(Option::Id::ROM_AUDIO_INTERPOLATION, true)->getValueInt();
    nFMInterpolation = ui->getConfig()->get(Option::Id::ROM_AUDIO_FMINTERPOLATION, true)->getValueInt();
    bForce60Hz = ui->getConfig()->get(Option::Id::ROM_FORCE_60HZ, true)->getValueBool();
    if (bForce60Hz) {
        nBurnFPS = 6000;
    }

    ///////////////
    // FBA DRIVER
    ///////////////
    EnableHiscores = 1;

    printf("Initialize driver...\n");
    // some drivers require audio buffer to be allocated for DrvInit, add a "dummy" one...
    auto *aud = new Audio(audio_freq);
    nBurnSoundRate = aud->getSampleRate();
    nBurnSoundLen = aud->getSamples();
    pBurnSoundOut = (INT16 *) malloc(aud->getSamplesSize());
    if (DrvInit((int) nBurnDrvActive, false) != 0) {
        printf("\nDriver initialisation failed\n");
        delete (aud);
        ui->getUiProgressBox()->setVisibility(Visibility::Hidden);
        ui->getUiMessageBox()->show("ERROR", "DRIVER INIT FAILED", "OK");
        stop();
        return -1;
    }
    delete (aud);
    free(pBurnSoundOut);
    nFramesEmulated = 0;
    nFramesRendered = 0;
    nCurrentFrame = 0;
    setFrameDuration(1.0f / ((float) nBurnFPS / 100.0f));
    //printf("frame_duration: %f\n", getFrameDuration());
    ///////////////
    // FBA DRIVER
    ///////////////

    ///////////
    // AUDIO
    //////////
    addAudio(audio_freq, Audio::toSamples(audio_freq, (float) nBurnFPS / 100.0f));
    if (audio->isAvailable()) {
        nBurnSoundRate = audio->getSampleRate();
        nBurnSoundLen = audio->getSamples();
        pBurnSoundOut = (INT16 *) malloc(audio->getSamplesSize());
    }
    audio_sync = ui->getConfig()->get(Option::Id::ROM_AUDIO_SYNC, true)->getValueBool();
    targetFps = (float) nBurnFPS / 100;
    printf("FORCE_60HZ: %i, AUDIO_SYNC: %i, FPS: %f (BURNFPS: %f)\n",
           bForce60Hz, audio_sync, (float) nBurnFPS / 100.0f, targetFps);
    ///////////
    // AUDIO
    //////////

    //////////
    // VIDEO
    //////////
    int w, h;
    BurnDrvGetFullSize(&w, &h);
    nBurnBpp = 2;
    BurnHighCol = myHighCol16;
    BurnRecalcPal();
    auto v = new PFBAVideo(ui, (void **) &pBurnDraw, &nBurnPitch, Vector2f((float) w, (float) h));
    addVideo(v);
    textureRect = {0, 0, (float) w, (float) h};
    //////////
    // VIDEO
    //////////

    return UIEmu::load(game);
}

// need for some games
void Reinitialise(void) {
    int w, h;
    BurnDrvGetFullSize(&w, &h);
    auto v = new PFBAVideo(uiInstance, (void **) &pBurnDraw, &nBurnPitch, Vector2f((float) w, (float) h));
    uiInstance->getUiEmu()->addVideo(v);
    textureRectInstance->left = 0;
    textureRectInstance->top = 0;
    textureRectInstance->width = (float) w;
    textureRectInstance->height = (float) h;
}

void PFBAGuiEmu::stop() {
    DrvExit();
    UIEmu::stop();
}

void PFBAGuiEmu::updateFb() {
    if (pBurnDraw == nullptr) {
        video->getTexture()->lock(&textureRect, (void **) &pBurnDraw, &nBurnPitch);
        BurnDrvFrame();
        video->getTexture()->unlock();
    }
}

bool PFBAGuiEmu::onInput(c2d::Input::Player *players) {
    if (ui->getUiMenu()->isVisible() || ui->getUiStateMenu()->isVisible()) {
        return UIEmu::onInput(players);
    }

    // TODO: cross2d: add universal input rotation support
#if 0
    int rotation_config =
            getUi()->getConfig()->get(Option::Index::ROM_ROTATION, true)->getValueBool();
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
#endif

    return UIEmu::onInput(players);
}

void PFBAGuiEmu::onUpdate() {
    UIEmu::onUpdate();

    if (!isPaused()) {
        // update fbneo inputs
        InputMake(true);

        // handle diagnostic and reset switch
        unsigned int keys = ui->getInput()->getKeys(0);
        if (keys & Input::Key::Select) {
            if (clock.getElapsedTime().asSeconds() > 2) {
                if (pgi_reset) {
                    ui->getUiStatusBox()->show("RESET",
                                               "TIPS: YOU CAN PRESS START "
                                               "BUTTON 2 SECONDS FOR DIAG MENU...");
                    pgi_reset->Input.nVal = 1;
                    *(pgi_reset->Input.pVal) = pgi_reset->Input.nVal;
                }
                nCurrentFrame = 0;
                nFramesEmulated = 0;
                clock.restart();
            }
        } else if (keys & Input::Key::Start) {
            if (clock.getElapsedTime().asSeconds() > 2) {
                if (pgi_diag) {
                    ui->getUiStatusBox()->show("DIAGNOSTIC",
                                               "TIPS: YOU CAN PRESS COIN "
                                               "BUTTON 2 SECONDS TO RESET CURRENT GAME...");
                    pgi_diag->Input.nVal = 1;
                    *(pgi_diag->Input.pVal) = pgi_diag->Input.nVal;
                }
                clock.restart();
            }
        } else {
            clock.restart();
        }

        // update fbneo video buffer and audio
#ifdef __VITA__
        int skip = ui->getConfig()->get(Option::Id::ROM_FRAMESKIP, true)->getIndex();
#else
        int skip = 0;
#endif

        pBurnDraw = nullptr;
        frameskip++;

        if (frameskip > skip) {
            video->getTexture()->lock(&textureRect, (void **) &pBurnDraw, &nBurnPitch);
            nFramesRendered++;
        }

        BurnDrvFrame();
        nCurrentFrame++;

        if (frameskip > skip) {
            video->getTexture()->unlock();
            frameskip = 0;
        }

        if (audio && audio->isAvailable()) {
            audio->play(pBurnSoundOut, audio->getSamples(), audio_sync);
        }
    }
}
