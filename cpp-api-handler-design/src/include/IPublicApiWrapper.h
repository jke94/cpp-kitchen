#ifndef I_PUBLIC_API_WRAPPER_H
#define I_PUBLIC_API_WRAPPER_H

/**
 * @brief Foward declarations for publicApi types to avoid circular dependency.
 */
namespace publicApi
{
    enum class Result : int;
    using HANDLER = void*;
    using WidgetCallback = void (*)();

}; // namespace publicApi

namespace internalApi
{
    /**
     * @brief Interface for PublicApiWrapper to allow mocking in unit tests.
     */
    class IPublicApiWrapper
    {
    public:
        virtual ~IPublicApiWrapper() = default;
        virtual void openHandler(publicApi::HANDLER& handler, publicApi::Result& result) = 0;
        virtual void closeHandler(publicApi::HANDLER& handler, publicApi::Result& result) = 0;
        virtual void setWidgetNotificationCallback(publicApi::HANDLER handler, publicApi::Result& result, publicApi::WidgetCallback callback) = 0;
        virtual void startWidget(publicApi::HANDLER handler, publicApi::Result& result) = 0;

    protected:
        IPublicApiWrapper() = default;
        IPublicApiWrapper(const IPublicApiWrapper&) = delete;
        IPublicApiWrapper& operator=(const IPublicApiWrapper&) = delete;
    };

} // namespace publicApi

#endif // I_PUBLIC_API_WRAPPER_H