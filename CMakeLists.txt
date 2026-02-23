cmake_minimum_required(VERSION 3.10)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(ComodoreProject C)

# 1. Standard auf C90 setzen
set(CMAKE_C_STANDARD 90)

set(CC65_DIR "C:/cc65")
set(CC65_BIN "${CC65_DIR}/bin/cl65.exe")

# 3. Damit CLion die Header findet (kein rotes Unterstreichen mehr)
include_directories("C:/cc65/include")
# 4. Das Build-Target für den CBM 3001
add_custom_target(cbm_build ALL
        COMMAND ${CC65_BIN} -O -t pet main.c screen.c -o program.prg
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Erstelle CBM 3001 Programm (program.prg)..."
)
add_executable(new_target main.c screen.c)
