#ifndef TOOL_EVENT_H
#define TOOL_EVENT_H

#include <memory>
#include <string>
#include <vector>
/*! \brief Event

    the base class of all event class
*/
class Event
{
  public:
    using EventCallbackType = void (*)(std::unique_ptr<Event>);

  public:
    enum EventType
    {
        OnAnalyzingFile,
        EventCount,
    };

  public:
    Event(EventType t);

  public:
    EventType type;
};
/*! \brief EventCallbackTable

    used to dispatch events
*/
class EventCallbackTable
{
  public:
    EventCallbackTable();

    /*! \brief call the callback function of emitted event
        \param[in] ev emitted event
    */
    void callback(std::unique_ptr<Event> ev);
    /*! \brief set callback function of the specific event type
        \param[in] t event's type
        \param[in] c callback function
    */
    void set_callback(Event::EventType t, Event::EventCallbackType c);

  private:
    std::vector<Event::EventCallbackType> callbacks;
};
/*! \brief emitted when analyzer start to analyze a file
 */
class AnalyzingFileEvent : public Event
{
  public:
    AnalyzingFileEvent(const std::string &fn);

  public:
    std::string filename; //! the name of file currently analyzing
};
#endif