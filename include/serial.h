#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <string>
#include <vector>
#include <stdint.h>

namespace GAMEBOY
{
    enum class SerialEventType
    {
        SERIAL_OUT
    };

    class SerialEventSubscriber
    {
    public:
        virtual void receive(uint8_t data) = 0;
    };

    class SerialEventSupervisor
    {
    private:
        static SerialEventSupervisor* instance;
        std::vector<SerialEventSubscriber*> subscribers;
        SerialEventSupervisor() = default;
        ~SerialEventSupervisor() = default;
    public:
        static SerialEventSupervisor& getInstance()
        {
            if (!instance) {
                instance = new SerialEventSupervisor();
            }
            return *instance;
        }
        void subscribe(SerialEventType event, SerialEventSubscriber* subscriber);
        void publish(SerialEventType event, uint8_t data);
    };
};

#endif
