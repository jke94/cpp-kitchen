/**
 *  DYNAMIC CASTING.
 */

#include <iostream>
#include <random>

// ---------------------------------------------------------------
// --------- API CODE available to be used by the client ---------
// ---------------------------------------------------------------

class IDummy
{
protected:
    virtual ~IDummy() {};

public:
    virtual void saySomething() = 0;
};

IDummy* createDummy();
bool destroyDummy(IDummy* dummy);

// ---------------------------------------------------------------

// ---------------------------------------------------------------
// ---------------- INTERNAL IMPLEMENTATION ----------------------
// ---------------------------------------------------------------

enum class Type
{
    WTF = -1,
    TYPE_A = 0,
    TYPE_B = 1,
    TYPE_C = 2
};

class DummyA : public IDummy
{
public:
    DummyA();
    ~DummyA();
    void saySomething() override;
};

DummyA::DummyA(){}

DummyA::~DummyA(){}

void DummyA::saySomething()
{
    std::cout << "I am Dummy A! " << std::endl;
}

class DummyB : public IDummy
{
public:
    DummyB();
    ~DummyB();
    void saySomething() override;
};

DummyB::DummyB(){}

DummyB::~DummyB(){}

void DummyB::saySomething()
{
    std::cout << "I am Dummy B! " << std::endl;
}

class DummyDefault : public IDummy
{
public:
    DummyDefault();
    ~DummyDefault();
    void saySomething() override;
};

DummyDefault::DummyDefault(){}

DummyDefault::~DummyDefault(){}

void DummyDefault::saySomething()
{
    std::cout << "I am Dummy 'default'! I have not been implemented yet." << std::endl;
}

class IDummyFactory
{
protected:
    virtual ~IDummyFactory(){};

public:
    virtual IDummy* createDummy() = 0;
    virtual bool destroyDummy(IDummy* dummy) = 0;
};

class DummyFactory : public IDummyFactory
{
public:
    DummyFactory();
    ~DummyFactory();
    IDummy* createDummy();
    bool destroyDummy(IDummy* dummy);

private:
    Type generateRandomType();
    template <typename T>
    bool destroy(IDummy* dummy, const std::string& name);
};

DummyFactory::DummyFactory(){}

DummyFactory::~DummyFactory(){}

IDummy* DummyFactory::createDummy()
{
    IDummy* dummy = nullptr;

    // NOTE: Substitute by a real business ruleset.
    Type type = generateRandomType();

    switch (type)
    {
    case Type::TYPE_A:
        dummy = new DummyA();
        break;
    case Type::TYPE_B:
        dummy = new DummyB();
        break;    
    case Type::TYPE_C:
        dummy = new DummyDefault();
        break;        
    default:
        dummy = new DummyDefault();
        break;
    }

    return dummy;
}

Type DummyFactory::generateRandomType()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    // Distribution for values 0, 1 and 2.
    std::uniform_int_distribution<> dis(0, 2);

    int randomValue = dis(gen);
    Type type = static_cast<Type>(randomValue);

    return type;    
}

template <typename T>
bool DummyFactory::destroy(IDummy* dummy, const std::string& name)
{
    if (auto* d = dynamic_cast<T*>(dummy))
    {
        std::cout << name << " (" << dummy << ") is going to be destroyed." << std::endl;
        delete d;
        return true;
    }

    return false;
}

bool DummyFactory::destroyDummy(IDummy* dummy)
{
    bool destroyResult = destroy<DummyA>(dummy, "Dummy A") ||
        destroy<DummyB>(dummy, "Dummy B")  ||
        destroy<DummyDefault>(dummy, "Dummy 'default'");

    return destroyResult;
}

IDummy* createDummy()
{
    DummyFactory dummyFactory;
    return dummyFactory.createDummy();
}

bool destroyDummy(IDummy* dummy)
{
    DummyFactory dummyFactory;
    bool destroyResult = dummyFactory.destroyDummy(dummy);

    return destroyResult;
}

// ---------------------------------------------------------------

int main()
{
    IDummy* dummyInstance = createDummy();
    
    dummyInstance->saySomething();

    bool destroyResult = destroyDummy(dummyInstance);
    std::cout << "Destroy result: " << (destroyResult ? "true" : "false") << std::endl;

    return 0;
}