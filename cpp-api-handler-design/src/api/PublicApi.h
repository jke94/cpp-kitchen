#ifndef PUBLIC_API_H
#define PUBLIC_API_H

namespace publicApi
{
    enum class Result : int
    {
        SUCCESS = 0,
        FAILURE = 1,
        FAILURE_HANLDER_IS_NOT_NULL = 2,
    };

    using HANDLER = void*;
    using WidgetCallback = void (*)();

    void openHandler(HANDLER& handler, Result& result);
    
    void closeHandler(HANDLER& handler, Result& result);

    void setWidgetNotificationCallback(HANDLER handler, Result& result, WidgetCallback callback);
    
    void startWidget(HANDLER handler, Result& result);

} // namespace publicApi

#endif // PUBLIC_API_H