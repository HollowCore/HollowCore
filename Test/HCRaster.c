///
/// @file HCRaster.c
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 2/3/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#include "ctest.h"
#include "../Source/HollowCore.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Test Data
//----------------------------------------------------------------------------------------------------------------------------------
static const char* dinosaur = "m 134.51827,42.745763 c -16.01255,0 -29.04048,13.025256 -29.04048,29.035206 0,3.45164 2.79845,6.25017 6.24999,6.25017 3.45153,0 6.24999,-2.79853 6.24999,-6.25017 0,-9.115845 7.41881,-16.534865 16.5405,-16.534865 9.1217,0 16.54052,7.41902 16.54052,16.534865 0,3.45164 2.79845,6.25017 6.24999,6.25017 3.45153,0 6.24999,-2.79853 6.24999,-6.25017 0,-16.00995 -13.02794,-29.035206 -29.0405,-29.035206 z M 495.72086,350.60935 c -3.53088,-1.17497 -9.09728,-1.26652 -19.25962,-1.26652 -0.49745,-0.003 -1.01013,-0.003 -1.53808,0 -39.42867,0 -81.56424,-2.84431 -84.32913,-35.79504 -11.33422,-135.05921 -116.9829,-147.15671 -161.75824,-146.02447 -10.50414,0.14955 -20.07138,-4.68153 -26.00094,-13.18701 -5.93567,-8.51463 -7.15636,-18.9031 -3.44543,-28.30887 0.4028,-0.84231 9.96703,-20.87154 12.90587,-44.819331 C 216.36328,48.06512 205.71876,23.735847 181.51835,10.847922 152.49311,-4.6035481 121.14854,-3.5323515 90.881248,13.95775 86.559958,16.451099 81.701568,17.50704 76.922519,17.073678 61.660699,15.624052 33.441124,15.633208 15.637188,31.844586 5.2612254,41.28698 0,54.650994 0,71.564289 c 0,17.10862 5.7861249,31.311891 17.199686,42.210011 20.364358,19.45243 53.131048,22.86744 68.170092,23.36184 -7.434074,38.31281 -7.208249,70.08552 -7.192984,71.44359 0,32.52881 7.423389,82.77127 22.937366,123.33099 L 89.694108,437.47999 c -0.43945,6.22271 1.751715,12.41184 6.00586,16.97739 4.254145,4.56556 10.275252,7.18403 16.516092,7.18403 h 11.01111 l -0.9739,14.20022 c -0.4364,6.22576 1.75476,12.41489 6.00891,16.97739 4.25415,4.5625 10.2722,7.18098 16.51304,7.18098 h 46.24018 c 6.24084,0 12.2589,-2.61848 16.51305,-7.18403 4.25414,-4.56251 6.4453,-10.75164 6.01805,-16.72104 l -2.31323,-78.71002 c 7.28569,1.85857 15.16913,2.94808 23.24903,3.3513 l 0.8812,37.49759 c 0.9491,12.90623 11.83165,23.40761 24.26145,23.40761 h 9.24528 l 0.44705,13.77601 C 270.7852,491.26868 280.47146,500 296.58778,500 h 33.12985 c 8.62731,0 15.61887,-2.71004 20.21786,-7.84018 4.29382,-4.78529 6.12488,-11.31317 5.3009,-18.84511 l -5.1239,-49.2262 c 68.27996,23.81962 128.78099,-23.17567 149.24605,-64.79058 0.79655,-1.62052 0.8545,-3.50656 0.15565,-5.17286 -0.69579,-1.66326 -2.07824,-2.94808 -3.79333,-3.51572 z m -383.5048,98.53172 c -2.82593,0 -5.44433,-1.13833 -7.37304,-3.20748 -1.92566,-2.0661 -2.87781,-4.75782 -2.70081,-7.34273 l 9.08813,-83.98627 c 5.21583,9.92077 11.06757,18.67612 17.5781,25.66254 l -4.72373,68.87394 z m 147.40889,0 c -5.8197,0 -11.33422,-5.52383 -11.78283,-11.51154 l -0.85907,-36.83534 c 6.66847,-0.28345 13.34876,-1.02503 19.83793,-2.28926 l 1.64337,50.63614 z m 85.74819,-40.3728 c -2.03552,-0.90639 -4.40979,-0.66225 -6.21338,0.647 -1.80969,1.31229 -2.78014,3.4852 -2.54821,5.70694 l 6.19506,59.52004 c 0.4364,3.98876 -0.296,7.07417 -2.17895,9.17383 -2.72827,3.04574 -7.44018,3.68358 -10.91002,3.68358 h -33.12985 c -9.65881,0 -13.95262,-3.83617 -14.80101,-12.86046 l -2.71911,-84.11178 c -0.0613,-1.9013 -0.98572,-3.67136 -2.5116,-4.8097 -1.08947,-0.81179 -2.40173,-1.23905 -3.73534,-1.23905 -0.531,0 -1.06507,0.0671 -1.58692,0.2045 -21.36532,5.60929 -46.02655,4.95619 -64.3737,-1.69377 -1.94092,-0.70497 -4.10766,-0.40285 -5.79223,0.80874 -1.67846,1.21463 -2.64587,3.18001 -2.58483,5.24916 l 2.58178,87.67023 c 0.19835,2.8199 -0.75378,5.50857 -2.67944,7.57772 -1.92566,2.06607 -4.54406,3.2044 -7.36999,3.2044 h -46.24018 c -2.82592,0 -5.44433,-1.13833 -7.36999,-3.20443 -1.92871,-2.0661 -2.88085,-4.75782 -2.68249,-7.58993 l 6.74743,-98.35472 c 0.1251,-1.80669 -0.54019,-3.57675 -1.8219,-4.85548 -10.29509,-10.258 -19.01624,-26.00664 -26.14056,-44.22536 -0.04,-0.1453 -0.10415,-0.2724 -0.15415,-0.4135 -14.885312,-38.27199 -22.676822,-87.41654 -22.676822,-120.33751 -0.003,-0.35705 -0.2441,-36.12159 8.46252,-76.29297 0.40895,-1.8952 -0.08545,-3.87279 -1.33362,-5.35598 -1.251225,-1.48014 -3.091425,-2.24921 -5.056755,-2.21259 -0.4303,0.0245 -44.314526,1.70599 -66.943288,-19.94683 C 16.851762,96.143829 12.49996,85.303689 12.49996,71.564299 c 0,-13.24804 3.884883,-23.502225 11.550894,-30.475677 14.251689,-12.973377 38.510085,-12.811629 51.693665,-11.569527 7.406604,0.717182 14.813214,-0.933869 21.386694,-4.73646 26.342737,-15.2195321 53.497247,-16.2235841 78.512477,-2.899247 47.17707,25.116651 12.83568,97.982392 12.48473,98.714832 -0.0641,0.13425 -0.21055,0.48835 -0.2655,0.62563 -5.29784,13.37011 -3.53698,28.4218 4.71801,40.26294 8.33434,11.95711 21.85362,18.93972 36.55697,18.53077 51.29694,-1.15054 138.93111,14.59086 148.99885,134.57397 3.96728,47.30048 64.54155,47.19061 97.21363,47.25165 2.74048,-0.01 5.67626,-0.003 8.28857,0.0395 -22.85764,36.89981 -77.94791,73.78435 -138.26583,46.88543 z";

