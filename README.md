# flyos-topbar
flyos标题栏

编译运行需要dde桌面环境

目前所有插件都是移植dde-dock上的插件，有空再重写整个插件机制和所有插件

xcb部分代码参考dde-dock

## 已有插件

* 系统信息插件(内存网速显示，未完成)

### 以下插件为移植dock的插件
系统托盘(退出topbar会留一堆图标在桌面,解决方法，重启dde-dock)

* wifi
* 音量
* 电源
* 时间
* 分区挂载(disk-mount)(图标待更换)

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
* cmake -DCMAKE_INSTALL_PREFIX=/usr ..
* make
* sudo make install

## 下载

* alpm包(适用于arch/manjaro等arch衍生发行版)

[flyos-topbar-0.1.2](https://coding.net/u/xiayesuifeng/p/flyos-mirrors/git/raw/master/x86_64/flyos-topbar-0.1.2-1-x86_64.pkg.tar.xz)

* deb包(感谢[@jingle0927](https://github.com/jingle0927)提供的deb包,虽然是旧版)

旧版[topbar.zip](https://github.com/linuxflyos/flyos-topbar/files/1168638/topbar.zip)

## Usage (使用)

* flyos-topbar 或者 点击启动器上的标题栏图标

## License

FlyOS Topbar is licensed under [GPLv3](LICENSE).
