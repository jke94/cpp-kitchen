# ...existing code...
import ctypes
import platform
import os
import threading
from ctypes import c_void_p, c_int, POINTER, byref, CFUNCTYPE

EXTENSION_MAP = {
    "Windows": "dll",
    "Linux": "so",
    "Darwin": "dylib",
}

# Callback type: void callback(HANDLER)
WidgetCallback = CFUNCTYPE(None, c_void_p)

# Event to signal notification reception
_notification_event = threading.Event()

class ExampleApiLibWrapper:
    def __init__(self, lib_path: str | None = None):
        # cargar la librería (ruta por defecto relativa al script si no se pasa)
        if lib_path is None:
            system = platform.system()
            ext = EXTENSION_MAP.get(system)
            if not ext:
                raise RuntimeError(f"Unsupported OS: {system}")
            project_root = os.path.abspath(os.path.dirname(__file__))
            lib_path = os.path.join(project_root, "../../", f"exampleApiLib.{ext}")

        if not os.path.exists(lib_path):
            raise FileNotFoundError(f"Library not found at {lib_path}")

        # librería privada
        self._lib = ctypes.CDLL(lib_path)

        # mantener referencia al callback C wrapper para evitar GC
        self._cb_ref = None

        # configurar prototipos
        self._lib.openHandler.restype = None
        self._lib.openHandler.argtypes = [POINTER(c_void_p), POINTER(c_int)]

        self._lib.closeHandler.restype = None
        self._lib.closeHandler.argtypes = [POINTER(c_void_p), POINTER(c_int)]

        self._lib.setWidgetNotificationCallback.restype = None
        self._lib.setWidgetNotificationCallback.argtypes = [POINTER(c_void_p), POINTER(c_int), WidgetCallback]

        self._lib.startWidget.restype = None
        self._lib.startWidget.argtypes = [POINTER(c_void_p), POINTER(c_int)]

    # métodos equivalentes a los helpers previos
    def open(self):
        handler = c_void_p(None)
        result = c_int(1)  # FAILURE por defecto
        self._lib.openHandler(byref(handler), byref(result))
        return handler, result.value

    def close(self, handler: c_void_p):
        result = c_int(1)
        self._lib.closeHandler(byref(handler), byref(result))
        return result.value

    def set_callback(self, handler: c_void_p, cb):
        # almacenar la referencia al wrapper C para que no la libere el GC
        self._cb_ref = WidgetCallback(cb)
        result = c_int(1)
        self._lib.setWidgetNotificationCallback(byref(handler), byref(result), self._cb_ref)
        return result.value

    def start(self, handler: c_void_p):
        result = c_int(1)
        self._lib.startWidget(byref(handler), byref(result))
        return result.value

# Callback function to handle notifications
def _on_widget_notification(handler):
    # handler puede ser int o c_void_p; obtener el valor y formatear en hex
    try:
        addr = handler if isinstance(handler, int) else handler.value
    except Exception:
        addr = handler
    try:
        print(f"Widget notification received. handler={hex(addr)}")
    except Exception:
        print(f"Widget notification received. handler={addr}")
    _notification_event.set()

def main():
    wrapper = ExampleApiLibWrapper()  # carga la librería del path por defecto
    handler, res = wrapper.open()
    if res != 0:
        print("openHandler failed")
        return 1
    print(f"Handler opened: {hex(handler.value)}")

    if wrapper.set_callback(handler, _on_widget_notification) != 0:
        print("setWidgetNotificationCallback failed")
        wrapper.close(handler)
        return 1

    if wrapper.start(handler) != 0:
        print("startWidget failed")
        wrapper.close(handler)
        return 1

    # Wait for notification (max 10 seconds)
    if not _notification_event.wait(timeout=10):
        print("Timeout: no notification received")
    
    _notification_event.clear()

    if wrapper.close(handler) != 0:
        print("closeHandler failed")
        return 1

    print("Handler closed")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())