const char* maze = "M 140.37258300203047,185.62741699796953 A 32,32 0 1,1 163,195 M 196.94112549695427,129.05887450304573 A 48,48 0 0,1 207.34621756054176,144.6311952464757 M 207.34621756054176,181.3688047535243 A 48,48 0 1,1 144.63119524647567,118.65378243945824 M 138.50826032863427,222.12829008072237 A 64,64 0 1,1 208.25483399593904,117.74516600406096 M 222.12829008072237,138.50826032863427 A 64,64 0 0,1 208.25483399593904,208.25483399593904 M 147.39277423870971,84.537177567741566 A 80,80 0 0,1 241.46282243225843,147.39277423870973 M 243,163 A 80,80 0 0,1 193.6146745892072,236.91036260090294 M 178.60722576129029,241.46282243225843 A 80,80 0 0,1 118.55438135843184,229.51756898420365 M 106.43145750507621,219.5685424949238 A 80,80 0 0,1 96.482431015796379,207.44561864156816 M 84.537177567741566,178.60722576129029 A 80,80 0 0,1 83,163.00000000000003 M 84.537177567741566,147.39277423870973 A 80,80 0 0,1 118.55438135843181,96.482431015796379 M 126.26239049295131,74.3075648789165 A 96,96 0 0,1 181.72867091354831,68.844613081289879 M 242.82108278104434,109.66525763011819 A 96,96 0 0,1 251.69243512108352,126.26239049295137 M 257.15538691871012,144.27132908645169 A 96,96 0 0,1 259,163 M 257.15538691871012,181.72867091354831 A 96,96 0 0,1 144.27132908645169,257.15538691871012 M 109.66525763011822,242.82108278104437 A 96,96 0 0,1 95.117749006091444,230.88225099390857 M 74.3075648789165,199.73760950704866 A 96,96 0 0,1 68.844613081289879,181.72867091354834 M 67,163.00000000000003 A 96,96 0 0,1 74.30756487891648,126.26239049295137 M 141.14988393419358,53.152048594838206 A 112,112 0 0,1 163,51 M 184.85011606580636,53.152048594838192 A 112,112 0 0,1 225.22386609819546,69.875403422114928 M 256.12459657788509,100.77613390180454 A 112,112 0 0,1 272.84795140516178,141.14988393419364 M 275,163 A 112,112 0 0,1 163,275 M 100.77613390180459,256.12459657788509 A 112,112 0 0,1 59.5254923587359,205.86054442489012 M 53.152048594838192,184.85011606580642 A 112,112 0 0,1 53.152048594838192,141.14988393419364 M 59.525492358735889,120.13945557510992 A 112,112 0 0,1 69.8754034221149,100.7761339018046 M 83.804040507106663,83.804040507106691 A 112,112 0 0,1 120.13945557510988,59.525492358735917 M 138.02843878193554,37.459484108386519 A 128,128 0 0,1 163,35 M 187.97156121806441,37.459484108386505 A 128,128 0 0,1 211.9834793427315,44.7434198385553 M 253.50966799187808,72.490332008121911 A 128,128 0 0,1 269.4281103747258,91.8870101734909 M 288.5405158916135,138.02843878193556 A 128,128 0 0,1 288.5405158916135,187.97156121806441 M 281.25658016144473,211.98347934273147 A 128,128 0 0,1 269.4281103747258,234.11298982650905 M 234.11298982650908,269.4281103747258 A 128,128 0 0,1 211.9834793427315,281.25658016144473 M 163.00000000000003,291 A 128,128 0 0,1 72.490332008121925,253.5096679918781 M 44.743419838555326,211.98347934273156 A 128,128 0 0,1 35,163.00000000000003 M 37.459484108386505,138.02843878193556 A 128,128 0 0,1 44.743419838555312,114.01652065726847 M 56.571889625274181,91.887010173490964 A 128,128 0 0,1 91.887010173490921,56.57188962527421 M 163,19 A 144,144 0 0,1 177.11446820745672,19.693399359203653 M 204.80099352464259,25.200591654561919 A 144,144 0 0,1 230.88113010294364,36.003337933836875 M 243.0021135548227,43.268375828433477 A 144,144 0 0,1 254.35263291956494,51.686494715765875 M 264.82337649086287,61.176623509137158 A 144,144 0 0,1 296.03865268162531,107.89358573942707 M 300.79940834543811,121.19900647535744 A 144,144 0 0,1 304.23308037806521,134.90699362967752 M 306.30660064079632,148.88553179254325 A 144,144 0 0,1 307,163 M 304.23308037806521,191.09300637032246 A 144,144 0 0,1 254.35263291956494,274.31350528423411 M 243.0021135548227,282.73162417156652 A 144,144 0 0,1 191.09300637032251,304.23308037806521 M 177.11446820745675,306.30660064079632 A 144,144 0 0,1 134.90699362967752,304.23308037806521 M 121.19900647535745,300.79940834543811 A 144,144 0 0,1 51.686494715765917,254.352632919565 M 43.268375828433477,243.0021135548227 A 144,144 0 0,1 25.200591654561919,204.80099352464259 M 19.693399359203653,177.1144682074567 A 144,144 0 0,1 19,163.00000000000003 M 19.693399359203653,148.88553179254336 A 144,144 0 0,1 21.766919621934818,134.90699362967752 M 29.96134731837472,107.89358573942704 A 144,144 0 0,1 61.17662350913713,61.176623509137173 M 71.647367080434989,51.686494715765932 A 144,144 0 0,1 82.997886445177286,43.268375828433491 M 95.1188698970563,36.0033379338369 A 144,144 0 0,1 148.88553179254328,19.693399359203653 M 178.68274245272968,3.7704437324484843 A 160,160 0 1,1 163,3 M 163 3 L 163 35 M 163 115 L 163 131 M 194.21445152258053 6.0743551354831311 L 187.97156121806441 37.459484108386505 M 184.85011606580636 53.152048594838192 L 181.72867091354831 68.844613081289879 M 218.10641426057293 29.96134731837472 L 211.9834793427315 44.7434198385553 M 199.7376095070486 74.30756487891648 L 193.6146745892072 89.089637399097057 M 187.49173967136574 103.87170991927765 L 181.3688047535243 118.65378243945824 M 243.0021135548227 43.268375828433491 L 216.33474236988181 83.178917218955661 M 264.82337649086287 61.176623509137144 L 219.5685424949238 106.4314575050762 M 208.25483399593904 117.74516600406096 L 196.94112549695427 129.05887450304573 M 256.12459657788509 100.77613390180454 L 242.82108278104434 109.66525763011818 M 296.03865268162531 107.89358573942707 L 281.25658016144473 114.0165206572685 M 236.91036260090294 132.3853254107928 L 222.12829008072237 138.50826032863427 M 316.1104537171534 116.55445163928603 L 300.79940834543811 121.19900647535742 M 304.23308037806521 134.90699362967752 L 272.84795140516178 141.14988393419364 M 257.15538691871012 144.27132908645169 L 241.46282243225843 147.39277423870973 M 322.22955626755152 147.31725754727029 L 306.30660064079632 148.88553179254325 M 275 163 L 259 163 M 227 163 L 195 163 M 322.22955626755152 178.68274245272971 L 306.30660064079632 177.11446820745672 M 304.23308037806521 191.09300637032246 L 288.5405158916135 187.97156121806441 M 257.15538691871012 181.72867091354831 L 241.46282243225843 178.60722576129027 M 269.4281103747258 234.11298982650905 L 256.12459657788509 225.2238660981954 M 264.82337649086287 264.82337649086287 L 253.50966799187808 253.50966799187808 M 234.11298982650908 269.4281103747258 L 225.22386609819546 256.12459657788509 M 193.6146745892072 236.91036260090294 L 181.36880475352433 207.34621756054176 M 191.09300637032251 304.23308037806521 L 184.85011606580639 272.84795140516178 M 178.68274245272974 322.22955626755152 L 177.11446820745675 306.30660064079632 M 163.00000000000003 291 L 163 275 M 163 243 L 163 227 M 141.14988393419361 272.84795140516178 L 147.39277423870973 241.46282243225843 M 114.01652065726853 281.25658016144473 L 126.2623904929514 251.69243512108352 M 138.50826032863427 222.12829008072237 L 144.6311952464757 207.34621756054176 M 100.77613390180459 256.12459657788509 L 118.55438135843184 229.51756898420365 M 61.176623509137158 264.82337649086287 L 72.490332008121925 253.5096679918781 M 129.05887450304573 196.94112549695427 L 140.37258300203047 185.62741699796953 M 39.318327461962127 264.50292546618334 L 51.686494715765917 254.352632919565 M 43.268375828433491 243.0021135548227 L 96.482431015796379 207.44561864156816 M 29.961347318374749 218.10641426057302 L 44.743419838555326 211.98347934273156 M 74.3075648789165 199.73760950704866 L 103.87170991927766 187.49173967136579 M 6.0743551354831595 194.21445152258059 L 21.766919621934846 191.09300637032254 M 53.152048594838206 184.85011606580642 L 68.844613081289893 181.72867091354834 M 19 163.00000000000003 L 35 163.00000000000003 M 83 163.00000000000003 L 99 163 M 21.766919621934818 134.90699362967752 L 53.152048594838192 141.14988393419364 M 9.8895462828465668 116.55445163928607 L 25.200591654561919 121.19900647535746 M 29.96134731837472 107.89358573942704 L 74.30756487891648 126.26239049295137 M 69.875403422114914 100.7761339018046 L 96.482431015796365 118.55438135843185 M 83.804040507106663 83.804040507106691 L 95.117749006091429 95.117749006091458 M 74.108762716863652 29.964862031592759 L 91.887010173490921 56.57188962527421 M 109.66525763011819 83.178917218955661 L 118.55438135843183 96.482431015796379 M 107.89358573942698 29.961347318374749 L 138.50826032863421 103.87170991927766 M 138.02843878193554 37.459484108386519 L 141.14988393419358 53.152048594838206";

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCRaster, Creation) {
    HCRasterRef raster = HCRasterCreate(1, 1);
    ASSERT_EQUAL(HCRasterWidth(raster), 1);
    ASSERT_EQUAL(HCRasterHeight(raster), 1);
    HCRelease(raster);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCRaster, EqualHash) {
    HCRasterRef a = HCRasterCreate(1, 1);
    HCRasterRef b = HCRasterCreate(1, 1);
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

CTEST(HCRaster, Print) {
    HCRasterRef raster = HCRasterCreate(1, 1);
    HCObjectPrint((HCObjectRef)raster, stdout); // TODO: Not to stdout
    HCPrint(raster, stdout); // TODO: Not to stdout
    HCRelease(raster);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Pixel Operations
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCRaster, PixelOperations) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 50).g == HCColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 49, 50).g == HCColorGreen.g);
    HCRasterSetPixelAt(raster, 50, 50, HCColorGreen);
    ASSERT_TRUE(HCRasterPixelAt(raster, 50, 50).g == HCColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 49, 50).g == HCColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 51, 50).g == HCColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 49).g == HCColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 51).g == HCColorGreen.g);
    HCColor pixels[25*25];
    for (HCInteger i = 0; i < (HCInteger)(sizeof(pixels) / sizeof(HCColor)); i++) {
        pixels[i] = HCColorRed;
    }
    HCRasterSetPixelsAt(raster, 30, 40, 55, 65, pixels);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 50).g == HCColorGreen.g);
    ASSERT_FALSE(HCRasterPixelAt(raster, 49, 50).g == HCColorGreen.g);
    ASSERT_TRUE(HCRasterPixelAt(raster, 50, 50).r == HCColorRed.r);
    ASSERT_TRUE(HCRasterPixelAt(raster, 49, 50).r == HCColorRed.r);
    ASSERT_TRUE(HCRasterPixelAt(raster, 30, 50).r == HCColorRed.r);
    ASSERT_TRUE(HCRasterPixelAt(raster, 54, 50).r == HCColorRed.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 29, 50).r == HCColorRed.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 55, 50).r == HCColorRed.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 39).r == HCColorRed.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 65).r == HCColorRed.r);
    HCRasterSetPixelAt(raster, 30, 40, HCColorBlue);
    ASSERT_TRUE(HCRasterPixelAt(raster, 30, 40).b == HCColorBlue.b);
    HCRasterPixelsAt(raster, 30, 40, 55, 65, pixels);
    ASSERT_TRUE(pixels[0].b == HCColorBlue.b);
    ASSERT_FALSE(pixels[1].b == HCColorBlue.b);
    ASSERT_FALSE(pixels[1*(55-30)+0].b == HCColorBlue.b);
    HCRasterClear(raster);
    ASSERT_FALSE(HCRasterPixelAt(raster, 30, 40).b == HCColorBlue.b);
    HCRelease(raster);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Lookup Operations
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCRaster, Lookup) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(raster); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(raster); xIndex++) {
            HCRasterSetPixelAt(raster, xIndex, yIndex, yIndex < 50 ? (xIndex < 50 ? HCColorRed : HCColorGreen) : (xIndex < 50 ? HCColorWhite : HCColorBlue));
        }
    }
    ASSERT_TRUE(HCRasterPixelNearest(raster, 50.5, 50.5).a == HCRasterPixelFiltered(raster, 50.5, 50.5).a);
    ASSERT_TRUE(HCRasterPixelNearest(raster, 50.5, 50.5).r == HCRasterPixelFiltered(raster, 50.5, 50.5).r);
    ASSERT_TRUE(HCRasterPixelNearest(raster, 50.5, 50.5).g == HCRasterPixelFiltered(raster, 50.5, 50.5).g);
    ASSERT_TRUE(HCRasterPixelNearest(raster, 50.5, 50.5).b == HCRasterPixelFiltered(raster, 50.5, 50.5).b);
    HCRelease(raster);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Data Extraction
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCRaster, ExtractARGB8888) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterSetPixelAt(raster, 10, 20, HCColorWhite);
    uint32_t pixels[100*100];
    HCRasterExtractARGB8888(raster, pixels);
    ASSERT_TRUE(pixels[0] == 0);
    ASSERT_TRUE(pixels[20*100+10] == 0xFFFFFFFF);
    HCRelease(raster);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - File Operations
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCRaster, SaveLoad) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(raster); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(raster); xIndex++) {
            HCRasterSetPixelAt(raster, xIndex, yIndex, yIndex < 50 ? (xIndex < 50 ? HCColorRed : HCColorGreen) : (xIndex < 50 ? HCColorWhite : HCColorBlue));
        }
    }

    HCRasterSavePPM(raster, "test_ppm.ppm");
    HCRasterRef ppm = HCRasterCreateByLoadingPPM("test_ppm.ppm");
    ASSERT_TRUE(HCRasterIsEqual(raster, ppm));
    HCRelease(ppm);
    
    HCRasterSavePPMWithOptions(raster, "test_ppm_binary.ppm", true);
    HCRasterRef ppmBinary = HCRasterCreateByLoadingPPM("test_ppm_binary.ppm");
    ASSERT_TRUE(HCRasterIsEqual(raster, ppmBinary));
    HCRelease(ppmBinary);
    
    HCRasterSaveBMP(raster, "test_bmp.bmp");
    HCRasterRef bmp = HCRasterCreateByLoadingBMP("test_bmp.bmp");
    ASSERT_TRUE(HCRasterIsEqual(raster, bmp));
    HCRelease(bmp);
    
    HCRasterSaveBMPWithOptions(raster, "test_bmp_reversed.bmp", true);
    HCRasterRef bmpReversed = HCRasterCreateByLoadingBMP("test_bmp_reversed.bmp");
    ASSERT_TRUE(HCRasterIsEqual(raster, bmpReversed));
    HCRelease(bmpReversed);
    
    HCRelease(raster);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Line Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCRaster, DrawPoint) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 50).r == HCColorWhite.r);
    HCRasterDrawPoint(raster, 50, 50, HCColorWhite);
    ASSERT_TRUE(HCRasterPixelAt(raster, 50, 50).r == HCColorWhite.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 49, 50).r == HCColorWhite.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 51, 50).r == HCColorWhite.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 49).r == HCColorWhite.r);
    ASSERT_FALSE(HCRasterPixelAt(raster, 50, 51).r == HCColorWhite.r);
    HCRasterSaveBMP(raster, "point.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLine) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawLine(raster, 10, 80, 90, 20, HCColorYellow, HCColorMagenta);
    HCRasterSaveBMP(raster, "line.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurve) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawQuadraticCurve(raster, 10, 90, 50, 10, 90, 90, HCColorMagenta, HCColorCyan);
    HCRasterSaveBMP(raster, "quadratic_curve.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurve) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawCubicCurve(raster, 10, 90, 30, 10, 70, 10, 90, 90, HCColorCyan, HCColorYellow);
    HCRasterSaveBMP(raster, "cubic_curve.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArc) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawArc(raster, 25.0, 75.0, 75.0, 75.0, 32.5, 32.5, 0.0, true, true, HCColorRed, HCColorGreen);
    HCRasterSaveBMP(raster, "arc.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawEllipse) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawEllipse(raster, 50.0, 50.0, 30.0, 45.0, M_PI / 3.0, HCColorYellow, HCColorGreen);
    HCRasterSaveBMP(raster, "ellipse.bmp");
    HCRelease(raster);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Shape Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------

CTEST(HCRaster, DrawRectanglePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRectangle rectangle = HCRectangleMakeWithComponents(10.0, 20.0, 30.0, 40.0);
    HCPathRef path = HCPathCreateRectangle(rectangle);
    HCRasterDrawPath(raster, path, HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_rectangle.bmp");
    HCRelease(path);
    HCRelease(raster);
}

CTEST(HCRaster, DrawEllipsePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRectangle rectangle = HCRectangleMakeWithComponents(10.0, 20.0, 30.0, 40.0);
    HCPathRef path = HCPathCreateEllipse(rectangle);
    HCRasterDrawPath(raster, path, HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_ellipse.bmp");
    HCRelease(path);
    HCRelease(raster);
}

CTEST(HCRaster, DrawTriangle) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawTriangle(raster, 10, 80, 50, 20, 90, 70, HCColorRed, HCColorGreen, HCColorBlue);
    HCRasterSaveBMP(raster, "triangle.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, FillTriangle) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillTriangle(raster, 10, 80, 50, 20, 90, 70, HCColorRed, HCColorGreen, HCColorBlue);
    HCRasterSaveBMP(raster, "triangle_filled.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, FillTexturedTriangle) {
    HCRasterRef texture = HCRasterCreate(100, 100);
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(texture); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(texture); xIndex++) {
            HCRasterSetPixelAt(texture, xIndex, yIndex, yIndex < 50 ? (xIndex < 50 ? HCColorRed : HCColorGreen) : (xIndex < 50 ? HCColorWhite : HCColorBlue));
        }
    }
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillTexturedTriangle(raster, 10, 80, 50, 20, 90, 70, texture, 0, 0, 100, 0, 0, 100);
    HCRasterSaveBMP(texture, "triangle_texture.bmp");
    HCRasterSaveBMP(raster, "triangle_textured.bmp");
    HCRelease(raster);
    HCRelease(texture);
}

CTEST(HCRaster, DrawQuad) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawQuad(raster, 30, 20, 80, 10, 90, 90, 10, 80, HCColorRed, HCColorGreen, HCColorBlue, HCColorWhite);
    HCRasterSaveBMP(raster, "quad.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, FillQuad) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillQuad(raster, 30, 20, 80, 10, 90, 90, 10, 80, HCColorRed, HCColorGreen, HCColorBlue, HCColorWhite);
    HCRasterSaveBMP(raster, "quad_filled.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, FillTexturedQuad) {
    HCRasterRef texture = HCRasterCreate(100, 100);
    for (HCInteger yIndex = 0; yIndex < HCRasterHeight(texture); yIndex++) {
        for (HCInteger xIndex = 0; xIndex < HCRasterWidth(texture); xIndex++) {
            HCRasterSetPixelAt(texture, xIndex, yIndex, yIndex < 50 ? (xIndex < 50 ? HCColorRed : HCColorGreen) : (xIndex < 50 ? HCColorWhite : HCColorBlue));
        }
    }
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillTexturedQuad(raster, 30, 20, 80, 10, 90, 90, 10, 80, texture, 0, 0, 100, 0, 100, 100, 0, 100);
    HCRasterSaveBMP(texture, "quad_texture.bmp");
    HCRasterSaveBMP(raster, "quad_textured.bmp");
    HCRelease(raster);
    HCRelease(texture);
}

CTEST(HCRaster, Gradient) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterFillQuad(raster, -50, 50, 50, -50, 150, 50, 50, 150, HCColorRed, HCColorRed, HCColorGreen, HCColorGreen);
    HCRasterSaveBMP(raster, "gradient.bmp");
    HCRelease(raster);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Contour Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCRaster, DrawPolyline) {
    HCPoint startPoint = {.x = 25.0, .y = 25.0};
    HCPoint points[] = {
        {.x = 75.0, .y = 25.0},
        {.x = 75.0, .y = 75.0},
        {.x = 50.0, .y = 50.0},
    };
    
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPolyline(raster, startPoint, points, sizeof(points) / sizeof(HCPoint), false, HCRasterColorRotating);
    HCRasterSaveBMP(raster, "polyline.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawPolyquadratic) {
    HCPoint startPoint = {.x = 25.0, .y = 75.0};
    HCPoint points[] = {
        {.x = 50.0, .y = 0.0}, {.x = 75.0, .y = 75.0},
        {.x = 62.5, .y = 100.0}, {.x = 50.0, .y = 75.0},
    };
    
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPolyquadratic(raster, startPoint, points, sizeof(points) / sizeof(HCPoint) / 2, false, HCRasterColorRotating);
    HCRasterSaveBMP(raster, "polyquadratic.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawPolycubic) {
    HCPoint startPoint = {.x = 25.0, .y = 75.0};
    HCPoint points[] = {
        {.x = 32.5, .y = 10.0}, {.x = 62.5, .y = 10.0}, {.x = 75.0, .y = 75.0},
        {.x = 62.5, .y = 50.0}, {.x = 32.5, .y = 50.0}, {.x = 25.0, .y = 75.0},
    };
       
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPolycubic(raster, startPoint, points, sizeof(points) / sizeof(HCPoint) / 3, true, HCRasterColorRotating);
    HCRasterSaveBMP(raster, "polycubic.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLinearContour) {
    HCContourComponent components[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 25.0, .y = 25.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 75.0, .y = 25.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 75.0, .y = 75.0}},
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 50.0, .y = 50.0}},
    };
    HCContour* contour = HCContourInitInComponents(components, sizeof(components) / sizeof(HCContourComponent), false);
    
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawContour(raster, contour, HCRasterColorRotating);
    HCRasterSaveBMP(raster, "contour_linear.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticContour) {
    HCContourComponent components[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 25.0, .y = 75.0}},
        {.c0 = {.x = 50.0, .y = 0.0}, .c1 = HCPointInvalidStatic, .p = {.x = 75.0, .y = 75.0}},
        {.c0 = {.x = 62.5, .y = 100.0}, .c1 = HCPointInvalidStatic, .p = {.x = 50.0, .y = 75.0}},
    };
    HCContour* contour = HCContourInitInComponents(components, sizeof(components) / sizeof(HCContourComponent), false);
    
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawContour(raster, contour, HCRasterColorRotating);
    HCRasterSaveBMP(raster, "contour_quadratic.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicContour) {
    HCContourComponent components[] = {
        {.c0 = HCPointInvalidStatic, .c1 = HCPointInvalidStatic, .p = {.x = 25.0, .y = 75.0}},
        {.c0 = {.x = 32.5, .y = 10.0}, .c1 = {.x = 62.5, .y = 10.0}, .p = {.x = 75.0, .y = 75.0}},
        {.c0 = {.x = 62.5, .y = 50.0}, .c1 = {.x = 32.5, .y = 50.0}, .p = {.x = 25.0, .y = 75.0}},
    };
    HCContour* contour = HCContourInitInComponents(components, sizeof(components) / sizeof(HCContourComponent), true);
       
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawContour(raster, contour, HCRasterColorRotating);
    HCRasterSaveBMP(raster, "contour_cubic.bmp");
    HCRelease(raster);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Path Drawing Operations
//----------------------------------------------------------------------------------------------------------------------------------
CTEST(HCRaster, DrawHorizontalPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 H 50", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_horizontal.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawHorizontalRelativePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 h 40", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_horizontal_relative.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawHorizontalPolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 H 25 50 75 90", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_horizontal_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawHorizontalRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 h 15 25 25 15", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_horizontal_relative_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawVerticalPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 V 50", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_vertical.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawVerticalRelativePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 v 30", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_vertical_relative.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawVerticalPolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 V 25 50 75 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_vertical_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawVerticalRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 v 5 25 25 5", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_vertical_relative_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLinePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 L 60 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_line.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLineRelativePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 l 50 60", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_line_relative.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLinePolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 L 60 80 50 40 90 20", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_line_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawLineRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 20 l 50 60 -10 -40 40 -20", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_line_relative_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurvePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 Q 95 10 180 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_curve.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurveRelativePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 q 85 -70 170 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_curve_relative.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurvePolyPath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 Q 95 10 180 80 95 150 10 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_curve_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticCurveRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 q 85 -70 170 0 -85 70 -170 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_curve_relative_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticSmoothCurvePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 Q 52.5 10 95 80 T 180 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_smooth_curve.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticSmoothCurveRelativePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 Q 52.5 10 95 80 t 85 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_smooth_curve_relative.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticSmoothCurvePolyPath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 Q 52.5 10 95 80 T 180 80 95 80 10 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_smooth_curve_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawQuadraticSmoothCurveRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 Q 52.5 10 95 80 t 85 0 -85 0 -85 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_quadratic_smooth_curve_relative_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurvePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 90 C 30 10 70 10 90 90", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_curve.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurveRelativePath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 90 c 20 -80 60 -80 80 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_curve_relative.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurvePolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 90 C 30 10 70 10 90 90 70 40 30 40 10 90", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_curve_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicCurveRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 90 c 20 -80 60 -80 80 0 -20 -50 -60 -50 -80 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_curve_relative_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicSmoothCurvePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 C 40 10 65 10 95 80 S 150 150 180 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_smooth_curve.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicSmoothCurveRelativePath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 C 40 10 65 10 95 80 s 55 70 85 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_smooth_curve_relative.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicSmoothCurvePolyPath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 C 40 10 65 10 95 80 S 150 150 180 80 80 10 95 80 10 150 10 80", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_smooth_curve_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCubicSmoothCurveRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(190, 160);
    HCRasterDrawPathData(raster, "M 10 80 C 40 10 65 10 95 80 s 55 70 85 0 -100 -70 -85 0 -85 70 -85 0", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_cubic_smooth_curve_relative_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcPath) {
    HCRasterRef raster = HCRasterCreate(500, 500);
    HCRasterDrawPathData(raster,
        "M25 350 L 75 325"
        "A25  25 -30 0 1 125 300 L 175 275"
        "A25  50 -30 0 1 225 250 L 275 225"
        "A25  75 -30 0 1 325 200 L 375 175"
        "A25 100 -30 0 1 425 150 L 475 125", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcRelativePath) {
    HCRasterRef raster = HCRasterCreate(500, 500);
    HCRasterDrawPathData(raster,
        "M25 350 l 50 -25"
        "a25  25 -30 0 1 50 -25 l 50 -25"
        "a25  50 -30 0 1 50 -25 l 50 -25"
        "a25  75 -30 0 1 50 -25 l 50 -25"
        "a25 100 -30 0 1 50 -25 l 50 -25", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_relative.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcPolyPath) {
    HCRasterRef raster = HCRasterCreate(320, 320);
    HCRasterDrawPathData(raster, "M 10 315 L 110 215 A 30 50 60 0 1 162.55 162.55 30 50 30 1 0 215.10 110.10 L 315 10", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcRelativePolyPath) {
    HCRasterRef raster = HCRasterCreate(320, 320);
    HCRasterDrawPathData(raster, "M 10 315 L 110 215 a 30 50 60 0 1 52.55 -52.45 30 50 30 1 0 52.55 -52.45 L 315 10", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_relative_poly.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcSmallArcNegativeSweep) {
    HCRasterRef raster = HCRasterCreate(350, 200);
    HCRasterDrawPathData(raster, "M 125,75 a100,50 0 0,0 100,50", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_small_arc_negative_sweep.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcLargeArcNegativeSweep) {
    HCRasterRef raster = HCRasterCreate(350, 200);
    HCRasterDrawPathData(raster, "M 125,75 a100,50 0 1,0 100,50", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_large_arc_negative_sweep.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcSmallArcPositiveSweep) {
    HCRasterRef raster = HCRasterCreate(350, 200);
    HCRasterDrawPathData(raster, "M 125,75 a100,50 0 0,1 100,50", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_small_arc_positive_sweep.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawArcLargeArcPositiveSweep) {
    HCRasterRef raster = HCRasterCreate(350, 200);
    HCRasterDrawPathData(raster, "M 125,75 a100,50 0 1,1 100,50", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_large_arc_positive_sweep.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCircle90ArcPath) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCRasterDrawPathData(raster, "M 750 500 A 250 250 0 0 1 500 750", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_circle90.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCircle180ArcPath) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCRasterDrawPathData(raster, "M 750 500 A 250 250 0 0 1 250 500", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_circle180.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCircle270ArcPath) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCRasterDrawPathData(raster, "M 750 500 A 250 250 0 1 1 500 250", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_circle270.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCircle359ArcPath) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCRasterDrawPathData(raster, "M 750 500 A 250 250 0 1 1 749.96 495.64", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_arc_circle359.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCircleArcPaths) {
    for (HCInteger i = 0; i < 360; i += 30) {
        HCRasterRef raster = HCRasterCreate(1000, 1000);
        HCReal angleStart = i;
        HCReal angleEnd = i * 2;
        HCReal cx = 500;
        HCReal cy = 500;
        HCReal rx = 100;
        HCReal ry = 200;
        HCReal rotation = 0.0;
        HCReal x0 = cx + rx * cos(angleStart * M_PI / 180.0);
        HCReal y0 = cy + ry * sin(angleStart * M_PI / 180.0);
        HCReal x1 = cx + rx * cos(angleEnd * M_PI / 180.0);
        HCReal y1 = cy + ry * sin(angleEnd * M_PI / 180.0);
        HCInteger large_arc = (angleEnd - angleStart) < 180.0 ? 0 : 1;
        HCInteger sweep = 1;
        char pathData[1024];
        sprintf(pathData, "M %f %f A %f %f %f %lli %lli %f %f", x0, y0, rx, ry, rotation, large_arc, sweep, x1, y1);
        HCRasterDrawPathData(raster, pathData, HCRasterColorRotating);
        char bmpFileName[1024];
        sprintf(bmpFileName, "path_arc_circle_%i.bmp", (int)i);
        HCRasterSaveBMP(raster, bmpFileName);
        HCRelease(raster);
    }
}

CTEST(HCRaster, DrawPath) {
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawPathData(raster, "M 10 90 C 30 10 70 10 90 90 Z L 30 10 H 70 L 90 90 Q 50 10 10 90 Z", HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawManyQuadratics) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCRasterDrawPathData(raster,
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
        , HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_many_quadratics.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawManyCubics) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCRasterDrawPathData(raster,
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
        , HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_many_cubics.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawDinosaur) {
    HCRasterRef raster = HCRasterCreate(500, 500);
    HCRasterDrawPathData(raster, dinosaur, HCRasterColorRotating);
    HCRasterSaveBMP(raster, "path_dinosaur.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCurveCanonical) {
    // Define curve whose canonical representation is to be drawn
    HCPoint canonical = HCCurveCanonical(HCPointMake(0.0, 0.0), HCPointMake(0.0, 100.0), HCPointMake(100.0, 100.0), HCPointMake(100.0, 0.0));
    
    // Configure raster
    HCReal w = 1000.0;
    HCReal h = 1000.0;
    HCRasterRef raster = HCRasterCreate((HCInteger)w, (HCInteger)h);
    
    // Draw background
    HCColor backgroundColor = HCColorMake(1.0, 1.0, 1.0, 1.0);
    HCRasterFillQuad(raster, 0.0, 0.0, w, 0.0, w, h, 0.0, h, backgroundColor, backgroundColor, backgroundColor, backgroundColor);
    
    // Draw axes
    HCColor axisColor = HCColorMake(1.0, 0.0, 0.0, 0.0);
    HCRasterDrawLine(raster, w * 0.5, 0.0, w * 0.5, h, axisColor, axisColor);
    HCRasterDrawLine(raster, 0.0, h * 0.5, w, h * 0.5, axisColor, axisColor);
    
    // Draw grid
    HCColor gridColor = HCColorMake(0.25, 0.0, 0.0, 0.0);
    HCRasterDrawLine(raster, w * 0.1, 0.0, w * 0.1, h, gridColor, gridColor);
    HCRasterDrawLine(raster, w * 0.2, 0.0, w * 0.2, h, gridColor, gridColor);
    HCRasterDrawLine(raster, w * 0.3, 0.0, w * 0.3, h, gridColor, gridColor);
    HCRasterDrawLine(raster, w * 0.4, 0.0, w * 0.4, h, gridColor, gridColor);
    HCRasterDrawLine(raster, w * 0.5, 0.0, w * 0.5, h, gridColor, gridColor);
    HCRasterDrawLine(raster, w * 0.6, 0.0, w * 0.6, h, gridColor, gridColor);
    HCRasterDrawLine(raster, w * 0.7, 0.0, w * 0.7, h, gridColor, gridColor);
    HCRasterDrawLine(raster, w * 0.8, 0.0, w * 0.8, h, gridColor, gridColor);
    HCRasterDrawLine(raster, w * 0.9, 0.0, w * 0.9, h, gridColor, gridColor);
    HCRasterDrawLine(raster, 0.0, h * 0.1, w, h * 0.1, gridColor, gridColor);
    HCRasterDrawLine(raster, 0.0, h * 0.2, w, h * 0.2, gridColor, gridColor);
    HCRasterDrawLine(raster, 0.0, h * 0.3, w, h * 0.3, gridColor, gridColor);
    HCRasterDrawLine(raster, 0.0, h * 0.4, w, h * 0.4, gridColor, gridColor);
    HCRasterDrawLine(raster, 0.0, h * 0.5, w, h * 0.5, gridColor, gridColor);
    HCRasterDrawLine(raster, 0.0, h * 0.6, w, h * 0.6, gridColor, gridColor);
    HCRasterDrawLine(raster, 0.0, h * 0.7, w, h * 0.7, gridColor, gridColor);
    HCRasterDrawLine(raster, 0.0, h * 0.8, w, h * 0.8, gridColor, gridColor);
    HCRasterDrawLine(raster, 0.0, h * 0.9, w, h * 0.9, gridColor, gridColor);
    
    // Draw single inflection region
    HCColor singleInflectionColor = HCColorMake(0.5, 0.0, 1.0, 0.0);
    HCRasterFillQuad(raster, 0.0, h * 0.7, w, h * 0.7, w, h, 0.0, h, singleInflectionColor, singleInflectionColor, singleInflectionColor, singleInflectionColor);
    
    // Draw double inflection region
    HCColor doubleInflectionColor = HCColorMake(0.5, 1.0, 1.0, 0.0);
    HCPathRef cuspPath = HCPathCreate();
    for (HCReal x = -2.5; x <= 1.0; x += 1.0 / (w * 0.7)) {
        HCReal cuspEdge = (-1.0 * x * x + 2.0 * x + 3.0) * 0.25;
        HCReal xImage = w * 0.5 + x * w / 5.0;
        HCReal yImage = h * 0.5 + cuspEdge * h / 5.0;
        x == -2.5 ? HCPathMove(cuspPath, xImage, yImage) : HCPathAddLine(cuspPath, xImage, yImage);
    }
    HCPathRef doubleInflectionPath = HCPathCreateCopy(cuspPath);
    HCPathAddLine(doubleInflectionPath, 0.0, 700.0);
    HCPathClose(doubleInflectionPath);
    HCRasterDrawPath(raster, doubleInflectionPath, doubleInflectionColor);
    
    // Draw loop region
    HCColor loopColor = HCColorMake(0.5, 1.0, 0.0, 0.0);
    HCPathRef t0LoopPath = HCPathCreate();
    for (HCReal x = -2.5; x <= 0.0; x += 1.0 / (w * 0.5)) {
        HCReal t0LoopEdge = (-1.0 * x * x + 3.0 * x) * (1.0 / 3.0);
        HCReal xImage = w * 0.5 + x * w / 5.0;
        HCReal yImage = h * 0.5 + t0LoopEdge * h / 5.0;
        x == -2.5 ? HCPathMove(t0LoopPath, xImage, yImage) : HCPathAddLine(t0LoopPath, xImage, yImage);
    }
    HCPathRef t1LoopPath = HCPathCreate();
    for (HCReal x = 0.0; x <= 1.0; x += 1.0 / (w * 0.2)) {
        HCReal t1LoopEdge = (sqrt(-3.0 * x * x + 12.0 * x) - x) * 0.5;
        HCReal xImage = w * 0.5 + x * w / 5.0;
        HCReal yImage = h * 0.5 + t1LoopEdge * h / 5.0;
        x == 0.0 ? HCPathMove(t1LoopPath, xImage, yImage) : HCPathAddLine(t1LoopPath, xImage, yImage);
    }
    HCPathRef loopPath = HCPathCreate();
    HCPathMove(loopPath, HCPathCurrentPoint(cuspPath).x, HCPathCurrentPoint(cuspPath).y);
    for (HCInteger elementIndex = HCPathElementCount(cuspPath) - 1; elementIndex >= 0; elementIndex--) {
        HCPoint p = HCPathElementAt(cuspPath, elementIndex).points[0];
        HCPathAddLine(loopPath, p.x, p.y);
    }
    for (HCInteger elementIndex = 0; elementIndex < HCPathElementCount(t0LoopPath); elementIndex++) {
        HCPoint p = HCPathElementAt(t0LoopPath, elementIndex).points[0];
        HCPathAddLine(loopPath, p.x, p.y);
    }
    for (HCInteger elementIndex = 0; elementIndex < HCPathElementCount(t1LoopPath); elementIndex++) {
        HCPoint p = HCPathElementAt(t1LoopPath, elementIndex).points[0];
        HCPathAddLine(loopPath, p.x, p.y);
    }
    HCPathClose(loopPath);
    HCRasterDrawPath(raster, loopPath, loopColor);
    
    // Draw canonical curve
    HCColor curveColor = HCColorMake(0.5, 0.0, 0.0, 1.0);
    HCReal p0x = w * 0.5 + 0.0 * w / 5.0;
    HCReal p0y = h * 0.5 + 0.0 * h / 5.0;
    HCReal c0x = w * 0.5 + 0.0 * w / 5.0;
    HCReal c0y = h * 0.5 + 1.0 * h / 5.0;
    HCReal c1x = w * 0.5 + 1.0 * w / 5.0;
    HCReal c1y = h * 0.5 + 1.0 * h / 5.0;
    HCReal p1x = w * 0.5 + canonical.x * w / 5.0;
    HCReal p1y = h * 0.5 + canonical.y * h / 5.0;
    HCRasterDrawCubicCurve(raster, p0x, p0y, c0x, c0y, c1x, c1y, p1x, p1y, curveColor, curveColor);
    
    HCRasterSaveBMP(raster, "path_canonical.bmp");
    
    HCRelease(doubleInflectionPath);
    HCRelease(loopPath);
    HCRelease(t0LoopPath);
    HCRelease(t1LoopPath);
    HCRelease(cuspPath);
    HCRelease(raster);
}

CTEST(HCRaster, DrawCurveBoundsExtremaInflection) {
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    
    HCPoint q0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.8);
    HCPoint qc0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.2, HCRectangleMinY(r) + HCRectangleHeight(r) * 1.2);
    HCPoint qc1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.7, HCRectangleMinY(r) + HCRectangleHeight(r) * -0.3);
    HCPoint q1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.9);
    
    HCRectangle bounds = HCCurveBoundsCubic(q0, qc0, qc1, q1);
    HCReal minX = HCRectangleMinX(bounds);
    HCReal minY = HCRectangleMinY(bounds);
    HCReal maxX = HCRectangleMaxX(bounds);
    HCReal maxY = HCRectangleMaxY(bounds);
    
    HCInteger extremaCount = 0;
    HCReal extrema[9] = { NAN, NAN, NAN };
    HCCurveExtremaCubic(q0, qc0, qc1, q1, &extremaCount, extrema);
    
    HCInteger inflectionCount = 0;
    HCReal inflections[2] = { NAN, NAN };
    HCCurveInflectionsCubic(q0, qc0, qc1, q1, &inflectionCount, inflections);
    
    HCRasterRef raster = HCRasterCreate(100.0, 100.0);
    HCRasterDrawCubicCurve(raster, q0.x, q0.y, qc0.x, qc0.y, qc1.x, qc1.y, q1.x, q1.y, HCColorGreen, HCColorGreen);
    HCRasterDrawQuad(raster, minX, minY, maxX, minY, maxX, maxY, minX, maxY, HCColorYellow, HCColorYellow, HCColorYellow, HCColorYellow);
    for (HCInteger extremaIndex = 0; extremaIndex < extremaCount; extremaIndex++) {
        HCReal extreme = extrema[extremaIndex];
        HCPoint extremePoint = HCCurveValueCubic(q0, qc0, qc1, q1, extreme);
        HCRasterDrawEllipse(raster, extremePoint.x, extremePoint.y, HCRectangleWidth(r) * 0.025, HCRectangleHeight(r) * 0.025, 0.0, HCColorRed, HCColorRed);
    }
    for (HCInteger inflectionIndex = 0; inflectionIndex < inflectionCount; inflectionIndex++) {
        HCReal inflection = inflections[inflectionIndex];
        HCPoint inflectionPoint = HCCurveValueCubic(q0, qc0, qc1, q1, inflection);
        HCRasterDrawEllipse(raster, inflectionPoint.x, inflectionPoint.y, HCRectangleWidth(r) * 0.025, HCRectangleHeight(r) * 0.025, 0.0, HCColorCyan, HCColorCyan);
    }
    HCRasterSaveBMP(raster, "curve_cubic_bounds_extrema_inflection.bmp");
    HCRelease(raster);
}

void HCPointGrid(HCRectangle r, HCInteger countX, HCInteger countY, HCPoint* points) {
    HCReal xSpan = fmax(HCRealMinimumPositive, HCRectangleWidth(r) / (HCReal)(countX - 1));
    HCReal ySpan = fmax(HCRealMinimumPositive, HCRectangleHeight(r) / (HCReal)(countY - 1));
    for (HCReal y = HCRectangleMinY(r); y <= HCRectangleMaxY(r); y += ySpan) {
        for (HCReal x = HCRectangleMinX(r); x <= HCRectangleMaxX(r); x += xSpan) {
            *points++ = HCPointMake(x, y);
        }
    }
}

CTEST(HCRaster, DrawCurveTangentNormalFrame) {
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPoint p0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.2);
    HCPoint c0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.2, HCRectangleMinY(r) + HCRectangleHeight(r) * 1.2);
    HCPoint c1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.7, HCRectangleMinY(r) + HCRectangleHeight(r) * -0.3);
    HCPoint p1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.9);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    
    HCRasterRef raster = HCRasterCreate(100, 100);
    HCRasterDrawCubicCurve(raster, p0.x, p0.y, c0.x, c0.y, c1.x, c1.y, p1.x, p1.y, HCColorGreen, HCColorGreen);
        
    HCInteger count = 10;
    HCReal tStep = 1.0 / (HCReal)(count - 1);
    HCReal length = fmin(r.size.width, r.size.height) * 0.1;
    for (HCInteger tIndex = 0; tIndex < count; tIndex++) {
        HCReal t = (HCReal)tIndex * tStep;
        
        HCCurve tangent = HCCurveTangentUnit(curve, t);
        tangent.p1 = HCPointOffset(tangent.p0, (tangent.p1.x - tangent.p0.x) * length, (tangent.p1.y - tangent.p0.y) * length);
        HCRasterDrawLine(raster, tangent.p0.x, tangent.p0.y, tangent.p1.x, tangent.p1.y, HCColorGreen, HCColorWithAlpha(HCColorBlue, 0.9));
        
        HCCurve normal = HCCurveNormalUnit(curve, t);
        normal.p1 = HCPointOffset(normal.p0, (normal.p1.x - normal.p0.x) * length, (normal.p1.y - normal.p0.y) * length);
        HCRasterDrawLine(raster, normal.p0.x, normal.p0.y, normal.p1.x, normal.p1.y, HCColorGreen, HCColorWithAlpha(HCColorRed, 0.9));
    }
    
    HCRasterSaveBMP(raster, "curve_tangent_normal_frame.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCurveCurvature) {
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(1000.0, 1000.0));
    HCPoint p0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.4);
    HCPoint c0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.3, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.8);
    HCPoint c1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.7, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.2);
    HCPoint p1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.6);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCRasterDrawCubicCurve(raster, p0.x, p0.y, c0.x, c0.y, c1.x, c1.y, p1.x, p1.y, HCColorGreen, HCColorGreen);
        
    HCInteger count = 100;
    HCReal tStep = 1.0 / (HCReal)(count - 1);
    HCReal length = fmin(r.size.width, r.size.height) * 100.0;
    for (HCInteger tIndex = 0; tIndex < count; tIndex++) {
        HCReal t = (HCReal)tIndex * tStep;
        
        HCCurve curvature = HCCurveCurvatureNormal(curve, t);
        curvature.p1 = HCPointOffset(curvature.p0, (curvature.p1.x - curvature.p0.x) * length, (curvature.p1.y - curvature.p0.y) * length);
        HCRasterDrawLine(raster, curvature.p0.x, curvature.p0.y, curvature.p1.x, curvature.p1.y, HCColorGreen, HCColorWithAlpha(HCColorRed, 0.9));
    }
    
    HCRasterSaveBMP(raster, "curve_curvature.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawCurveNearestParameterLinear) {
    HCInteger countX = 5;
    HCInteger countY = 5;
    HCPoint points[countX * countY];
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPointGrid(r, countX, countY, points);
    
    HCPoint p0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.3);
    HCPoint p1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.7);
    for (HCInteger pIndex = 0; pIndex < countX * countY; pIndex++) {
        HCPoint p = points[pIndex];
        HCReal nearest = HCCurveParameterNearestPointLinear(p0, p1, p);
        HCPoint nearestPoint = HCCurveValueLinear(p0, p1, nearest);
            
        HCRasterRef raster = HCRasterCreate(100, 100);
        HCRasterDrawLine(raster, p0.x, p0.y, p1.x, p1.y, HCColorGreen, HCColorGreen);
        HCRasterDrawLine(raster, p.x, p.y, nearestPoint.x, nearestPoint.y, HCColorRed, HCColorRed);
            
        char bmpFileName[1024];
        sprintf(bmpFileName, "curve_nearest_linear_%i.bmp", (int)pIndex);
        HCRasterSaveBMP(raster, bmpFileName);
        HCRelease(raster);
    }
}

CTEST(HCRaster, DrawCurveNearestParameterQuadratic) {
    HCInteger countX = 5;
    HCInteger countY = 5;
    HCPoint points[countX * countY];
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPointGrid(r, countX, countY, points);
    
    HCPoint p0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.7);
    HCPoint  c = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.8, HCRectangleMinY(r) + HCRectangleHeight(r) * -0.2);
    HCPoint p1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.9);
    for (HCInteger pIndex = 0; pIndex < countX * countY; pIndex++) {
        HCPoint p = points[pIndex];
        HCReal nearest = HCCurveParameterNearestPointQuadratic(p0, c, p1, p);
        HCPoint nearestPoint = HCCurveValueQuadratic(p0, c, p1, nearest);
            
        HCRasterRef raster = HCRasterCreate(100, 100);
        HCRasterDrawQuadraticCurve(raster, p0.x, p0.y, c.x, c.y, p1.x, p1.y, HCColorGreen, HCColorGreen);
        HCRasterDrawLine(raster, p.x, p.y, nearestPoint.x, nearestPoint.y, HCColorRed, HCColorRed);
            
        char bmpFileName[1024];
        sprintf(bmpFileName, "curve_nearest_quadratic_%i.bmp", (int)pIndex);
        HCRasterSaveBMP(raster, bmpFileName);
        HCRelease(raster);
    }
}

CTEST(HCRaster, DrawCurveNearestParameterCubic) {
    HCInteger countX = 5;
    HCInteger countY = 5;
    HCPoint points[countX * countY];
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPointGrid(r, countX, countY, points);
    
    HCPoint p0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.2);
    HCPoint c0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.2, HCRectangleMinY(r) + HCRectangleHeight(r) * 1.2);
    HCPoint c1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.7, HCRectangleMinY(r) + HCRectangleHeight(r) * -0.3);
    HCPoint p1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.9);
    for (HCInteger pIndex = 0; pIndex < countX * countY; pIndex++) {
        HCPoint p = points[pIndex];
        HCReal nearest = HCCurveParameterNearestPointCubic(p0, c0, c1, p1, p);
        HCPoint nearestPoint = HCCurveValueCubic(p0, c0, c1, p1, nearest);
            
        HCRasterRef raster = HCRasterCreate(100, 100);
        HCRasterDrawCubicCurve(raster, p0.x, p0.y, c0.x, c0.y, c1.x, c1.y, p1.x, p1.y, HCColorGreen, HCColorGreen);
        HCRasterDrawLine(raster, p.x, p.y, nearestPoint.x, nearestPoint.y, HCColorRed, HCColorRed);
            
        char bmpFileName[1024];
        sprintf(bmpFileName, "curve_nearest_cubic_%i.bmp", (int)pIndex);
        HCRasterSaveBMP(raster, bmpFileName);
        HCRelease(raster);
    }
}

