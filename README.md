[![linux-release](https://github.com/Cpasjuste/pemu/actions/workflows/linux-release.yml/badge.svg)](https://github.com/Cpasjuste/pemu/actions/workflows/linux-release.yml)
[![linux-dev](https://github.com/Cpasjuste/pemu/actions/workflows/linux-dev.yml/badge.svg)](https://github.com/Cpasjuste/pemu/actions/workflows/linux-dev.yml)

[![windows-release](https://github.com/Cpasjuste/pemu/actions/workflows/windows-release.yml/badge.svg)](https://github.com/Cpasjuste/pemu/actions/workflows/windows-release.yml)
[![windows-dev](https://github.com/Cpasjuste/pemu/actions/workflows/windows-dev.yml/badge.svg)](https://github.com/Cpasjuste/pemu/actions/workflows/windows-dev.yml)

[![switch-release](https://github.com/Cpasjuste/pemu/actions/workflows/switch-release.yml/badge.svg)](https://github.com/Cpasjuste/pemu/actions/workflows/switch-release.yml)
[![switch-dev](https://github.com/Cpasjuste/pemu/actions/workflows/switch-dev.yml/badge.svg)](https://github.com/Cpasjuste/pemu/actions/workflows/switch-dev.yml)

[![3ds-release](https://github.com/Cpasjuste/pemu/actions/workflows/3ds-release.yml/badge.svg)](https://github.com/Cpasjuste/pemu/actions/workflows/3ds-release.yml)
[![3ds-dev](https://github.com/Cpasjuste/pemu/actions/workflows/3ds-dev.yml/badge.svg)](https://github.com/Cpasjuste/pemu/actions/workflows/3ds-dev.yml)

[![ps4-release](https://github.com/Cpasjuste/pemu/actions/workflows/ps4-release.yml/badge.svg)](https://github.com/Cpasjuste/pemu/actions/workflows/ps4-release.yml)
[![ps4-dev](https://github.com/Cpasjuste/pemu/actions/workflows/ps4-dev.yml/badge.svg)](https://github.com/Cpasjuste/pemu/actions/workflows/ps4-dev.yml)

[![vita-release](https://github.com/Cpasjuste/pemu/actions/workflows/vita-release.yml/badge.svg)](https://github.com/Cpasjuste/pemu/actions/workflows/vita-release.yml)
[![vita-dev](https://github.com/Cpasjuste/pemu/actions/workflows/vita-dev.yml/badge.svg)](https://github.com/Cpasjuste/pemu/actions/workflows/vita-dev.yml)

## pEMU: Portable Emulator

**<ins>Building</ins>**
- [Linux](https://github.com/Cpasjuste/pemu/blob/master/.github/workflows/linux-release.yml)
- [Windows](https://github.com/Cpasjuste/pemu/blob/master/.github/workflows/windows-release.yml)
- [Nintendo Switch](https://github.com/Cpasjuste/pemu/blob/master/.github/workflows/switch-release.yml)
- [Sony PS4](https://github.com/Cpasjuste/pemu/blob/master/.github/workflows/ps4-release.yml)
- [Sony PS VITA](https://github.com/Cpasjuste/pemu/blob/master/.github/workflows/vita-release.yml)

**<ins>Common instructions</ins>**
- "pEMU" emulators use [embedded (romfs)](https://github.com/Cpasjuste/pemu/tree/master/pfbneo/data/common/romfs) "gamelist.xml" files to manage your rom files.
Since version 6.7, the emulator will also look inside your configured rom folders for a "gamelist.xml" file (EmulationStation format, which can be made with ["Skraper"](https://www.skraper.net/) for example).
This will overwrite the embedded "gamelist.xml" database. Please take note that only the "\<image>" and "\<video>" media tags are supported.
- Default skins are also [embedded (romfs)](https://github.com/Cpasjuste/pemu/tree/master/data/common/romfs/skins/default) in the application.
You can overwrite parts of the default theme by creating a folder named "default" in the "skins" directory of the data folder, and adding a ["config.cfg.override"](https://github.com/Cpasjuste/pemu/tree/master/pgen/data/common/romfs/skins/default/config.cfg.override) (and/or a ["config.cfg.override.43"](https://github.com/Cpasjuste/pemu/tree/master/data/common/romfs/skins/default/config.cfg.override.43) for 4/3 screens) file.
You can also create a new directory with a new name to create a new theme, the [default](https://github.com/Cpasjuste/pemu/tree/master/data/common/romfs/skins/default) one is a good start to look at.
Please note that the theme code/api is subject to change without warning, do not loose too much time on this...

**<ins>Custom instructions</ins>**
- [pfbneo](https://github.com/Cpasjuste/pemu/tree/master/pfbneo)
- [pgen](https://github.com/Cpasjuste/pemu/tree/master/pgen)
- [pnes](https://github.com/Cpasjuste/pemu/tree/master/pnes)
- [psnes](https://github.com/Cpasjuste/pemu/tree/master/psnes)