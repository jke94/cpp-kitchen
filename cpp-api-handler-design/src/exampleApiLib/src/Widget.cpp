#include "Widget.h"

#include <chrono>
#include <random>
#include <thread>

#include "PublicApi.h"

namespace internalApi
{
    void Widget::notifyWakeUp()
    {
    }

    void Widget::setNotificationCallback(void (*callback)(void*))
    {
        m_callback = callback;
    }

    void Widget::start()
    {
        // Copy callback to avoid using `this` inside the thread after deletion.
        void (*cb)(void*) = m_callback;
        HANDLER hanlder = reinterpret_cast<HANDLER>(this); // capture current handler value

        std::thread([cb, hanlder]()
        {
            static thread_local std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<int> dist(100, 1000); // ms
            std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng)));

            if (cb)
            {
                cb(hanlder); // notify with the actual handler
            }

        }).detach();
    }

} // namespace internalApi