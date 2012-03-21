#ifndef DMCR_SETTINGS_H
#define DMCR_SETTINGS_H

#include <string>
#include <unordered_map>

namespace dmcr {

class Settings {
public:
    static Settings& get();

    void load(const std::string& path);
    void loadArgs(int argc, char* argv[]);

    int readInt(const std::string& key, int def);
    std::string readString(const std::string& key, std::string def);

private:
    Settings();

    std::unordered_map<std::string, std::string> m_keys;
};

}

#endif

