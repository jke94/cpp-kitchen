#ifndef PUBLIC_API_WRAPPER_H
#define PUBLIC_API_WRAPPER_H

#include "IPublicApiWrapper.h"

namespace internalApi
{
    class PublicApiWrapper final : public IPublicApiWrapper
    {
    public:
        void openHandler(HANDLER& handler, Result& result) override;
        void closeHandler(HANDLER& handler, Result& result) override;
        void setWidgetNotificationCallback(HANDLER& handler, Result& result, WidgetCallback callback) override;
        void startWidget(HANDLER& handler, Result& result) override;
    };
} // namespace publicApi

#endif // PUBLIC_API_WRAPPER_H