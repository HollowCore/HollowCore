///
/// @file HCPath.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 12/28/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include "../Source/HollowCore.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Test Data
//----------------------------------------------------------------------------------------------------------------------------------
static const char* knight = "M 233.154600 499.852390 C 186.489170 501.367520 133.293860 490.730200 105.279900 449.572540 C 83.696854 420.308090 72.905126 367.924030 74.301501 332.517340 C 76.669165 301.304880 80.624943 290.258840 67.234778 262.220580 C 53.844615 234.182350 23.108510 189.151850 22.783181 154.980470 C 17.914125 106.842320 35.815674 56.202836 74.605945 26.020173 C 93.892230 10.755450 118.685370 -1.596812 143.583580 0.168380 C 162.484450 17.733402 111.534780 37.549445 140.507960 55.388182 C 167.929270 75.628386 119.280470 83.348389 111.148800 102.004320 C 97.849308 135.719660 103.128640 176.962640 127.610230 203.287770 C 148.579160 204.778680 170.410970 198.778690 191.843170 199.116140 C 245.221480 197.320660 298.853700 200.737710 351.215720 211.576010 C 375.277380 208.701460 391.074880 183.856510 398.801620 162.525660 C 409.364100 127.539670 392.664200 87.643914 361.152360 69.599216 C 344.987860 55.675277 387.746510 47.057520 360.248120 37.258850 C 349.114390 30.249324 314.230410 4.103343 344.266730 0.217148 C 397.339550 -1.469575 450.817850 30.581808 469.250340 81.307642 C 487.654610 128.981080 487.199280 189.073470 451.990250 229.155830 C 435.906060 250.001960 413.686260 259.468610 411.293310 289.643950 C 411.871870 335.016970 414.729530 382.326870 398.285710 425.518010 C 383.534280 466.355220 343.074130 492.433840 300.962290 496.869800 C 278.519200 500.323760 255.792960 500.155080 233.154600 499.852390 Z M 236.128450 482.360030 C 280.895680 483.212170 333.464210 483.377470 365.624250 446.926700 C 397.506130 408.253530 394.729580 354.350610 394.629320 307.182000 C 393.229350 281.483990 393.776240 262.364110 416.046400 241.405100 C 443.264700 218.828250 464.995740 186.382060 463.693150 149.732260 C 466.838070 98.708433 440.461370 42.454904 389.649230 25.279979 C 378.286650 19.357232 361.863340 19.756511 379.738290 30.451090 C 396.346820 38.762917 396.704350 54.695620 383.450130 63.228330 C 406.221550 84.440568 422.372470 114.482630 418.902220 146.397480 C 417.638180 182.156520 393.544200 215.301060 361.090260 229.588110 C 339.880990 229.709030 319.118840 221.844340 297.789340 220.534850 C 240.111220 214.062160 181.615210 214.796780 124.175550 223.122050 C 83.339625 191.921040 73.853034 126.663500 102.628740 83.796755 C 108.709820 73.858051 133.816910 64.642286 111.169990 57.481684 C 89.639616 49.108850 138.566920 10.370996 105.002230 27.043714 C 61.066202 49.623544 35.637314 99.992039 40.007019 148.759530 C 36.678723 187.768670 62.701028 218.197250 82.183929 249.793560 C 92.556549 267.263820 92.051678 285.169920 93.856094 312.488660 C 93.537930 355.679350 91.021399 396.163690 116.044140 434.442430 C 138.186880 472.568280 185.461870 483.388080 226.505390 482.181760 C 229.712850 482.252260 232.920560 482.312000 236.128430 482.360120 Z M 166.658690 429.221440 C 142.986810 430.531040 118.886320 412.225560 119.939540 387.217410 C 115.369390 360.367190 141.805710 334.035480 168.662290 341.581620 C 197.728860 346.598340 217.499920 383.687830 198.625430 408.802630 C 191.562880 419.767450 180.094470 428.626280 166.658690 429.221440 Z M 332.623480 432.158520 C 309.035280 433.269390 285.841480 414.511160 286.736390 389.970940 C 282.291590 362.174020 310.529160 335.697480 338.071540 344.933880 C 363.540450 351.343160 381.266850 381.532430 368.768750 406.024890 C 361.870780 419.923250 348.949130 431.998390 332.623480 432.158520 Z";

