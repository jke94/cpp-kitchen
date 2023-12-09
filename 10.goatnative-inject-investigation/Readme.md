# Investigation about: Autodesk/goatnative-inject repository

- [Autodesk/goatnative-inject](https://github.com/Autodesk/goatnative-inject)


## A. Examples

### A.1. Example with several levels of dependencies.

```mermaid
classDiagram

class IDummy{
    <<interface>>
}

class IBasicDummy{
    <<interface>>
}

class ISuperDummy {
    <<interface>>
}

class IMegaDummy{
    <<interface>>
}

MegaDummy --|> IMegaDummy : EXTENDS
SuperDummy --|> ISuperDummy : EXTENDS
BasicDummy --|> IBasicDummy : EXTENDS
Dummy --|> IDummy : EXTENDS

SuperDummy ..> IDummy : USE
SuperDummy ..> IBasicDummy : USE
MegaDummy ..> ISuperDummy : USE

```

### A.2. Example with several levels of dependencies and templates.

```mermaid
classDiagram

class IReadImage~T~{
    <<interface>>
}

class IReadJPGImage{
    <<interface>>
}

class IReadTIFFImage{
    <<interface>>
}

class IServiceImageReader{
    <<interface>>
}

IReadJPGImage --|> IReadImage : EXTENDS
IReadTIFFImage --|> IReadImage : EXTENDS

ReadJPGImage --|> IReadImage~IReadJPGImage~ : EXTENDS
ReadTIFFImage --|> IReadImage~IReadJPGImage~ : EXTENDS

ServiceImageReader --|> IServiceImageReader : EXTENDS
ServiceImageReader ..> IReadImage~IReadJPGImage~ : USE
ServiceImageReader ..> IReadImage~IReadJPGImage~ : USE
```

## B. Useful links to design graphs based on Mermaid syntax and IoC concepts.

- [Mermaid - Class diagrams](https://mermaid.js.org/syntax/classDiagram.html)
- [Mermaid - Online designer](https://mermaid.live)
- [UML Class Diagram Relationships Explained with Examples](https://creately.com/guides/class-diagram-relationships/)
- [Dependency Inversion Implies Interfaces Are Owned by High-level Modules](https://mikhail.io/2016/05/dependency-inversion-implies-interfaces-are-owned-by-high-level-modules/)