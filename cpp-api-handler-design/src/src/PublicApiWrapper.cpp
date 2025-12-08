#include "PublicApiWrapper.h"
#include "PublicApi.h"
#include "Widget.h"

namespace internalApi
{
    void PublicApiWrapper::openHandler(HANDLER& handler, Result& result)
    {
        Widget* widget = new Widget();

        if(!widget)
        {
            result = Result::FAILURE;
            return;
        }

        handler = reinterpret_cast<HANDLER>(widget);
        result = Result::SUCCESS;
    }

    void PublicApiWrapper::closeHandler(HANDLER& handler, Result& result)
    {
        if(handler == nullptr)
        {
            result = Result::FAILURE;
            return;
        }
        
        IWidget* widget = reinterpret_cast<Widget*>(handler);
        delete widget;

        handler = nullptr;
        result = Result::SUCCESS;
    }

    void PublicApiWrapper::setWidgetNotificationCallback(HANDLER& handler, Result& result, WidgetCallback callback)
    {
        if (handler == nullptr)
        {
            result = Result::FAILURE;
            return;
        }

        IWidget* widget = reinterpret_cast<Widget*>(handler);

        if (!widget)
        {
            result = Result::FAILURE;
            return;
        }

        widget->setNotificationCallback(callback);
        result = Result::SUCCESS;
    }

    void PublicApiWrapper::startWidget(HANDLER& handler, Result& result)
    {
        if(handler == nullptr)
        {
            result = Result::FAILURE;
            return;
        }

        IWidget* widget = reinterpret_cast<Widget*>(handler);
        
        if(!widget)
        {
            result = Result::FAILURE;
            return;
        }

        widget->start();
        result = Result::SUCCESS;
    }
    
} // namespace publicApi