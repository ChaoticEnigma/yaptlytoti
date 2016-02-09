# yaptlytoti
YAPTLYTOTI (Yet Another Program That Lets You Talk Over The Internet) or YAP for short.

## Compilation

### Linux Dependencies
* CMake
* Qt5.5
* Install `mesa-common-dev` (if you do not have graphics drivers installed)

### Windows Dependenceis
* CMake
* Qt5.5

#### Compiling from Terminal

    git clone https://github.com/ChaoticCrux/yaptlytoti.git yap
    mkdir yap-build
    cd yap-build
    cmake ../yap # if qt5.5 was installed via a package manager
    cmake ../yap -DCMAKE_PREFIX_PATH=/opt/Qt/5.5/gcc_64 # if qt5.5 was installed manually (the path may be different)
    make

#### Developing with Qt Creator
1. Open Qt Creator
2. File -> Open Project
3. Open CMakeLists.txt
4. Configure Project (assuming Desktop kit, or some similar kit)
5. Run CMake (with no Arguments) -> Finish
6. The project should be set up and ready to build and run

## Alternate Names:
YAV / YAVoIP (Yet Another VoIP)
YAVC (Yet Another VoIP Client)
