#ifndef PUBLIC_API_H
#define PUBLIC_API_H

#ifdef __cplusplus
    #define API_EXTERN_C extern "C"
#else
    #define API_EXTERN_C
#endif

/**
 * Define API_BUILD when building the shared library (DLL/.so/.dylib)
 * Consumers must NOT define API_BUILD.
 */

#if defined(_WIN32) || defined(_WIN64)
    #if defined(API_BUILD)
        #define API_EXPORT API_EXTERN_C __declspec(dllexport)
    #else
        #define API_EXPORT API_EXTERN_C __declspec(dllimport)
    #endif
#elif defined(__GNUC__) || defined(__clang__)
    #if defined(API_BUILD)
        #define API_EXPORT API_EXTERN_C __attribute__((visibility("default")))
    #else
        #define API_EXPORT API_EXTERN_C
    #endif
#else
    #error "Unsupported platform — only Windows, Linux and macOS are supported"
#endif


using HANDLER = void*;
using WidgetCallback = void (*)(HANDLER);

enum Result
{
    SUCCESS = 0,
    FAILURE = 1
};

API_EXPORT void openHandler(HANDLER& handler, Result& result);

API_EXPORT void closeHandler(HANDLER& handler, Result& result);

API_EXPORT void setWidgetNotificationCallback(HANDLER& handler, Result& result, WidgetCallback callback);

API_EXPORT void startWidget(HANDLER& handler, Result& result);

#endif // PUBLIC_API_H