#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "IHandler.h"
#include <string>

class XMLParser 
{

public:
    void parse(const std::string& xmlData);

private:
    std::string getVersion(const std::string& xmlData);
};

#endif