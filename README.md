# TupoyeMenu

![](https://img.shields.io/badge/dynamic/json?color=ffab00&label=Online%20Version&query=%24.game.online&url=https%3A%2F%2Fraw.githubusercontent.com%2FYimMenu%2FYimMenu%2Fmaster%2Fmetadata.json&style=flat-square&labelColor=000000) ![](https://img.shields.io/badge/dynamic/json?color=ffab00&label=Game%20Build&query=%24.game.build&url=https%3A%2F%2Fraw.githubusercontent.com%2FYimMenu%2FYimMenu%2Fmaster%2Fmetadata.json&style=flat-square&labelColor=000000)

A garbage fork of YimMenu made by an idiot

## Using TupoyMenu master is not recommended lite branch is preffered
This fork aims to make the game more enjoyable by adding untested broken features that crash your game.
Strictly for educational purposes.

YimMenu is originally based of off [BigBaseV2](https://github.com/Pocakking/BigBaseV2) which was an amazing base at the time but nowadays is a bit dated.
So here I am with an up-to-date menu focusing on protecting the user from toxic modders.

## Table of contents

- [TupoyeMenu](#tupoyemenu)
  - [Using TupoyMenu master is not recommended lite branch is preffered](#using-tupoymenu-master-is-not-recommended-lite-branch-is-preffered)
  - [Table of contents](#table-of-contents)
  - [How to compile TupoyeMenu](#how-to-compile-tupoyemenu)
  - [Staying Up To Date](#staying-up-to-date)
  - [Project Structure](#project-structure)
  - [Contributing](#contributing)
 
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
- `views/` all gui windows are, except handling windows for some reason

## Contributing

You're free to contribute to TupoyeMenu as long as the features are useful, not overly toxic and do not contain anything money related that might get the menu targeted by Take2.

Make sure to read the [CONTRIBUTING.md](CONTRIBUTING.md) file.
