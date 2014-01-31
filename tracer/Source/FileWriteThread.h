//
//  FileWriteThread.h
//  rayverb
//
//  Created by Reuben Thomas on 12/01/2014.
//
//

#ifndef __rayverb__FileWriteThread__
#define __rayverb__FileWriteThread__

#include "JuceHeader.h"

class JSONWriteThread:
public Thread
{
public:
    JSONWriteThread (const File & f, const var & v);
    JSONWriteThread();
    virtual ~JSONWriteThread();
    
    virtual void run();
    
    virtual void setVar (const var & v);
    virtual void setFile (const File & f);
    
    virtual void setParameters (const File & f, const var & v);
private:
    
    File file;
    var thisVar;
};

#endif /* defined(__rayverb__FileWriteThread__) */