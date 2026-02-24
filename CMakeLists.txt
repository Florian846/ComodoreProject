cmake_minimum_required(VERSION 3.10)
project(ComodoreProject C)

# 1. Standard auf C90 setzen
set(CMAKE_C_STANDARD 90)

set(CC65_DIR "C:/cc65")
set(CC65_BIN "${CC65_DIR}/bin/cl65.exe")

# 2. Header-Pfad für die IDE (damit include "ScreenLoader.h" gefunden wird)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("C:/cc65/include")

# Das Build-Target für den Compiler
add_custom_target(cbm_build ALL
        COMMAND ${CC65_BIN} -O -t pet -I . main.c ScreenManager/ScreenManager.c TitelScreen/TitelScreen.c ScreenLoader/ScreenLoader.c TitelScreen/Titel.c OptionsScreen/OptionsScreen.c OptionsScreen/Options.c Game/Game.c Card/Card.c Card/CardManager.c Player/Player.c -o program.prg
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Erstelle CBM Programm (program.prg)..."
)

# Für die IDE (Pfade ergänzt)
add_executable(new_target
        main.c
        GameConfig.h
        ScreenManager/ScreenManager.c
        ScreenManager/ScreenManager.h
        TitelScreen/Titel.c
        TitelScreen/Titel.h
        TitelScreen/TitelScreen.c
        ScreenLoader/ScreenLoader.c
        ScreenLoader/ScreenLoader.h
        ScreenLoader/Screens.h
        OptionsScreen/OptionsScreen.c
        OptionsScreen/Options.c
        OptionsScreen/Options.h
        Game/Game.c
        Game/Game.h
        Card/Card.c
        Card/Card.h
        Card/CardManager.c
        Card/CardManager.h
        Player/Player.c
        Player/Player.h
        Objects/Stack.c
)




