//
//  Material.h
//  ultraray
//
//  Created by Reuben Thomas on 06/11/2013.
//  Copyright (c) 2013 Reuben Thomas. All rights reserved.
//

#ifndef ultraray_Material_h
#define ultraray_Material_h

#include "Numerical.h"
#include <array>

namespace Rayverb {

class Material {
public:
    Real diffuse;
    Real specular;
    
    Material(const Real diffuse, const Real specular):
    diffuse(diffuse), specular(specular) {}
    
    Material(): Material(0, 0) {}
};

const int BANDS = 3;
typedef array<Material, BANDS> Surface;
typedef array<Real, BANDS> VolumeCollection;
    
}

#endif