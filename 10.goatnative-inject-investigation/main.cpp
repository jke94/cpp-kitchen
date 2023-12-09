//
//  main.cpp
//  Injector
//
//  Created by Max Raskin on 1/17/15.
//  Copyright (c) 2015 Max Raskin. All rights reserved.
//

#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

#include "Injector.h"

class IDummy
{
    public:
        virtual int get_number() = 0;
};

class Dummy : public IDummy
{
    public:
        int get_number() override
        {
            return 1994;
        }
};

class ISuperDummy
{
    public:
        virtual int get_number() = 0;
};

class SuperDummy : public ISuperDummy
{
    private:
        std::shared_ptr<IDummy> dummy_;

    public:
        SuperDummy(std::shared_ptr<IDummy> dummy) : dummy_ (dummy){}
        SuperDummy();

        int get_number() override
        {
            return 1995;
        }
};

class IMegaDummy
{
    public:
        virtual int get_number() = 0;
        virtual std::shared_ptr<ISuperDummy> get_super_dummy() = 0;
};

class MegaDummy : public IMegaDummy
{
    private:
        std::shared_ptr<ISuperDummy> super_dummy_;

    public:
        MegaDummy(std::shared_ptr<ISuperDummy> super_dummy) : super_dummy_ (super_dummy){}
        MegaDummy();

        int get_number() override
        {
            return 1996;
        }

        std::shared_ptr<ISuperDummy> get_super_dummy() override
        {
            return super_dummy_;
        }

};

std::string get_spreader_bar(char character, int length)
{
    std::string value = "";

    for(int i=0; i<length; i++)
    {
        value = value + character;
    }

    return value;
}

void example_with_several_dependencies_between_classes()
{
    goatnative::Injector injector;

    // Building dependencies.

    injector.registerClass<Dummy>();
    injector.registerInterface<IDummy,Dummy>();

    injector.registerClass<SuperDummy,IDummy>();
    injector.registerInterface<ISuperDummy,SuperDummy>();

    injector.registerClass<MegaDummy,ISuperDummy>();
    injector.registerInterface<IMegaDummy,MegaDummy>();


    // Example A: Get object and access to de function exported through the interface.

    std::shared_ptr<IDummy> dummy = injector.getInstance<IDummy>();
    std::shared_ptr<ISuperDummy> super_dummy = injector.getInstance<ISuperDummy>();
    std::shared_ptr<IMegaDummy> mega_dummy = injector.getInstance<IMegaDummy>();

    std::cout << "Memory: " << dummy.get() << ", value: " << dummy.get()->get_number() << std::endl;
    std::cout << "Memory: " << super_dummy.get() << ", value: " << super_dummy->get_number() << std::endl;
    std::cout << "Memory: " << mega_dummy.get() << ", value: " << mega_dummy.get()->get_number() << std::endl;

    std::cout << get_spreader_bar('-', 85) <<std::endl;

    // Example B: Get object and access to de function exported through the interface.

    std::vector<std::shared_ptr<IMegaDummy>> mega_dummy_vector;

    int n_items = 9;

    for(int i=0; i<n_items; i++)
    {
        mega_dummy_vector.push_back(injector.getInstance<IMegaDummy>());
    }

    for (int i=0; i<mega_dummy_vector.size(); i++)
    {
        std::cout << "Item " << i << ": Memory: " << mega_dummy_vector[i] << 
            ", accessing to internal dependency (ISuperDummy): " << 
            mega_dummy_vector[i].get()->get_super_dummy().get()->get_number() << std::endl;
    } 
}

int main(int argc, const char * argv[]) {

    example_with_several_dependencies_between_classes();

    return 0;
}