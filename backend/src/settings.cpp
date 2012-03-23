#include "settings.h"
#include <fstream>
#include "util.h"
#include <iostream>
#include <vector>

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

void Settings::loadArgs(int argc, char* argv[])
{
    std::vector<std::string> args(argv, argv+argc);

    for (unsigned i = 0; i < args.size(); ++i) {
        std::string& s = args.at(i);
        if (s.length() > 2 && s[0] == '-' && s[1] == '-' 
                && i+1 < args.size())
        {
            std::string v = args.at(i+1);
            std::string k = s.substr(2);
            m_keys.insert({k,v});
            ++i;
        }
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

std::string Settings::readString(const std::string& key, std::string def) {
    if (m_keys.count(key) == 1) {
        return m_keys[key];
    } else return def;
}

