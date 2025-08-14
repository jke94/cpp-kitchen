/**
 * @file How to use `toString` function after use `IToString` interface. This example requires `c++11` at least.
 * @author Javier Carracedo (@jke94 Github)
 */

#include <iostream>
#include <memory>
#include <ostream>
#include <string>

namespace testing
{
    class IToString
    {
    public:
        virtual ~IToString() = default;
        virtual std::string toString() const = 0;
    
    protected:
        IToString() = default;
        IToString(const IToString&) = default;
        IToString& operator=(const IToString&) = default;
        IToString(IToString&&) = default;
        IToString& operator=(IToString&&) = default;        
    };

    class IThing
    {
    public:
        virtual ~IThing() = default;
    
    protected:
        IThing() = default;
        IThing(const IThing&) = default;
        IThing& operator=(const IThing&) = default;
        IThing(IThing&&) = default;
        IThing& operator=(IThing&&) = default; 
    };

    class Thing final : public IThing, 
                        public IToString
    {
    public:
        Thing(std::string id);
        std::string toString() const override;

    private:
        std::string id_;
    };

    class SuperThing final : public IThing, 
                             public IToString
    {
    public:
        SuperThing(std::string id);
        std::string toString() const override;

    private:
        std::string id_;
    };

    class HyperThing final : public IThing
    {
    public:
        HyperThing(std::string id);

    private:
        std::string id_;
    };

}; // namespace testing

namespace str
{
    template<class T>
    std::string to_string(T* t);

}; // namespace str

using namespace testing;

int main()
{
    IThing *p_thing = new Thing("QWE765");
    std::shared_ptr<IThing> thingUniquePtr = std::make_shared<Thing>("RTE123");
    std::shared_ptr<IThing> superThingUniquePtr = std::make_shared<SuperThing>("SUPER321");
    std::shared_ptr<IThing> hyperThingUniquePtr = std::make_shared<HyperThing>("HYPER678");

    std::cout << str::to_string(p_thing) << std::endl;
    std::cout << str::to_string(thingUniquePtr.get()) << std::endl;
    std::cout << str::to_string(superThingUniquePtr.get()) << std::endl;
    std::cout << str::to_string(hyperThingUniquePtr.get()) << std::endl; // NOTE: HyperThing does not implement IToString

    delete p_thing;

    return 0;
}

namespace testing
{
    Thing::Thing(std::string id) : id_(id)
    {

    }

    std::string Thing::toString() const
    {
        return "Thing(id=" + id_ + ")";
    }

    SuperThing::SuperThing(std::string id) : id_(id)
    {

    }

    std::string SuperThing::toString() const
    {
        return "SuperThing(id=" + id_ + ")";
    }

    HyperThing::HyperThing(std::string id) : id_(id)
    {

    }

}; // namespace testing

namespace str
{
    template<class T>
    std::string to_string(T* t)
    {
        const IToString* item = dynamic_cast<IToString*>(t);

        if(!item)
        {
            const std::string errorMsg = "[ERROR] The item does not implemented 'IToString' contract.";

            return errorMsg;
        }

        return item->toString();
    }

}; // namespace str