CTEST(HCRaster, DrawCurveDistanceIsomap) {
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(192.0, 108.0));
    HCReal distanceMax = fmin(r.size.width, r.size.height) * 0.5;
    HCRasterRef raster = HCRasterCreate((HCInteger)HCRectangleWidth(r), (HCInteger)HCRectangleHeight(r));
    
    HCPoint p0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.2);
    HCPoint c0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.2, HCRectangleMinY(r) + HCRectangleHeight(r) * 1.2);
    HCPoint c1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.7, HCRectangleMinY(r) + HCRectangleHeight(r) * -0.3);
    HCPoint p1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.9);
    HCCurve curve = HCCurveMakeCubic(p0, c0, c1, p1);
    
    for (HCInteger y = 0; y < HCRasterHeight(raster); y++) {
        for (HCInteger x = 0; x < HCRasterWidth(raster); x++) {
            HCPoint p = HCPointMake(
                HCRectangleMinX(r) + HCRectangleWidth(r) * ((HCReal)x + 0.5) / (HCReal)(HCRasterWidth(raster)),
                HCRectangleMinY(r) + HCRectangleHeight(r) * ((HCReal)y + 0.5) / (HCReal)(HCRasterHeight(raster)));
            
            HCReal distance = HCCurveDistanceFromPoint(curve, p);
            HCRasterSetPixelAt(raster, x, y, HCColorScale(HCColorGreen, distance / distanceMax));
        }
    }
    
    HCRasterDrawCurves(raster, &curve, 1, HCColorYellow, HCColorYellow);
    
    HCRasterSaveBMP(raster, "curve_distance_isomap.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawMouldLinear) {
    HCInteger countX = 5;
    HCInteger countY = 5;
    HCPoint points[countX * countY];
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPointGrid(r, countX, countY, points);
    
    HCPoint p0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.3);
    HCPoint p1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.7);
    for (HCInteger pIndex = 0; pIndex < countX * countY; pIndex++) {
        HCPoint p = points[pIndex];
        HCCurveMouldLinear(p0, p1, 0.5, p);
            
        HCRasterRef raster = HCRasterCreate(100, 100);
        HCRasterDrawLine(raster, p0.x, p0.y, p1.x, p1.y, HCColorGreen, HCColorGreen);
        HCRasterDrawLine(raster, p0.x, p0.y, p1.x, p1.y, HCColorBlue, HCColorBlue);
            
        char bmpFileName[1024];
        sprintf(bmpFileName, "curve_mould_linear_%i.bmp", (int)pIndex);
        HCRasterSaveBMP(raster, bmpFileName);
        HCRelease(raster);
    }
}

