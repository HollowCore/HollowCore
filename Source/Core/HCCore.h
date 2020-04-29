///
/// @file HCCore.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 12/28/18
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCCore_h
#define HCCore_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <float.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Value Types
//----------------------------------------------------------------------------------------------------------------------------------

/// A value containing @c false or @c true.
///
/// Has a bit width of at least 1.
/// A value of @c false is represented by all bits being @c 0.
/// All other values are considered to be @c true.
typedef bool HCBoolean;

/// A signed integer value.
///
/// Has a bit width of exactly 64.
/// Negative values are represented using 2s complement representation.
typedef int64_t HCInteger;

/// A signed floating-point value.
///
/// Has a bit width of exactly 64.
/// Uses IEEE 754 representation.
typedef double HCReal;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Boolean Constants
//----------------------------------------------------------------------------------------------------------------------------------

/// The false boolean value.
///
/// Provided only for use where @c false is unavailable. Prefer using the value @c false over this value.
#define HCBooleanFalse ((HCBoolean)0)

/// The true boolean value.
///
/// Provided only for use where @c true is unavailable. Prefer using the value @c true over this value.
#define HCBooleanTrue ((HCBoolean)1)

/// The number of bits used to represent a boolean value.
#define HCBooleanBitWidth ((HCInteger)(sizeof(HCBoolean) * 8))

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Integer Constants
//----------------------------------------------------------------------------------------------------------------------------------

/// The zero integer value.
#define HCIntegerZero ((HCInteger)0)

/// The minimum value an integer may contain. Is guaranteed negative.
#define HCIntegerMinimum ((HCInteger)INT64_MIN)

/// The maximum value an integer may contain. Is guaranteed positive.
#define HCIntegerMaximum ((HCInteger)INT64_MAX)

/// The number of bits used to represent an integer value.
#define HCIntegerBitWidth ((HCInteger)(sizeof(HCInteger) * 8))

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Real Constants
//----------------------------------------------------------------------------------------------------------------------------------

/// The zero real value.
#define HCRealZero ((HCReal)0.0)

/// The minimum finite negative value a real may contain. Adjacent to negative infinity.
#define HCRealMinimumNegative ((HCReal)-DBL_MAX)

/// The maximum finite negative value a real may contain. Adjacent to zero.
#define HCRealMaximumNegative ((HCReal)-DBL_TRUE_MIN)

/// The maximum finite positive value a real may contain. Adjacent to zero.
#define HCRealMinimumPositive ((HCReal)DBL_TRUE_MIN)

/// The maximum finite positive value a real may contain. Adjacent to positive infinity.
#define HCRealMaximumPositive ((HCReal)DBL_MAX)

/// The real value corresponding to positive infinity.
#define HCRealInfinity ((HCReal)INFINITY)

/// The real value corresponding to negative infinity.
#define HCRealNegativeInfinity ((HCReal)-INFINITY)

/// An invalid real value that does not correspond to a number.
#define HCRealInvalid ((HCReal)NAN)

/// The smallest real value for which @c 1.0 plus the value is not equal to @c 1.0.
#define HCRealEpsilon ((HCReal)DBL_EPSILON)

/// The number of significant digits in a real value
#define HCRealSignificantDigits ((HCInteger)DBL_DIG)

/// The number of bits used to represent a real value.
#define HCRealBitWidth ((HCInteger)(sizeof(HCReal) * 8))

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Types
//----------------------------------------------------------------------------------------------------------------------------------

/// A unit of storage of an 8-bit width.
typedef uint8_t HCByte;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Boolean Operations
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines equality of a pair of boolean values.
///
/// If @c HCBooleanIsEqual() returns @c true for a pair of boolean values, @c HCBooleanHashValue() will return the same value for them.
///
/// @param self A boolean value to equate.
/// @param other Another boolean value to equate.
/// @returns @c true if @c self and @c other both contain @c false or both do not contain @c false. Returns @c false otherwise.
HCBoolean HCBooleanIsEqual(HCBoolean self, HCBoolean other);

/// Provides a hashing value corresponding to a boolean value.
///
/// @param self Boolean value to hash.
/// @returns A value suitable to use as a hash value to represent @c self or to combine with other values to form an aggregate hash value.
HCInteger HCBooleanHashValue(HCBoolean self);

