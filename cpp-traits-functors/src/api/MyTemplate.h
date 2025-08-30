#ifndef MY_TEMPLATE_H
#define MY_TEMPLATE_H

namespace cppTraitsAndFunctorsApi
{
    /**
     * @brief Generic template class that uses traits to perform operations.
     * 
     * @tparam T The type of the value to be processed.
     * @tparam Traits A traits class that provides a static method `doSomething`.
     */
    template <typename T, typename Traits>
    class MyTemplate 
    {
    public:
        void execute(const T& valor) 
        {
            Traits::doSomething(valor);
        }
    };

} // namespace cppTraitsAndFunctorsApi

#endif // MY_TEMPLATE_H
