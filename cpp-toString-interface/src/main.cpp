/**
 * @file Function templates + toString interface.
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

}; // namespace testing

namespace toString
{
    template<class T>
    std::ostream &operator<<(std::ostream &os, T* thing);

}; // namespace toString

using namespace testing;
using namespace toString;

int main()
{
    IThing *p_thing = new Thing("QWE765");
    std::unique_ptr<IThing> thingUniquePtr = std::make_unique<Thing>("RTE123");
    std::unique_ptr<IThing> superThingUniquePtr = std::make_unique<SuperThing>("SUPER321");

    std::cout << thingUniquePtr.get() << std::endl;
    std::cout << superThingUniquePtr.get() << std::endl;
    std::cout << p_thing << std::endl;

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

}; // namespace testing

namespace toString
{
    template<class T>
    std::ostream &operator<<(std::ostream &os, T* thing)
    {
        IToString* item = dynamic_cast<IToString*>(thing);

        if(!item)
        {
            // TODO: Throw exception.
            return os;
        }

        os << item->toString();

        return os;
    }

}; // namespace toString