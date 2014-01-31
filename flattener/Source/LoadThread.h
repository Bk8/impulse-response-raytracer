//
//  LoadThread.h
//  rayverb
//
//  Created by Reuben Thomas on 18/01/2014.
//
//

#ifndef __rayverb__LoadThread__
#define __rayverb__LoadThread__

#include "JuceHeader.h"
#include "ThreadWithCallbacks.h"
#include "Adapters.h"
#include "Scene.h"

class JSONLoadThread:
public ThreadWithListener
{
public:
    JSONLoadThread (const File & f);
    JSONLoadThread();
    virtual ~JSONLoadThread();
    
    virtual void run();
    
    virtual void setFile (const File & f);
    
    virtual var getVar() const;
    
    std::vector<RayTrace> getRaytrace() const;
    
private:
    File file;
    var thisVar;
};

#endif /* defined(__rayverb__LoadThread__) */
