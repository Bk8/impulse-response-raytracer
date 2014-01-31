//
//  Mic.cpp
//  ultraray
//
//  Created by Reuben Thomas on 09/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#include "Mic.h"

#include <random>

Mic::Mic(const Vec & location):
location(location) {}

Mic::Mic(): Mic(Vec(0, 0, 0)) {}

Vec unitVec() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static uniform_real_distribution<Real> r;
    static uniform_real_distribution<Real> phi;
    
    const Real R = r(gen);
    const Real PHI = phi(gen);
    const Real Z2 = sqrt(1 - (R * R));
    return Vec(Z2 * cos(PHI), Z2 * sin(PHI), R);
}

Ray Mic::createRay() const {
    return Ray(location, unitVec());
}