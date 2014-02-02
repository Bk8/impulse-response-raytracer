//
//  Scene.cpp
//  ultraray
//
//  Created by Reuben Thomas on 07/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#include "Scene.h"
#include "Biquad.h"

#include <iostream>

vector<Reflection> trace(const vector<Primitive *> & primitive,
                         const Ray & ray,
                         const Real distance,
                         const VolumeCollection & volume) {
    const Real VOLUME_THRESHOLD = 0.001;
    
    bool allLower = true;
    for (auto i = volume.begin(); i != volume.end(); ++i)
        if (*i > VOLUME_THRESHOLD) {
            allLower = false;
            break;
        }
    
    if (allLower) return vector<Reflection>();
    
    const Maybe<const Primitive *> MAYBE_CLOSEST = ray.closest(primitive);
    if (MAYBE_CLOSEST.isNothing()) return vector<Reflection>();
    
    const Primitive * CLOSEST = MAYBE_CLOSEST.fromJust();
    
    const Ray REFLECTED_RAY = CLOSEST->reflect(ray);
    const Vec INTERSECTION = REFLECTED_RAY.position;
    
    const Real NEW_DISTANCE = distance + (ray.position - INTERSECTION).length();
    
    VolumeCollection newVolume;
    auto i = newVolume.begin();
    auto j = CLOSEST->material.begin();
    auto k = volume.begin();
    for (;  i != newVolume.end() &&
            j != CLOSEST->material.end() &&
            k != volume.end(); ++i, ++j, ++k) {
        *i = *k * j->specular;
    }
    
    const Reflection REFLECTION(CLOSEST->material,
                                INTERSECTION,
                                CLOSEST->findNormal(INTERSECTION),
                                NEW_DISTANCE,
                                newVolume,
                                CLOSEST->isSource);
    
    vector<Reflection> ret(1, REFLECTION);
    
    if (!CLOSEST->isSource) {
        const vector<Reflection> TRACE = trace(primitive,
                                               REFLECTED_RAY,
                                               NEW_DISTANCE,
                                               newVolume);
                
        ret.insert(ret.end(), TRACE.begin(), TRACE.end());
    }
    
    return ret;
}

vector<Reflection> itrace(const vector<Primitive *> & primitive,
                          const Ray & ray,
                          const Real distance,
                          const VolumeCollection & volume) {
    const Real VOLUME_THRESHOLD = 0.001;
    
    Ray currentRay = ray;
    Real currentDistance = distance;
    VolumeCollection currentVolume = volume;
    vector<Reflection> ret;
    
    while (true) {
        bool allLower = true;
        for (auto i = currentVolume.begin(); i != currentVolume.end(); ++i) {
            if (*i > VOLUME_THRESHOLD) {
                allLower = false;
                break;
            }
        }
        if (allLower) break;
        
        const Maybe<const Primitive *> MAYBE_CLOSEST = currentRay.closest(primitive);
        if (MAYBE_CLOSEST.isNothing()) break;
        
        const Primitive * CLOSEST = MAYBE_CLOSEST.fromJust();
        
        const Ray REFLECTED_RAY = CLOSEST->reflect(currentRay);
        const Vec INTERSECTION = REFLECTED_RAY.position;
        
        const Real NEW_DISTANCE = currentDistance + (currentRay.position - INTERSECTION).length();
        
        VolumeCollection newVolume;
        auto i = newVolume.begin();
        auto j = CLOSEST->material.begin();
        auto k = currentVolume.begin();
        for (;
             i != newVolume.end() &&
             j != CLOSEST->material.end() &&
             k != currentVolume.end();
             ++i, ++j, ++k) {
            *i = *k * j->specular;
        }
        
        const Reflection REFLECTION(CLOSEST->material,
                                    INTERSECTION,
                                    CLOSEST->findNormal(INTERSECTION),
                                    NEW_DISTANCE,
                                    newVolume,
                                    CLOSEST->isSource);
        
//        vector<Reflection> ret(1, REFLECTION);
        ret.push_back(REFLECTION);
        
        if (CLOSEST->isSource) break;   //  or do some special handling
        
        currentRay = REFLECTED_RAY;
        currentDistance = NEW_DISTANCE;
        currentVolume = newVolume;
    }
    
    return ret;
}

vector<Primitive *> getSources(const vector<Primitive *> & primitive) {
    vector<Primitive *> ret;
    for (auto i = primitive.begin(); i != primitive.end(); ++i)
        if ((*i)->isSource) ret.push_back(*i);
    return ret;
}

vector<Impulse> toImpulsesAllSources(const vector<Primitive *> & primitive,
                                     const Reflection & reflection) {
    vector<Impulse> ret(primitive.size());
    auto i = primitive.begin();
    auto j = ret.begin();
    for (;  i != primitive.end() &&
            j != ret.end(); ++i, ++j)
        *j = Impulse((Sphere *)*i, reflection);
    return ret;
}

