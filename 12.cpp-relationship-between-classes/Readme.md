# Relationships between classes applied in C++ language with UML.

## A. Relationships between classes

1. Association (knows a)
2. Composition (has a)
3. Aggregation (has a)
4. Dependency (uses a)
5. Inheritance (is a)

Examples using [mermaid](https://mermaid.js.org/intro/) Markdown.

```mermaid
classDiagram

classA --> classB : Association
classC --* classD : Composition
classE --o classF : Aggregation
classG ..> classH : Dependency
classJ --|> classK : Inheritance
```

### A.1. Association

- Un objeto es consciente de otro. Contiene un puntero o referencia a otro objeto (isntancia).

- Code example:

```
class Engine
{

};

class Car
{
    public:
        void start_engine(Engine* engine){}
};

int main()
{
    Engine* engine = new Engine();
    Car car;
    car.start_engine(engine);
    
    return 0;
}
```

- Diagram exmaple

```mermaid
classDiagram
Car --> Engine 

```

### A.2. Composition

- Code example:

```
// TODO
```

### A.3. Aggregation

- Code example:

```
// TODO
```

### A.4. Dependency

- Code example:

```
// TODO
```

### A.5. Inheritance

- Code example:

```
// TODO
```

## Useful links with information.

- [creately: UML Class Diagram Relationships Explained with Examples](https://creately.com/guides/class-diagram-relationships/)

- [blog.visual-paradigm: ¿Cuáles Son Los Seis Tipos De Relaciones En Los Diagramas De Clases UML?](https://blog.visual-paradigm.com/es/what-are-the-six-types-of-relationships-in-uml-class-diagrams/)