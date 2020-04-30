///
/// @file HCNumber.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 12/28/18
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCNumber_h
#define HCNumber_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------

/// Type of @c HCObject instances.
extern HCType HCNumberType;

/// A reference to an @c HCNumber instance.
typedef struct HCNumber* HCNumberRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------

/// Creates a number containing a @c false value.
/// @returns A reference to an @c HCNumber containing @c false.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCNumberRef HCNumberCreate(void);

/// Creates a number containing a boolean value.
/// @param value The boolean value the number should contain.
/// @returns A reference to an @c HCNumber containing @c value.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCNumberRef HCNumberCreateWithBoolean(HCBoolean value);

/// Creates a number containing an integer value.
/// @param value The integer value the number should contain.
/// @returns A reference to an @c HCNumber containing @c value.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCNumberRef HCNumberCreateWithInteger(HCInteger value);

/// Creates a number containing a real value.
/// @param value The real value the number should contain.
/// @returns A reference to an @c HCNumber containing @c value.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCNumberRef HCNumberCreateWithReal(HCReal value);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a number is equal to another.
///
/// Equality between numbers created with different basic types is evaluated with the following equivalencies:
///
/// @code{.unparsed}
/// false == HCInteger
/// false == HCReal
/// true == HCInteger
/// true == HCReal
/// integer == real if floor(real) == integer && floor(real) - real == 0.0
/// @endcode
///
/// All other pairings are not equal.
///
/// @param self A reference to the number.
/// @param other The number to evaluate equality against.
/// @returns The return value of the type corresponding equality function @c HCBooleanIsEqual(), @c HCIntegerIsEqual(), or @c HCRealIsEqual() in the case both numbers contain values of the same type, or as described above if they are of mixed type.
HCBoolean HCNumberIsEqual(HCNumberRef self, HCNumberRef other);

/// Calculates a hash value for the number.
/// @param self A reference to the number.
/// @returns A hash value determined using only the content of the number.
HCInteger HCNumberHashValue(HCNumberRef self);

/// Prints a number to a stream.
/// @param self A reference to the number.
/// @param stream The stream to which the number should be printed.
void HCNumberPrint(HCNumberRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a number was created with a boolean value.
/// @param self A reference to the number.
/// @returns @c true if the number was created using @c HCNumberCreateWithBoolean() or @c false otherwise.
HCBoolean HCNumberIsBoolean(HCNumberRef self);

/// Interprets a number as a boolean value.
/// @param self A reference to the number.
/// @returns @c false if the contained value is @c false, @c 0, or @c 0.0. Otherwise returns @c true.
HCBoolean HCNumberAsBoolean(HCNumberRef self);

/// Determines if a number was created with an integer value.
/// @param self A reference to the number.
/// @returns @c true if the number was created using @c HCNumberCreateWithInteger() or @c false otherwise.
HCBoolean HCNumberIsInteger(HCNumberRef self);

/// Interprets a number as an integer value.
/// @param self A reference to the number.
/// @returns the contained value if it was created with an integer. Returns @c floor(value) if the contained value is a real. Returns @c 0 if the contained value is a boolean @c false. Returns @c 1 if the contained value is a boolean @c true.
HCInteger HCNumberAsInteger(HCNumberRef self);

/// Determines if a number was created with a real value.
/// @param self A reference to the number.
/// @returns @c true if the number was created using @c HCNumberCreateWithReal() or @c false otherwise.
HCBoolean HCNumberIsReal(HCNumberRef self);

/// Interprets a number as a real value.
/// @param self A reference to the number.
/// @returns the contained value if it was created with a real. If the contained value is an integer, returns the integer value as a real value with as much precision as possible. Returns @c 0.0 if the contained value is a boolean @c false. Returns @c 1 if the contained value is a boolean @c true.
HCReal HCNumberAsReal(HCNumberRef self);

#endif /* HCNumber_h */
