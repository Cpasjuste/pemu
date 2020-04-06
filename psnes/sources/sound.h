/*****************************************************************************\
     Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.
                This file is licensed under the Snes9x License.
   For further information, consult the LICENSE file in the root directory.
\*****************************************************************************/

#ifndef __GTK_SOUND_H
#define __GTK_SOUND_H

#define sound_buffer_size 256

void S9xPortSoundInit();

void S9xPortSoundDeinit();

void S9xSoundStart();

void S9xSoundStop();

#endif /* __GTK_SOUND_H */