static const char* maze = "M 140.37258300203047,185.62741699796953 A 32,32 0 1,1 163,195 M 196.94112549695427,129.05887450304573 A 48,48 0 0,1 207.34621756054176,144.6311952464757 M 207.34621756054176,181.3688047535243 A 48,48 0 1,1 144.63119524647567,118.65378243945824 M 138.50826032863427,222.12829008072237 A 64,64 0 1,1 208.25483399593904,117.74516600406096 M 222.12829008072237,138.50826032863427 A 64,64 0 0,1 208.25483399593904,208.25483399593904 M 147.39277423870971,84.537177567741566 A 80,80 0 0,1 241.46282243225843,147.39277423870973 M 243,163 A 80,80 0 0,1 193.6146745892072,236.91036260090294 M 178.60722576129029,241.46282243225843 A 80,80 0 0,1 118.55438135843184,229.51756898420365 M 106.43145750507621,219.5685424949238 A 80,80 0 0,1 96.482431015796379,207.44561864156816 M 84.537177567741566,178.60722576129029 A 80,80 0 0,1 83,163.00000000000003 M 84.537177567741566,147.39277423870973 A 80,80 0 0,1 118.55438135843181,96.482431015796379 M 126.26239049295131,74.3075648789165 A 96,96 0 0,1 181.72867091354831,68.844613081289879 M 242.82108278104434,109.66525763011819 A 96,96 0 0,1 251.69243512108352,126.26239049295137 M 257.15538691871012,144.27132908645169 A 96,96 0 0,1 259,163 M 257.15538691871012,181.72867091354831 A 96,96 0 0,1 144.27132908645169,257.15538691871012 M 109.66525763011822,242.82108278104437 A 96,96 0 0,1 95.117749006091444,230.88225099390857 M 74.3075648789165,199.73760950704866 A 96,96 0 0,1 68.844613081289879,181.72867091354834 M 67,163.00000000000003 A 96,96 0 0,1 74.30756487891648,126.26239049295137 M 141.14988393419358,53.152048594838206 A 112,112 0 0,1 163,51 M 184.85011606580636,53.152048594838192 A 112,112 0 0,1 225.22386609819546,69.875403422114928 M 256.12459657788509,100.77613390180454 A 112,112 0 0,1 272.84795140516178,141.14988393419364 M 275,163 A 112,112 0 0,1 163,275 M 100.77613390180459,256.12459657788509 A 112,112 0 0,1 59.5254923587359,205.86054442489012 M 53.152048594838192,184.85011606580642 A 112,112 0 0,1 53.152048594838192,141.14988393419364 M 59.525492358735889,120.13945557510992 A 112,112 0 0,1 69.8754034221149,100.7761339018046 M 83.804040507106663,83.804040507106691 A 112,112 0 0,1 120.13945557510988,59.525492358735917 M 138.02843878193554,37.459484108386519 A 128,128 0 0,1 163,35 M 187.97156121806441,37.459484108386505 A 128,128 0 0,1 211.9834793427315,44.7434198385553 M 253.50966799187808,72.490332008121911 A 128,128 0 0,1 269.4281103747258,91.8870101734909 M 288.5405158916135,138.02843878193556 A 128,128 0 0,1 288.5405158916135,187.97156121806441 M 281.25658016144473,211.98347934273147 A 128,128 0 0,1 269.4281103747258,234.11298982650905 M 234.11298982650908,269.4281103747258 A 128,128 0 0,1 211.9834793427315,281.25658016144473 M 163.00000000000003,291 A 128,128 0 0,1 72.490332008121925,253.5096679918781 M 44.743419838555326,211.98347934273156 A 128,128 0 0,1 35,163.00000000000003 M 37.459484108386505,138.02843878193556 A 128,128 0 0,1 44.743419838555312,114.01652065726847 M 56.571889625274181,91.887010173490964 A 128,128 0 0,1 91.887010173490921,56.57188962527421 M 163,19 A 144,144 0 0,1 177.11446820745672,19.693399359203653 M 204.80099352464259,25.200591654561919 A 144,144 0 0,1 230.88113010294364,36.003337933836875 M 243.0021135548227,43.268375828433477 A 144,144 0 0,1 254.35263291956494,51.686494715765875 M 264.82337649086287,61.176623509137158 A 144,144 0 0,1 296.03865268162531,107.89358573942707 M 300.79940834543811,121.19900647535744 A 144,144 0 0,1 304.23308037806521,134.90699362967752 M 306.30660064079632,148.88553179254325 A 144,144 0 0,1 307,163 M 304.23308037806521,191.09300637032246 A 144,144 0 0,1 254.35263291956494,274.31350528423411 M 243.0021135548227,282.73162417156652 A 144,144 0 0,1 191.09300637032251,304.23308037806521 M 177.11446820745675,306.30660064079632 A 144,144 0 0,1 134.90699362967752,304.23308037806521 M 121.19900647535745,300.79940834543811 A 144,144 0 0,1 51.686494715765917,254.352632919565 M 43.268375828433477,243.0021135548227 A 144,144 0 0,1 25.200591654561919,204.80099352464259 M 19.693399359203653,177.1144682074567 A 144,144 0 0,1 19,163.00000000000003 M 19.693399359203653,148.88553179254336 A 144,144 0 0,1 21.766919621934818,134.90699362967752 M 29.96134731837472,107.89358573942704 A 144,144 0 0,1 61.17662350913713,61.176623509137173 M 71.647367080434989,51.686494715765932 A 144,144 0 0,1 82.997886445177286,43.268375828433491 M 95.1188698970563,36.0033379338369 A 144,144 0 0,1 148.88553179254328,19.693399359203653 M 178.68274245272968,3.7704437324484843 A 160,160 0 1,1 163,3 M 163 3 L 163 35 M 163 115 L 163 131 M 194.21445152258053 6.0743551354831311 L 187.97156121806441 37.459484108386505 M 184.85011606580636 53.152048594838192 L 181.72867091354831 68.844613081289879 M 218.10641426057293 29.96134731837472 L 211.9834793427315 44.7434198385553 M 199.7376095070486 74.30756487891648 L 193.6146745892072 89.089637399097057 M 187.49173967136574 103.87170991927765 L 181.3688047535243 118.65378243945824 M 243.0021135548227 43.268375828433491 L 216.33474236988181 83.178917218955661 M 264.82337649086287 61.176623509137144 L 219.5685424949238 106.4314575050762 M 208.25483399593904 117.74516600406096 L 196.94112549695427 129.05887450304573 M 256.12459657788509 100.77613390180454 L 242.82108278104434 109.66525763011818 M 296.03865268162531 107.89358573942707 L 281.25658016144473 114.0165206572685 M 236.91036260090294 132.3853254107928 L 222.12829008072237 138.50826032863427 M 316.1104537171534 116.55445163928603 L 300.79940834543811 121.19900647535742 M 304.23308037806521 134.90699362967752 L 272.84795140516178 141.14988393419364 M 257.15538691871012 144.27132908645169 L 241.46282243225843 147.39277423870973 M 322.22955626755152 147.31725754727029 L 306.30660064079632 148.88553179254325 M 275 163 L 259 163 M 227 163 L 195 163 M 322.22955626755152 178.68274245272971 L 306.30660064079632 177.11446820745672 M 304.23308037806521 191.09300637032246 L 288.5405158916135 187.97156121806441 M 257.15538691871012 181.72867091354831 L 241.46282243225843 178.60722576129027 M 269.4281103747258 234.11298982650905 L 256.12459657788509 225.2238660981954 M 264.82337649086287 264.82337649086287 L 253.50966799187808 253.50966799187808 M 234.11298982650908 269.4281103747258 L 225.22386609819546 256.12459657788509 M 193.6146745892072 236.91036260090294 L 181.36880475352433 207.34621756054176 M 191.09300637032251 304.23308037806521 L 184.85011606580639 272.84795140516178 M 178.68274245272974 322.22955626755152 L 177.11446820745675 306.30660064079632 M 163.00000000000003 291 L 163 275 M 163 243 L 163 227 M 141.14988393419361 272.84795140516178 L 147.39277423870973 241.46282243225843 M 114.01652065726853 281.25658016144473 L 126.2623904929514 251.69243512108352 M 138.50826032863427 222.12829008072237 L 144.6311952464757 207.34621756054176 M 100.77613390180459 256.12459657788509 L 118.55438135843184 229.51756898420365 M 61.176623509137158 264.82337649086287 L 72.490332008121925 253.5096679918781 M 129.05887450304573 196.94112549695427 L 140.37258300203047 185.62741699796953 M 39.318327461962127 264.50292546618334 L 51.686494715765917 254.352632919565 M 43.268375828433491 243.0021135548227 L 96.482431015796379 207.44561864156816 M 29.961347318374749 218.10641426057302 L 44.743419838555326 211.98347934273156 M 74.3075648789165 199.73760950704866 L 103.87170991927766 187.49173967136579 M 6.0743551354831595 194.21445152258059 L 21.766919621934846 191.09300637032254 M 53.152048594838206 184.85011606580642 L 68.844613081289893 181.72867091354834 M 19 163.00000000000003 L 35 163.00000000000003 M 83 163.00000000000003 L 99 163 M 21.766919621934818 134.90699362967752 L 53.152048594838192 141.14988393419364 M 9.8895462828465668 116.55445163928607 L 25.200591654561919 121.19900647535746 M 29.96134731837472 107.89358573942704 L 74.30756487891648 126.26239049295137 M 69.875403422114914 100.7761339018046 L 96.482431015796365 118.55438135843185 M 83.804040507106663 83.804040507106691 L 95.117749006091429 95.117749006091458 M 74.108762716863652 29.964862031592759 L 91.887010173490921 56.57188962527421 M 109.66525763011819 83.178917218955661 L 118.55438135843183 96.482431015796379 M 107.89358573942698 29.961347318374749 L 138.50826032863421 103.87170991927766 M 138.02843878193554 37.459484108386519 L 141.14988393419358 53.152048594838206";

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCPath, Creation) {
    HCPathRef path = HCPathCreate();
    ASSERT_EQUAL(HCPathElementCount(path), 0);
    HCRelease(path);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCPath, EqualHash) {
    HCPathRef a = HCPathCreateWithSVGPathData("M 1 2 L 1 2 Q 1 2 3 4 C 1 2 3 4 5 6 Z");
    HCPathRef b = HCPathCreateWithSVGPathData("M 1 2 L 1 2 Q 1 2 3 4 C 1 2 3 4 5 6 Z");
    ASSERT_TRUE(HCObjectIsEqual((HCObjectRef)a, (HCObjectRef)a));
    ASSERT_FALSE(HCObjectIsEqual((HCObjectRef)a, (HCObjectRef)b));
    ASSERT_EQUAL(HCObjectHashValue((HCObjectRef)a), HCObjectHashValue((HCObjectRef)a));
    ASSERT_TRUE(HCIsEqual(a, a));
    ASSERT_TRUE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCPath, Print) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 1 2 L 1 2 Q 1 2 3 4 C 1 2 3 4 5 6 Z");
    HCObjectPrint((HCObjectRef)path, stdout); // TODO: Not to stdout
    HCPrint(path, stdout); // TODO: Not to stdout
    HCRelease(path);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
