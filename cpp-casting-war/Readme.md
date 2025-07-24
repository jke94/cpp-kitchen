# cpp-casting-war

How C++ casting can be used to isolate implementation and offer the client to program logic against the interface.

```
classDiagram
    class IDevice {
        <<interface>>
        +~IDevice()
    }
    class IPrimiumDevice {
        <<interface>>
        +specialFeature()
        +setAuthentication(user, passwordA, passwordB)
        +~IPrimiumDevice()
    }
    class IUltraDevice {
        <<interface>>
        +ultraFeature()
        +setAuthentication(token)
        +~IUltraDevice()
    }
    class PrimiumDevice {
        +specialFeature()
        +setAuthentication(user, passwordA, passwordB)
        +~PrimiumDevice()
    }
    class UltraDevice {
        +ultraFeature()
        +setAuthentication(token)
        +~UltraDevice()
    }

    IPrimiumDevice --|> IDevice
    IUltraDevice --|> IDevice
    PrimiumDevice --|> IPrimiumDevice
    UltraDevice --|> IUltraDevice
```