#pragma once

#ifndef OS_SHARED_LIBRARY_H_
#define OS_SHARED_LIBRARY_H_

#pragma region Include files
#include <string>
#include <vector>
#pragma endregion

namespace os {

/// \brief Portable Shared Library class
/// Implemented for Windows and Linux
/// * Windows: uses Win32 API
/// * Linux: uses `ldl` (dlopen, dlclose, dlsym)
class SharedLibrary
{
public:
    /// \brief Create an uninitialized shared library
    SharedLibrary();
    /// \brief Try to load shared library from supplied path
    /// \param path absolute or relative path
    explicit SharedLibrary(const char *path);

    /// \brief Try to load shared library from supplied path
    /// \param path absolute or relative path
    explicit SharedLibrary(const std::string &path);

    /// no (default) copy construction allowed
    SharedLibrary(const SharedLibrary&) = delete;
    /// no (default) copy assignment allowed
    SharedLibrary&operator=(const SharedLibrary&) = delete;

    /// move construction
    SharedLibrary (SharedLibrary&&) noexcept;
    /// move assignment
    SharedLibrary &operator=(SharedLibrary &&) noexcept;

    /// \brief Try to load shared library from supplied path
    /// \param path absolute or relative path
    bool load(const char *path);

    /// \brief Try to load shared library from supplied path
    /// \param path absolute or relative path
    bool load(const std::string &path);

    /// \brief Unload shared library
    virtual ~SharedLibrary(void);

    /// \brief Test whether shared library has been in fact loaded
    /// \return `true` if and only if shared library has in fact been loaded
    bool isLoaded() const;

    /// \brief allow use in boolean expressions
    /// \return `true` iff `isLoaded`
    explicit operator bool() const { return isLoaded();}

    /// \brief allow use in boolean expressions
    /// \return `true` iff `not isLoaded`
    bool operator !() const { return !isLoaded();}

    /// \brief Unloads the shared library
    void unload();

    /// \brief Releases wrapped handle without unloading the library
    /// \note The state of the object will be as if default-initialised
    using Handle = void *;
    Handle release();

    /// \brief Wrap given handle (usually loaded externally)
    /// \note The previously wrapped library pointer will be unloaded, if any
    void reset(Handle hLibrary);

    /// \brief Get shared library path. Not necessarily the same as construction argument.
    std::string getPath() const;

    /// \brief Get shared library path from address within loaded module.
    /// \returns string representing the path of the shared library which
    ///          contains the given address, or an empty string on any error.
    /// \remarks This can be used to identify the location a particular shared library
    ///          was loaded from, similar to os::getExecutableName.
    static std::string getPathFromAddress(const void *address);

    /// \brief Get pointer to library symbol (usually a function)
    /// \param name symbol name to retrieve address of
    /// \return address to symbol or `NULL` if not found
    void* getSymAddress(const char *name) const;

    /// \brief Assign address of function to function pointer
    /// Avoids excessive type-casting code which is as matter of fact not valid ISO C++
    /// (I would say this is the proper way of assigning addresses to functions)
    /// Note: care must be taken because the type of the func parameter is in not checked for compatibility
    /// \param name name of desired function
    /// \param fun preferably a reference to a function pointer. This is not verified at compile time.
    ///            The function pointer will hold the address to the specified function or NULL when this was not found.
    /// \return `true` on success, `false` when `fun` is `NULL`
    template <class Function>
    bool getProcAddress(const char *name, Function* &rPFunc) const
    {
        void * const symAddress = getSymAddress(name);
        reinterpret_cast<void*&>(rPFunc) = symAddress;
        return symAddress != nullptr;
    }

    /// \brief get list of loaded modules
    static bool getLoadedModuleList(std::vector<std::string> *result);

    /// \brief get last error string
    static std::string getLastErrorString();

private:
    /// Private implementation
    struct Impl;
    /// Pointer to private data
    Impl *p;
};

} // namespace os

#endif // OS_SHARED_LIBRARY_H_