// HCPath.h
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCPath, CreateWithElements) {
    HCPoint points[] = {
        {.x = 1.0, .y = 2.0},
        {.x = 3.0, .y = 4.0},
        {.x = 5.0, .y = 6.0},
    };
    HCPathElement elements[] = {
        {.command = HCPathCommandMove, .points = points},
        {.command = HCPathCommandAddLine, .points = points},
        {.command = HCPathCommandAddQuadraticCurve, .points = points},
        {.command = HCPathCommandAddCubicCurve, .points = points},
        {.command = HCPathCommandCloseContour, .points = NULL},
    };
    HCPathRef path = HCPathCreateWithElements(elements, sizeof(elements) / sizeof(HCPathElement));
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(1.0, 2.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(1.0, 2.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(1.0, 2.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(3.0, 4.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(1.0, 2.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[1], HCPointMake(3.0, 4.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(5.0, 6.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandCloseContour);
    HCRelease(path);
}

CTEST(HCPath, CreateTranslate) {
    HCPoint points[] = {
        {.x = 1.0, .y = 2.0},
        {.x = 3.0, .y = 4.0},
        {.x = 5.0, .y = 6.0},
    };
    HCPathElement elements[] = {
        {.command = HCPathCommandMove, .points = points},
        {.command = HCPathCommandAddLine, .points = points},
        {.command = HCPathCommandAddQuadraticCurve, .points = points},
        {.command = HCPathCommandAddCubicCurve, .points = points},
        {.command = HCPathCommandCloseContour, .points = NULL},
    };
    HCPathRef path = HCPathCreateWithElements(elements, sizeof(elements) / sizeof(HCPathElement));
    HCPathRef translatedPath = HCPathCreateByTranslatingPath(path, 10.0, -5.0);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(translatedPath, 0).points[0], HCPointTranslate(points[0], 10.0, -5.0)));
    HCRelease(path);
    HCRelease(translatedPath);
}

CTEST(HCPath, CreateScale) {
    HCPoint points[] = {
        {.x = 1.0, .y = 2.0},
        {.x = 3.0, .y = 4.0},
        {.x = 5.0, .y = 6.0},
    };
    HCPathElement elements[] = {
        {.command = HCPathCommandMove, .points = points},
        {.command = HCPathCommandAddLine, .points = points},
        {.command = HCPathCommandAddQuadraticCurve, .points = points},
        {.command = HCPathCommandAddCubicCurve, .points = points},
        {.command = HCPathCommandCloseContour, .points = NULL},
    };
    HCPathRef path = HCPathCreateWithElements(elements, sizeof(elements) / sizeof(HCPathElement));
    HCPathRef scaledPath = HCPathCreateByScalingPath(path, 10.0, -5.0);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(scaledPath, 0).points[0], HCPointScale(points[0], 10.0, -5.0)));
    HCRelease(path);
    HCRelease(scaledPath);
}

CTEST(HCPath, CreateRotate) {
    HCPoint points[] = {
        {.x = 1.0, .y = 2.0},
        {.x = 3.0, .y = 4.0},
        {.x = 5.0, .y = 6.0},
    };
    HCPathElement elements[] = {
        {.command = HCPathCommandMove, .points = points},
        {.command = HCPathCommandAddLine, .points = points},
        {.command = HCPathCommandAddQuadraticCurve, .points = points},
        {.command = HCPathCommandAddCubicCurve, .points = points},
        {.command = HCPathCommandCloseContour, .points = NULL},
    };
    HCPathRef path = HCPathCreateWithElements(elements, sizeof(elements) / sizeof(HCPathElement));
    HCPathRef rotatedPath = HCPathCreateByRotatingPath(path, M_PI_2);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(rotatedPath, 0).points[0], HCPointRotate(points[0], cos(M_PI_2), sin(M_PI_2))));
    HCRelease(path);
    HCRelease(rotatedPath);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
// HCPath+Shapes.h
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCPath, Rectangle) {
    HCRectangle rectangle = HCRectangleMakeWithComponents(10.0, 20.0, 30.0, 40.0);
    HCPathRef path = HCPathCreateRectangle(rectangle);
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(HCRectangleMinX(rectangle), HCRectangleMinY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(HCRectangleMaxX(rectangle), HCRectangleMinY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(HCRectangleMaxX(rectangle), HCRectangleMaxY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(HCRectangleMinX(rectangle), HCRectangleMaxY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandCloseContour);
    HCRelease(path);
}

CTEST(HCPath, Ellipse) {
    HCRectangle rectangle = HCRectangleMakeWithComponents(10.0, 20.0, 30.0, 40.0);
    HCPathRef path = HCPathCreateEllipse(rectangle);
    ASSERT_EQUAL(HCPathElementCount(path), 6);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(HCRectangleMaxX(rectangle), HCRectangleMidY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(HCRectangleMidX(rectangle), HCRectangleMaxY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(HCRectangleMinX(rectangle), HCRectangleMidY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(HCRectangleMidX(rectangle), HCRectangleMinY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[2], HCPointMake(HCRectangleMaxX(rectangle), HCRectangleMidY(rectangle))));
    ASSERT_TRUE(HCPathElementAt(path, 5).command == HCPathCommandCloseContour);
    HCRelease(path);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
// HCPath+SVG.h
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCPath, HorizontalPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 H 50");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(50.0, 20.0)));
    HCRelease(path);
}

CTEST(HCPath, HorizontalRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 h 40");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(50.0, 20.0)));
    HCRelease(path);
}

CTEST(HCPath, HorizontalPolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 H 25 50 75 90");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(25.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(50.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(75.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(90.0, 20.0)));
    HCRelease(path);
}

CTEST(HCPath, HorizontalRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 h 15 25 25 15");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(25.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(50.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(75.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(90.0, 20.0)));
    HCRelease(path);
}

CTEST(HCPath, VerticalPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 V 50");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 50.0)));
    HCRelease(path);
}

CTEST(HCPath, VerticalRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 v 30");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 50.0)));
    HCRelease(path);
}

CTEST(HCPath, VerticalPolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 V 25 50 75 80");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 25.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(10.0, 50.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(10.0, 75.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, VerticalRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 v 5 25 25 5");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 25.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(10.0, 50.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(10.0, 75.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, LinePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 L 60 80");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(60.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, LineRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 l 50 60");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(60.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, LinePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 L 60 80 50 40 90 20");
    ASSERT_EQUAL(HCPathElementCount(path), 4);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(60.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(50.0, 40.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(90.0, 20.0)));
    HCRelease(path);
}

CTEST(HCPath, LineRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 l 50 60 -10 -40 40 -20");
    ASSERT_EQUAL(HCPathElementCount(path), 4);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(60.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(50.0, 40.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(90.0, 20.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticCurvePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 95 10 180 80");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(95.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticCurveRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 q 85 -70 170 0");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(95.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticCurvePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 95 10 180 80 95 150 10 80");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(95.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(180.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(95.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticCurveRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 q 85 -70 170 0 -85 70 -170 0");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(95.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(180.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(95.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticSmoothCurvePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 52.5 10 95 80 T 180 80");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(52.5, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(95, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(137.5, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticSmoothCurveRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 52.5 10 95 80 t 85 0");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(52.5, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(95, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(137.5, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticSmoothCurvePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 52.5 10 95 80 T 180 80 95 80 10 80");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(52.5, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(95, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(137.5, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(180.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(222.5, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[1], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(-32.5, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[1], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, QuadraticSmoothCurveRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 52.5 10 95 80 t 85 0 -85 0 -85 0");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(52.5, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(95, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(137.5, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(180.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(222.5, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[1], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(-32.5, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[1], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicCurvePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 C 30 10 70 10 90 90");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(70.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(90.0, 90.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicCurveRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 c 20 -80 60 -80 80 0");
    ASSERT_EQUAL(HCPathElementCount(path), 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(70.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(90.0, 90.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicCurvePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 C 30 10 70 10 90 90 70 40 30 40 10 90");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(70.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(90.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(70.0, 40.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(30.0, 40.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(10.0, 90.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicCurveRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 c 20 -80 60 -80 80 0 -20 -50 -60 -50 -80 0");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(70.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(90.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(70.0, 40.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(30.0, 40.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(10.0, 90.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicSmoothCurvePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 C 40 10 65 10 95 80 S 150 150 180 80");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(40.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(65.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(125.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(150.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicSmoothCurveRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 C 40 10 65 10 95 80 s 55 70 85 0");
    ASSERT_EQUAL(HCPathElementCount(path), 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(40.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(65.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(125.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(150.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(180.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicSmoothCurvePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 C 40 10 65 10 95 80 S 150 150 180 80 80 10 95 80 10 150 10 80");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(40.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(65.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(125.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(150.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(180.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(210.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[1], HCPointMake(80.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(110.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[1], HCPointMake(10.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[2], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, CubicSmoothCurveRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 C 40 10 65 10 95 80 s 55 70 85 0 -100 -70 -85 0 -85 70 -85 0");
    ASSERT_EQUAL(HCPathElementCount(path), 5);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(40.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(65.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(125.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[1], HCPointMake(150.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[2], HCPointMake(180.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(210.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[1], HCPointMake(80.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(95.0, 80.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(110.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[1], HCPointMake(10.0, 150.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[2], HCPointMake(10.0, 80.0)));
    HCRelease(path);
}

CTEST(HCPath, ArcPath) {
    HCPathRef path = HCPathCreateWithSVGPathData(
        "M25 350 L 75 325"
        "A25  25 -30 0 1 125 300 L 175 275"
        "A25  50 -30 0 1 225 250 L 275 225"
        "A25  75 -30 0 1 325 200 L 375 175"
        "A25 100 -30 0 1 425 150 L 475 125"
    );
    ASSERT_EQUAL(HCPathElementCount(path), 14);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(25.0, 350.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(75.0, 325.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(125.0, 300.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(175.0, 275.0)));
    ASSERT_TRUE(HCPathElementAt(path, 5).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 6).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 6).points[2], HCPointMake(225.0, 250.0)));
    ASSERT_TRUE(HCPathElementAt(path, 7).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 7).points[0], HCPointMake(275.0, 225.0)));
    ASSERT_TRUE(HCPathElementAt(path, 8).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 9).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 9).points[2], HCPointMake(325.0, 200.0)));
    ASSERT_TRUE(HCPathElementAt(path, 10).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 10).points[0], HCPointMake(375.0, 175.0)));
    ASSERT_TRUE(HCPathElementAt(path, 11).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 12).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 12).points[2], HCPointMake(425.0, 150.0)));
    ASSERT_TRUE(HCPathElementAt(path, 13).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 13).points[0], HCPointMake(475.0, 125.0)));
    HCRelease(path);
}

CTEST(HCPath, ArcRelativePath) {
    HCPathRef path = HCPathCreateWithSVGPathData(
        "M25 350 l 50 -25"
        "a25  25 -30 0 1 50 -25 l 50 -25"
        "a25  50 -30 0 1 50 -25 l 50 -25"
        "a25  75 -30 0 1 50 -25 l 50 -25"
        "a25 100 -30 0 1 50 -25 l 50 -25"
    );
    ASSERT_EQUAL(HCPathElementCount(path), 14);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(25.0, 350.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(75.0, 325.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(125.0, 300.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(175.0, 275.0)));
    ASSERT_TRUE(HCPathElementAt(path, 5).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 6).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 6).points[2], HCPointMake(225.0, 250.0)));
    ASSERT_TRUE(HCPathElementAt(path, 7).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 7).points[0], HCPointMake(275.0, 225.0)));
    ASSERT_TRUE(HCPathElementAt(path, 8).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 9).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 9).points[2], HCPointMake(325.0, 200.0)));
    ASSERT_TRUE(HCPathElementAt(path, 10).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 10).points[0], HCPointMake(375.0, 175.0)));
    ASSERT_TRUE(HCPathElementAt(path, 11).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 12).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 12).points[2], HCPointMake(425.0, 150.0)));
    ASSERT_TRUE(HCPathElementAt(path, 13).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 13).points[0], HCPointMake(475.0, 125.0)));
    HCRelease(path);
}

CTEST(HCPath, ArcPolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 315 L 110 215 A 30 50 60 0 1 162.55 162.55 30 50 30 1 0 215.10 110.10 L 315 10");
    ASSERT_EQUAL(HCPathElementCount(path), 8);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 315.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(110.0, 215.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(162.55, 162.55)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 5).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 6).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 6).points[2], HCPointMake(215.10, 110.10)));
    ASSERT_TRUE(HCPathElementAt(path, 7).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 7).points[0], HCPointMake(315.0, 10.0)));
    HCRelease(path);
}

CTEST(HCPath, ArcRelativePolyPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 315 L 110 215 a 30 50 60 0 1 52.55 -52.45 30 50 30 1 0 52.55 -52.45 L 315 10");
    ASSERT_EQUAL(HCPathElementCount(path), 8);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 315.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(110.0, 215.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[2], HCPointMake(162.55, 162.55)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 5).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(path, 6).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsSimilar(HCPathElementAt(path, 6).points[2], HCPointMake(215.10, 110.10), 0.000001));
    ASSERT_TRUE(HCPathElementAt(path, 7).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 7).points[0], HCPointMake(315.0, 10.0)));
    HCRelease(path);
}