vector<Impulse> traceToImpulse(const vector<Primitive *> & primitive,
                               const Ray & ray) {
    VolumeCollection startingVolumes;
    startingVolumes.fill(1);
    const vector<Reflection> REFLECTION = itrace(primitive, ray, 0, startingVolumes);
    const vector<Primitive *> SOURCES = getSources(primitive);
    
    vector<Impulse> ret;
    for (auto i = REFLECTION.begin(); i != REFLECTION.end(); ++i) {
        const vector<Impulse> IMPULSE = toImpulsesAllSources(SOURCES, *i);
        for (auto j = IMPULSE.begin(); j != IMPULSE.end(); ++j)
            ret.push_back(*j);
    }
    return ret;
}

RayTrace traceDirection(const vector<Primitive *> & primitive,
                        const Ray & ray) {
    return pair<Vec, vector<Impulse> >(ray.direction, traceToImpulse(primitive, ray));
}

vector<RayTrace> Scene::traceMic(const vector<Primitive *> & primitive,
                                 Mic mic,
                                 const int rays,
                                 double & ratioDone) {
    vector<RayTrace> ret;
    for (int i = 0; i != rays; ++i) {
        ratioDone = i / (rays - 1.0);
        
        RayTrace r = traceDirection(primitive, mic.createRay());
        if (! r.second.empty())
            ret.push_back(r);
    }
    return ret;
}

const uint64_t lastSample(const vector<Impulse> & impulse, const Real sampleRate) {
    uint64_t last = 0;
    for (auto i = impulse.begin(); i != impulse.end(); ++i)
        last = max(last, i->getPositionInSamples(sampleRate));
    return last;
}

const uint64_t lastSample(const vector<RayTrace> & raytrace, const Real sampleRate) {
    uint64_t last = 0;
    for (auto i = raytrace.begin(); i != raytrace.end(); ++i) {
        last = max(last, lastSample(i->second, sampleRate));
    }
    return last;
}

vector<Real> createChannel(const uint64_t samples,
                           const Real sampleRate,
                           const vector<RayTrace> & raytrace,
                           const Speaker & speaker,
                           const uint64_t band) {
    vector<Real> channel(samples, 0);
    
    for (auto i = raytrace.begin(); i != raytrace.end(); ++i) {
        const float ATTENUATION = speaker.attenuation(i->first);
        for (auto j = i->second.begin(); j != i->second.end(); ++j)
        {
            uint64_t samplePos = j->getPositionInSamples (sampleRate);
            if (samplePos < samples)
                channel[samplePos] += ATTENUATION * j->amplitude[band];
        }
    }
    return channel;
}

vector<vector<Real> > createBand(const uint64_t samples,
                                 const Real sampleRate,
                                 const vector<RayTrace> & raytrace,
                                 const vector<Speaker> & speaker,
                                 const uint64_t band) {
    vector<vector<Real> > sampleData;
    for (auto i = speaker.begin(); i != speaker.end(); ++i)
        sampleData.push_back(createChannel(samples, sampleRate, raytrace, *i, band));
    return sampleData;
}

void simpleFilterBand(vector<vector<Real> > & sampleData, const Real sampleRate, const Real cutoff) {
    for (auto i = sampleData.begin(); i != sampleData.end(); ++i) {
        const Real FREQ = 200;
        const Real x = exp(-2.0 * M_PI * FREQ / sampleRate);
        const Real a0 = 1.0 - x;
        
        Real out = 0;
        for (auto j = i->begin(); j != i->end(); ++j) {
            *j = out += a0 * (*j - out);
        }
    }
}

void filterBand(vector<vector<Real> > & sampleData, const Biquad & biquad) {
    for (auto i = sampleData.begin(); i != sampleData.end(); ++i) {
        Biquad thisBiquad = biquad;
        for (auto j = i->begin(); j != i->end(); ++j)
            *j = thisBiquad.process(*j);
    }
}

float halfPowerPoint(const float F0, const float Q, const bool BELOW) {
    const float EXTRA = (F0 / (2 * Q));
    return BELOW ? F0 - EXTRA : F0 + EXTRA;
}

vector<vector<Real> > flattenBands(const vector<vector<vector<Real> > > & sampleData) {
    const uint64_t CHANNELS = sampleData.front().size();
    const uint64_t SAMPLES = sampleData.front().front().size();
    vector<vector<Real> > outData (CHANNELS, vector<Real>(SAMPLES, 0));
    for (uint64_t i = 0; i != BANDS; ++i)
        for (uint64_t j = 0; j != CHANNELS; ++j)
            for (uint64_t k = 0; k != SAMPLES; ++k)
                outData[j][k] += sampleData[i][j][k];
    return outData;
}

template <typename T, typename S>
struct subtract: public binary_function<T, T, T> {
    T operator() (const T & a, const T & b) {
        T ret (a.size());
        std::transform (a.begin(), a.end(), b.begin(), ret.begin(), std::minus<S>());
        return ret;
    }
};

vector<vector<Real> > nestedSubtract (const vector<vector<Real> > & a,
                                      const vector<vector<Real> > & b) {
    vector<vector<Real> > ret (a.size());
    std::transform (a.begin(), a.end(), b.begin(), ret.begin(), subtract<vector<Real>, Real>());
    return ret;
}

