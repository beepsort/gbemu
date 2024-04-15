#include "gameboy/serial.h"

GAMEBOY::SerialEventSupervisor* GAMEBOY::SerialEventSupervisor::instance = nullptr;

void GAMEBOY::SerialEventSupervisor::subscribe(SerialEventType event, SerialEventSubscriber* subscriber)
{
    subscribers.push_back(subscriber);
}
void GAMEBOY::SerialEventSupervisor::publish(SerialEventType event, uint8_t data)
{
    for (auto it=subscribers.begin(); it!=subscribers.end(); it++)
    {
        SerialEventSubscriber* subscriber = *it;
        subscriber->receive(data);
    }
}