CTEST(HCPath, Path) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 C 30 10 70 10 90 90 Z L 30 10 H 70 L 90 90 Q 50 10 10 90 Z");
    ASSERT_EQUAL(HCPathElementCount(path), 8);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(70.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(90.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandCloseContour);
    ASSERT_TRUE(HCPathElementAt(path, 3).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 3).points[0], HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPathElementAt(path, 4).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 4).points[0], HCPointMake(70.0, 10.0)));
    ASSERT_TRUE(HCPathElementAt(path, 5).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 5).points[0], HCPointMake(90.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 6).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 6).points[0], HCPointMake(50.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 6).points[1], HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPathElementAt(path, 7).command == HCPathCommandCloseContour);
    HCRelease(path);
}

CTEST(HCPath, ManyQuadratics) {
    HCPathRef path = HCPathCreateWithSVGPathData(
        "M 300 500 Q 500  1500 700 500 "
        "M 280 500 Q 500  1400 720 500 "
        "M 260 500 Q 500  1300 740 500 "
        "M 240 500 Q 500  1200 760 500 "
        "M 220 500 Q 500  1100 780 500 "
        "M 200 500 Q 500  1000 800 500 "
        "M 180 500 Q 500   900 820 500 "
        "M 160 500 Q 500   800 840 500 "
        "M 140 500 Q 500   700 860 500 "
        "M 120 500 Q 500   600 880 500 "
        "M 100 500 Q 500   500 900 500 "
        "M 120 500 Q 500   400 880 500 "
        "M 140 500 Q 500   300 860 500 "
        "M 160 500 Q 500   200 840 500 "
        "M 180 500 Q 500   100 820 500 "
        "M 200 500 Q 500     0 800 500 "
        "M 220 500 Q 500  -100 780 500 "
        "M 240 500 Q 500  -200 760 500 "
        "M 260 500 Q 500  -300 740 500 "
        "M 280 500 Q 500  -400 720 500 "
        "M 300 500 Q 500  -500 700 500 "
    );
    ASSERT_EQUAL(HCPathElementCount(path), 42);
    HCRelease(path);
}

CTEST(HCPath, ManyCubics) {
    HCPathRef path = HCPathCreateWithSVGPathData(
        "M 300 500 C 500  1500 500  1500 700 500 "
        "M 280 500 C 500  1400 500  1400 720 500 "
        "M 260 500 C 500  1300 500  1300 740 500 "
        "M 240 500 C 500  1200 500  1200 760 500 "
        "M 220 500 C 500  1100 500  1100 780 500 "
        "M 200 500 C 500  1000 500  1000 800 500 "
        "M 180 500 C 500   900 500   900 820 500 "
        "M 160 500 C 500   800 500   800 840 500 "
        "M 140 500 C 500   700 500   700 860 500 "
        "M 120 500 C 500   600 500   600 880 500 "
        "M 100 500 C 500   500 500   500 900 500 "
        "M 120 500 C 500   400 500   400 880 500 "
        "M 140 500 C 500   300 500   300 860 500 "
        "M 160 500 C 500   200 500   200 840 500 "
        "M 180 500 C 500   100 500   100 820 500 "
        "M 200 500 C 500     0 500     0 800 500 "
        "M 220 500 C 500  -100 500  -100 780 500 "
        "M 240 500 C 500  -200 500  -200 760 500 "
        "M 260 500 C 500  -300 500  -300 740 500 "
        "M 280 500 C 500  -400 500  -400 720 500 "
        "M 300 500 C 500  -500 500  -500 700 500 "
    );
    ASSERT_EQUAL(HCPathElementCount(path), 42);
    HCRelease(path);
}

CTEST(HCPath, Knight) {
    HCPathRef path = HCPathCreateWithSVGPathData(knight);
    ASSERT_EQUAL(HCPathElementCount(path), 57);
    HCRelease(path);
}

CTEST(HCPath, MoveImplicitLine) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 30 40 50 60");
    ASSERT_TRUE(HCPathElementCount(path) == 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(30.0, 40.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(50.0, 60.0)));
    HCRelease(path);
}

CTEST(HCPath, MoveImplicitLineRelative) {
    HCPathRef path = HCPathCreateWithSVGPathData("m 10 20 30 40 50 60");
    ASSERT_TRUE(HCPathElementCount(path) == 3);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(40.0, 60.0)));
    ASSERT_TRUE(HCPathElementAt(path, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 2).points[0], HCPointMake(90.0, 120.0)));
    HCRelease(path);
}

CTEST(HCPath, DegenerateLine) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 L 10 10");
    ASSERT_TRUE(HCPathElementCount(path) == 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 10.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 10.0)));
    HCRelease(path);
}

CTEST(HCPath, DegenerateQuadratic) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 Q 10 10 10 10");
    ASSERT_TRUE(HCPathElementCount(path) == 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 10.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(10.0, 10.0)));
    HCRelease(path);
}