CTEST(HCRaster, DrawMouldQuadratic) {
    HCInteger countX = 5;
    HCInteger countY = 5;
    HCPoint points[countX * countY];
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPointGrid(r, countX, countY, points);
    
    HCPoint p0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.7);
    HCPoint  c = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.8, HCRectangleMinY(r) + HCRectangleHeight(r) * -0.2);
    HCPoint p1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.9);
    for (HCInteger pIndex = 0; pIndex < countX * countY; pIndex++) {
        HCPoint p = points[pIndex];
        HCPoint cp;
        HCCurveMouldQuadratic(p0, c, p1, 0.5, p, &cp);
        
        HCRasterRef raster = HCRasterCreate(100, 100);
        HCRasterDrawQuadraticCurve(raster, p0.x, p0.y, c.x, c.y, p1.x, p1.y, HCColorGreen, HCColorGreen);
        HCRasterDrawQuadraticCurve(raster, p0.x, p0.y, cp.x, cp.y, p1.x, p1.y, HCColorBlue, HCColorBlue);
            
        char bmpFileName[1024];
        sprintf(bmpFileName, "curve_mould_quadratic_%i.bmp", (int)pIndex);
        HCRasterSaveBMP(raster, bmpFileName);
        HCRelease(raster);
    }
}

CTEST(HCRaster, DrawMouldCubic) {
    HCInteger countX = 5;
    HCInteger countY = 5;
    HCPoint points[countX * countY];
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPointGrid(r, countX, countY, points);
    
    HCPoint p0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.2);
    HCPoint c0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.2, HCRectangleMinY(r) + HCRectangleHeight(r) * 1.2);
    HCPoint c1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.7, HCRectangleMinY(r) + HCRectangleHeight(r) * -0.3);
    HCPoint p1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.9);
    for (HCInteger pIndex = 0; pIndex < countX * countY; pIndex++) {
        HCPoint p = points[pIndex];
        HCPoint c0p;
        HCPoint c1p;
        HCCurveMouldCubic(p0, c0, c1, p1, 0.5, p, &c0p, &c1p);
        HCRasterRef raster = HCRasterCreate(100, 100);
        HCRasterDrawCubicCurve(raster, p0.x, p0.y, c0.x, c0.y, c1.x, c1.y, p1.x, p1.y, HCColorGreen, HCColorGreen);
        HCRasterDrawCubicCurve(raster, p0.x, p0.y, c0p.x, c0p.y, c1p.x, c1p.y, p1.x, p1.y, HCColorBlue, HCColorBlue);
            
        char bmpFileName[1024];
        sprintf(bmpFileName, "curve_mould_cubic_%i.bmp", (int)pIndex);
        HCRasterSaveBMP(raster, bmpFileName);
        HCRelease(raster);
    }
}

