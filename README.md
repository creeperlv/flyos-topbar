# flyos-topbar
flyos标题栏

编译运行需要dde桌面环境

目前所有插件都是移植dde-dock上的插件，有空再重写整个插件机制和所有插件

借鉴了deepin-topbar上部分xcb调用代码

## Build Dependencies (编译依赖)

* cmake
* qt5-tools

## Dependencies (依赖)

* qt5-base
* qt5-svg
* deepin-tool-kit
* deepin-daemon
* deepin-control-center
* deepin-dock

## Installation (安装)

* mkdir build
* cd build
* cmake ..
* make
* sudo make install

## Usage (使用)

* flyos-topbar

## License

FlyOS Topbar is licensed under [GPLv3](LICENSE).
