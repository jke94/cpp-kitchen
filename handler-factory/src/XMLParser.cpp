#include <iostream>
#include <memory>

#include "XMLParser.h"
#include "HandlerFactory.h"

void XMLParser::parse(const std::string& xmlData) 
{
    std::string version = getVersion(xmlData);

    std::shared_ptr<IHandler> handler = HandlerFactory::getHandler(version);
    
    handler->handle(xmlData);
}

std::string XMLParser::getVersion(const std::string& xmlData) 
{
    // TODO: Use XML real parser library:

    if (xmlData.find("<version>1.0</version>") != std::string::npos) 
    {
        return "1.0";
    } 
    else if (xmlData.find("<version>2.0</version>") != std::string::npos) 
    {
        return "2.0";
    }
    throw std::invalid_argument("Version not found");
}