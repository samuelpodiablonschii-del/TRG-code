#define TRG_NETWORK_DLL_EXPORT __declspec(dllexport)

#include <string>
#include <windows.h>

TRG_NETWORK_DLL_EXPORT std::string httpGet(std::string url, std::string& error);
TRG_NETWORK_DLL_EXPORT std::string httpPost(std::string url, std::string body, std::string& error);
TRG_NETWORK_DLL_EXPORT bool networkEnabled = false;
