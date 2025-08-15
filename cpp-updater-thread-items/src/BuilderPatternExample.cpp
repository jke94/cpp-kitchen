/**
 * Example about builder pattern in C++ language.
 * How to build:
 *      g++ src/BuilderPatternExample.cpp -std=c++14 -o builderPatternExample
 */

#include <iostream>
#include <memory>
#include <string>

namespace builderPatternExample
{
    class IThing
    {
    public:
        virtual ~IThing() = default;
        virtual void displayInfo() const = 0;

    protected:
        IThing() = default;
        IThing(const IThing&) = default;
        IThing& operator=(const IThing&) = default;
        IThing(IThing&&) = default;
        IThing& operator=(IThing&&) = default;        
    };

    class Thing final : public IThing
    {
    public:
        Thing(const std::string& name, int version);
        void displayInfo() const override;
    private:
        std::string name_;
        int version_;
    };

    class IThingBuilder
    {
    public:
        virtual ~IThingBuilder() = default;
        virtual IThingBuilder& withName(const std::string& nombre) = 0;
        virtual IThingBuilder& withVersion(int version) = 0;
        virtual std::unique_ptr<IThing> build() const = 0;
        
    protected:
        IThingBuilder() = default;
        IThingBuilder(const IThingBuilder&) = default;
        IThingBuilder& operator=(const IThingBuilder&) = default;
        IThingBuilder(IThingBuilder&&) = default;
        IThingBuilder& operator=(IThingBuilder&&) = default;  
    };

    class ThingBuilder final : public IThingBuilder
    {
    public:
        IThingBuilder& withName(const std::string& nombre);
        IThingBuilder& withVersion(int version);
        std::unique_ptr<IThing> build() const override;

    private:
        std::string name_ = "None";
        int version_ = 1;
    };

} // namespace builderPatternExample

using namespace builderPatternExample;

int main()
{
    // Builder declaration.
    ThingBuilder thingBuilder;

    // Use of builder to build object.
    std::unique_ptr<IThing> thing = thingBuilder.withName("TestThing")
                .withVersion(2)
                .build();

    // Using it.
    thing->displayInfo();

    return 0;
}

namespace builderPatternExample
{
    Thing::Thing(const std::string& name, int version) : 
    name_(name),
    version_(version) 
    {
    }

    void Thing::displayInfo() const
    {
        std::cout << "Thing Name: " << name_ << ", Version: " << version_ << std::endl;
    }

    IThingBuilder& ThingBuilder::withName(const std::string& nombre) 
    {
        name_ = nombre;
        return *this;
    }

    IThingBuilder& ThingBuilder::withVersion(int version) 
    {
        version_ = version;
        return *this;
    }

    std::unique_ptr<IThing> ThingBuilder::build() const 
    {
        return std::make_unique<Thing>(name_, version_);
    }

} // namespace builderPatternExample
