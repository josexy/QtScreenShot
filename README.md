# QtScreenShot
The QtScreenShot was written by Qt and UI design referenced from Snipaste and Flameshot(because I use snipaste on Windows :) )

### Compile
#### Clone repository
Linux:
```bash
git clone https://github.com/josexy/QtScreenShot.git --recursive
cd QtScreenShot
```
Windows:
Please use Github Desktop

#### Install QHotkey library
The QHotkey is a cross-platform global hotkey library.Before compiling QtScreenShot, you must had already compiled QHotkey library.

```bash
cd QHotkey
qmake
make
make install
cd ..
```

#### QtScreenShot
```bash
qmake
make
cd bin
./QtScreenShot
```
On Linux, you should run the `QtScreenShot` in terminal as well after setting `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./` under the `QtScreenShot` location and copy the libQHotkey.so\* to `QtScreenShot` path.

On Windows, you should copy the QHotkey.dll to `QtScreenShot` path.

#### Need
- Python with requests(Upload image)
- QHotkey library(Global hotkey)
- Smms account(Store image)
- Proxy(Optional)

### Screenshot Example

<img src="screenshots/01.png" style="zoom:80%;" />

<img src="screenshots/02.png" style="zoom:80%;" />
