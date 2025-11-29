#include "PublicApi.h"
#include "Widget.h"

using namespace internalApi;

namespace publicApi
{
    void openHandler(HANDLER& handler, Result& result)
    {
        if(handler != nullptr)
        {
            result = Result::FAILURE_HANLDER_IS_NOT_NULL;
            return;
        }

        Widget* widget = new Widget();

        if(!widget)
        {
            result = Result::FAILURE;
            return;
        }

        handler = reinterpret_cast<HANDLER>(widget);
        result = Result::SUCCESS;
    }

    void closeHandler(HANDLER& handler, Result& result)
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

    void setWidgetNotificationCallback(HANDLER handler, Result& result, void (*callback)())
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

    void startWidget(HANDLER handler, Result& result)
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

} // namespace apiHandlerDesign