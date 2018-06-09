
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


// materials/Iris.cpp*
//#include "materials/irisbxdf.h"
#include "materials/iris.h"
#include "paramset.h"
#include "reflection.h"
#include "interaction.h"
#include "texture.h"
#include "interaction.h"

#include "reflection.h"
#include "spectrum.h"
#include "sampler.h"
#include "sampling.h"
#include "interpolation.h"
#include "scene.h"
#include "interaction.h"
#include "stats.h"
#include <stdarg.h>



namespace pbrt {

//IrisBXDFReflection::~IrisBXDFReflection() {}

// IrisMaterial Method Definitions
Float IrisBXDFReflection::Pdf(const Vector3f &wo, const Vector3f &wi) const {

    if (!SameHemisphere(wo, wi)) return 0;
    Vector3f wh = Normalize(wo + wi);
    return 1 / (2 * Pi); 
    //return distribution->Pdf(wo, wh) / (4 * Dot(wo, wh));
}

Spectrum IrisBXDFReflection::Sample_f(const Vector3f &wo, Vector3f *wi,
                                        const Point2f &u, Float *pdf,
                                        BxDFType *sampledType) const {

    // Sample microfacet orientation $\wh$ and reflected direction $\wi$
    if (wo.z == 0) return 0.;
    *wi = UniformSampleHemisphere(u); 
    if (pdf) *pdf = 0;
    if (!SameHemisphere(wo, *wi)) return Spectrum(0.f);
    if (pdf) *pdf = Pdf(wo, *wi);
    return f(wo, *wi);
}

Spectrum IrisBXDFReflection::f(const Vector3f &wo, const Vector3f &wi) const {
    

    const Float cosTheta0 = wo.z; 
    if(cosTheta0 <= 0.0f) return Spectrum(0.);

    const Float caustic_CosThetaI = wi.z; 
    if(caustic_CosThetaI <= 0.0f) return Spectrum(0.); 
    //printf("%f\n", caustic_power); 
    const Float alphaP = caustic_power; 

    //const float normalization_factor = (alphaP + 2.0) / (2.0 * M_PI); 
	const float norm_factor = (alphaP + 2.0) / (2.0 * Pi);
    const float normalizedPhong = norm_factor * pow(caustic_CosThetaI, alphaP);

    Spectrum F = 1 ; // fresnel->Evaluate(Dot(wi, Normalize(wi + wo)));
   
    const Spectrum fr = R * F * normalizedPhong; 

    return fr; 
}


}  // namespace pbrt
