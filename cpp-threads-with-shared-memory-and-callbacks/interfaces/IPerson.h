#ifndef IPERSON_H
#define IPERSON_H

#include <string>

class IPerson
{
    public:
        virtual ~IPerson() {};
        virtual std::string get_name() = 0;
        virtual int get_ppm() = 0;
};

#endif