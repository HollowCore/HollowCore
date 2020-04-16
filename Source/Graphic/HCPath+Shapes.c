//
//  HCPath+Shapes.c
//  HollowCore
//
//  Created by Matt Stoker on 4/15/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#include "HCPath+Shapes.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCPathRef HCPathCreateRectangle(HCRectangle rectangle) {
    HCPathRef self = HCPathCreateEmpty();
    HCPathMove(self, HCRectangleMinX(rectangle), HCRectangleMinY(rectangle));
    HCPathAddLine(self, HCRectangleMaxX(rectangle), HCRectangleMinY(rectangle));
    HCPathAddLine(self, HCRectangleMaxX(rectangle), HCRectangleMaxY(rectangle));
    HCPathAddLine(self, HCRectangleMinX(rectangle), HCRectangleMaxY(rectangle));
    HCPathClose(self);
    return self;
}

HCPathRef HCPathCreateEllipse(HCRectangle ellipseBounds) {
    static HCReal controlAdjustment = 0.552284749830793; //4.0 / 3.0 * (tan(M_PI * 0.5) * 0.25);
    HCPathRef self = HCPathCreateEmpty();
    
    HCReal p0x = HCRectangleMaxX(ellipseBounds);
    HCReal p0y = HCRectangleMidY(ellipseBounds);
    HCPathMove(self, p0x, p0y);
    
    HCReal c0x = HCRectangleMaxX(ellipseBounds);
    HCReal c0y = HCRectangleMidY(ellipseBounds) + controlAdjustment * HCRectangleHeight(ellipseBounds) * 0.5;
    HCReal c1x = HCRectangleMidX(ellipseBounds) + controlAdjustment * HCRectangleWidth(ellipseBounds) * 0.5;
    HCReal c1y = HCRectangleMaxY(ellipseBounds);
    HCReal p1x = HCRectangleMidX(ellipseBounds);
    HCReal p1y = HCRectangleMaxY(ellipseBounds);
    HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, p1x, p1y);
    
    c0x = HCRectangleMidX(ellipseBounds) - controlAdjustment * HCRectangleWidth(ellipseBounds) * 0.5;
    c0y = HCRectangleMaxY(ellipseBounds);
    c1x = HCRectangleMinX(ellipseBounds);
    c1y = HCRectangleMidY(ellipseBounds) + controlAdjustment * HCRectangleHeight(ellipseBounds) * 0.5;
    p1x = HCRectangleMinX(ellipseBounds);
    p1y = HCRectangleMidY(ellipseBounds);
    HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, p1x, p1y);
    
    c0x = HCRectangleMinX(ellipseBounds);
    c0y = HCRectangleMidY(ellipseBounds) - controlAdjustment * HCRectangleHeight(ellipseBounds) * 0.5;
    c1x = HCRectangleMidX(ellipseBounds) - controlAdjustment * HCRectangleWidth(ellipseBounds) * 0.5;
    c1y = HCRectangleMinY(ellipseBounds);
    p1x = HCRectangleMidX(ellipseBounds);
    p1y = HCRectangleMinY(ellipseBounds);
    HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, p1x, p1y);
    
    c0x = HCRectangleMidX(ellipseBounds) + controlAdjustment * HCRectangleWidth(ellipseBounds) * 0.5;
    c0y = HCRectangleMinY(ellipseBounds);
    c1x = HCRectangleMaxX(ellipseBounds);
    c1y = HCRectangleMidY(ellipseBounds) - controlAdjustment * HCRectangleHeight(ellipseBounds) * 0.5;
    p1x = HCRectangleMaxX(ellipseBounds);
    p1y = HCRectangleMidY(ellipseBounds);
    HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, p1x, p1y);
    
    HCPathClose(self);
    
    return self;
}
