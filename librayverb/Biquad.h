//
//  Biquad.h
//  impulseConverter
//
//  Created by Reuben Thomas on 02/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#ifndef __impulseConverter__Biquad__
#define __impulseConverter__Biquad__

#include "Numerical.h"

class Biquad {
    Real a0, a1, a2;
    Real b0, b1, b2;
    
    Real i0, i1, i2;
    Real o1, o2;
public:
    Biquad(const Real a0,
           const Real a1,
           const Real a2,
           const Real b0,
           const Real b1,
           const Real b2);
    Biquad(): Biquad(1, 0, 0, 1, 0, 0) {}
    virtual ~Biquad() {}
    virtual Real process(const Real IN);
};

Biquad lopass      (const Real FS, const Real F0, const Real Q);
Biquad hipass      (const Real FS, const Real F0, const Real Q);
Biquad bandpass    (const Real FS, const Real F0, const Real Q);

#endif /* defined(__impulseConverter__Biquad__) */