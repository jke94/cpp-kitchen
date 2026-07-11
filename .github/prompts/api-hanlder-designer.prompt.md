---
agent: 'agent'
description: 'Define una API en C++ para construir una librería con las condiciones especificadas en el prompt.'
---

# Objetivo

Define una API en C++ para construir una librería con las siguientes condiciones.

## API Arquitectura

- Diseño de definición que permita P/Invoke desde C#
- Sin STL en la frontera ABI.
- Diseño basado en handler (`using Handler = void*`).
- No quiero utilizar JSON o XML para intercambiar información. Se utilizará un diseño de structs o enumerados.
- La API permitirá obtener información através del handler.
- La API permitirá subscribirse a a eventos de la librería a través del handler.
- Cada handler, internamente tendrá asociado una implementación de una clase. La cual, permitirá acceder a los datos internamente.
- TODAS las funciones de la API devuelven `void`. El valor de finalización de la función se pasa por referencia y será un enumerado con estados. Ejempl:

```cpp
enum class
{ 
  OK = 0,
  Error = 1
};
```

- Para evitar romper ABI cuando la librería crezca, añadir desde el inicio:

- TODOS los structs públicos han de comenzar con `StructSize`:

```cpp
struct DeviceInfo
{
    uint32_t StructSize;

    const char* IpAddress;

    const char* Model;

    const char* SerialNumber;

    const char* FirmwareVersion;
};
```

- Para permitir extender estructuras en versiones futuras sin romper binarios existentes:

```cpp
struct EventData
{
    uint32_t StructSize;

    EventType EventType;

    const void* Payload;
};
```

- `extern "C"` en toda la API exportada.
- Tipos de tamaño fijo (uint32_t, uint64_t, etc.).
- Nunca exponer clases C++.
- Nunca exponer referencias C++ ni excepciones.
- Callbacks usando punteros a función C.
- Todas las cadenas como const char*.
- El ownership de memoria debe estar perfectamente definido.
- Versionado explícito de la API.
- El cliente será responsable de crear y destruir el handler. La librería no debe crear ni destruir handlers. Por tanto, la creación y destrucción de handlers será responsabilidad del cliente. La librería solo proporcionará funciones para crear y destruir handlers, pero no realizará estas operaciones por sí misma.

## Coding-style

- Usa PascalCase como 'naming-convection'

## API use cases

- La API permite conectarse a dispositivos, dado una IP. Previa creación del handler.
- LA API expondrá un método para crear hanlder y eliminar recursos asociados al hanlder.

## Observación de diseño

Para una API que va a crecer, suele ser recomendable separar el concepto de handler del de dispositivo conectado:

```
Handler
 └── DeviceSession (implementación interna)

DeviceSession
 ├── Conexión
 ├── Estado
 ├── Callbacks
 └── Caché de datos
```

De esta forma cada Handler representa una sesión independiente y puede gestionar un dispositivo distinto simultáneamente, manteniendo una ABI estable para futuras versiones.

## Patrón interno recomendado

Internamente mantener exactamente esta relación:

```cpp
Handler (void*)
    │
    ▼
DeviceSession
    │
    ├── ConnectionManager
    ├── DeviceCache
    ├── EventDispatcher
    ├── DeviceInfo
    ├── ConnectionState
    └── CallbackRegistry
```

donde:

```cpp
using Handler = DeviceSession*;
```