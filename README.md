# Just™ game ENgine (JNG)
A 3D/2D game engine developed as an educational project with hope to be usefull.

## Prerequisites:
- CMake >=3.20 (3.22 tested)
- VulkanSDK 1.3.x.x (1.3.216.0 tested)
  - VULKAN_SDK environment variable has to be set to the installation directory

## How to build:
Just™ clone the repository with dependencies:
```
git clone --recurse-submodules --shallow-submodules https://github.com/Kostu96/just-game-engine.git
cd just-game-engine
```
then Just™ try:
```
mkdir build
cd build
cmake ..
```
and it might Just™ work for you.
If not better instructions will come in the future...

Build using your build system (Visual Studio 2022 is tested).

## Sctipt API
Lua Script API [referene](docs/LuaScriptAPI/LuaScriptAPI.md)
