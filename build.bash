#!/bin/bash

CXX=clang++
CXXFLAGS="-std=c++17 -Wall -Wno-missing-field-initializers -O2"
RAYLIB_PATH=$(brew --prefix raylib)

$CXX $CXXFLAGS \
main.cpp \
src/systems/SpawnSystem.cpp \
src/systems/CollisionSystem.cpp \
src/systems/GameplaySystem.cpp \
src/systems/RenderSystem.cpp \
src/systems/MovementSystem.cpp \
src/world/world.cpp \
src/levels/level.cpp \
src/cursor/Cursor.cpp \
src/spatial/Grid.cpp \
src/assets/AssetManager.cpp \
src/editor/Editor.cpp \
src/camera/CameraController.cpp \
src/serialization/WorldSerializer.cpp \
-o main \
-I$RAYLIB_PATH/include \
-I src \
-L$RAYLIB_PATH/lib \
-lraylib \
-framework OpenGL \
-framework IOKit \
-framework Cocoa \
-framework CoreVideo

