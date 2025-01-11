#include <iostream>
#include <string>

#include "XMLParser.h"

int main() 
{
    XMLParser parser;

    std::string xmlData1 = "<root><version>1.0</version></root>";
    std::string xmlData2 = "<root><version>2.0</version></root>";
    std::string xmlData3 = "<root><version>3.0</version></root>";

    try 
    {
        parser.parse(xmlData1);
        parser.parse(xmlData2);
        parser.parse(xmlData3);
    } 
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
