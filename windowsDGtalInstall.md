# Installation of Dgtal on Windows
These main steps are based on Visual Studio 2022, and the overall demarch is to generate first a **.sln** project from *cmake* command line and then open and build the project unde VS 2022.

## Dependancies installation

### Install Git:
  - Download and install [git](https://git-scm.com/download/win) if not already present.
  
### Install Cmake:
  - Download [cmake](https://cmake.org/download)
  - During the installation select the installation be sure to select the option to have it the system path:
    <center> <img width="50%" src="https://user-images.githubusercontent.com/772865/197339178-ebaf2aaf-7a70-40ba-8789-cc157e40a72c.png"></img> </center>

### Zlib installation:
  -  Get [ZLib](http://zlib.net) source: http://zlib.net/zlib1213.zip (copy past link in new windows since the url is not in https) or [here](https://ipol-geometry.loria.fr/Sites/zlib-1.2.11.zip).
  -  Unzip the library in the directory of your choice.
  -  Open PowerShell.
  -  Go to the directory from the unziped library.
  -  Do a classical: 
    ``mkdir build; cd build; cmake..``
     <center> <img width="50%" src="https://user-images.githubusercontent.com/772865/197238765-2315e467-0c02-4a0e-941b-899e5c5b6c95.png"></img> </center>
   
  
  - The previous command should have generated the VS project **zlib.sln** that you can open.
  - Generate the solution and locate from the log the location of the dll library path: 
     <center> <img width="50%" src="https://user-images.githubusercontent.com/772865/197240541-25a4bf0a-288a-4051-95cb-5e581adf2caf.png"></img> </center>



  - Copy the file zconf.h located in zlib1213\zlib-1.2.13\build into the source directory: zlib1213\zlib-1.2.13 

### Boost installation:
  - Download boost for instance from here: https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/boost_1_80_0.zip
  - Move it on C:\Program Files\ to make easier the detection from cmake.
  - Uncompress it (be patient around 10 minutes to uncompress it).
  - Clic on 
    ``C:\Program Files\boost_1_80_0\bootstrap.bat``
  
## DGtal build

### From a powershell:
  - Clone the DGtal project:
     ``git clone https://github.com/DGtal-team/DGtal.git``
  - Go to the DGtal dir and make the classical: 
    ``cd DGtal; mkdir build; cd build;``
  - Then from the path of the previous steps:
   ``cmake .. -DZLIB_LIBRARY="C:\yourChosenPath\zlib1213\zlib-1.2.13\build\Debug\" -DZLIB_INCLUDE_DIR="C:\yourChosenPath\zlib1213\zlib-1.2.13``   
    This command will generate the VS DGtal project **DGtal.sln**
### From visual studio code:
   - Open the **DGtal.sln** VS  project generated from the previous step.
   - Generate the solution from VS code interface.
   ![Capture d’écran 2022-10-21 182709](https://user-images.githubusercontent.com/772865/197244027-9cc913d2-d492-435c-bc95-e9734e21a321.png)
   - If now errors you can now exploit the generated library 🎉.
