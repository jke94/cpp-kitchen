#include "PublicApiWrapper.h"

using namespace internalApi;

void openHandler(HANDLER& handler, Result& result)
{
    PublicApiWrapper apiWrapper;
    apiWrapper.openHandler(handler, result);
}

void closeHandler(HANDLER& handler, Result& result)
{
    PublicApiWrapper apiWrapper;
    apiWrapper.closeHandler(handler, result);
}

void setWidgetNotificationCallback(HANDLER& handler, Result& result, WidgetCallback callback)
{
    PublicApiWrapper apiWrapper;
    apiWrapper.setWidgetNotificationCallback(handler, result, callback);
}

void startWidget(HANDLER& handler, Result& result)
{
    PublicApiWrapper apiWrapper;
    apiWrapper.startWidget(handler, result);
}