CTEST(HCRaster, DrawCurveIntersectionsLinearLinear) {
    HCInteger countX = 2;// 5
    HCInteger countY = 2;// 5
    HCPoint points[countX * countY];
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPointGrid(r, countX, countY, points);
    
    HCPoint q0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.3);
    HCPoint q1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.7);
    for (HCInteger p0Index = 0; p0Index < countX * countY; p0Index++) {
        HCPoint p0 = points[p0Index];
        for (HCInteger p1Index = 0; p1Index < countX * countY; p1Index++) {
            HCPoint p1 = points[p1Index];
            
            HCInteger intersectionCount = 1;
            HCReal intersectionT[intersectionCount];
            HCReal intersectionU[intersectionCount];
            HCCurveIntersectionLinearLinear(p0, p1, q0, q1, &intersectionCount, intersectionT, intersectionU);
            if (intersectionCount == 0) {
                continue;
            }
            
            HCRasterRef raster = HCRasterCreate(100, 100);
            HCRasterDrawLine(raster, p0.x, p0.y, p1.x, p1.y, HCColorBlue, HCColorBlue);
            
            for (HCInteger intersectionIndex = 0; intersectionIndex < intersectionCount; intersectionIndex++) {
                HCPoint intersection = HCCurveValueLinear(q0, q1, intersectionU[intersectionIndex]);
                HCRasterDrawLine(raster, q0.x, q0.y, q1.x, q1.y, HCColorGreen, HCColorGreen);
                HCRasterDrawEllipse(raster, intersection.x, intersection.y, HCRectangleWidth(r) * 0.025, HCRectangleHeight(r) * 0.025, 0.0, HCColorRed, HCColorRed);
            }
            
            char bmpFileName[1024];
            sprintf(bmpFileName, "curve_intersection_ll_%i_%i.bmp", (int)p0Index, (int)p1Index);
            HCRasterSaveBMP(raster, bmpFileName);
            HCRelease(raster);
        }
    }
}

