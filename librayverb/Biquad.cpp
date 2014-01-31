//
//  Biquad.cpp
//  impulseConverter
//
//  Created by Reuben Thomas on 02/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#include "Biquad.h"

#include <cmath>

Biquad::Biquad(const Real a0,
                 const Real a1,
                 const Real a2,
                 const Real b0,
                 const Real b1,
                 const Real b2):
a0(a0),
a1(a1),
a2(a2),
b0(b0),
b1(b1),
b2(b2),
i0(0),
i1(0),
i2(0),
o1(0),
o2(0) {}

//y[n] = (b0/a0)*x[n] + (b1/a0)*x[n-1] + (b2/a0)*x[n-2]
//                    - (a1/a0)*y[n-1] - (a2/a0)*y[n-2]

Real Biquad::process(const Real IN) {
    i2 = i1;
    i1 = i0;
    i0 = IN;
    
    const Real o0 = ((b0 / a0) * i0 +
                     (b1 / a0) * i1 +
                     (b2 / a0) * i2 -
                     (a1 / a0) * o1 -
                     (a2 / a0) * o2);
    
    o2 = o1;
    o1 = o0;
    
    return o0;
}

Real calcA(const Real GAIN) {
    return sqrt(pow(10, GAIN / 20));
}

Real calcW0(const Real FS, const Real F0) {
    return (2 * M_PI * F0) / FS;
}

Real calcAlpha(const Real SIN_W0, const Real Q) {
    return SIN_W0 / (2 * Q);
}

Biquad createFilter(const Real FS,
                     const Real F0,
                     const Real Q,
                     Biquad (* f) (const Real, const Real, const Real,
                                    const Real, const Real, const Real)) {
    const Real W0 = calcW0(FS, F0);
    const Real SIN_W0 = sin(W0);
    return f(calcA(0), W0, cos(W0), SIN_W0, calcAlpha(SIN_W0, Q), Q);
}

Biquad lopass(const Real A,
               const Real W0,
               const Real COS_W0,
               const Real SIN_W0,
               const Real ALPHA,
               const Real BETA) {
    return Biquad(1 + ALPHA,
                   -2 * COS_W0,
                   1 - ALPHA,
                   (1 - COS_W0) / 2,
                   1 - COS_W0,
                   (1 - COS_W0) / 2);
}

Biquad lopass(const Real FS, const Real F0, const Real Q) {
    return createFilter(FS, F0, Q, lopass);
}

Biquad hipass(const Real A,
               const Real W0,
               const Real COS_W0,
               const Real SIN_W0,
               const Real ALPHA,
               const Real BETA) {
    return Biquad(1 + ALPHA,
                   -2 * COS_W0,
                   1 - ALPHA,
                   (1 + COS_W0) / 2,
                   -1 - COS_W0,
                   (1 + COS_W0) / 2);
}

Biquad hipass(const Real FS, const Real F0, const Real Q) {
    return createFilter(FS, F0, Q, hipass);
}

Biquad bandpass(const Real A,
                 const Real W0,
                 const Real COS_W0,
                 const Real SIN_W0,
                 const Real ALPHA,
                 const Real BETA) {
    return Biquad(1 + ALPHA,
                   -2 * COS_W0,
                   1 - ALPHA,
                   ALPHA,
                   0,
                   -ALPHA);
}

Biquad bandpass(const Real FS, const Real F0, const Real Q) {
    return createFilter(FS, F0, Q, bandpass);
}