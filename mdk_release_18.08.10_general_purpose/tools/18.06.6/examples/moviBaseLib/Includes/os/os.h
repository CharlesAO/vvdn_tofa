#pragma once

#ifndef OS_OS_H_
#define OS_OS_H_

#pragma region Include files
#include <string>
#include <vector>
#pragma endregion

namespace os {

/// \brief get Directory separator
extern const char PathSeparator;

/// \brief get Path list separator
extern const char PathListSeparator;

/// \brief get null file name
extern const char * const NullDeviceName;

/// \deprecated OBSOLETE: use getPathSeparator
inline char getDirectorySeparator() { return PathSeparator; }

/// \brief determine if character is directory separator
bool isPathSeparator(char c);

/// \brief get current directory to string
std::string getCurrentDirectory();

/// \brief get current directory to buffer
bool getCurrentDirectory(char *dir, size_t max_size);

/// \brief get home directory of user
std::string getHomeDirectory();

/// \brief get current directory -- safe version for character arrays
template <size_t N>
inline void getCurrentDirectory(char (&filePath)[N])
{
    getCurrentDirectory(filePath, N);
}

/// \brief set current directory to buffer content
bool setCurrentDirectory(const char *dir);

/// \brief set current directory to string
inline bool setCurrentDirectory(const std::string &dir) { return setCurrentDirectory(dir.c_str()); }

/// \brief get real path name (absolute path from relative, symlinks resolved, file must be accessible)
std::string getRealPath(const char *path);

/// \brief get real path (std::string argument overload)
inline std::string getRealPath(const std::string &str) { return getRealPath(str.c_str()); }

/// \brief strip last component from path.
std::string dirName(const std::string &path);

/// \brief strip leading directory from path
std::string baseName(const std::string &path);

/// \brief determine if path has directory part
bool hasDirName(const std::string &path);

/// \brief `true` if absolute path
bool isAbsolutePath(const std::string &path);

/// \brief `true` if relative path
bool isRelativePath(const std::string &path);

/// \brief Combine two paths
/// \details Empty components are ignored.
///          The first path is ignored if the second path is an absolute path.
///          Adds OS-appropriate directory separator characters between path and subpath.
std::string pathCombine(const std::string &path, const std::string &subpath);

template <typename S, typename... T>
std::string pathCombine(const std::string &path, const std::string &subpath, S &&subpath2, T&&... args)
{
    return pathCombine(path, pathCombine(subpath, std::forward<S>(subpath2), std::forward<T>(args)...) );
}

/// \brief Split path into components by path separator. No additional processing.
void pathSplit(const std::string &path, std::vector<std::string> *components);

/// \brief Join path from components (concatenate using path separator)
std::string pathJoin(const std::vector<std::string> &components);

/// \brief get parent directory of path, specified level upwards (default is one)
/// \param path - path of which parent directory we are interested (need not to exist)
/// \param level - number of levels to go upwards (default is one)
/// \return parent directory of `path`, `level` levels upwards
/// \details The function tries to remove path component(s) from end of `path` while this is logically possile.
///          It will then append ".." component(s) to it if necessary.
/// \note    Empty and "." components are removed automatically from the path's end until reaching the desired level.
///          The function will not append ".." components if it reaches the root of an absolute path during removal.
std::string getParentDir(const std::string &path, int level = 1);

/// \brief Environment is vector of strings
using Environment = std::vector<std::string>;

/// \brief get environment (NAME=VALUE string list)
bool getEnvironment(Environment *environment);

/// \brief get environment (NAME=VALUE string list)
/// \throws std::system_error on failure
Environment getEnvironment();

/// \brief get environment variable value
const char *getEnvVar(const Environment &environment, const char *varName);

/// \brief set environment variable value
void setEnvVar(Environment &environment, const char *varName, const char *value);

/// \brief get last OS error code (os-dependent)
int getLastErrorCode();

/// \brief get last error as string
std::string getLastErrorString();

/// \brief get name of process executable
std::string getExecutableName();

/// \brief get directory part of process executable with separator
std::string getExecutablePath();

} // namespace os

#endif // OS_COMMON_H_
