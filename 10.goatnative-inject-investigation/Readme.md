# Investigation about: Autodesk/goatnative-inject repository

- [Autodesk/goatnative-inject](https://github.com/Autodesk/goatnative-inject)


## Example

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