CTEST(HCPath, DegenerateCubic) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 C 10 10 10 10 10 10");
    ASSERT_TRUE(HCPathElementCount(path) == 2);
    ASSERT_TRUE(HCPathElementAt(path, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 0).points[0], HCPointMake(10.0, 10.0)));
    ASSERT_TRUE(HCPathElementAt(path, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[0], HCPointMake(10.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[1], HCPointMake(10.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathElementAt(path, 1).points[2], HCPointMake(10.0, 10.0)));
    HCRelease(path);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - SVG Path Data Conversion
// HCPath+SVG.h
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCPath, SVGConversion) {
void HCPathPrintSVGPathData(HCPathRef self, FILE* stream);
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 L 30 10 30 20 10 20 Z");
    HCPathPrintSVGPathData(path, stdout); // TODO: Not to stdout
    HCRelease(path);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCPath, Size) {
    HCPathRef emptyPath = HCPathCreate();
    ASSERT_TRUE(HCRectangleIsEqual(HCPathBounds(emptyPath), HCRectangleZero));
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 L 30 10 30 20 10 20 Z");
    ASSERT_TRUE(HCRectangleIsEqual(HCPathBounds(path), HCRectangleMake(HCPointMake(10.0, 10.0), HCSizeMake(20.0, 10.0))));
    HCRelease(emptyPath);
    HCRelease(path);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Polylines
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCPath, PolylineFromLinear) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 L 60 80 30 10 Z M 1 2 L 6 8 3 1");
    
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylineCount(path), 7));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylinePointCount(path, 0), 0));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylinePointCount(path, 1), 2));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 1, 0), HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 1, 1), HCPointMake(60.0, 80.0)));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylinePointCount(path, 2), 2));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 2, 0), HCPointMake(60.0, 80.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 2, 1), HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylinePointCount(path, 3), 2));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 3, 0), HCPointMake(30.0, 10.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 3, 1), HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylinePointCount(path, 4), 0));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylinePointCount(path, 5), 2));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 5, 0), HCPointMake(1.0, 2.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 5, 1), HCPointMake(6.0, 8.0)));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylinePointCount(path, 6), 2));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 6, 0), HCPointMake(6.0, 8.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 6, 1), HCPointMake(3.0, 1.0)));
    
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 1)[0], HCPathPolylinePointAt(path, 1, 0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 1)[1], HCPathPolylinePointAt(path, 1, 1)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 2)[0], HCPathPolylinePointAt(path, 2, 0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 2)[1], HCPathPolylinePointAt(path, 2, 1)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 3)[0], HCPathPolylinePointAt(path, 3, 0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 3)[1], HCPathPolylinePointAt(path, 3, 1)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 5)[0], HCPathPolylinePointAt(path, 5, 0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 5)[1], HCPathPolylinePointAt(path, 5, 1)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 6)[0], HCPathPolylinePointAt(path, 6, 0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 6)[1], HCPathPolylinePointAt(path, 6, 1)));
    
    HCRelease(path);
}

CTEST(HCPath, PolylineFromQuadratic) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 95 10 180 80");
    
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylineCount(path), 2));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylinePointCount(path, 0), 0));
    ASSERT_TRUE(HCPathPolylinePointCount(path, 1) > 10);
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 1, 0), HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 1, HCPathPolylinePointCount(path, 1) - 1), HCPointMake(180.0, 80.0)));
    
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 1)[0], HCPathPolylinePointAt(path, 1, 0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 1)[HCPathPolylinePointCount(path, 1) - 1], HCPathPolylinePointAt(path, 1, HCPathPolylinePointCount(path, 1) - 1)));
    
    HCRelease(path);
}

CTEST(HCPath, PolylineFromCubic) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 C 30 10 70 10 90 90");
    
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylineCount(path), 2));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylinePointCount(path, 0), 0));
    ASSERT_TRUE(HCPathPolylinePointCount(path, 1) > 10);
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 1, 0), HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylinePointAt(path, 1, HCPathPolylinePointCount(path, 1) - 1), HCPointMake(90.0, 90.0)));
    
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 1)[0], HCPathPolylinePointAt(path, 1, 0)));
    ASSERT_TRUE(HCPointIsEqual(HCPathPolylineAt(path, 1)[HCPathPolylinePointCount(path, 1) - 1], HCPathPolylinePointAt(path, 1, HCPathPolylinePointCount(path, 1) - 1)));
    
    HCRelease(path);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Polyline / Path Element Correspondence
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCPath, PolylineIndexFromElement) {
    HCPathRef path = HCPathCreateWithSVGPathData(knight);
    for (HCInteger elementIndex = 0; elementIndex < HCPathElementCount(path); elementIndex++) {
        ASSERT_TRUE(HCIntegerIsEqual(HCPathIndexOfPolylineContainingElement(path, elementIndex), elementIndex));
    }
    HCRelease(path);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contours
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCPath, PathFromContour) {
    HCContourComponent components[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 10.0, .y = 10.0}},
        {.c0 = {.x = 15.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 20.0, .y = 10.0}},
        {.c0 = {.x = 30.0, .y = 22.5}, .c1 = {.x = 30.0, .y = 27.5}, .p = {.x = 20.0, .y = 20.0}},
        {.c0 = {.x = 15.0, .y = 30.0}, .c1 = HCPointInvalidStatic, .p = {.x = 10.0, .y = 20.0}},
    };
    HCContour* contour = HCContourInitInComponents(components, sizeof(components) / sizeof(HCContourComponent), true);
    HCPathRef path = HCPathCreateWithContour(contour);
    
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 1));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(path, 0), HCContourCurveCount((HCContour*)contour)));
    for (HCInteger elementIndex = 0; elementIndex < HCContourCurveCount(contour); elementIndex++) {
        ASSERT_TRUE(HCCurveIsEqual(HCPathContourCurveAt(path, 0, elementIndex), HCContourCurveAt(contour, elementIndex)));
    }
    
    HCRelease(path);
}

CTEST(HCPath, ContourFromLinear) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 20 L 60 80 30 10 Z M 1 2 L 6 8 3 1");
    
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 2));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(path, 0), 2));
    ASSERT_TRUE(HCCurveIsEqual(HCPathContourCurveAt(path, 0, 0), HCCurveMakeLinear(HCPointMake(10.0, 20.0), HCPointMake(60.0, 80.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCPathContourCurveAt(path, 0, 1), HCCurveMakeLinear(HCPointMake(60.0, 80.0), HCPointMake(30.0, 10.0))));
    
    ASSERT_TRUE(HCIntegerIsEqual(HCContourCurveCount(HCPathContourAt(path, 0)), HCPathContourCurveCount(path, 0)));
    ASSERT_TRUE(HCBooleanIsEqual(HCContourIsClosed(HCPathContourAt(path, 0)), true));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(HCPathContourAt(path, 0)), HCPointMake(10.0, 20.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(HCPathContourAt(path, 0)), HCPointMake(10.0, 20.0)));
    
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(path, 1), 2));
    ASSERT_TRUE(HCCurveIsEqual(HCPathContourCurveAt(path, 1, 0), HCCurveMakeLinear(HCPointMake(1.0, 2.0), HCPointMake(6.0, 8.0))));
    ASSERT_TRUE(HCCurveIsEqual(HCPathContourCurveAt(path, 1, 1), HCCurveMakeLinear(HCPointMake(6.0, 8.0), HCPointMake(3.0, 1.0))));
    
    ASSERT_TRUE(HCIntegerIsEqual(HCContourCurveCount(HCPathContourAt(path, 1)), HCPathContourCurveCount(path, 1)));
    ASSERT_TRUE(HCBooleanIsEqual(HCContourIsClosed(HCPathContourAt(path, 1)), false));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(HCPathContourAt(path, 1)), HCPointMake(1.0, 2.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(HCPathContourAt(path, 1)), HCPointMake(3.0, 1.0)));
    
    HCRelease(path);
}

CTEST(HCPath, ContourFromQuadratic) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 80 Q 95 10 180 80");
    
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 1));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(path, 0), 1));
    ASSERT_TRUE(HCCurveIsEqual(HCPathContourCurveAt(path, 0, 0), HCCurveMakeQuadratic(HCPointMake(10.0, 80.0), HCPointMake(95.0, 10.0), HCPointMake(180.0, 80.0))));
    
    ASSERT_TRUE(HCIntegerIsEqual(HCContourCurveCount(HCPathContourAt(path, 0)), HCPathContourCurveCount(path, 0)));
    ASSERT_TRUE(HCBooleanIsEqual(HCContourIsClosed(HCPathContourAt(path, 0)), false));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(HCPathContourAt(path, 0)), HCPointMake(10.0, 80.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(HCPathContourAt(path, 0)), HCPointMake(180.0, 80.0)));
    
    HCRelease(path);
}

CTEST(HCPath, ContourFromCubic) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 90 C 30 10 70 10 90 90");
    
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 1));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(path, 0), 1));
    ASSERT_TRUE(HCCurveIsEqual(HCPathContourCurveAt(path, 0, 0), HCCurveMakeCubic(HCPointMake(10.0, 90.0), HCPointMake(30.0, 10.0), HCPointMake(70.0, 10.0), HCPointMake(90.0, 90.0))));
    
    ASSERT_TRUE(HCIntegerIsEqual(HCContourCurveCount(HCPathContourAt(path, 0)), HCPathContourCurveCount(path, 0)));
    ASSERT_TRUE(HCBooleanIsEqual(HCContourIsClosed(HCPathContourAt(path, 0)), false));
    ASSERT_TRUE(HCPointIsEqual(HCContourStartPoint(HCPathContourAt(path, 0)), HCPointMake(10.0, 90.0)));
    ASSERT_TRUE(HCPointIsEqual(HCContourEndPoint(HCPathContourAt(path, 0)), HCPointMake(90.0, 90.0)));
    
    HCRelease(path);
}

