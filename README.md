# Just™ game ENgine (JNG)
A 3D/2D game engine developed as an educational project with hope to be usefull.

## Prerequisites:
- CMake >=3.20 (3.22 tested)
- VulkanSDK 1.3.x.x (1.3.216.0 tested)
  - VULKAN_SDK environment variable has to be set to the installation directory

## How to build:
Just™ clone the repository with dependencies:
```
git clone --recursive https://github.com/Kostu96/just-game-engine.git
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

## Usage:
For now JNG is ditributed as a package with static libraries (compiled for Visual Studio 2022).
To use it:
- Build package using /scripts/prepare_package.bat or download prebuilt from GitHub
- Set additional include directory in your project to {package_dir}/include
- Link to libraries in {package_dir}/lib/{config}, where {config} is debug or release depending on your configuration
- Copy contents of the {package_dir}/bin into your application working directory
- Define the following:
  - JNG_DEBUG - [optional] if you want to use JNG debug features (eg. logging)
  - SPDLOG_COMPILED_LIB - [required] - needed by spdlog library
  - IMGUI_DISABLE_INCLUDE_IMCONFIG_H - [required] - needed by Dear ImGui library
  - IMGUI_USER_CONFIG="jng/imconfig.hpp" - [required] - same as above