CTEST(HCRaster, DrawCurveIntersectionsLinearQuadratic) {
    HCInteger countX = 2;// 5
    HCInteger countY = 2;// 5
    HCPoint points[countX * countY];
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPointGrid(r, countX, countY, points);
    
    HCPoint q0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.7);
    HCPoint qc = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.8, HCRectangleMinY(r) + HCRectangleHeight(r) * -0.2);
    HCPoint q1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.9);
    for (HCInteger p0Index = 0; p0Index < countX * countY; p0Index++) {
        HCPoint p0 = points[p0Index];
        for (HCInteger p1Index = 0; p1Index < countX * countY; p1Index++) {
            HCPoint p1 = points[p1Index];
            
            HCInteger intersectionCount = 2;
            HCReal intersectionT[intersectionCount];
            HCReal intersectionU[intersectionCount];
            HCCurveIntersectionLinearQuadratic(p0, p1, q0, qc, q1, &intersectionCount, intersectionT, intersectionU);
            if (intersectionCount == 0) {
                continue;
            }
            
            HCRasterRef raster = HCRasterCreate(100, 100);
            HCRasterDrawLine(raster, p0.x, p0.y, p1.x, p1.y, HCColorBlue, HCColorBlue);
            
            for (HCInteger intersectionIndex = 0; intersectionIndex < intersectionCount; intersectionIndex++) {
                HCPoint intersection = HCCurveValueQuadratic(q0, qc, q1, intersectionU[intersectionIndex]);
                HCRasterDrawQuadraticCurve(raster, q0.x, q0.y, qc.x, qc.y, q1.x, q1.y, HCColorGreen, HCColorGreen);
                HCRasterDrawEllipse(raster, intersection.x, intersection.y, HCRectangleWidth(r) * 0.025, HCRectangleHeight(r) * 0.025, 0.0, HCColorRed, HCColorRed);
            }
            
            char bmpFileName[1024];
            sprintf(bmpFileName, "curve_intersection_lq_%i_%i.bmp", (int)p0Index, (int)p1Index);
            HCRasterSaveBMP(raster, bmpFileName);
            HCRelease(raster);
        }
    }
}