CTEST(HCPath, ContourParameterCorrespondence) {
    HCPathRef singleContourPath = HCPathCreateWithSVGPathData("M 10 20 C 20 10 40 5 50 25");
    
    ASSERT_TRUE(HCPathContourCount(singleContourPath) == 1);
    ASSERT_TRUE(HCPathContourContainingParameter(singleContourPath, 0.25) == 0);
    ASSERT_TRUE(HCPathContourParameterForParameter(singleContourPath, 0.25) == 0.25);
    ASSERT_TRUE(HCPathParameterForContourParameter(singleContourPath, 0, 0.25) == 0.25);
    
    HCPathRef path = HCPathCreateWithSVGPathData(knight);
    
    ASSERT_TRUE(HCPathContourCount(path) == 4);
    ASSERT_TRUE(HCPathContourContainingParameter(path, 0.30) == 1);
    ASSERT_TRUE(HCPathContourParameterForParameter(path, 0.30) == 0.19999999999999996);
    ASSERT_TRUE(HCPathParameterForContourParameter(path, 1, 0.5) == 0.375);
    
    for (HCInteger contourIndex = 0; contourIndex < HCPathContourCount(path); contourIndex++) {
        for (HCReal t = 0.0; t != 1.0; t = t + 0.001 < 1.0 ? t + 0.001 : 1.0) {
            if (HCPathContourContainingParameter(path, t) == contourIndex) {
                HCReal ct = HCPathParameterForContourParameter(path, contourIndex, HCPathContourParameterForParameter(path, t));
                ASSERT_TRUE(HCRealIsSimilar(ct, t, 0.000001));
            }
        }
    }
    
    HCRelease(singleContourPath);
    HCRelease(path);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour / Path Element Correspondence
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCPath, ContourIndexFromElement) {
    HCListRef subPaths = HCListCreate();
    HCContourComponent a[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 0.0, .y = 0.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 5.0, .y = 0.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 5.0, .y = 5.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 0.0, .y = 5.0}},
    };
    HCContour* contourA = HCContourInitInComponents(a, sizeof(a) / sizeof(HCContourComponent), true);
    HCPathRef pathA = HCPathCreateWithContour(contourA);
    HCListAddObject(subPaths, pathA);
    HCContourComponent b[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 10.0, .y = 10.0}},
        {.c0 = {.x = 15.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 20.0, .y = 10.0}},
        {.c0 = {.x = 30.0, .y = 22.5}, .c1 = {.x = 30.0, .y = 27.5}, .p = {.x = 20.0, .y = 20.0}},
        {.c0 = {.x = 15.0, .y = 30.0}, .c1 = HCPointInvalidStatic, .p = {.x = 10.0, .y = 20.0}},
    };
    HCContour* contourB = HCContourInitInComponents(b, sizeof(b) / sizeof(HCContourComponent), true);
    HCPathRef pathB = HCPathCreateWithContour(contourB);
    HCListAddObject(subPaths, pathB);
    HCPathRef path = HCPathCreateWithSubpaths(subPaths);
    HCRelease(subPaths);
    
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 2));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(path, 0), HCContourCurveCount(contourA)));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(path, 1), HCContourCurveCount(contourB)));
    for (HCInteger elementIndex = 0; elementIndex < HCPathElementCount(pathA); elementIndex++) {
        ASSERT_TRUE(HCIntegerIsEqual(HCPathIndexOfContourContainingElement(path, elementIndex), 0));
    }
    for (HCInteger elementIndex = 0; elementIndex < HCPathElementCount(pathB); elementIndex++) {
        ASSERT_TRUE(HCIntegerIsEqual(HCPathIndexOfContourContainingElement(path, HCPathElementCount(pathA) + elementIndex), 1));
    }
    
    ASSERT_TRUE(HCContourIsEqual(HCPathCurrentContour(pathA), contourA));
    ASSERT_TRUE(HCContourIsEqual(HCPathCurrentContour(pathB), contourB));
    ASSERT_TRUE(HCContourIsEqual(HCPathCurrentContour(path), contourB));
    
    HCRelease(pathA);
    HCRelease(pathB);
    HCRelease(path);
}

