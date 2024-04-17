- Windows use
- - Need to have python3
- - Need to have premake (Latest better)
- - Need to have conan and in the path
- - Go to tools and Execute GenDepsAndMakeSolution.py with python
- - If the first time press "y" to install dependecies otherwise isn't necessary
- - Go to build/visual_studio and Execute Engine.sln (Compile pressing F5)
- - Each time you add any file. Is recommended to execute GenDepsAndMakeSolution.py

- Linux use
- - Need to have vscode
- - Need to have python3
- - Go to tools and Execute python3 GenDepsAndMakeSolution.py
- - Install everything the script want
- - Now open the full project with vscode (There are a simple compile task for debug and release use ctrl + shift + b to compile)

- Organization
- - To have files organizated you need to save all assets in assets/something
- - Also you have in engine.h paths to that folder
