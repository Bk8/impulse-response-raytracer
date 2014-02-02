//
//  Adapters.h
//  rayverb
//
//  Created by Reuben Thomas on 24/01/2014.
//
//

#ifndef rayverb_Adapters_h
#define rayverb_Adapters_h

#include "Numerical.h"
#include "Scene.h"
#include "JuceHeader.h"
#include <functional>

//  toVar

template <typename IN>
struct toVar: public std::unary_function <IN, var>
{
    var operator() (const IN & in) const
    {
        return var();
    }
};

template <typename T>
var getVar (const T & t)
{
    return toVar <T> () (t);
}

template <typename T> struct toVar <Vec3 <T> >
{
    var operator() (const Vec3 <T> & in) const
    {
        DynamicObject * obj = new DynamicObject();
        obj->setProperty ("x", in.x);
        obj->setProperty ("y", in.y);
        obj->setProperty ("z", in.z);
        return var (obj);
    }
};

template <> struct toVar <VolumeCollection>
{
    var operator() (const VolumeCollection & in) const
    {
        var ret;
        for (auto i = in.begin(); i != in.end(); ++i)
        {
            ret.append (*i);
        }
        return ret;
    }
};

template <> struct toVar <Impulse>
{
    var operator() (const Impulse & in) const
    {
        DynamicObject * obj = new DynamicObject();
        obj->setProperty ("p", in.samplePosition);
        obj->setProperty ("a", getVar (in.amplitude));
        return var (obj);
    }
};

template <> struct toVar <RayTrace>
{
    var operator() (const RayTrace & in) const
    {
        DynamicObject * obj = new DynamicObject();
        obj->setProperty ("d", getVar (in.first));
        
        var impulseVector;
        for (auto i = in.second.begin(); i != in.second.end(); ++i)
        {
            impulseVector.append (getVar (*i));
        }
        
        obj->setProperty ("i", impulseVector);
        
        return var (obj);
    }
};

template <typename T> struct toVar <std::vector <T> >
{
    var operator() (const std::vector <T> & in) const
    {
        var ret;
        for (auto i = in.begin(); i != in.end(); ++i)
        {
            ret.append (getVar (*i));
        }
        return ret;
    }
};

template <> struct toVar <Speaker>
{
    var operator() (const Speaker & in) const
    {
        DynamicObject * obj = new DynamicObject();
        obj->setProperty ("d", getVar (in.direction));
        obj->setProperty ("s", in.directionality);
        return var (obj);
    }
};


//  fromVar

template <typename RET>
struct fromVar: public std::unary_function <var, RET>
{
    RET operator() (const var & in) const
    {
        return RET();
    }
};

template <typename T> struct fromVar <Vec3 <T> >
{
    Vec3 <T> operator() (const var & in) const
    {
        return Vec3 <T> (in.getProperty("x", 0),
                         in.getProperty("y", 0),
                         in.getProperty("z", 0));
    }
};

template <> struct fromVar <VolumeCollection>
{
    VolumeCollection operator() (const var & in) const
    {
        VolumeCollection ret;
        for (uint64_t i = 0; i != ret.size(); ++i)
        {
            ret[i] = in[i];
        }
        return ret;
    }
};

template <> struct fromVar <Impulse>
{
    Impulse operator() (const var & in) const
    {
        return Impulse (in.getProperty ("p", 0),
                        fromVar <VolumeCollection> () (in.getProperty("a", 0)));
    }
};

template <> struct fromVar <RayTrace>
{
    RayTrace operator() (const var & in) const
    {
        Vec direction = fromVar <Vec> () (in.getProperty ("d", 0));
        
        var iv = in.getProperty("i", 0);
        std::vector <Impulse> impulseVector (iv.size());
        for (uint64_t i = 0; i != iv.size(); ++i)
        {
            impulseVector[i] = fromVar <Impulse> () (iv[i]);
        }
        return RayTrace (direction, impulseVector);
    }
};

template <typename T> struct fromVar <std::vector <T> >
{
    std::vector <T> operator() (const var & in) const
    {
        std::vector <T> ret (in.size());
        for (uint64_t i = 0; i != in.size(); ++i)
        {
            ret[i] = fromVar <T> () (in[i]);
        }
        return ret;
    }
};

template <> struct fromVar <Speaker>
{
    Speaker operator() (const var & in) const
    {
        return Speaker (fromVar <Vec> () (in.getProperty ("d", 0)),
                        in.getProperty ("s", 0));
        
    }
};

#endif