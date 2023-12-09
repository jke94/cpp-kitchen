#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>

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

class IBasicDummy
{
    public:
        virtual int get_other_number() = 0;
};

class BasicDummy : public IBasicDummy
{
    public:
        int get_other_number() override
        {
            return 994;
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
        std::shared_ptr<IBasicDummy> basic_dummy_;

    public:
        SuperDummy(
            std::shared_ptr<IDummy> dummy, 
            std::shared_ptr<IBasicDummy> basic_dummy)
        {
            dummy_ = dummy;
            basic_dummy_ = basic_dummy;
        }

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

template<typename T>
class IReadImage
{
    public:
        virtual std::string read_image() = 0;
};

enum IMAGE_TYPE
{
    JPG = 0,
    TIFF = 1
};

class IReadJPGImage 
{

};

class ReadJPGImage : public IReadImage<IReadJPGImage>
{
    public:
        std::string read_image() override
        {
            return "Reading a JPG image...";
        }
};

class IReadTIFFImage 
{

};

class ReadTIFFImage : public IReadImage<IReadTIFFImage>
{
    public:
        std::string read_image() override
        {
            return "Reading a TIFF image...";
        }
};

class IServiceImageReader
{
    public:
        virtual std::string read_image(IMAGE_TYPE image_type, std::string& file_name) = 0;
};

class ServiceImageReader : public IServiceImageReader
{
    private:
        std::shared_ptr<IReadImage<IReadJPGImage>> read_jpg_image_;
        std::shared_ptr<IReadImage<IReadTIFFImage>> read_tiff_image_;

    public:
        ServiceImageReader(
            std::shared_ptr<IReadImage<IReadJPGImage>> read_jpg_image,
            std::shared_ptr<IReadImage<IReadTIFFImage>> read_tiff_image)
            {
                read_jpg_image_ = read_jpg_image;
                read_tiff_image_ = read_tiff_image;
            }

        std::string read_image(IMAGE_TYPE image_type, std::string& file_name) override
        {
            std::string message;

            switch (image_type)
            {
                case IMAGE_TYPE::JPG:
                    message = read_jpg_image_->read_image() + "'" + file_name + "'";
                    break;
                case IMAGE_TYPE::TIFF:
                    message = read_tiff_image_->read_image() + "'" + file_name + "'";
                    break;
                default:
                    message = "READ ERROR";
            }

            return message;
        }
};

void example_with_several_dependencies_between_classes()
{
    goatnative::Injector injector;

    // Building dependencies.

    // Dependencies to build example of classes that implements and extends interfaces.
    injector.registerClass<Dummy>();
    injector.registerInterface<IDummy,Dummy>();

    injector.registerClass<BasicDummy>();
    injector.registerInterface<IBasicDummy,BasicDummy>();

    injector.registerClass<SuperDummy,IDummy, IBasicDummy>();
    injector.registerInterface<ISuperDummy,SuperDummy>();

    injector.registerClass<MegaDummy,ISuperDummy>();
    injector.registerInterface<IMegaDummy,MegaDummy>();

    // Dependencies for service image reader.
    injector.registerClass<ReadJPGImage>();
    injector.registerClass<ReadTIFFImage>();
    injector.registerInterface<IReadImage<IReadJPGImage>,ReadJPGImage>();
    injector.registerInterface<IReadImage<IReadTIFFImage>,ReadTIFFImage>();

    injector.registerClass<ServiceImageReader, IReadImage<IReadJPGImage>, IReadImage<IReadTIFFImage>>();
    injector.registerInterface<IServiceImageReader, ServiceImageReader>();

    // Example A: Get object and access to de function exported through the interface.

    std::shared_ptr<IDummy> dummy = injector.getInstance<IDummy>();
    std::shared_ptr<IBasicDummy> basic_dummy = injector.getInstance<IBasicDummy>();
    std::shared_ptr<ISuperDummy> super_dummy = injector.getInstance<ISuperDummy>();
    std::shared_ptr<IMegaDummy> mega_dummy = injector.getInstance<IMegaDummy>();

    std::cout << "Memory: " << dummy.get() << ", value: " << dummy.get()->get_number() << std::endl;
    std::cout << "Memory: " << basic_dummy.get() << ", value: " << basic_dummy.get()->get_other_number() << std::endl;
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

    std::cout << get_spreader_bar('-', 85) <<std::endl;

    // Example C: Service to read images.

    std::shared_ptr<IServiceImageReader> service_image_reader_a = injector.getInstance<IServiceImageReader>();
    std::shared_ptr<IServiceImageReader> service_image_reader_b = injector.getInstance<IServiceImageReader>();

    std::string iron_maiden_file = "Iron_Maiden.jpg";
    std::string metallica_file = "Metallica.tiff";

    std::cout << "Memory: " << service_image_reader_a << ", DOING OPERATION: " << 
        service_image_reader_a.get()->read_image(IMAGE_TYPE::JPG, iron_maiden_file) << std::endl;
    
    std::cout << "Memory: " << service_image_reader_b << ", DOING OPERATION: " << 
        service_image_reader_b.get()->read_image(IMAGE_TYPE::TIFF, metallica_file) << std::endl;
}

int main(int argc, const char * argv[]) 
{
    example_with_several_dependencies_between_classes();

    return 0;
}