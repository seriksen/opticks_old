#pragma once

#include <string>

class fsutil {
    public:
       static std::string FormPath(const char* path, const char* sub=NULL, const char* name=NULL);
       static std::string FindFile(const char* dirlist, const char* sub, const char* name=NULL, const char* dirlist_delim=";");
       static std::string Stem(const char* path);
       static std::string Name(const char* path);

       static bool ExistsNativeFile(const std::string& native);
       static bool ExistsNativeDir(const std::string& native);
       static bool ExistsFile(const char* path, const char* sub=NULL, const char* name=NULL);
       static bool ExistsDir(const char* path, const char* sub=NULL, const char* name=NULL);
       static void CreateDir(const char* path, const char* sub=NULL);
    private:
       static void setOpticksPathPrefix(const char* prefix);
       static void setOpticksPathPrefixFromEnv(const char* envvar="OPTICKS_PATH_PREFIX");
       static void dumpOpticksPathPrefix(const char* msg="fsutil::dumpOpticksPathPrefix");
    private:
       static char* OPTICKS_PATH_PREFIX ; 
};


