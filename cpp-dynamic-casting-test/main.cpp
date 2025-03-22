#include <iostream>
#include <memory>

// ----------------- API ----------------------

class ISubsytemA
{
public:
    virtual ~ISubsytemA() {};
};

class ILetter
{
    virtual ISubsytemA* getSubsytemA() = 0;

public:
    virtual ~ILetter() {};
};

ILetter* createLetter();

// --------------------------------------------

// -------------- IMPLEMENTATION --------------

class IBaseSubsytem
{
public:
    ~IBaseSubsytem() = default;
};

class SubsytemA : public IBaseSubsytem
{
public:
    SubsytemA() = default;
    ~SubsytemA() = default;
};

class IBaseLetter : public ILetter
{
public:
    virtual ISubsytemA* getSubsytemA() = 0;
    virtual ~IBaseLetter() {};
};

class Alpha : public IBaseLetter
{
private:
    ISubsytemA* subsystemA_;
public:
    Alpha() = default;
    ~Alpha() = default;
    ISubsytemA* getSubsytemA() override
    {
        return subsystemA_;
    };
};

class Betha : public IBaseLetter
{
private:
    ISubsytemA* subsystemA_;    
public:
    Betha() = default;
    ~Betha() = default;
    ISubsytemA* getSubsytemA() override
    {
        return subsystemA_;
    };
};

void processingItem(ILetter* letter)
{
    if(dynamic_cast<Alpha*>(letter))
    {
        Alpha* alpha = dynamic_cast<Alpha*>(letter);
        alpha->getSubsytemA();
        std::cout << "You are Alpha: " << letter << std::endl;
    }
    else
    {
        std::cout << "Ups! You are Betha: " << letter << std::endl;
    }
}

int counter_ = 0;

ILetter* createLetter()
{
    // Bussiness logic.
    
    if(counter_ < 2)
    {
        counter_++;
        return new Alpha();
    }
    
    counter_++;
    return new Betha();
}

// --------------------------------------------------

// ----------------- CLIENT CODE ---------------------

int main()
{
    std::shared_ptr<ILetter> itemA = std::shared_ptr<ILetter>(createLetter());
    std::shared_ptr<ILetter> itemB = std::shared_ptr<ILetter>(createLetter());
    std::shared_ptr<ILetter> itemC = std::shared_ptr<ILetter>(createLetter());

    processingItem(itemA.get());
    processingItem(itemB.get());
    processingItem(itemC.get());

    return 0;
}