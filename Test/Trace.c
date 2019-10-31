//
//  Trace.c
//  Test
//
//  Created by Matt Stoker on 10/20/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowCore.h"

CTEST(Trace, Trace) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    
    HCSetRef objects = HCSetCreate();
    HCSetAddObjectReleased(objects, HCSphereCreate(HCVectorMake(0.0, 0.0, 0.0), 0.5));
    HCSetAddObjectReleased(objects, HCSphereCreate(HCVectorMake(0.0, 0.0, 0.0), 0.5)); // Tests equality, should discard
    HCPrimitiveIntersect(NULL, HCRayInvalid); // Just to call the function for coverage, as no primitives without an intersection are defined yet
    
    fprintf(stdout, "Scene:\n");
    HCPrint(objects, stdout);
    fprintf(stdout, "\n");
    
    HCVector cameraOrigin = HCVectorMake(0.0, 0.0, 1.0);
    HCVector cameraTarget = HCVectorMake(0.0, 0.0, 0.0);
    HCVector cameraUp = HCVectorMake(0.0, 1.0, 0.0);
    
    HCVector cameraKAxis = HCVectorNormalize(HCVectorSubtract(cameraTarget, cameraOrigin));
    HCVector cameraIAxis = HCVectorNormalize(HCVectorCross(cameraKAxis, cameraUp));
    HCVector cameraJAxis = HCVectorNormalize(HCVectorCross(cameraIAxis, cameraKAxis));
    HCReal cameraFieldOfView = M_PI * 0.75;
    HCReal cameraAspectRatio = (HCReal)HCRasterWidth(raster) / (HCReal)HCRasterHeight(raster);
    
    HCVector cameraViewU = HCVectorScale(cameraIAxis, tanf(cameraFieldOfView * 0.5f));
    HCVector cameraViewV = HCVectorScale(cameraJAxis, HCVectorMagnitude(cameraViewU) / cameraAspectRatio);

    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(raster); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(raster); xIndex++) {
            HCReal pointU = ((((HCReal)xIndex + 0.5) / (HCReal)HCRasterWidth(raster)) - 0.5) * 2.0;
            HCReal pointV = ((((HCReal)yIndex + 0.5) / (HCReal)HCRasterHeight(raster)) - 0.5) * 2.0;
            HCVector direction = HCVectorAdd(cameraKAxis, HCVectorAdd(HCVectorScale(cameraViewU, pointU), HCVectorScale(cameraViewV, pointV)));
            HCRay r = HCRayMake(cameraOrigin, direction);
            HCColor c = HCColorBlack;
            for (HCSetIterator i = HCSetIterationBegin(objects); !HCSetIterationHasEnded(&i); HCSetIterationNext(&i)) {
                HCPrimitiveRef object = i.object;
                // TODO: How to call sub-class polymorphic function? Need HCObjectType() call?
                if (!isnan(HCSphereIntersect((HCSphereRef)object, r))) {
                    c = HCColorWhite;
                }
            }
            HCRasterSetPixelAt(raster, xIndex, yIndex, c);
        }
    }
    
    HCRasterSaveBMP(raster, "trace.bmp");
    HCRasterSavePPM(raster, "trace.ppm");
    
    HCRelease(objects);
    HCRelease(raster);
}
