# cpp-IoC-and-DI

- IoC (Inversión de Control): En lugar de que un objeto cree o busque sus dependencias, el control sobre esas dependencias se invierte y se les proporciona desde fuera.

- DI (Dependency Injection): Es una manera específica de lograr IoC. Las dependencias se “inyectan” en una clase (por constructor, método o propiedad).


## RAII

*Resource Acquisition Is Initialization* (Adquisición de Recursos es Inicialización). Es un principio fundamental en C++ para gestionar recursos de manera segura y automática. RAII consiste en asociar la vida útil de un recurso (memoria, archivo, socket, mutex, etc.) al ciclo de vida de un objeto. Cuando el objeto se crea, adquiere el recurso; y cuando el objeto se destruye (fuera de alcance), libera automáticamente ese recurso.


- _Sin RAII (mala práctica)_

```
void sinRAII() 
{
    int* ptr = new int(42);
    // Si hay una excepción antes del delete, hay fuga
    delete ptr;
}
```

- Con RAII (buena práctica con `std::unique_ptr`)

```
void conRAII() 
{
    std::unique_ptr<int> ptr = std::make_unique<int>(42);
    // ptr se destruye automáticamente al salir del scope
}
```


## Build and run samples

```
g++ ./src/source-code-without-IoC-and-DI.cpp -o ./source-code-without-IoC-and-DI
```

```
g++ ./src/source-code-with-IoC-and-DI.cpp -o ./source-code-with-IoC-and-DI
```