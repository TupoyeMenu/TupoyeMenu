# TupoyeMenu-lite

![](https://img.shields.io/badge/dynamic/json?color=ffab00&label=Online%20Version&query=%24.game.online&url=https%3A%2F%2Fraw.githubusercontent.com%2FTupoyeMenu%2FTupoyeMenu%2Flite%2Fmetadata.json&style=flat-square&labelColor=000000) ![](https://img.shields.io/badge/dynamic/json?color=ffab00&label=Game%20Build&query=%24.game.build&url=https%3A%2F%2Fraw.githubusercontent.com%2FTupoyeMenu%2FTupoyeMenu%2Flite%2Fmetadata.json&style=flat-square&labelColor=000000)

A branch of TupoyeMenu attemting to make less bloated and more stable menu.

Strictly for educational purposes.

TupoyeMenu-lite is based of off [TupoyeMenu](https://github.com/TupoyeMenu/TupoyeMenu) wich is based off of [YimMenu](https://github.com/YimMenu/YimMenu) wich based of off [BigBaseV2](https://github.com/Pocakking/BigBaseV2) which was an amazing base at the time but nowadays is a bit dated.

## Table of contents

- [TupoyeMenu-lite](#tupoyemenu-lite)
  - [Table of contents](#table-of-contents)
  - [Changes from TupoyeMenu](#changes-from-tupoyemenu)
    - [Removed](#removed)
    - [Reworked](#reworked)
  - [How to compile TupoyeMenu](#how-to-compile-tupoyemenu)
  - [Staying Up To Date](#staying-up-to-date)
  - [Project Structure](#project-structure)
  - [Contributing](#contributing)
  - [Credits](#credits)
  - [Donate](#donate)

## Changes from TupoyeMenu
### Removed
| Feature          | Reason                 |
|------------------|------------------------|
| cpr and related  | Bloat                  |
| HUD color        | Low quality?           |
| SuperMan         | Low quality            |
| Super jump       | Low quality            |
| Beast jump       | Low quality            |
| PartWater        | Useless                |
| Train features   | Useless, doesn't work? |
| Ped flag debug   | Idk, low quality bloat?|
| Persist car      | Low quality, bloat     |
| Translations     | Bloat, pain to maintain, unreadeble code, cpr dependent, internet dependent, learn English |
| Handling editor  | Bloat?                 |
| Fancy spinner    | Bloat, only used once  |
| Custom chat      | Low quality            |
| Show transaction error | Breaks gameplay for no reason |
| PTFX             | Useless bloat          |
| Voice chat audio | Useless bloat          |
| Orbital drone    | Bloat                  |
| Vehicle Controller | Bloat                |
| Squad Spawner    | Bloat?                 |
| Aimbot & triggerbot | Bloat, cheats bad, L, noob, low quality, useless, cringe, learn how to aim |

### Reworked
| Feature | Reason                                   | Description   |
|---------|------------------------------------------|---------------|
| GUI     | Bloated, pain to navigate, inconsistant  | Reworked GUI for easyer navigation and better look |


## How to compile TupoyeMenu

Read the [SETUP](https://github.com/YimMenu/YimMenu/wiki/Setup-your-PC-for-YimMenu-Development) guide.
And replace YimMenu with TupoyeMenu.
You can also use some cmake options.
 - `GEN_PDB`: Generate pdb.
 - `ENABLE_CRASHES` Enables the crash options, very few very old, not even working.
 - `ENABLE_SOCIALCLUB` Enable Social Club hooking.
 - `ENABLE_ASI_LOADER` Enable shitty built in ASI Loader.

  
## Staying Up To Date
```bash
git pull https://github.com/tupoy-ya/TupoyeMenu.git
```

CMake should be handling removed / added files automatically without any user input.

If this is not the case for some reason you'll have to redo the steps in the [Making changes to the source code section of the SETUP](https://github.com/YimMenu/YimMenu/wiki/Setup-your-PC-for-YimMenu-Development#making-changes-to-the-source-code).

If you are doing custom modifications to the codebase and have a fork you are on your own for staying up to date with upstream (this repository), google stuff like "merge from upstream" and learn how to use Git.

## Project Structure
- `native_hooks/` hooks to natives
- `backend/` all features that should be ran in a loop are in here sorted by category
- `gui/` includes everything related to UI elements
- `hooks/` function hooks
- `native_hooks/` hooks to natives
- `services/` service files to interact and maintain stuff
- `util/` general utility functions to keep code as compact and as much in one place as possible
- `views/` all gui windows are here

## Contributing

You're free to contribute to TupoyeMenu as long as the features are useful, not overly toxic and do not contain anything money related that might get the menu targeted by Take2.

Make sure to read the [CONTRIBUTING.md](CONTRIBUTING.md) file.

## Credits
Menus - Bases we use:
 - [YimMenu](https://github.com/YimMenu/YimMenu) TupoyeMenu is entirely based on YimMenu.
 - [BigBaseV2-fix](https://bitbucket.org/gir489/bigbasev2-fix) Asi Loader, YimMenu is based on BigBaseV2.
 - [BigBaseV2](https://github.com/Pocakking/BigBaseV2) YimMenu is based on BigBaseV2.

Libraries we use:
 - [GTAV-Classes](https://github.com/Yimura/GTAV-Classes) GTA 5 class defenitions.
 - [AsyncLogger](https://github.com/Yimura/AsyncLogger) The log library we use.
 - [imgui_markdown](https://github.com/juliettef/imgui_markdown) markdown for Dear ImGui.
 - [ImGui](https://github.com/ocornut/imgui) User inteface.
 - [nlohmann json](https://github.com/nlohmann/json) Json parsing.
 - [pugixml](https://github.com/zeux/pugixml) XML parsing.
 - [MinHook](https://github.com/TsudaKageyu/minhook) Hooking functions.

## Donate
I don't recommend donating bacause i'm just a moron that has not done any important changes to this menu.
If you still want to do it here is my XMR address:
`8Bma7KYCG4xPWibfHNZ7sYHYjN8jzDV165qqUPfpbELVC9dTDZuJFHSd21RbGgB8vKHnHZWwQ2zmPfWzZ8JMujYzLGxn1vm`