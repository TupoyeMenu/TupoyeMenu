# TupoyeMenu-lite

![](https://img.shields.io/badge/dynamic/json?color=ffab00&label=Online%20Version&query=%24.game.online&url=https%3A%2F%2Fraw.githubusercontent.com%2FTupoyeMenu%2FTupoyeMenu%2Flite%2Fmetadata.json&style=flat-square&labelColor=000000) ![](https://img.shields.io/badge/dynamic/json?color=ffab00&label=Game%20Build&query=%24.game.build&url=https%3A%2F%2Fraw.githubusercontent.com%2FTupoyeMenu%2FTupoyeMenu%2Flite%2Fmetadata.json&style=flat-square&labelColor=000000)

A fork of TupoyeMenu attemting to make less bloated and more stable menu.

Strictly for educational purposes.

TupoyeMenu-lite is based of off TupoyeMenu wich is based off of YimMenu wich based of off [BigBaseV2](https://github.com/Pocakking/BigBaseV2) which was an amazing base at the time but nowadays is a bit dated.

## Table of contents

- [TupoyeMenu-lite](#tupoyemenu-lite)
  - [Table of contents](#table-of-contents)
  - [Changes from TupoyeMenu](#changes-from-tupoyemenu)
    - [Removed](#removed)
    - [Reworked](#reworked)
  - [Compiling](#compiling)
  - [Project Structure](#project-structure)
  - [Features](#features)
  - [Contributing](#contributing)

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
| 

### Reworked
| Feature | Reason                                   | Description   |
|---------|------------------------------------------|---------------|
| GUI     | Bloated, pain to navigate, inconsistant  | Reworked GUI for easyer navigation and better look |

## Compiling
For instructions on how to build go to https://github.com/YimMenu/YimMenu

## Project Structure
- `native_hooks/` hooks to natives
- `backend/` all features that should be ran in a loop are in here sorted by category
- `gui/` includes everything related to UI elements
- `hooks/` function hooks
- `native_hooks/` hooks to natives
- `services/` service files to interact and maintain stuff
- `util/` general utility functions to keep code as compact and as much in one place as possible
- `views/` all gui windows are here

## Features

Below is an incomplete list of feature that I believe are notable to this "base" or menu.

 - Smaller then TupoyeMenu
 - Bigger then YimMenu wtf how???
 
## Contributing

You're free to contribute to TupoyeMenu as long as the features are useful, not overly toxic and do not contain anything money related that might get the menu targeted by Take2.

Make sure to read the [CONTRIBUTING.md](CONTRIBUTING.md) file.
