#include "platform/plugin_loader.h"
#include <iostream>
#include <stdexcept>

#if PLATFORM_WINDOWS
#include <windows.h>
#elif PLATFORM_POSIX
#include <dlfcn.h>
#endif

std::vector<PluginInfo> PluginLoader::loadedPlugins;

PluginInfo PluginLoader::loadPlugin(const std::string& pluginPath) {
    PluginInfo info = {};
    info.path = pluginPath;
    info.name = getPluginName(pluginPath);
    info.isLoaded = false;
    
#if PLATFORM_WINDOWS
    info.handle = LoadLibraryA(pluginPath.c_str());
    if (info.handle == NULL) {
        throw std::runtime_error("Failed to load plugin: " + std::to_string(GetLastError()));
    }
#elif PLATFORM_POSIX
    info.handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
    if (info.handle == NULL) {
        throw std::runtime_error("Failed to load plugin: " + std::string(dlerror()));
    }
#endif
    
    info.isLoaded = true;
    loadedPlugins.push_back(info);
    
    std::cout << "Successfully loaded plugin: " << info.name << std::endl;
    return info;
}

bool PluginLoader::unloadPlugin(const PluginInfo& plugin) {
    if (!plugin.isLoaded) {
        return false;
    }
    
#if PLATFORM_WINDOWS
    bool result = FreeLibrary(plugin.handle) != 0;
#elif PLATFORM_POSIX
    bool result = dlclose(plugin.handle) == 0;
#endif
    
    if (result) {
        // Remove from loaded plugins list
        for (auto it = loadedPlugins.begin(); it != loadedPlugins.end(); ++it) {
            if (it->handle == plugin.handle) {
                loadedPlugins.erase(it);
                break;
            }
        }
        std::cout << "Successfully unloaded plugin: " << plugin.name << std::endl;
    }
    
    return result;
}

bool PluginLoader::isPluginLoaded(const PluginInfo& plugin) {
    return plugin.isLoaded && plugin.handle != NULL;
}

void* PluginLoader::getFunction(const PluginInfo& plugin, const std::string& functionName) {
    if (!isPluginLoaded(plugin)) {
        return nullptr;
    }
    
#if PLATFORM_WINDOWS
    return GetProcAddress(plugin.handle, functionName.c_str());
#elif PLATFORM_POSIX
    return dlsym(plugin.handle, functionName.c_str());
#endif
}

std::vector<PluginInfo> PluginLoader::getLoadedPlugins() {
    return loadedPlugins;
}

PluginInfo PluginLoader::findPluginByName(const std::string& name) {
    for (const auto& plugin : loadedPlugins) {
        if (plugin.name.find(name) != std::string::npos) {
            return plugin;
        }
    }
    
    return {}; // Return empty PluginInfo if not found
}

bool PluginLoader::validatePlugin(const std::string& pluginPath) {
    // Basic validation - check if file exists and has correct extension
#if PLATFORM_WINDOWS
    if (pluginPath.find(".dll") == std::string::npos) {
        return false;
    }
#elif PLATFORM_POSIX
    if (pluginPath.find(".so") == std::string::npos) {
        return false;
    }
#endif
    
    // Try to load and immediately unload to validate
    try {
        PluginInfo info = {};
        info.path = pluginPath;
        info.name = getPluginName(pluginPath);
        info.isLoaded = false;
        
#if PLATFORM_WINDOWS
        info.handle = LoadLibraryA(pluginPath.c_str());
        if (info.handle == NULL) {
            return false;
        }
        FreeLibrary(info.handle);
#elif PLATFORM_POSIX
        info.handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
        if (info.handle == NULL) {
            return false;
        }
        dlclose(info.handle);
#endif
        
        return true;
    } catch (...) {
        return false;
    }
}

std::string PluginLoader::getPluginName(const std::string& path) {
    size_t lastSlash = path.find_last_of("\\/");
    if (lastSlash != std::string::npos) {
        std::string filename = path.substr(lastSlash + 1);
        
        // Remove extension
        size_t lastDot = filename.find_last_of('.');
        if (lastDot != std::string::npos) {
            return filename.substr(0, lastDot);
        }
        return filename;
    }
    return path;
}
