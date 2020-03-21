//
//  HCPath+SVG.c
//  HollowCore
//
//  Created by Matt Stoker on 3/21/20.
//  Copyright © 2020 HollowCore. All rights reserved.
//

#include "HCPath+SVG.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCPathRef HCPathCreateWithSVGPathData(const char* path) {
    HCPathRef self = HCPathCreateEmpty();
    HCPathParse(self, path);
    return self;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Parsing
//----------------------------------------------------------------------------------------------------------------------------------
void HCPathParse(HCPathRef self, const char* path) {
    // Define state data for parsing the SVG path string
    char type = '\0';
    HCReal subpathStartX = 0.0;
    HCReal subpathStartY = 0.0;
    HCReal subpathEndX = subpathStartX;
    HCReal subpathEndY = subpathStartY;
    HCReal currentX = subpathEndX;
    HCReal currentY = subpathEndY;
    HCReal previousControlX = currentX;
    HCReal previousControlY = currentY;
    HCInteger argumentsExpected = 0;
    HCInteger argumentsCount = 0;
    HCReal arguments[10];
    
    // Parse path data according to SVG path element data string format
    const char* current = path;
    while (*current != '\0') {
        char c = *current;
        switch (c) {
            case '\n': // Fallthrough
            case '\r': // Fallthrough
            case ',': // Fallthrough
            case ' ': current++; break;
            case 'M': type = 'M'; argumentsExpected = 2; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'm': type = 'm'; argumentsExpected = 2; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'L': type = 'L'; argumentsExpected = 2; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'l': type = 'l'; argumentsExpected = 2; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'H': type = 'H'; argumentsExpected = 1; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'h': type = 'h'; argumentsExpected = 1; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'V': type = 'V'; argumentsExpected = 1; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'v': type = 'v'; argumentsExpected = 1; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'Q': type = 'Q'; argumentsExpected = 4; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'q': type = 'q'; argumentsExpected = 4; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'T': type = 'T'; argumentsExpected = 2; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 't': type = 't'; argumentsExpected = 2; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'C': type = 'C'; argumentsExpected = 6; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'c': type = 'c'; argumentsExpected = 6; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'S': type = 'S'; argumentsExpected = 4; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 's': type = 's'; argumentsExpected = 4; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'A': type = 'A'; argumentsExpected = 7; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'a': type = 'a'; argumentsExpected = 7; currentX = subpathEndX; currentY = subpathEndY; current++; break;
            case 'Z': // Fallthrough
            case 'z':
                // Close path has no arguments, so just perform the action and move on
                HCPathClose(self);
                subpathEndX = subpathStartX;
                subpathEndY = subpathStartY;
                currentX = subpathEndX;
                currentY = subpathEndY;
                current++;
                break;
            default: {
                // Parse an argument number
                char* end = (char*)current;
                double argument = strtod(current, &end);
                if (end <= current) {
                    // Invalid argument, ignore it and dump the current path sub-component
                    type = '\0';
                    current++;
                    break;
                }
                
                // Save the argument and advance to the character after the argument
                arguments[argumentsCount] = argument;
                argumentsCount++;
                current = end;
                
                // When enough arguments have been parsed, execute the draw command specified
                if (argumentsCount == argumentsExpected) {
                    // Consume arguments
                    argumentsCount = 0;
                    
                    // Execute the command
                    switch (type) {
                        case 'm':
                            arguments[0] += subpathEndX;
                            arguments[1] += subpathEndY;
                            // Fallthrough
                        case 'M': {
                            subpathStartX = arguments[0];
                            subpathStartY = arguments[1];
                            subpathEndX = subpathStartX;
                            subpathEndY = subpathStartY;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathMove(self, subpathStartX, subpathStartY);
                            type = type == 'm' ? 'l' : 'L';
                        } break;
                        case 'l':
                            arguments[0] += subpathEndX;
                            arguments[1] += subpathEndY;
                            // Fallthrough
                        case 'L': {
                            HCReal x = arguments[0];
                            HCReal y = arguments[1];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathAddLine(self, x, y);
                        } break;
                        case 'h':
                            arguments[0] += subpathEndX;
                            // Fallthrough
                        case 'H': {
                            HCReal x = arguments[0];
                            HCReal y = subpathEndY;
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathAddLine(self, x, y);
                        } break;
                        case 'v':
                            arguments[0] += subpathEndY;
                            // Fallthrough
                        case 'V': {
                            HCReal x = subpathEndX;
                            HCReal y = arguments[0];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = currentX;
                            previousControlY = currentY;
                            HCPathAddLine(self, x, y);
                        } break;
                        case 'q':
                            arguments[0] += subpathEndX;
                            arguments[1] += subpathEndY;
                            arguments[2] += subpathEndX;
                            arguments[3] += subpathEndY;
                            // Fallthrough
                        case 'Q': {
                            HCReal cx = arguments[0];
                            HCReal cy = arguments[1];
                            HCReal x = arguments[2];
                            HCReal y = arguments[3];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = cx;
                            previousControlY = cy;
                            HCPathAddQuadraticCurve(self, cx, cy, x, y);
                        } break;
                        case 't':
                            arguments[0] += subpathEndX;
                            arguments[1] += subpathEndY;
                            // Fallthrough
                        case 'T': {
                            HCReal cx = 2.0 * subpathEndX - previousControlX;
                            HCReal cy = 2.0 * subpathEndY - previousControlY;
                            HCReal x = arguments[0];
                            HCReal y = arguments[1];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = cx;
                            previousControlY = cy;
                            HCPathAddQuadraticCurve(self, cx, cy, x, y);
                        } break;
                        case 'c':
                            arguments[0] += subpathEndX;
                            arguments[1] += subpathEndY;
                            arguments[2] += subpathEndX;
                            arguments[3] += subpathEndY;
                            arguments[4] += subpathEndX;
                            arguments[5] += subpathEndY;
                            // Fallthrough
                        case 'C': {
                            HCReal c0x = arguments[0];
                            HCReal c0y = arguments[1];
                            HCReal c1x = arguments[2];
                            HCReal c1y = arguments[3];
                            HCReal x = arguments[4];
                            HCReal y = arguments[5];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = c1x;
                            previousControlY = c1y;
                            HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, x, y);
                        } break;
                        case 's':
                            arguments[0] += subpathEndX;
                            arguments[1] += subpathEndY;
                            arguments[2] += subpathEndX;
                            arguments[3] += subpathEndY;
                            // Fallthrough
                        case 'S': {
                            HCReal c0x = 2.0 * subpathEndX - previousControlX;
                            HCReal c0y = 2.0 * subpathEndY - previousControlY;
                            HCReal c1x = arguments[0];
                            HCReal c1y = arguments[1];
                            HCReal x = arguments[2];
                            HCReal y = arguments[3];
                            subpathEndX = x;
                            subpathEndY = y;
                            previousControlX = c1x;
                            previousControlY = c1y;
                            HCPathAddCubicCurve(self, c0x, c0y, c1x, c1y, x, y);
                        } break;
                        case 'a':
                            arguments[5] += subpathEndX;
                            arguments[6] += subpathEndY;
                            // Fallthrough
                        case 'A': {
//                            HCReal xr = arguments[0];
//                            HCReal yr = arguments[1];
//                            HCReal rotation = arguments[2] / 180.0 * M_PI;
//                            HCBoolean largeArc = arguments[3] == 0.0 ? false : true;
//                            HCBoolean sweep = arguments[4] == 0.0 ? false : true;
//                            HCReal x = arguments[5];
//                            HCReal y = arguments[6];
//                            subpathEndX = x;
//                            subpathEndY = y;
//                            previousControlX = currentX;
//                            previousControlY = currentY;
//                            HCPathAddArc(self, x, y, xr, yr, rotation, largeArc, sweep);
                        } break;
                    }
                }
            }
        }
    }
}
