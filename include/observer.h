#ifndef jule_observer_h
#define  jule_observer_h

#include<functional>

#define event_callback(type, function)\
void callback(type arg0) override { function(arg0); }

//forward declaration
template<typename Data>
class EventSource;

/*
interface for an event listener.
needs to define a callback, that will be invoked, whenever the event is fired. 
the callback will get a specific Data argument, that is assembled by the Event trigger,
so when overriding this method it can safely be assumed, that the relevant data is passed
over the data argument.
*/
template<typename Data>
class Observer
{
friend EventSource<Data>;
    virtual void callback(Data) = 0;
};

/*
interface for an event trigger.
needs to define an assemble_data method, that independently generates the neccessary data
for an event listener (Observer) to react on;
*/
template<typename Data>
class EventSource
{
public:
    typedef void (*delegate)(Data);
protected:
    friend Observer<Data>;

    std::vector<Observer<Data>*> observers;
    std::vector<delegate> delegates;

    /*should generate and return a Data object, that can be passed (by value!) to any listener's/observer's callback routine*/
    virtual Data assemble_data(Data*) = 0;

public:
    void add_listener(Observer<Data> *obs)  { observers.push_back(obs);}
    void add_delegate(delegate d)           { delegates.push_back(d);  }

    void trigger()
    {
        Data data = assemble_data(nullptr);
        for(Observer<Data> *o : observers)
            o->callback(data);
        for(delegate &del : delegates)
            (del)(data);

    }
};
#endif