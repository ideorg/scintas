#ifndef SCINTAS_CONFIG_H
#define SCINTAS_CONFIG_H

#include <string>
#include <vector>

class Config {
    std::string iniPath;
    std::vector<std::string> mru;
public:
    Config();
};


#endif //SCINTAS_CONFIG_H
