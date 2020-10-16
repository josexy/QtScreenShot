# QtScreenShot
The QtScreenShot was written by Qt and UI design referenced from Snipaste and Flameshot(because I use snipaste on Windows :) )

### Clone repository
Linux:
```bash
git clone https://github.com/josexy/QtScreenShot.git --recursive
cd QtScreenShot
```
Windows:
Please use Github Desktop

### Compile & Install QHotkey library
The QHotkey is a cross-platform global hotkey library.Before compiling QtScreenShot, you must had already compiled QHotkey library.

```bash
cd QHotkey
qmake
make
make install
cd ..
```

### QtScreenShot
```bash
qmake
make
cd bin
./QtScreenShot
```
On Linux, you could directly run the `QtScreenShot` binary file in terminal as well after running command `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./`. In addition, you must copy the libQHotkey.so\* to `QtScreenShot` directory if you don't install QHotkey library.

On Windows, you should copy the QHotkey.dll to `QtScreenShot` path.That is easily.

### Need
- Python with requests library(Upload image)
- QHotkey library(Global hotkey)
- Smms account(Image Server)
- Proxy(Optional)

### Screenshot Example

<img src="screenshots/01.png" style="zoom:80%;" />

<img src="screenshots/02.png" style="zoom:80%;" />
