//
//  Impulse.cpp
//  ultraray
//
//  Created by Reuben Thomas on 07/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#include "Impulse.h"

Impulse::Impulse(const Real samplePosition,
                 const VolumeCollection & amplitude):
samplePosition(samplePosition),
amplitude(amplitude) {}

Impulse::Impulse(const Sphere * source, const Reflection & reflection)  {
    samplePosition = SECONDS_PER_METRE * (reflection.distance +
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

uint64_t Impulse::getPositionInSamples (const Real sampleRate) const
{
    return round (sampleRate * samplePosition);
}

Real Impulse::getPositionInSeconds() const
{
    return samplePosition;
}