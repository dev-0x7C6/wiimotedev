![wiimotedev_logo](https://devwork.space/wp-content/uploads/2018/10/g3460.png)


<img align="left" src="https://devwork.space/projects/wiimotedev/wiimotedev-ir-demo-2022-09-06.gif" width="500" style="display:block;float:none;margin-left:auto;margin-right:auto;width:95%" />

A userspace service that distrbute connected wiiremotes events using dbus and emulates keyboard, mouse or joysticks.

Requirements:
```sh
 spdlog
 eigen
 xwiimote-ng
```

Compilation:
```sh
cmake .
make all -j$(nproc)
make install
```