CTEST(HCPath, Contours) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 1 2 L 3 4 M 3 4 Q 5 6 7 8 L 9 0 Z M 5 6 C 7 8 9 0 1 2 Z");
    HCListRef contours = HCPathContourPathsRetained(path);
    ASSERT_EQUAL(HCListCount(contours), 3);
    HCPathRef contour0 = HCListObjectAtIndex(contours, 0);
    ASSERT_EQUAL(HCPathElementCount(contour0), 2);
    ASSERT_TRUE(HCPathElementAt(contour0, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(contour0, 1).command == HCPathCommandAddLine);
    HCPathRef contour1 = HCListObjectAtIndex(contours, 1);
    ASSERT_EQUAL(HCPathElementCount(contour1), 4);
    ASSERT_TRUE(HCPathElementAt(contour1, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(contour1, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPathElementAt(contour1, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPathElementAt(contour1, 3).command == HCPathCommandCloseContour);
    HCPathRef contour2 = HCListObjectAtIndex(contours, 2);
    ASSERT_EQUAL(HCPathElementCount(contour2), 3);
    ASSERT_TRUE(HCPathElementAt(contour2, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(contour2, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(contour2, 2).command == HCPathCommandCloseContour);
    HCRelease(path);
    HCRelease(contours);
}

CTEST(HCPath, OpenContours) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 1 2 L 3 4 M 3 4 Q 5 6 7 8 L 9 0 Z M 5 6 C 7 8 9 0 1 2 Z");
    HCListRef openContours = HCPathOpenContourPathsRetained(path);
    ASSERT_EQUAL(HCListCount(openContours), 1);
    HCPathRef contour0 = HCListObjectAtIndex(openContours, 0);
    ASSERT_EQUAL(HCPathElementCount(contour0), 2);
    ASSERT_TRUE(HCPathElementAt(contour0, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(contour0, 1).command == HCPathCommandAddLine);
    HCRelease(path);
    HCRelease(openContours);
}

CTEST(HCPath, ClosedContours) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 1 2 L 3 4 M 3 4 Q 5 6 7 8 L 9 0 Z M 5 6 C 7 8 9 0 1 2 Z");
    HCListRef closedContours = HCPathClosedContourPathsRetained(path);
    ASSERT_EQUAL(HCListCount(closedContours), 2);
    HCPathRef contour0 = HCListObjectAtIndex(closedContours, 0);
    ASSERT_EQUAL(HCPathElementCount(contour0), 4);
    ASSERT_TRUE(HCPathElementAt(contour0, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(contour0, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPathElementAt(contour0, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPathElementAt(contour0, 3).command == HCPathCommandCloseContour);
    HCPathRef contour1 = HCListObjectAtIndex(closedContours, 1);
    ASSERT_EQUAL(HCPathElementCount(contour1), 3);
    ASSERT_TRUE(HCPathElementAt(contour1, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(contour1, 1).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(contour1, 2).command == HCPathCommandCloseContour);
    HCRelease(path);
    HCRelease(closedContours);
}

CTEST(HCPath, OpenContoursAsPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 1 2 L 3 4 M 3 4 Q 5 6 7 8 L 9 0 Z M 5 6 C 7 8 9 0 1 2 Z");
    HCPathRef open = HCPathOpenContoursAsPathRetained(path);
    ASSERT_EQUAL(HCPathElementCount(open), 2);
    ASSERT_TRUE(HCPathElementAt(open, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(open, 1).command == HCPathCommandAddLine);
    HCRelease(path);
    HCRelease(open);
}

CTEST(HCPath, ClosedContoursAsPath) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 1 2 L 3 4 M 3 4 Q 5 6 7 8 L 9 0 Z M 5 6 C 7 8 9 0 1 2 Z");
    HCPathRef closed = HCPathClosedContoursAsPathRetained(path);
    ASSERT_EQUAL(HCPathElementCount(closed), 7);
    ASSERT_TRUE(HCPathElementAt(closed, 0).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(closed, 1).command == HCPathCommandAddQuadraticCurve);
    ASSERT_TRUE(HCPathElementAt(closed, 2).command == HCPathCommandAddLine);
    ASSERT_TRUE(HCPathElementAt(closed, 3).command == HCPathCommandCloseContour);
    ASSERT_TRUE(HCPathElementAt(closed, 4).command == HCPathCommandMove);
    ASSERT_TRUE(HCPathElementAt(closed, 5).command == HCPathCommandAddCubicCurve);
    ASSERT_TRUE(HCPathElementAt(closed, 6).command == HCPathCommandCloseContour);
    HCRelease(path);
    HCRelease(closed);
}

CTEST(HCPath, ContourStrangePaths) {
    HCPathRef empty = HCPathCreate();
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(empty), 0));
    HCPathRemoveElement(empty);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(empty), 0));
    HCRelease(empty);
    
    HCPathRef singleMove = HCPathCreateWithSVGPathData("M 1 2");
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(singleMove), 1));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(singleMove, 0), 0));
    ASSERT_TRUE(!HCPathContourIsClosed(singleMove, 0));
    ASSERT_TRUE(HCIntegerIsEqual(HCContourComponentCount(HCPathContourAt(singleMove, 0)), 1));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(HCPathContourAt(singleMove, 0), 0), HCContourComponentMakeLinear(HCPointMake(1.0, 2.0))));
    HCPathRemoveElement(singleMove);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(singleMove), 0));
    HCRelease(singleMove);
    
    HCPathRef singleLine = HCPathCreateWithSVGPathData("L 1 2");
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(singleLine), 1));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(singleLine, 0), 1));
    ASSERT_TRUE(HCCurveIsEqual(HCPathContourCurveAt(singleLine, 0, 0), HCCurveMakeLinear(HCPointMake(0.0, 0.0), HCPointMake(1.0, 2.0))));
    ASSERT_TRUE(!HCPathContourIsClosed(singleLine, 0));
    ASSERT_TRUE(HCIntegerIsEqual(HCContourComponentCount(HCPathContourAt(singleLine, 0)), 2));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(HCPathContourAt(singleLine, 0), 0), HCContourComponentMakeLinear(HCPointMake(0.0, 0.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(HCPathContourAt(singleLine, 0), 1), HCContourComponentMakeLinear(HCPointMake(1.0, 2.0))));
    HCPathRemoveElement(singleLine);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(singleLine), 0));
    HCRelease(singleLine);
    
    HCPathRef singleQuadratic = HCPathCreateWithSVGPathData("Q 1 2 3 4");
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(singleQuadratic), 1));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(singleQuadratic, 0), 1));
    ASSERT_TRUE(HCCurveIsEqual(HCPathContourCurveAt(singleQuadratic, 0, 0), HCCurveMakeQuadratic(HCPointMake(0.0, 0.0), HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0))));
    ASSERT_TRUE(!HCPathContourIsClosed(singleQuadratic, 0));
    ASSERT_TRUE(HCIntegerIsEqual(HCContourComponentCount(HCPathContourAt(singleQuadratic, 0)), 2));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(HCPathContourAt(singleQuadratic, 0), 0), HCContourComponentMakeLinear(HCPointMake(0.0, 0.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(HCPathContourAt(singleQuadratic, 0), 1), HCContourComponentMakeQuadratic(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0))));
    HCPathRemoveElement(singleQuadratic);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(singleQuadratic), 0));
    HCRelease(singleQuadratic);
    
    HCPathRef singleCubic = HCPathCreateWithSVGPathData("C 1 2 3 4 5 6");
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(singleCubic), 1));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(singleCubic, 0), 1));
    ASSERT_TRUE(HCCurveIsEqual(HCPathContourCurveAt(singleCubic, 0, 0), HCCurveMakeCubic(HCPointMake(0.0, 0.0), HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0))));
    ASSERT_TRUE(!HCPathContourIsClosed(singleCubic, 0));
    ASSERT_TRUE(HCIntegerIsEqual(HCContourComponentCount(HCPathContourAt(singleCubic, 0)), 2));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(HCPathContourAt(singleCubic, 0), 0), HCContourComponentMakeLinear(HCPointMake(0.0, 0.0))));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(HCPathContourAt(singleCubic, 0), 1), HCContourComponentMakeCubic(HCPointMake(1.0, 2.0), HCPointMake(3.0, 4.0), HCPointMake(5.0, 6.0))));
    HCPathRemoveElement(singleCubic);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(singleCubic), 0));
    HCRelease(singleCubic);
    
    HCPathRef singleClose = HCPathCreateWithSVGPathData("Z");
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(singleClose), 0));
    HCPathRemoveElement(singleClose);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(singleClose), 0));
    HCRelease(singleClose);
    
    HCPathRef doubleMove = HCPathCreateWithSVGPathData("M 1 2 M 3 4");
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(doubleMove), 2));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(doubleMove, 0), 0));
    ASSERT_TRUE(HCIntegerIsEqual(HCContourComponentCount(HCPathContourAt(doubleMove, 0)), 1));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(HCPathContourAt(doubleMove, 0), 0), HCContourComponentMakeLinear(HCPointMake(1.0, 2.0))));
    ASSERT_TRUE(HCIntegerIsEqual(HCContourComponentCount(HCPathContourAt(doubleMove, 1)), 1));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(HCPathContourAt(doubleMove, 1), 0), HCContourComponentMakeLinear(HCPointMake(3.0, 4.0))));
    HCPathRemoveElement(doubleMove);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(doubleMove), 1));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(doubleMove, 0), 0));
    ASSERT_TRUE(HCIntegerIsEqual(HCContourComponentCount(HCPathContourAt(doubleMove, 0)), 1));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(HCPathContourAt(doubleMove, 0), 0), HCContourComponentMakeLinear(HCPointMake(1.0, 2.0))));
    ASSERT_TRUE(!HCPathContourIsClosed(doubleMove, 0));
    HCPathRemoveElement(doubleMove);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(doubleMove), 0));
    HCRelease(doubleMove);
    
    HCPathRef doubleClose = HCPathCreateWithSVGPathData("Z Z");
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(doubleClose), 0));
    HCPathRemoveElement(doubleClose);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(doubleClose), 0));
    HCRelease(doubleClose);
    
    HCPathRef moveClose = HCPathCreateWithSVGPathData("M 1 2 Z");
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(moveClose), 1));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(moveClose, 0), 0));
    ASSERT_TRUE(HCPathContourIsClosed(moveClose, 0));
    ASSERT_TRUE(HCIntegerIsEqual(HCContourComponentCount(HCPathContourAt(moveClose, 0)), 1));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(HCPathContourAt(moveClose, 0), 0), HCContourComponentMakeLinear(HCPointMake(1.0, 2.0))));
    ASSERT_TRUE(HCContourIsClosed(HCPathContourAt(moveClose, 0)));
    HCPathRemoveElement(moveClose);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(moveClose), 1));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCurveCount(moveClose, 0), 0));
    ASSERT_TRUE(!HCPathContourIsClosed(moveClose, 0));
    ASSERT_TRUE(HCIntegerIsEqual(HCContourComponentCount(HCPathContourAt(moveClose, 0)), 1));
    ASSERT_TRUE(HCContourComponentIsEqual(HCContourComponentAt(HCPathContourAt(moveClose, 0), 0), HCContourComponentMakeLinear(HCPointMake(1.0, 2.0))));
    ASSERT_TRUE(!HCContourIsClosed(HCPathContourAt(moveClose, 0)));
    HCPathRemoveElement(moveClose);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(moveClose), 0));
    HCRelease(moveClose);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCPath, Operations) {
    // NOTE: These tests provide only detection of a change in functionality, as the values were obtained by running the test itself
    
    HCPathRef path = HCPathCreateWithSVGPathData(maze);
    
    ASSERT_TRUE(HCPointIsSimilar(HCPathValue(path, 0.25), HCPointMake(150.5, 35.6), 0.1));
    ASSERT_TRUE(HCCurveIsSimilar(HCPathTangent(path, 0.25), HCCurveMakeLinear(HCPathValue(path, 0.25), HCPointMake(175.5, 33.2)), 0.1));
    ASSERT_TRUE(HCCurveIsSimilar(HCPathTangentUnit(path, 0.25), HCCurveMakeLinear(HCPathValue(path, 0.25), HCPointMake(151.5, 35.5)), 0.1));
    ASSERT_TRUE(HCCurveIsSimilar(HCPathNormal(path, 0.25), HCCurveMakeLinear(HCPathValue(path, 0.25), HCPointMake(152.9, 60.6)), 0.1));
    ASSERT_TRUE(HCCurveIsSimilar(HCPathNormalUnit(path, 0.25), HCCurveMakeLinear(HCPathValue(path, 0.25), HCPointMake(150.6, 36.6)), 0.1));
    ASSERT_TRUE(HCRealIsSimilar(HCPathCurvature(path, 0.25), 0.00781, 0.00001));
    ASSERT_TRUE(HCCurveIsSimilar(HCPathCurvatureNormal(path, 0.25), HCCurveMakeLinear(HCPathValue(path, 0.25), HCPointMake(150.5, 35.6)), 0.1));
    ASSERT_TRUE(HCRealIsSimilar(HCPathParameterAtLength(path, 1000.0), 0.09457, 0.00001));
    ASSERT_TRUE(HCRealIsSimilar(HCPathParameterNearestPoint(path, HCPointMake(100.0, 100.0)), 0.96226, 0.00001));
    ASSERT_TRUE(HCRealIsSimilar(HCPathDistanceFromPoint(path, HCPointMake(100.0, 100.0)), 74.66778, 0.00001));
    
    HCPathRef otherPath = HCPathCreateWithSVGPathData(knight);
    
    HCInteger intersectionCount = 1000;
    HCReal t[intersectionCount];
    HCReal u[intersectionCount];
    HCPathIntersections(path, otherPath, &intersectionCount, t, u);
    ASSERT_TRUE(intersectionCount == 69);
    
    HCRelease(path);
    HCRelease(otherPath);
}