CTEST(HCRaster, DrawCurveIntersectionsLinearCubic) {
    HCInteger countX = 2;// 5
    HCInteger countY = 2;// 5
    HCPoint points[countX * countY];
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPointGrid(r, countX, countY, points);
    
    HCPoint q0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.2);
    HCPoint qc0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.2, HCRectangleMinY(r) + HCRectangleHeight(r) * 1.2);
    HCPoint qc1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.7, HCRectangleMinY(r) + HCRectangleHeight(r) * -0.3);
    HCPoint q1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.9);
    for (HCInteger p0Index = 0; p0Index < countX * countY; p0Index++) {
        HCPoint p0 = points[p0Index];
        for (HCInteger p1Index = 0; p1Index < countX * countY; p1Index++) {
            HCPoint p1 = points[p1Index];
            
            HCInteger intersectionCount = 3;
            HCReal intersectionT[intersectionCount];
            HCReal intersectionU[intersectionCount];
            HCCurveIntersectionLinearCubic(p0, p1, q0, qc0, qc1, q1, &intersectionCount, intersectionT, intersectionU);
            if (intersectionCount == 0) {
                continue;
            }
            
            HCRasterRef raster = HCRasterCreate(100, 100);
            HCRasterDrawLine(raster, p0.x, p0.y, p1.x, p1.y, HCColorBlue, HCColorBlue);
            
            for (HCInteger intersectionIndex = 0; intersectionIndex < intersectionCount; intersectionIndex++) {
                HCPoint intersection = HCCurveValueCubic(q0, qc0, qc1, q1, intersectionU[intersectionIndex]);
                HCRasterDrawCubicCurve(raster, q0.x, q0.y, qc0.x, qc0.y, qc1.x, qc1.y, q1.x, q1.y, HCColorGreen, HCColorGreen);
                HCRasterDrawEllipse(raster, intersection.x, intersection.y, HCRectangleWidth(r) * 0.025, HCRectangleHeight(r) * 0.025, 0.0, HCColorRed, HCColorRed);
            }
            
            char bmpFileName[1024];
            sprintf(bmpFileName, "curve_intersection_lc_%i_%i.bmp", (int)p0Index, (int)p1Index);
            HCRasterSaveBMP(raster, bmpFileName);
            HCRelease(raster);
        }
    }
}

CTEST(HCRaster, DrawCurveIntersectionsQuadraticQuadratic) {
    HCInteger countX = 2;// 5
    HCInteger countY = 2;// 5
    HCPoint points[countX * countY];
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPointGrid(r, countX, countY, points);
    
    HCPoint q0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.7);
    HCPoint qc = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.8, HCRectangleMinY(r) + HCRectangleHeight(r) * -0.2);
    HCPoint q1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.9);
    for (HCInteger p0Index = 0; p0Index < countX * countY; p0Index++) {
        HCPoint p0 = points[p0Index];
        for (HCInteger pcIndex = 0; pcIndex < countX * countY; pcIndex++) {
            HCPoint pc = points[pcIndex];
            for (HCInteger p1Index = 0; p1Index < countX * countY; p1Index++) {
                HCPoint p1 = points[p1Index];
        
                HCInteger intersectionCount = 4;
                HCReal intersectionT[intersectionCount];
                HCReal intersectionU[intersectionCount];
                HCCurveIntersectionQuadraticQuadratic(p0, pc, p1, q0, qc, q1, &intersectionCount, intersectionT, intersectionU);
                if (intersectionCount == 0) {
                    continue;
                }
                
                HCRasterRef raster = HCRasterCreate(100, 100);
                HCRasterDrawQuadraticCurve(raster, p0.x, p0.y, pc.x, pc.y, p1.x, p1.y, HCColorBlue, HCColorBlue);
                
                for (HCInteger intersectionIndex = 0; intersectionIndex < intersectionCount; intersectionIndex++) {
                    HCPoint intersection = HCCurveValueQuadratic(q0, qc, q1, intersectionU[intersectionIndex]);
                    HCRasterDrawQuadraticCurve(raster, q0.x, q0.y, qc.x, qc.y, q1.x, q1.y, HCColorGreen, HCColorGreen);
                    HCRasterDrawEllipse(raster, intersection.x, intersection.y, HCRectangleWidth(r) * 0.025, HCRectangleHeight(r) * 0.025, 0.0, HCColorRed, HCColorRed);
                }
                
                char bmpFileName[1024];
                sprintf(bmpFileName, "curve_intersection_qq_%i_%i_%i.bmp", (int)p0Index, (int)pcIndex, (int)p1Index);
                HCRasterSaveBMP(raster, bmpFileName);
                HCRelease(raster);
            }
        }
    }
}

CTEST(HCRaster, DrawCurveIntersectionsQuadraticCubic) {
    HCInteger countX = 2;// 5
    HCInteger countY = 2;// 5
    HCPoint points[countX * countY];
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPointGrid(r, countX, countY, points);
    
    HCPoint q0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.2);
    HCPoint qc0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.2, HCRectangleMinY(r) + HCRectangleHeight(r) * 1.2);
    HCPoint qc1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.7, HCRectangleMinY(r) + HCRectangleHeight(r) * -0.3);
    HCPoint q1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.9);
    for (HCInteger p0Index = 0; p0Index < countX * countY; p0Index++) {
        HCPoint p0 = points[p0Index];
        for (HCInteger pcIndex = 0; pcIndex < countX * countY; pcIndex++) {
            HCPoint pc = points[pcIndex];
            for (HCInteger p1Index = 0; p1Index < countX * countY; p1Index++) {
                HCPoint p1 = points[p1Index];
        
                HCInteger intersectionCount = 6;
                HCReal intersectionT[intersectionCount];
                HCReal intersectionU[intersectionCount];
                HCCurveIntersectionQuadraticCubic(p0, pc, p1, q0, qc0, qc1, q1, &intersectionCount, intersectionT, intersectionU);
                if (intersectionCount == 0) {
                    continue;
                }
                
                HCRasterRef raster = HCRasterCreate(100, 100);
                HCRasterDrawQuadraticCurve(raster, p0.x, p0.y, pc.x, pc.y, p1.x, p1.y, HCColorBlue, HCColorBlue);
                
                for (HCInteger intersectionIndex = 0; intersectionIndex < intersectionCount; intersectionIndex++) {
                    HCPoint intersection = HCCurveValueCubic(q0, qc0, qc1, q1, intersectionU[intersectionIndex]);
                    HCRasterDrawCubicCurve(raster, q0.x, q0.y, qc0.x, qc0.y, qc1.x, qc1.y, q1.x, q1.y, HCColorGreen, HCColorGreen);
                    HCRasterDrawEllipse(raster, intersection.x, intersection.y, HCRectangleWidth(r) * 0.025, HCRectangleHeight(r) * 0.025, 0.0, HCColorRed, HCColorRed);
                }
                
                char bmpFileName[1024];
                sprintf(bmpFileName, "curve_intersection_qc_%i_%i_%i.bmp", (int)p0Index, (int)pcIndex, (int)p1Index);
                HCRasterSaveBMP(raster, bmpFileName);
                HCRelease(raster);
            }
        }
    }
}

