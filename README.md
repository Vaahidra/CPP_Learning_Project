# tower_control
guide planes to land safely

### Requirements

The following software is necessary to compile and run the program
1. C++ compiler capable of compiling C++17 code (for example, gcc version 9 or above)
2. freeglut
3. OpenGL version 1.1 or above
4. CMake version 3 or above to generate the project

#### Linux

Make sure your gcc version is 9 or above.
If it's not, and your package manager doesn't propose it, then you may need to upgrade your system first.

Then install freeglut using your favorite package manager.

#### Mac

Install freeglut using a package manager like `brew`.

**Attention**:
For macOS > 10.8, you'll need to install and run XQuartz.

**Troubleshoot**:
If you get messages like `<some lib> was built for newer macOS version (XX) than being linked (XX)` try updating/installing `freeglut` with brew!

#### Windows

Install the MSYS2 package manager by following the instructions on this page : https://www.msys2.org/.
Once you have finished, you can install freeglut by tapping `pacman -S mingw-w64-x86_64-freeglut` in the MSYS2 console.

Edit the "Path" environment variable (the user or system one):
1. Add "C:\msys64\mingw64\bin" to it (if you installed MSYS2 elsewhere, then update the path accordingly).
2. If you had a previous MinGW version installed (usually located in C:\mingw"), remove its bin folder from the Path.

Open the project inside VSCode.
Run the command `Preferences: Open User Settings`.
Search for the setting `CMake: Mingw Search Dirs` and add `"C:\msys64\mingw64` to it (without the bin folder).
Restart VSCode, run the command `CMake: Scan For Kits`, then `CMake: Edit User-Local CMake Kits`.
Add the following code inside the JSON of the MSYS2 compilers:
```json
"preferredGenerator": {
  "name": "MinGW Makefiles"
},
"environmentVariables": {
  "CMT_MINGW_PATH": "C:\\msys64\\mingw64\\bin"
}
```
Restart VSCode again, and run `CMake: Configure`.

### Build

You can build and execute the program from VSCode, as usual.

If you want to build it from a terminal, place yourself inside the top-level directory of the project, and use the following commands: 
```
mkdir build
cd build
cmake ..
make
```
