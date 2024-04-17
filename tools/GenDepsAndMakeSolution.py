
import platform
import os

script_path = os.path.dirname(os.path.abspath(__file__))

os.chdir(script_path)

system = platform.system()

def win_conan():
      os.chdir('../')
      os.system('conan install -if build/deps/Release -s build_type=Release -s compiler="Visual Studio" -s compiler.runtime=MD --build=* tools')
      os.system('conan install -if build/deps/RelWithDebInfo -s build_type=RelWithDebInfo -s compiler="Visual Studio" -s compiler.runtime=MD --build=* tools')
      os.system('conan install -if build/deps/Debug -s build_type=Debug -s compiler="Visual Studio" -s compiler.runtime=MDd --build=* tools')
      os.chdir("tools")

def main():
    if system == "Windows":
      os.system('cls')
      if (input("Ejecutar Conan para la instalacion de librerias? (y/n): ").lower()) == 'y':
        win_conan()
        print("Conan executed")

      # Esperar a que el usuario presione Enter para salir
      input("Presiona Enter para continuar")

      os.system('premake5 vs2019')
      
    elif system == "Linux":
      os.system('clear')
      if (input("Desea descargar e instalar dependencias? (y/N): ").lower()) == 'y':
        os.system('sudo apt install build-essential libglew-dev libglfw3 libglfw3-dev freeglut3 freeglut3-dev')
        os.system('sudo apt-get install libopenal-dev')
    
if __name__ == "__main__":
    main()

# Esperar a que el usuario presione Enter para salir
input("Presiona Enter para salir...")