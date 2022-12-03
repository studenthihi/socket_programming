# socket_programming

# Usage
## Download wxWidget
```console
$ git clone --recurse-submodules https://github.com/wxWidgets/wxWidgets.git
```

## Configure with GTK
```console
$ mkdir buildgtk
$ cd buildgtk
$ ../configure --with-gtk
$ make
$ sudo make install
$ sudo ldconfig
```
## Dowload project
```
iris@iris-lei:~/socket_programming$ make
iris@iris-lei:~/socket_programming$ ./test
```


<!-- ## Configure with CMake
1. Install CMake
2. Open CMake gui

```
cmake-gui
```
3. Create Build folder

```
cd wxWidgets
mkdir mybuild
```

4. Type the paths to Source and Build folder
e.g: 
Source: /home/iris/wxWidgets
Build: /home/iris/wxWidgets/mybuild

    Press Configure button and then Generate button
5. Build
```
cd ~/wxWidgets/mybuild
make -j8
make install
``` -->
<!-- 6.  -->
