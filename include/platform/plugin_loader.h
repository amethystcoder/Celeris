#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H

#include "platform/platform.h"
#include <string>
#include <vector>
#include <memory>

#if PLATFORM_WINDOWS
    typedef HMODULE PluginHandle;
#elif PLATFORM_POSIX
    typedef void* PluginHandle;
#endif

struct PluginInfo {
    PluginHandle handle;
    std::string path;
    std::string name;
    bool isLoaded;
};

class PluginLoader {
public:
    // Plugin loading and unloading
    static PluginInfo loadPlugin(const std::string& pluginPath);
    static bool unloadPlugin(const PluginInfo& plugin);
    static bool isPluginLoaded(const PluginInfo& plugin);
    
    // Plugin function access
    static void* getFunction(const PluginInfo& plugin, const std::string& functionName);
    
    // Plugin enumeration
    static std::vector<PluginInfo> getLoadedPlugins();
    static PluginInfo findPluginByName(const std::string& name);
    
    // Plugin validation
    static bool validatePlugin(const std::string& pluginPath);

private:
    static std::string getPluginName(const std::string& path);
    static std::vector<PluginInfo> loadedPlugins;
};

#endif // PLUGIN_LOADER_H
