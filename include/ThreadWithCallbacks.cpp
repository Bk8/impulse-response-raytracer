//
//  ThreadWithCallbacks.cpp
//  rayverb
//
//  Created by Reuben Thomas on 09/12/2013.
//
//

#include "ThreadWithCallbacks.h"

ThreadWithListener::ThreadWithListener (const String & threadName):
Thread (threadName)
{
    
}

void ThreadWithListener::threadStarted()
{    
    startTimer (timerInterval);
}

void ThreadWithListener::startThread()
{
    Thread::startThread();
    
    threadStarted();
}

void ThreadWithListener::startThread (int priority)
{
    Thread::startThread (priority);
    
    threadStarted();
}

void ThreadWithListener::timerCallback()
{
    if (! isThreadRunning())
    {
        stopTimer();
        
        listenerList.call (&Listener::threadStopped, this);
    }
}

void ThreadWithListener::addListener (Listener * listener)
{
    listenerList.add (listener);
}

void ThreadWithListener::removeListener (Listener * listener)
{
    listenerList.remove (listener);
}