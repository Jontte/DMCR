#include "settings.h"
#include <fstream>
#include "util.h"
#include <iostream>

using namespace dmcr;

Settings& Settings::get() {
    static Settings* instance = nullptr;
    if (!instance)
        instance = new Settings();

    return *instance;
}

Settings::Settings() {
}

void Settings::load(const std::string& path)
{
    std::ifstream fp(path, std::ifstream::in);

    while (fp.good()) {
        std::string line;
        std::getline(fp, line);
        if (line[0] == '#' || line.length() == 0)
            continue;
        unsigned int split_pt = line.find_first_of(" \t");
        if (split_pt == std::string::npos)
            throw "FOO!";

        m_keys.insert({line.substr(0, split_pt),
                       line.substr(split_pt+1)});
    }
}

int Settings::readInt(const std::string& key, int def)
{
    if (m_keys.count(key) == 1) {
        std::string v = m_keys[key];
        return dmcr::lexical_cast<int>(v);
    } else
        return def;
}

