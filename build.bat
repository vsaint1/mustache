@ECHO off

ECHO Building in progress

cmake -S . -B ./build

ECHO Building project debug build

cmake --build ./build --config Debug --target ALL_BUILD -j 14

ECHO Build done

ECHO Building project release build

cmake --build ./build --config Release --target ALL_BUILD -j 14

ECHO Build done