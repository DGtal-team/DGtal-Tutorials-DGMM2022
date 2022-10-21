# Installation of Dgtal on Windows
These main steps are based on Visual Studio 2022

## Main steps:
### Install Cmake:
  - https://cmake.org/download/
  - During the installation select the installation be sure to select the option to have it the system path:
  - 
![Capture d’écran 2022-10-21 155556](https://user-images.githubusercontent.com/772865/197239144-99bbe38d-8648-41b1-81be-e6c5387b65ee.png)

### Zlib installation:
  -  Get lib source: http://zlib.net/zlib1213.zip or here https://ipol-geometry.loria.fr/Sites/zlib-1.2.11.zip
  -  Unzip the library in the directory of your choice.
  -  Open PowerShell:
  -  Go to the directory from the unziped library.
  -  Do a classical: 
    ``mkdir build; cd build; cmake..``
   ![Capture d’écran 2022-10-21 174657](https://user-images.githubusercontent.com/772865/197238765-2315e467-0c02-4a0e-941b-899e5c5b6c95.png)
  
  - The previous command should have generated the visual studio project: zlib.sln that you can open.
  - Generate the solution and locate from the log the location of the dll library path: 
![Capture d’écran 2022-10-21 180707](https://user-images.githubusercontent.com/772865/197240541-25a4bf0a-288a-4051-95cb-5e581adf2caf.png)


  - Copy the file zconf.h located in zlib1213\zlib-1.2.13\build into the source directory: zlib1213\zlib-1.2.13 

### Boost installation:
  - Download boost for instance from here: https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/boost_1_80_0.zip
  - Move it on C:\Program Files\ to make easier the detection from cmake.
  - Uncompress it (be patient around 10 minutes to uncompress it).
  - Clic on C:\Program Files\boost_1_80_0\bootstrap.bat
  
