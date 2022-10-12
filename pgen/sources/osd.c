//
// Created by cpasjuste on 21/03/2022.
//

#include <stdio.h>
#include "osd.h"
#include "sms_ntsc.h"
#include "md_ntsc.h"
#include "ym2612.h"
#include "input.h"

t_config config;
sms_ntsc_t *sms_ntsc;
md_ntsc_t *md_ntsc;

char GG_ROM[256];
char AR_ROM[256];
char SK_ROM[256];
char SK_UPMEM[256];
char GG_BIOS[256];
char MD_BIOS[256];
char CD_BIOS_EU[256];
char CD_BIOS_US[256];
char CD_BIOS_JP[256];
char MS_BIOS_US[256];
char MS_BIOS_EU[256];
char MS_BIOS_JP[256];

void osd_input_update() {}

void set_paths_default(const char *biosPath) {
    snprintf(GG_ROM, 256, "%s/ggenie.bin", biosPath);
    snprintf(AR_ROM, 256, "%s/areplay.bin", biosPath);
    snprintf(SK_ROM, 256, "%s/sk.bin", biosPath);
    snprintf(SK_UPMEM, 256, "%s/sk2chip.bin", biosPath);
    snprintf(CD_BIOS_US, 256, "%s/bios_CD_U.bin", biosPath);
    snprintf(CD_BIOS_EU, 256, "%s/bios_CD_E.bin", biosPath);
    snprintf(CD_BIOS_JP, 256, "%s/bios_CD_J.bin", biosPath);
    snprintf(MD_BIOS, 256, "%s/bios_MD.bin", biosPath);
    snprintf(MS_BIOS_US, 256, "%s/bios_U.sms", biosPath);
    snprintf(MS_BIOS_EU, 256, "%s/bios_E.sms", biosPath);
    snprintf(MS_BIOS_JP, 256, "%s/bios_J.sms", biosPath);
    snprintf(GG_BIOS, 256, "%s/bios.gg", biosPath);
}

void set_config_defaults() {
    int i;

    /* sound options */
    config.psg_preamp = 150;
    config.fm_preamp = 100;
    config.cdda_volume = 100;
    config.pcm_volume = 100;
    config.hq_fm = 1;
    config.hq_psg = 1;
    config.filter = 1;
    config.low_freq = 880;
    config.high_freq = 5000;
    config.lg = 100;
    config.mg = 100;
    config.hg = 100;
    config.lp_range = 0x9999; /* 0.6 in 0.16 fixed point */
    config.ym2612 = YM2612_DISCRETE;
    config.ym2413 = 2; /* = AUTO (0 = always OFF, 1 = always ON) */
#if HAVE_YM3438_CORE
    config.ym3438 = 0;
#endif
#if HAVE_OPLL_CORE
    config.opll = 0;
#endif
    config.mono = 0;

    /* system options */
    config.system = 0; /* = AUTO (or SYSTEM_SG, SYSTEM_MARKIII, SYSTEM_SMS, SYSTEM_SMS2, SYSTEM_GG, SYSTEM_MD) */
    config.region_detect = 0; /* = AUTO (1 = USA, 2 = EUROPE, 3 = JAPAN/NTSC, 4 = JAPAN/PAL) */
    config.vdp_mode = 0; /* = AUTO (1 = NTSC, 2 = PAL) */
    config.master_clock = 0; /* = AUTO (1 = NTSC, 2 = PAL) */
    config.force_dtack = 0;
    config.addr_error = 1;
    config.bios = 0;
    config.lock_on = 0; /* = OFF (or TYPE_SK, TYPE_GG & TYPE_AR) */
    config.add_on = 0; /* = HW_ADDON_AUTO (or HW_ADDON_MEGACD, HW_ADDON_MEGASD & HW_ADDON_ONE) */
    config.ntsc = 0;
    config.lcd = 0; /* 0.8 fixed point */

    /* display options */
    config.overscan = 0;       /* 3 = all borders (0 = no borders , 1 = vertical borders only, 2 = horizontal borders only) */
    config.gg_extra = 0;       /* 1 = show extended Game Gear screen (256x192) */
    config.render = 0;       /* 1 = double resolution output (only when interlaced mode 2 is enabled) */

    /* controllers options */
    input.system[0] = SYSTEM_GAMEPAD;
    input.system[1] = SYSTEM_GAMEPAD;
    config.gun_cursor[0] = 1;
    config.gun_cursor[1] = 1;
    config.invert_mouse = 0;
    for (i = 0; i < MAX_INPUTS; i++) {
        /* autodetected control pad type */
        config.input[i].padtype = DEVICE_PAD2B | DEVICE_PAD3B | DEVICE_PAD6B;
    }
}
