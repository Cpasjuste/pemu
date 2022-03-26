//
// Created by cpasjuste on 21/03/2022.
//

#ifndef PEMU_OSD_H
#define PEMU_OSD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

#define MAX_INPUTS 4

typedef struct {
    int8 device;
    uint8 port;
    uint8 padtype;
} t_input_config;

typedef struct {
    char version[16];
    uint8 hq_fm;
    uint8 filter;
    uint8 hq_psg;
    uint8 ym2612;
    uint8 ym2413;
#ifdef HAVE_YM3438_CORE
    uint8 ym3438;
#endif
#ifdef HAVE_OPLL_CORE
    uint8 opll;
#endif
    uint8 mono;
    int16 psg_preamp;
    int16 fm_preamp;
    int16 cdda_volume;
    int16 pcm_volume;
    uint16 lp_range;
    int16 low_freq;
    int16 high_freq;
    int16 lg;
    int16 mg;
    int16 hg;
    uint8 system;
    uint8 region_detect;
    uint8 master_clock;
    uint8 vdp_mode;
    uint8 force_dtack;
    uint8 addr_error;
    uint8 bios;
    uint8 lock_on;
    uint8 add_on;
    uint8 overscan;
    uint8 aspect;
    uint8 ntsc;
    uint8 lcd;
    uint8 gg_extra;
    uint8 left_border;
    uint8 render;
    t_input_config input[MAX_INPUTS];
    uint8 invert_mouse;
    uint8 gun_cursor[2];
    uint32 overclock;
    uint8 no_sprite_limit;
#ifdef USE_PER_SOUND_CHANNELS_CONFIG
    unsigned int psg_ch_volumes[4];
    int32 md_ch_volumes[6];
    signed int sms_fm_ch_volumes[9];
#endif
} t_config;

extern t_config config;

extern char GG_ROM[256];
extern char AR_ROM[256];
extern char SK_ROM[256];
extern char SK_UPMEM[256];
extern char GG_BIOS[256];
extern char MD_BIOS[256];
extern char CD_BIOS_EU[256];
extern char CD_BIOS_US[256];
extern char CD_BIOS_JP[256];
extern char MS_BIOS_US[256];
extern char MS_BIOS_EU[256];
extern char MS_BIOS_JP[256];

extern void osd_input_update(void);

extern int load_archive(char *filename, unsigned char *buffer, int maxsize, char *extension);

extern void set_paths_default(const char *biosPath);

extern void set_config_defaults();

#ifdef __cplusplus
}
#endif

#endif //PEMU_OSD_H
