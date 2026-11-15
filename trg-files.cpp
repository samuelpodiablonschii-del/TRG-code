#define TRG_FILES_DLL_EXPORT __declspec(dllexport)

#include <string>
#include <filesystem>
#include <fstream>

TRG_FILES_DLL_EXPORT bool filesEnabled = false;
TRG_FILES_DLL_EXPORT bool jsonEnabled = false;

TRG_FILES_DLL_EXPORT bool fileExists(const std::string& path);
TRG_FILES_DLL_EXPORT std::string fileRead(const std::string& path);
TRG_FILES_DLL_EXPORT bool fileWrite(const std::string& path, const std::string& content);
TRG_FILES_DLL_EXPORT bool fileCreate(const std::string& path, const std::string& content);