/// Prints a text representation of a boolean value to a stream.
/// @param self A boolean value to print.
/// @param stream Stream to which @c self will be printed.
void HCBooleanPrint(HCBoolean self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Integer Operations
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines equality of a pair of integer values.
///
/// If @c HCIntegerIsEqual() returns @c true for a pair of integer values, @c HCIntegerHashValue() will return the same value for them.
///
/// @param self An integer value to equate.
/// @param other Another integer value to equate.
/// @returns @c true if @c self and @c other both contain the same integer value. Returns @c false otherwise.
HCBoolean HCIntegerIsEqual(HCInteger self, HCInteger other);

/// Provides a hashing value corresponding to an integer value.
///
/// @param self An integer value to hash.
/// @returns A value suitable to use as a hash value to represent @c self or to combine with other values to form an aggregate hash value.
HCInteger HCIntegerHashValue(HCInteger self);

/// Prints a text representation of an integer value to a stream.
/// @param self Integer value to print.
/// @param stream Stream to which @c self will be printed.
void HCIntegerPrint(HCInteger self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Real Operations
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a real value contains a value that does not represent a number.
///
/// @param self A real value to examine.
/// @param @c true if @c self contains @c HCRealInvalid or any other value that does not represent a number. @c HCRealInfinity and @c HCRealNegativeInfinity are considered valid real values.
HCBoolean HCRealIsInvalid(HCReal self);

/// Determines if a real value contains an infinite value.
///
/// @param self A real value to examine.
/// @param @c true if @c self contains @c HCRealInfinity or @c HCRealNegativeInfinity.
HCBoolean HCRealIsInfinite(HCReal self);

/// Determines if a real value contains a finite value.
///
/// @param self A real value to examine.
/// @param @c true if @c self is not @c HCRealInfinity or @c HCRealNegativeInfinity and @c HCRealIsInvalid() returns @c false.
HCBoolean HCRealIsFinite(HCReal self);

/// Determines approximate equality of a pair of real values.
///
/// @param self A real value to equate.
/// @param other Another real value to equate.
/// @param dissimilarity The distance @c self can be from @c other before they are considered dissimilar.
/// @returns @c true if the difference of @c self and @c other are less than dissimilarity. Returns @c false otherwise.
HCBoolean HCRealIsSimilar(HCReal self, HCReal other, HCReal dissimilarity);

/// Determines equality of a pair of real values.
///
/// If @c HCRealIsEqual() returns @c true for a pair of real values, @c HCRealHashValue() will return the same value for them.
///
/// @param self A real value to equate.
/// @param other Another real value to equate.
/// @returns @c true if @c self and @c other  are finite and contain the same real value. Returns @c false otherwise.
/// @todo What about @c NAN? @c +Inf? @c -Inf?
HCBoolean HCRealIsEqual(HCReal self, HCReal other);

/// Provides a hashing value corresponding to a real value.
///
/// @param self A real value to hash.
/// @returns A value suitable to use as a hash value to represent @c self or to combine with other values to form an aggregate hash value. In addition, for values of @c self exactly representing the same value as an @c HCInteger value, this will be the same value as returned from @c HCIntegerHashValue().
HCInteger HCRealHashValue(HCReal self);

/// Prints a text representation of a real value to a stream.
/// @param self Real value to print.
/// @param stream Stream to which @c self will be printed.
void HCRealPrint(HCReal self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Foundation
//----------------------------------------------------------------------------------------------------------------------------------

/// The name of a type.
///
/// Contains the same value for all instances of a type.
typedef const char* const HCTypeName;

/// Information about a specific type.
///
/// Contains the same value for all instances of a type.
typedef const struct HCTypeData* HCType;

/// Information related to a type common to all instances of that type.
///
/// Includes identifying information for a type and the ancestors of that type.
typedef const struct HCTypeData {
    HCTypeName name;
    HCType ancestor;
} HCTypeData;

/// A reference to an instance of an object.
///
/// Used when the specific reference type of an object cannot be used.
typedef void* HCRef;

#endif /* HCCore_h */
