#ifndef PUBLIC_API_H
#define PUBLIC_API_H

#ifdef _WIN32
#define API_EXPORT extern "C" __declspec(dllexport)
#else
#define API_EXPORT extern "C"
#endif

enum Result
{
    SUCCESS = 0,
    FAILURE = 1
};

using HANDLER = void*;
using WidgetCallback = void (*)();

API_EXPORT void openHandler(HANDLER& handler, Result& result);

API_EXPORT void closeHandler(HANDLER& handler, Result& result);

API_EXPORT void setWidgetNotificationCallback(HANDLER& handler, Result& result, WidgetCallback callback);

API_EXPORT void startWidget(HANDLER& handler, Result& result);

#endif // PUBLIC_API_H