
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_MATERIALS_Iris_H
#define PBRT_MATERIALS_Iris_H

// materials/Iris.h*
//#include "materials/irisbxdf.h"
#include "pbrt.h"
#include "material.h"
#include "pbrt.h"
#include "material.h"
#include "geometry.h"
#include "microfacet.h"
#include "shape.h"
#include "spectrum.h"
#include "reflection.h"


//#include "materials/irisbxdf.h"

namespace pbrt {

// IrisMaterial Declarations
class IrisBXDFReflection : public MicrofacetReflection {
  public:
    // IrisBXDFReflection Public Methods
    IrisBXDFReflection(const Spectrum &R,
                         MicrofacetDistribution *distribution, Fresnel *fresnel, Float caustic_power)
        : MicrofacetReflection(R, distribution, fresnel),
          R(R),
          distribution(distribution),
          fresnel(fresnel),
          caustic_power(caustic_power)
           {}
    Spectrum f(const Vector3f &wo, const Vector3f &wi) const;
    Spectrum Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u,
                      Float *pdf, BxDFType *sampledType) const;
    Float Pdf(const Vector3f &wo, const Vector3f &wi) const;
    

  private:
    // IrisBXDFReflection Private Data
    const Spectrum R;
    const MicrofacetDistribution *distribution;
    const Fresnel *fresnel;
    const Float caustic_power ; 
};

class IrisMaterial : public Material {
  public:
    // IrisMaterial Public Methods
    IrisMaterial(const std::shared_ptr<Texture<Spectrum>> &kd,
                        const std::shared_ptr<Texture<Spectrum>> &ks,
                        const std::shared_ptr<Texture<Float>> &rough,
                        const std::shared_ptr<Texture<Spectrum>> &refl,
                        const std::shared_ptr<Texture<Spectrum>> &trans,
                        const std::shared_ptr<Texture<Float>> &bump,
                        Float caus, 
                        bool remap) {
        Kd = kd;
        Ks = ks;
        roughness = rough;
        reflect = refl;
        transmit = trans;
        bumpMap = bump;
        remapRoughness = remap;
        caustic = caus; 
    }
    void ComputeScatteringFunctions(SurfaceInteraction *si, MemoryArena &arena,
                                    TransportMode mode,
                                    bool allowMultipleLobes) const;

  private:
    // IrisMaterial Private Data
    std::shared_ptr<Texture<Spectrum>> Kd, Ks;
    std::shared_ptr<Texture<Float>> roughness;
    std::shared_ptr<Texture<Spectrum>> reflect, transmit;
    std::shared_ptr<Texture<Float>> bumpMap;
    Float caustic; 
    bool remapRoughness;
};

IrisMaterial *CreateIrisMaterial(const TextureParams &mp);

}  // namespace pbrt

#endif  // PBRT_MATERIALS_Iris_H
