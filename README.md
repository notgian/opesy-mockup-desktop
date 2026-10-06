# CSOPESY Mockup Desktop
This project was made in submission to CSOPESY

Section S03
- GIAN LORENZO ORTHA
- GIANELA KIM AGSALON
- MARC JARED SEAN ERCIA
- THEON SCHUYLER GARCIA

# Cloning the Repo

Cloning the repo will have an extra step, because imgui was included as a submodule
```cmd
# clone the repo
git clone https://github.com/notgian/opesy-mockup-desktop.git

# update the imgui submodule
cd opesy-mockup-desktop
git submodule update --init --recursive
```


# Building and Running
In compliance with the requirements to build the application from the IDE, a custom vscode build task has been included.

PRE-REQUISITES:
1. You MUST be on WINDOWS.
2. You MUST have a modern mingw build version i.e. from [WinLibs](https://winlibs.com/)

## How to Build and Run
1. From `main.cpp` click the run button from vscode.
2. You may be prompted for what build task to use. Click the one that indicates `C++ Launch and Run preLaunchTask: cppbuild`.

<img width="725" height="236" alt="image" src="https://github.com/user-attachments/assets/44e7849d-7d25-47ca-8b57-5b9161b91997" />

3. The application will build and be run in the VSCode integrated terminal. If all goes well, the window will pop up and all is good.
