//
//  ThreadWithCallbacks.h
//  rayverb
//
//  Created by Reuben Thomas on 09/12/2013.
//
//

#ifndef __rayverb__ThreadWithCallbacks__
#define __rayverb__ThreadWithCallbacks__

#include "JuceHeader.h"

class ThreadWithListener:
public Thread,
public Timer
{
public:
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        virtual void threadStopped (ThreadWithListener * const) = 0;
    };
    
    ThreadWithListener (const String & threadName);
    
    void startThread();
    void startThread (int priority);
    
    void timerCallback();
    
    void addListener (Listener * listener);
    void removeListener (Listener * listener);
    
private:
    void threadStarted();
    
    ListenerList<Listener> listenerList;
    
    static const int timerInterval = 100;
};

#endif /* defined(__rayverb__ThreadWithCallbacks__) */