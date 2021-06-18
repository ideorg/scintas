# scintas
Editor with Scintilla. Clone it with submodules:<br>
git clone --recurse-submodules https://github.com/ideorg/scintas

## Build on Linux (Debian based): <br>
sudo apt install build-essential <br>
sudo apt install cmake <br>
sudo apt install libwxgtk3.0-dev (Debian) or <br>
sudo apt install libwxgtk3.0-gtk3-dev (Ubuntu) <br>
mkdir build <br>
cd build <br>
cmake .. <br>
make -j4 <br>
strip scintas <br>