vector<vector<Real> > createSampleData (const uint64_t samples,
                                        const Real sampleRate,
                                        const vector<RayTrace> & raytrace,
                                        const vector<Speaker> & speaker,
                                        const array<Real, BANDS - 1> midpoint) {
    vector<vector<vector<Real> > > sampleData;
    for (int i = 0; i != BANDS; ++i) {
        vector<vector<Real> > thisBand = createBand(samples, sampleRate, raytrace, speaker, i);
        
        if (i != BANDS - 1) simpleFilterBand(thisBand, sampleRate, midpoint[i]);

        if (i != 0) {
            vector<vector<Real> > thisBandCopy = thisBand;
            simpleFilterBand(thisBandCopy, sampleRate, midpoint[i - 1]);
            sampleData.push_back(nestedSubtract(thisBand, thisBandCopy));
        } else {
            sampleData.push_back(thisBand);
        }

    }
    
    vector<vector<Real> > flattened = flattenBands(sampleData);
    
//    return flattened;
    
    vector<vector<Real> > flattenedCopy = flattened;
    simpleFilterBand(flattenedCopy, sampleRate, 0);
    return nestedSubtract(flattened, flattenedCopy);
}

//vector<vector<Real> > createSampleData(const uint64_t samples,
//                                       const vector<RayTrace> & raytrace,
//                                       const vector<Speaker> & speaker) {
//    vector<vector<vector<Real> > > sampleData;
//    for (int i = 0; i != BANDS; ++i) {
//        vector<vector<Real> > thisBand = createBand(samples, raytrace, speaker, i);
//        
//        const float Q = 0.75;
//        const float MID_BAND = 1000;
//        const float BELOW_HP_POINT = halfPowerPoint(MID_BAND, Q, true);
//        const float ABOVE_HP_POINT = halfPowerPoint(MID_BAND, Q, false);
//        
//        Biquad bq;
//        
//        switch (i) {
//            case 0: bq = lopass   (SAMPLE_RATE, BELOW_HP_POINT, Q); break;
//            case 1: bq = bandpass (SAMPLE_RATE, MID_BAND,       Q); break;
//            case 2: bq = hipass   (SAMPLE_RATE, ABOVE_HP_POINT, Q); break;
//        }
//        
//        filterBand(thisBand, bq);
//        
//        sampleData.push_back(thisBand);
//    }
//    
//    vector<vector<Real> > flattened = flattenBands(sampleData);
//    
//    Biquad bq = hipass(SAMPLE_RATE, 20, 0.5);
//    
//    filterBand(flattened, bq);
//    
//    return flattened;
//}

Real maximumAmplitude(const vector<vector<Real> > & sampleData) {
    Real amp = 0;
    for (auto i = sampleData.begin(); i != sampleData.end(); ++i)
        for (auto j = i->begin(); j != i->end(); ++j)
            amp = max(amp, fabs(*j));
    return amp;
}

void applyGain(vector<Real> & sampleData, const Real coeff) {
    for (auto i = sampleData.begin(); i != sampleData.end(); ++i)
        *i *= coeff;
}

void applyGain(vector<vector<Real> > & sampleData, const Real coeff) {
    for (auto i = sampleData.begin(); i != sampleData.end(); ++i)
        applyGain(*i, coeff);
}

void normalize(vector<vector<Real> > & sampleData) {
    applyGain(sampleData, 0.99 / maximumAmplitude(sampleData));
}

vector<Real> interleave(const vector<vector<Real> > & sampleData) {
    const uint64_t CHANNELS = sampleData.size();
    const uint64_t SAMPLES = sampleData.front().size();
    
    vector<Real> output(CHANNELS * SAMPLES);
    auto k = output.begin();
    for (uint64_t i = 0; i != SAMPLES; ++i)
        for (uint64_t j = 0; j != CHANNELS; ++j)
            (*k++) = sampleData[j][i];
    return output;
}

vector<vector<Real> > Scene::getChannelSamples(const vector<RayTrace> & raytrace,
                                               const vector<Speaker> & speaker,
                                               const Real sampleRate,
                                               const array<Real, BANDS - 1> midpoint) {
    const uint64_t MAX_SAMPLE = 1000000;
    const uint64_t SAMPLES = min(MAX_SAMPLE, lastSample(raytrace, sampleRate));
    
    vector<vector<Real> > sampleData = createSampleData(SAMPLES, sampleRate, raytrace, speaker, midpoint);
    normalize(sampleData);
    return sampleData;
}

Scene::~Scene() {
    clearPrimitives();
}

void Scene::addPrimitive(Rayverb::Primitive *p) {
    primitive.push_back(p);
}

void Scene::clearPrimitives() {
    for (auto i = primitive.begin(); i != primitive.end(); ++i) {
        delete *i;
    }
    primitive.clear();
}

void Scene::setMic(const Rayverb::Mic &m) {
    mic = m;
}

vector<RayTrace> Scene::trace(const int rays, double &ratioDone) const {
    return traceMic(primitive, mic, rays, ratioDone);
}