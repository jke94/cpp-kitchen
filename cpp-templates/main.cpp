#include <iostream>
#include <bitset>

// Template de clase con parámetros múltiples y por defecto
template <typename T, typename U, typename V = char>
class ClassTemplate
{
private:
    T var1;
    U var2;
    V var3;

public:
    ClassTemplate(T v1, U v2, V v3) : var1(v1), var2(v2), var3(v3) {}

    void print()
    {
        std::cout << "var1 = " << var1 << std::endl;
        std::cout << "var2 = " << var2 << std::endl;
        std::cout << "var3 = " << var3 << std::endl;
    }
};

int main()
{
    // // Objeto con int, double y char
    // ClassTemplate<int, double> obj1(5, 5.5, 'H');
    // std::cout << "Valores obj1" << std::endl;
    // obj1.print();

    // // Objeto con float, char y bool
    // ClassTemplate<float, char, bool> obj2(6.3f, 'm', false);
    // std::cout << std::boolalpha << "\nValores obj2" << std::endl;
    // obj2.print();

    unsigned short int numero{15}; // 15 en binario es 1111
    std::cout << std::bitset<16>(numero) << std::endl;

    std::bitset<128> bits("1111111111111111111111111111111111111111111111111111111111111"); // 1111 en decimal es 15
    std::cout << bits.to_string() << std::endl;
    std::cout << bits.to_ullong() << std::endl;
    // 2 305 843 009 213 693 951

    return 0;
}