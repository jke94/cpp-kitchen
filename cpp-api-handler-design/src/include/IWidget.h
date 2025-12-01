#ifndef I_WIDGET_H
#define I_WIDGET_H

namespace internalApi
{
    class IWidget
    {
    public:
        virtual ~IWidget() = default;
        virtual void setNotificationCallback(void (*callback)()) = 0;
        virtual void notifyWakeUp() = 0;
        virtual void start() = 0;

    protected:
        IWidget() = default;
        IWidget(const IWidget&) = delete;
        IWidget& operator=(const IWidget&) = delete;
    };

} // namespace internalApi

#endif // I_WIDGET_H