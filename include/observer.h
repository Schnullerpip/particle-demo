#ifndef jule_observer_h
#define  jule_observer_h

//event data
struct WASD_key_input {
    int w;
    int a;
    int s;
    int d;
};


template<typename Data>
class Observable;

template<typename Data>
class Observer
{
    friend Observable<Data>;

    virtual void callback(Data data) = 0;
public:
    void register_at(Observable<Data>* observable)
    {
        observable->observers.push_back(this);
    }

};

template<typename Data>
class Observable
{
    friend Observer<Data>;
    std::vector<Observer<Data>*> observers;

    virtual Data assemble_data() = 0;

public:
    void notify()
    {
        for(Observer<Data> *o : observers)
            o->callback(assemble_data());
    }
};



#endif