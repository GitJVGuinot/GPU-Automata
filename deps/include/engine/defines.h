#ifndef __DEFINES_H__
#define __DEFINES_H__ 1

#ifdef _WIN32
#define CLEAR_CONSOLE fprintf(stdout, "%d\n", system("cls"));
#define FONT_PATH_R "C:/Windows/Fonts/Arial.ttf"
#define ASSETS(str) ("../../assets/" str)
#define FONTS(str) ("../../assets/fonts/" str)
#define IMAGE(str) ("../../assets/image/" str)
#define SHADER(str) ("../../assets/shader/" str)
#define MUSIC(str) ("../../assets/music/" str)
#define OBJ(str) ("../../assets/objects/" str)
#elif __linux__
#define FONT_PATH_R "/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf"
#define CLEAR_CONSOLE fprintf(stdout, "%d\n", system("clear"));
#define ASSETS(str) ("../../assets/" str)
#define FONTS(str) ("../../assets/fonts/" str)
#define IMAGE(str) ("../../assets/image/" str)
#define SHADER(str) ("../../assets/shader/" str)
#define MUSIC(str) ("../../assets/music/" str)
#define OBJ(str) ("../../assets/objects/" str)
#endif
#define PRINT_ARGS                    \
  fprintf(stdout, "Checking args\n"); \
  for (s32 i = 0; i < argc; i++)      \
    fprintf(stdout, "%d - %s\n", i, argv[i]);

#define DESTROY(ptr)  \
  if (ptr != nullptr) \
  {                   \
    std::free(ptr);   \
    ptr = nullptr;    \
  }

#define CLEAR(x) memset(x, 0, sizeof(x));

#endif /* __DEFINES_H__ */