CTEST(HCRaster, DrawCurveIntersectionsCubicCubic) {
    HCInteger countX = 2;// 5
    HCInteger countY = 2;// 5
    HCPoint points[countX * countY];
    HCRectangle r = HCRectangleMake(HCPointZero, HCSizeMake(100.0, 100.0));
    HCPointGrid(r, countX, countY, points);
    
    HCPoint q0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.1, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.2);
    HCPoint qc0 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.2, HCRectangleMinY(r) + HCRectangleHeight(r) * 1.2);
    HCPoint qc1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.7, HCRectangleMinY(r) + HCRectangleHeight(r) * -0.3);
    HCPoint q1 = HCPointMake(HCRectangleMinX(r) + HCRectangleWidth(r) * 0.9, HCRectangleMinY(r) + HCRectangleHeight(r) * 0.9);
    for (HCInteger p0Index = 0; p0Index < countX * countY; p0Index++) {
        HCPoint p0 = points[p0Index];
        for (HCInteger pc0Index = 0; pc0Index < countX * countY; pc0Index++) {
            HCPoint pc0 = points[pc0Index];
            for (HCInteger pc1Index = 0; pc1Index < countX * countY; pc1Index++) {
                HCPoint pc1 = points[pc1Index];
                for (HCInteger p1Index = 0; p1Index < countX * countY; p1Index++) {
                    HCPoint p1 = points[p1Index];
        
                    HCInteger intersectionCount = 9;
                    HCReal intersectionT[intersectionCount];
                    HCReal intersectionU[intersectionCount];
                    HCCurveIntersectionCubicCubic(p0, pc0, pc1, p1, q0, qc0, qc1, q1, &intersectionCount, intersectionT, intersectionU);
                    if (intersectionCount == 0) {
                        continue;
                    }
                    
                    HCRasterRef raster = HCRasterCreate(100, 100);
                    HCRasterDrawCubicCurve(raster, p0.x, p0.y, pc0.x, pc0.y, pc1.x, pc1.y, p1.x, p1.y, HCColorBlue, HCColorBlue);
                    
                    for (HCInteger intersectionIndex = 0; intersectionIndex < intersectionCount; intersectionIndex++) {
                        HCPoint intersection = HCCurveValueCubic(q0, qc0, qc1, q1, intersectionU[intersectionIndex]);
                        HCRasterDrawCubicCurve(raster, q0.x, q0.y, qc0.x, qc0.y, qc1.x, qc1.y, q1.x, q1.y, HCColorGreen, HCColorGreen);
                        HCRasterDrawEllipse(raster, intersection.x, intersection.y, HCRectangleWidth(r) * 0.025, HCRectangleHeight(r) * 0.025, 0.0, HCColorRed, HCColorRed);
                    }
                    
                    char bmpFileName[1024];
                    sprintf(bmpFileName, "curve_intersection_cc_%i_%i_%i_%i.bmp", (int)p0Index, (int)pc0Index, (int)pc1Index, (int)p1Index);
                    HCRasterSaveBMP(raster, bmpFileName);
                    HCRelease(raster);
                }
            }
        }
    }
}

CTEST(HCRaster, DrawContourIntersections) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    
//    HCPathRef pPath = HCPathCreateWithSVGPathData(
//        "M 800 500 "
//        "A 300 200 0 1 1 799.95 495.0 "
//    );
//    const HCContour* pContour = HCPathContourAt(pPath, 0);
//    HCPathRef qPath = HCPathCreateWithSVGPathData(
//        "M 750 500 "
//        "A 250 250 0 1 1 749.95 495.0 "
//    );
//    const HCContour* qContour = HCPathContourAt(qPath, 0);
    
    HCPathRef pPath = HCPathCreateWithSVGPathData(dinosaur);
    const HCContour* pContour = HCPathContourAt(pPath, 1);
    HCPathRef qPath = HCPathCreateWithSVGPathData(
        "M 750 500 "
        "A 250 250 0 1 1 749.95 495.0 "
        "L 700 500 "
        "A 200 200 0 1 1 699.95 495.0 "
        "L 650 500 "
        "A 150 150 0 1 1 649.95 495.0 "
        "L 600 500 "
        "A 100 100 0 1 1 599.95 495.0 "
        "L 550 500 "
        "A  50  50 0 1 1 549.95 495.0 "
        "L 500 500 "
    );
    const HCContour* qContour = HCPathContourAt(qPath, 0);
    
    HCRasterDrawContour(raster, pContour, HCRasterColorRotating);
    HCRasterDrawContour(raster, qContour, HCRasterColorRotating);
    
    HCInteger count = (HCContourComponentCount(pContour) + HCContourComponentCount(qContour)) * 9;
    HCReal t[count];
    HCReal u[count];
    HCContourIntersection(qContour, pContour, &count, u, t);
    
    for (HCInteger intersectionIndex = 0; intersectionIndex < count; intersectionIndex++) {
        HCPoint intersectionPointT = HCContourValue(pContour, t[intersectionIndex]);
        HCPoint intersectionPointU = HCContourValue(qContour, u[intersectionIndex]);
        HCColor color = HCPointIsSimilar(intersectionPointT, intersectionPointU, 1.0) ? HCColorGreen : HCColorRed;
        HCRasterDrawEllipse(raster, intersectionPointT.x, intersectionPointT.y, 2.5, 2.5, 0, color, color);
        HCRasterDrawEllipse(raster, intersectionPointU.x, intersectionPointU.y, 2.5, 2.5, 0, color, color);
    }
    
//    for (HCInteger pCurveIndex = 0; pCurveIndex < HCContourCurveCount(pContour); pCurveIndex++) {
//        HCReal ct = HCContourParameterForCurveParameter(pContour, pCurveIndex, 0.25);
//        HCPoint p = HCContourValue(pContour, ct);
//        HCRasterDrawEllipse(raster, p.x, p.y, 2.5, 2.5, 0, HCColorCyan, HCColorCyan);
//    }
//
//    for (HCInteger qCurveIndex = 0; qCurveIndex < HCContourCurveCount(qContour); qCurveIndex++) {
//        HCReal cu = HCContourParameterForCurveParameter(qContour, qCurveIndex, 0.25);
//        HCPoint q = HCContourValue(qContour, cu);
//        HCRasterDrawEllipse(raster, q.x, q.y, 2.5, 2.5, 0, HCColorMagenta, HCColorMagenta);
//    }
    
    HCRasterSaveBMP(raster, "contour_intersections.bmp");
    HCRelease(raster);
    HCRelease(pPath);
    HCRelease(qPath);
}

CTEST(HCRaster, DrawTestCurveIntersections) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    
    HCCurve pCurve = HCCurveMakeCubic(
        HCPointMake(499.99975000012154, 749.99974999987148),
        HCPointMake(361.92856254249222, 749.99961192868204),
        HCPointMake(249.99986192906064, 638.07068745762240),
        HCPointMake(250.00000000025005, 499.99949999999296));
    
    HCCurve qCurve = HCCurveMakeCubic(
        HCPointMake(296.58778000000001, 500.0),
        HCPointMake(NAN, NAN),
        HCPointMake(NAN, NAN),
        HCPointMake(329.71762999999999, 500.0));
    
    HCRasterDrawCurves(raster, &pCurve, 1, HCColorGreen, HCColorGreen);
    HCRasterDrawCurves(raster, &qCurve, 1, HCColorBlue, HCColorBlue);
    
    HCInteger count = 9;
    HCReal t[count];
    HCReal u[count];
    HCCurveIntersection(pCurve, qCurve, &count, u, t);
    
    for (HCInteger intersectionIndex = 0; intersectionIndex < count; intersectionIndex++) {
        HCPoint intersectionPointT = HCCurveValue(pCurve, t[intersectionIndex]);
        HCPoint intersectionPointU = HCCurveValue(qCurve, u[intersectionIndex]);
        HCColor color = HCPointIsSimilar(intersectionPointT, intersectionPointU, 1.0) ? HCColorGreen : HCColorRed;
        HCRasterDrawEllipse(raster, intersectionPointT.x, intersectionPointT.y, 2.5, 2.5, 0, color, color);
        HCRasterDrawEllipse(raster, intersectionPointU.x, intersectionPointU.y, 2.5, 2.5, 0, color, color);
    }
    
    HCRasterSaveBMP(raster, "curve_intersections.bmp");
    HCRelease(raster);
}

CTEST(HCRaster, DrawPathIntersection) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCPathRef path = HCPathCreateWithSVGPathData(
        "M 200 500 C 500  1000 500  1000 800 500 Z "
        "M 200 500 C 500     0 500     0 800 500 Z "
    );
    HCPathRef rectangle = HCPathCreateWithSVGPathData("M 100 300 L 900 300 900 700 100 700 Z");
    for (HCInteger y = 0; y < HCRasterHeight(raster); y++) {
        for (HCInteger x = 0; x < HCRasterWidth(raster); x++) {
            HCPoint point = HCPointMake(x + 0.5, y + 0.5);
            HCBoolean pathContainsPoint = HCPathContainsPoint(path, point);
            HCBoolean rectangleContainsPoint = HCPathContainsPoint(rectangle, point);
            HCRasterSetPixelAt(raster, x, y, pathContainsPoint ? (rectangleContainsPoint ? HCColorGreen : HCColorBlue) : (rectangleContainsPoint ? HCColorYellow : HCColorBlack));
        }
    }
    HCRasterSaveBMP(raster, "path_intersection.bmp");
    HCRelease(rectangle);
    HCRelease(path);
    HCRelease(raster);
}

CTEST(HCRaster, DrawPathIntersectionNonZero) {
    HCRasterRef raster = HCRasterCreate(1000, 1000);
    HCPathRef path = HCPathCreateWithSVGPathData(
        "M 200 500 C 500  1000 500  1000 800 500 Z "
        "M 200 500 C 500     0 500     0 800 500 Z "
    );
    HCPathRef rectangle = HCPathCreateWithSVGPathData("M 100 300 L 900 300 900 700 100 700 Z");
    for (HCInteger y = 0; y < HCRasterHeight(raster); y++) {
        for (HCInteger x = 0; x < HCRasterWidth(raster); x++) {
            HCPoint point = HCPointMake(x + 0.5, y + 0.5);
            HCBoolean pathContainsPoint = HCPathContainsPointNonZero(path, point);
            HCBoolean rectangleContainsPoint = HCPathContainsPointNonZero(rectangle, point);
            HCRasterSetPixelAt(raster, x, y, pathContainsPoint ? (rectangleContainsPoint ? HCColorGreen : HCColorBlue) : (rectangleContainsPoint ? HCColorYellow : HCColorBlack));
        }
    }
    HCRasterSaveBMP(raster, "path_intersection.bmp");
    HCRelease(rectangle);
    HCRelease(path);
    HCRelease(raster);
}
