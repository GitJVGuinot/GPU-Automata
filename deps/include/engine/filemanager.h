#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__ 1

#include <string>

struct Header
{
  const char file_name_[256];
  const char key_file_name_[256];

  size_t file_data_size_;
};

struct FileArray
{
  Header* headers;
  
  std::string* files_data;
  size_t files_count;
};

bool operator==(const Header& h_1, const Header& h_2);
bool operator==(const FileArray& f_1, const FileArray& f_2);

/**
 * @brief Load the source code of a file.
 *
 * @param file Path of the file to upload.
 *
 * @return A string of characters that represents the contents of the file.
 */
std::string LoadSourceFromFile(const char* file);
void SaveSourceInFile(const char* file, std::string save_data);

std::string LoadSourceFromBinary(const char *file);
void SaveSourceInBinary(const char* file, std::string save_data);

void GroupFilesInBinary(const FileArray &file_array, const char* name_no_extension);
const FileArray LoadGroupFilesBinary(const char* name_no_extension);

std::string EncryptData(std::string decrypted_data, char key);
std::string DecryptData(std::string encrypted_data, char key);

/**
 * @struct FConfigFileData
 */
typedef struct
{
  int count;
  float *values;
} FConfigFileData;

/**
 * @brief Load a array of values setted in a config file
 *
 * To use is need to have some rules.
 * In the file you need to write the value name and the values (If have more than one)
 * Examples ->
 *  Health: 10
 *  Str: 1.2
 *  Vel: 12, 20.5, 7
 *
 * @param file Path of the file to read.
 * @param var_name value name.
 *
 * @return A array of values and the count
 * Take into acount that you can have more than one value per line.
 */
FConfigFileData ReadVarFromFile(const char *file, const char *var_name);

/**
 * @struct CConfigFileData
 */
typedef struct
{
  int count;
  char **values;
} CConfigFileData;

/**
 * @brief Load an array of strings set in a config file
 *
 * To use is need to have some rules.
 * In the file you need to write the value name and the values (If have more than one)
 * Examples ->
 *  Health: good, excellent
 *  Str: strong, weak
 *  Vel: fast, moderate, slow
 *
 * @param file Path of the file to read.
 * @param var_name value name.
 *
 * @return A array of strings and the count
 * Take into account that you can have more than one value per line.
 */
CConfigFileData ReadStringFromFile(const char *file, const char *var_name);

#endif /* __FILEMANAGER_H__ */