// TODO: Test these!
//HCPoint HCPathValue(HCPathRef self, HCReal t);
//HCCurve HCPathTangent(HCPathRef self, HCReal t);
//HCCurve HCPathTangentUnit(HCPathRef self, HCReal t);
//HCCurve HCPathNormal(HCPathRef self, HCReal t);
//HCCurve HCPathNormalUnit(HCPathRef self, HCReal t);
//HCReal HCPathCurvature(HCPathRef self, HCReal t);
//HCCurve HCPathCurvatureNormal(HCPathRef self, HCReal t);
//HCReal HCPathParameterAtLength(HCPathRef self, HCReal d);
//HCReal HCPathParameterNearestPoint(HCPathRef self, HCPoint p);
//HCReal HCPathDistanceFromPoint(HCPathRef self, HCPoint p);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Intersection
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCPath, ContainsPoint) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 L 30 10 30 20 10 20 Z M 18 14 L 22 14 22 16 18 16 Z");
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 15.0)));
    ASSERT_TRUE(HCPathContainsPoint(path, HCPointMake(11.0, 15.0)));
    ASSERT_TRUE(HCPathContainsPoint(path, HCPointMake(29.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(9.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(31.0, 15.0)));
    ASSERT_TRUE(HCPathContainsPoint(path, HCPointMake(20.0, 11.0)));
    ASSERT_TRUE(HCPathContainsPoint(path, HCPointMake(20.0, 19.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 9.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 21.0)));
    HCRelease(path);
}

CTEST(HCPath, OpenPathContainsPoint) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 L 30 10 30 20 10 20");
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(11.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(29.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(9.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(31.0, 15.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 11.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 19.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 9.0)));
    ASSERT_FALSE(HCPathContainsPoint(path, HCPointMake(20.0, 21.0)));
    HCRelease(path);
}

CTEST(HCPath, ContainsPointNonZero) {
    HCPathRef path = HCPathCreateWithSVGPathData("M 10 10 L 30 10 30 20 10 20 Z M 18 14 L 22 14 22 16 18 16 Z");
    // TODO: Enable when this test is available
//    ASSERT_TRUE(HCPathContainsPointNonZero(path, HCPointMake(20.0, 15.0)));
//    ASSERT_TRUE(HCPathContainsPointNonZero(path, HCPointMake(11.0, 15.0)));
//    ASSERT_TRUE(HCPathContainsPointNonZero(path, HCPointMake(29.0, 15.0)));
//    ASSERT_FALSE(HCPathContainsPointNonZero(path, HCPointMake(9.0, 15.0)));
//    ASSERT_FALSE(HCPathContainsPointNonZero(path, HCPointMake(31.0, 15.0)));
//    ASSERT_TRUE(HCPathContainsPointNonZero(path, HCPointMake(20.0, 11.0)));
//    ASSERT_TRUE(HCPathContainsPointNonZero(path, HCPointMake(20.0, 19.0)));
//    ASSERT_FALSE(HCPathContainsPointNonZero(path, HCPointMake(20.0, 9.0)));
//    ASSERT_FALSE(HCPathContainsPointNonZero(path, HCPointMake(20.0, 21.0)));
    ASSERT_FALSE(HCPathContainsPointNonZero(path, HCPointZero));
    HCRelease(path);
}

CTEST(HCPath, LinePathLinePathIntersection) {
    HCPathRef a = HCPathCreateWithSVGPathData("M 10 20 L 90 100");
    HCPathRef b = HCPathCreateWithSVGPathData("M 70 10 L 20 80");
    ASSERT_TRUE(HCPathIntersectsPath(a, b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCPath, QuadraticPathQuadraticPathIntersection) {
    HCPathRef a = HCPathCreateWithSVGPathData("M 10 30 Q 50 90 90 30");
    HCPathRef b = HCPathCreateWithSVGPathData("M 10 70 Q 50 10 90 70");
    ASSERT_TRUE(HCPathIntersectsPath(a, b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCPath, CubicPathCubicPathIntersection) {
    HCPathRef a = HCPathCreateWithSVGPathData("M 10 30 C 30 90 70 90 90 30");
    HCPathRef b = HCPathCreateWithSVGPathData("M 10 70 C 30 10 70 10 90 70");
    ASSERT_TRUE(HCPathIntersectsPath(a, b));
    HCRelease(a);
    HCRelease(b);
}

// TODO: Test these!
//void HCPathIntersections(HCPathRef self, HCPathRef other, HCInteger* count, HCReal* t, HCReal* u);
//void HCPathForEachIntersection(HCPathRef self, HCPathRef other, HCPathIntersectionFunction intersection, void* context);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Manipulation
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCPath, MutabilityAndCurrentPoint) {
    HCPathRef path = HCPathCreate();
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointZero));
    HCPathMove(path, 5.0, 10.0);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(5.0, 10.0)));
    HCPathAddLine(path, 50.0, 100.0);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(50.0, 100.0)));
    HCPathAddQuadraticCurve(path, 8.0, 40.0, -20.0, 60.0);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(-20.0, 60.0)));
    HCPathAddCubicCurve(path, 80.0, 45.0, -10.0, -80.0, 35.0, 77.0);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(35.0, 77.0)));
    HCPathClose(path);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(5.0, 10.0)));
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(35.0, 77.0)));
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(-20.0, 60.0)));
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(50.0, 100.0)));
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointMake(5.0, 10.0)));
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointZero));
    HCRelease(path);
}

CTEST(HCPath, MutabilityAndContours) {
    HCPathRef path = HCPathCreate();
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointZero));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 0));
    HCPathAddLine(path, 50.0, 100.0);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 1));
    HCPathAddQuadraticCurve(path, 8.0, 40.0, -20.0, 60.0);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 1));
    HCPathAddCubicCurve(path, 80.0, 45.0, -10.0, -80.0, 35.0, 77.0);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 1));
    HCPathClose(path);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 1));
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 1));
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 1));
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 1));
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 0));
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathContourCount(path), 0));
    HCRelease(path);
}

CTEST(HCPath, MutabilityAndPolylines) {
    HCPathRef path = HCPathCreate();
    ASSERT_TRUE(HCPointIsEqual(HCPathCurrentPoint(path), HCPointZero));
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylineCount(path), 0));
    HCPathAddLine(path, 50.0, 100.0);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylineCount(path), 1));
    HCPathAddQuadraticCurve(path, 8.0, 40.0, -20.0, 60.0);
    ASSERT_TRUE(HCPathPolylineCount(path) > 1);
    HCPathAddCubicCurve(path, 80.0, 45.0, -10.0, -80.0, 35.0, 77.0);
    ASSERT_TRUE(HCPathPolylineCount(path) > 1);
    HCPathClose(path);
    ASSERT_TRUE(HCPathPolylineCount(path) > 1);
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCPathPolylineCount(path) > 1);
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCPathPolylineCount(path) > 1);
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylineCount(path), 1));
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylineCount(path), 0));
    HCPathRemoveElement(path);
    ASSERT_TRUE(HCIntegerIsEqual(HCPathPolylineCount(path), 0));
    HCRelease(path);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Boolean Operations
// HCPath+SetOperations.h
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCPath, Union) {
    HCPathRef a = HCPathCreateWithSVGPathData("M 10 0 C 30 15 70 15 90 0 C 100 15 100 35 90 50 C 70 35 30 35 10 50 C 0 35 0 15 10 0 Z");
    HCPathRef b = HCPathCreateByTranslatingPath(a, 20.0, 5.0);
    HCPathRef c = HCPathCombine(a, b, HCPathCombineOperationUnion);
    ASSERT_TRUE(!HCPathIsEmpty(c));
    
    HCRasterRef raster = HCRasterCreate(160.0, 90.0);
    HCRasterDrawPath(raster, a, HCColorRed);
    HCRasterDrawPath(raster, b, HCColorGreen);
    HCRasterDrawPath(raster, c, HCColorBlue);
    
    HCRasterSaveBMP(raster, "union.bmp");
    HCRasterSavePPM(raster, "union.ppm");
    
    HCRelease(a);
    HCRelease(b);
    HCRelease(c);
    HCRelease(raster);
}
