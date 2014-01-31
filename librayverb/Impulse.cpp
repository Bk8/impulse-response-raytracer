//
//  Impulse.cpp
//  ultraray
//
//  Created by Reuben Thomas on 07/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#include "Impulse.h"

//Impulse::Impulse(const var & v) {
//    samplePosition = (int64)v.getProperty("p", 0);
//    amplitude = varToVolumeCollection (v.getProperty("a", 0));
//}

Impulse::Impulse(const uint64_t samplePosition,
                 const VolumeCollection & amplitude):
samplePosition(samplePosition),
amplitude(amplitude) {}

Impulse::Impulse(const Sphere * source, const Reflection & reflection)  {
    samplePosition = SAMPLES_PER_UNIT * (reflection.distance +
                                         (source->origin -
                                          reflection.position).length() -
                                         source->radius);
    
    if (reflection.direct)
    {
        auto i = amplitude.begin();
        auto j = reflection.volume.begin();
        for (;
             i != amplitude.end() &&
             j != reflection.volume.end();
             ++i, ++j)
        {
            *i = *j;
        }
    }
    else
    {
        auto i = amplitude.begin();
        auto j = reflection.volume.begin();
        auto k = reflection.surface.begin();
        for (;
             i != amplitude.end() &&
             j != reflection.volume.end() &&
             k != reflection.surface.end();
             ++i, ++j, ++k)
        {
            *i = *j * Vec::dot(reflection.normal,
                               (source->origin -
                                reflection.position).normalized()) * (k->diffuse);
        }
    }
}

Impulse::Impulse(): Impulse(0, {0, 0, 0}) {}

//var Impulse::getVar() const
//{
//    DynamicObject * obj = new DynamicObject();
//    obj->setProperty ("p", (int64)samplePosition);
//    obj->setProperty ("a", volumeCollectionToVar (amplitude));
//    return var (obj);
//}

//var Impulse::volumeCollectionToVar (const VolumeCollection & vc)
//{
//    var ret;
//    for (auto i = vc.begin(); i != vc.end(); ++i)
//    {
//        ret.append (*i);
//    }
//    return ret;
//}

//VolumeCollection Impulse::varToVolumeCollection(const juce::var &vc)
//{
//    VolumeCollection ret;
//    for (uint64_t i = 0; i != ret.size(); ++i)
//    {
//        ret[i] = vc[i];
//    }
//    return ret;
//}
