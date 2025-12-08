#ifndef WIDGET_H
#define WIDGET_H

#include "IWidget.h"

namespace internalApi
{
    class Widget final : public IWidget
    {
    public:
        Widget() = default;
        ~Widget() override = default;

        void setNotificationCallback(void (*callback)(void*)) override;
        void notifyWakeUp() override;
        void start() override;

    private:
        void (*m_callback)(void*) = nullptr;
    };

} // namespace internalApi

#endif // WIDGET_H