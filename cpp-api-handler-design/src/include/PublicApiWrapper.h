#ifndef PUBLIC_API_WRAPPER_H
#define PUBLIC_API_WRAPPER_H

#include "IPublicApiWrapper.h"

namespace internalApi
{
    class PublicApiWrapper final : public IPublicApiWrapper
    {
    public:
        void openHandler(publicApi::HANDLER& handler, publicApi::Result& result) override;
        void closeHandler(publicApi::HANDLER& handler, publicApi::Result& result) override;
        void setWidgetNotificationCallback(publicApi::HANDLER handler, publicApi::Result& result, publicApi::WidgetCallback callback) override;
        void startWidget(publicApi::HANDLER handler, publicApi::Result& result) override;
    };
} // namespace publicApi

#endif // PUBLIC_API_WRAPPER_H