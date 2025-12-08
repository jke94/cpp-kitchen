#ifndef I_PUBLIC_API_WRAPPER_H
#define I_PUBLIC_API_WRAPPER_H

#include "PublicApi.h"

namespace internalApi
{
    /**
     * @brief Interface for PublicApiWrapper to allow mocking in unit tests.
     */
    class IPublicApiWrapper
    {
    public:
        virtual ~IPublicApiWrapper() = default;
        virtual void openHandler(HANDLER& handler, Result& result) = 0;
        virtual void closeHandler(HANDLER& handler, Result& result) = 0;
        virtual void setWidgetNotificationCallback(HANDLER& handler, Result& result, WidgetCallback callback) = 0;
        virtual void startWidget(HANDLER& handler, Result& result) = 0;

    protected:
        IPublicApiWrapper() = default;
        IPublicApiWrapper(const IPublicApiWrapper&) = delete;
        IPublicApiWrapper& operator=(const IPublicApiWrapper&) = delete;
    };

} // namespace publicApi

#endif // I_PUBLIC_API_WRAPPER_H