#include "Event.h"

Event::Event(EventType t) : type(t)
{
}

static void default_callback(std::unique_ptr<Event>)
{
}
EventCallbackTable::EventCallbackTable() : callbacks(Event::EventCount)
{
    for (auto &i : callbacks)
    {
        i = default_callback;
    }
}

void EventCallbackTable::callback(std::unique_ptr<Event> ev)
{
    callbacks[ev->type](std::move(ev));
}
void EventCallbackTable::set_callback(Event::EventType t, Event::EventCallbackType c)
{
    callbacks[t] = c;
}

// AnalyzingFileEvent
AnalyzingFileEvent::AnalyzingFileEvent(const std::string &fn) : Event(OnAnalyzingFile), filename(fn)
{
}