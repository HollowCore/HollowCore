///
/// @file HCString.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 1/17/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///
/// @brief Type for storing Unicode-compliant string data.
///

#ifndef HCString_h
#define HCString_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------

/// Type of @c HCString instances.
extern HCType HCStringType;

/// A reference to an @c HCString instance.
typedef struct HCString* HCStringRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------

/// A UTF-8 code unit.
typedef uint8_t HCStringCodeUnit;

/// A Unicode code point stored as a UTF-32 code unit.
typedef uint32_t HCStringCodePoint;

/// Text encodings supported by @c HCString.
typedef enum HCStringEncoding {
    HCStringEncodingUTF8,
    HCStringEncodingUTF16,
    HCStringEncodingUTF32
} HCStringEncoding;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------

/// Creates an empty string.
/// @returns A reference to the created string.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCStringRef HCStringCreate(void);

/// Creates a string from encoded text.
/// @param encoding The encoding to use to interpret @c bytes to create the string.
/// @param size The size in bytes of @c bytes.
/// @param bytes Memory to interpret using the text encoding scheme indicated in @c encoding to create the string.
/// @returns A reference to the created string.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCStringRef HCStringCreateWithBytes(HCStringEncoding encoding, HCInteger size, const HCByte* bytes);

/// Creates a string from a UTF-8 encoded, null-terminated text buffer.
/// @param value A null-terminated buffer containing text data encoded using UTF-8.
/// @returns A reference to the created string.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCStringRef HCStringCreateWithCString(const char* value);

/// Creates a string from a boolean value.
/// @param value The boolean value the string should communicate. @c false will result in a string containing @c ⊭. @c true will result in a string containing @c ⊨.
/// @returns A reference to the created string.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCStringRef HCStringCreateWithBoolean(HCBoolean value);

/// Creates a string from an integer value.
/// @param value The integer value the string should communicate.
/// @returns A reference to the created string.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCStringRef HCStringCreateWithInteger(HCInteger value);

/// Creates a string from a real value.
/// @param value The real value the string should communicate. The string is created with enough precision to ensure that @c HCStringAsReal() will be equal to @c value.
/// @returns A reference to the created string.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCStringRef HCStringCreateWithReal(HCReal value);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a string is equal to another string.
/// @param self A reference to the string.
/// @param other The other string to evaluate equality against.
/// @returns @c true if @c self and @c other contain exactly the same UTF-8 code units.
/// @todo Should return @c true if the contained Unicode strings in @c self and @c other are equal according to the Unicode 13.0 standard for Canonical Equivalence. Do not rely on the results of this function for canonically equivalent string comparison, as its results will change in a future version. For a stable exact equivalence relation, see @c HCStringContainsSameCodeUnits().
HCBoolean HCStringIsEqual(HCStringRef self, HCStringRef other);

/// Calculates a hash value for the string.
/// @param self A reference to the string.
/// @returns A hash value determined using only the content of the string.
/// @todo Must hash canonically equivalent strings to the same value when @c HCStringIsEqual() supports Unicode Canonical Equivalence.
HCInteger HCStringHashValue(HCStringRef self);

/// Prints a string to a stream.
/// @param self A reference to the string.
/// @param stream The stream to which the string should be printed.
void HCStringPrint(HCStringRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a string contains no code units.
/// @param self A reference to the string.
/// @returns @c true if @c HCStringCodeUnitCount() is @c 0, or @c false otherwise.
HCBoolean HCStringIsEmpty(HCStringRef self);

/// Obtains the number of UTF-8 code units in a string.
/// @param self A reference to the string.
/// @returns The number of UTF-8 code units that make up the string value contained in the string.
HCInteger HCStringCodeUnitCount(HCStringRef self);

/// Indexes a string by its UTF-8 code units.
/// @param self A reference to the string.
/// @param codeUnitIndex An index between @c 0 and @c HCStringCodeUnitCount(). Passing values outside this range results in undefined behavior.
/// @returns The UTF-8 code unit found at @c codeUnitIndex in the string.
HCStringCodeUnit HCStringCodeUnitAtIndex(HCStringRef self, HCInteger codeUnitIndex);

/// Extracts UTF-8 code units from string.
/// @param self A reference to the string.
/// @param codeUnitIndex An index between @c 0 and @c HCStringCodeUnitCount() - @c count. Passing values outside this range results in undefined behavior.
/// @param count The number of UTF-8 code units to extract from the string.
/// @param destination pointer to a buffer of at least @c count bytes in length into which the UTF-8 code units should be stored.
void HCStringExtractCodeUnits(HCStringRef self, HCInteger codeUnitIndex, HCInteger count, HCStringCodeUnit* destination);

/// Obtains the number of UTF-32 code units in a string.
/// @param self A reference to the string.
/// @returns The number of UTF-32 code units that make up the string value contained in the string.
HCInteger HCStringCodePointCount(HCStringRef self);

/// Indexes a string by its UTF-32 code units.
/// @param self A reference to the string.
/// @param codePointIndex An index between @c 0 and @c HCStringCodePointCount(). Passing values outside this range results in undefined behavior.
/// @returns The UTF-32 code unit found at @c codePointIndex in the string.
HCStringCodePoint HCStringCodePointAtIndex(HCStringRef self, HCInteger codePointIndex);

/// Extracts UTF-32 code units from string.
/// @param self A reference to the string.
/// @param codePointIndex An index between @c 0 and @c HCStringCodePointCount() - @c count. Passing values outside this range results in undefined behavior.
/// @param count The number of UTF-32 code units to extract from the string.
/// @param destination pointer to a buffer of at least @code count * sizeof(HCStringCodePoint) @endcode bytes in length into which the UTF-32 code units should be stored.
void HCStringExtractCodePoints(HCStringRef self, HCInteger codePointIndex, HCInteger count, HCStringCodePoint* destination);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a string is equal to its C-style string equivalent.
/// Strings containing the Unicode Null code point do not exhibit this behavior.
/// @param self A reference to the string.
/// @returns The result of @code HCStringIsEqualToCString(self, HCStringAsCString(self)) @endcode.
HCBoolean HCStringIsCString(HCStringRef self);

/// Interprets a string as a C-style string.
/// @param self A reference to the string.
/// @returns A null-terminated, UTF-8 encoded text buffer with the same content as @c self as far as is possible to produce. Strings containing the Unicode Null code point will be truncated at that code point.
const char* HCStringAsCString(HCStringRef self);

/// Determines if a string contains a boolean value.
/// @param self A reference to the string.
/// @returns @c true if the string is exactly equal to one of: @c ⊭, @c ⊨, @c 0, @c 1, @c f, @c t, @c false, @c true, or @c false otherwise.
HCBoolean HCStringIsBoolean(HCStringRef self);

/// Interprets a string as a boolean value.
/// @param self A reference to the string.
/// @returns @c true if the string begins with: @c ⊭, @c ⊨, @c 0, @c 1, @c f, @c t, @c false, @c true, or @c false otherwise. May return @c true under other conditions, such as strings beginning with whitespace. For an exact relation, first test the string with @c HCStringIsBoolean().
HCBoolean HCStringAsBoolean(HCStringRef self);

/// Determines if a string contains an integer value.
/// @param self A reference to the string.
/// @returns @c true if the string contains only characters used to encode an integer value and nothing else, and that integer value can fit in an @c HCInteger without loss of precision. Otherwise returns @c false.
HCBoolean HCStringIsInteger(HCStringRef self);

/// Interprets a string as an integer value.
/// @param self A reference to the string.
/// @returns The integer value of the string insofar as it could be determined. If the string could not be interpreted as an integer value, returns @c 0. May skip string content to reach integer-encoding characters, such as strings beginning with whitespace. For an exact relation, first test the string with @c HCStringIsInteger().
HCInteger HCStringAsInteger(HCStringRef self);

/// Determines if a string contains a real value.
/// @param self A reference to the string.
/// @returns @c true if the string contains only characters used to encode a real value and nothing else, or the word @c INFINITY ignoring case. Otherwise returns @c false.
HCBoolean HCStringIsReal(HCStringRef self);

/// Interprets a string as a real value.
/// @param self A reference to the string.
/// @returns The real value of the string insofar as it could be determined. If the string could not be interpreted as a real value, returns @c 0.0. May skip string content to reach real-encoding characters, such as strings beginning with whitespace. For an exact relation, first test the string with @c HCStringIsReal().
HCReal HCStringAsReal(HCStringRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Comparison
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a string is equal to a C-style string.
/// @param self A reference to the string.
/// @param string A null-terminated buffer containing text data encoded using UTF-8.
/// @returns the same result as calling @c HCStringIsEqual() on @c self and @c HCStringCreateWithCString(string).
HCBoolean HCStringIsEqualToCString(HCStringRef self, const char* string);

/// Determines if a string contains the same UTF-8 code unts as another string.
/// @param self A reference to the string.
/// @param other The string to evaluate code units against.
/// @returns @c true if @c self and @c string contain exactly the same UTF-8 code units in the same order, or @c false otherwise.
HCBoolean HCStringContainsSameCodeUnits(HCStringRef self, HCStringRef other);

/// Determines if a string contains the same UTF-8 code units as a C-style string.
/// @param self A reference to the string.
/// @param string A null-terminated buffer containing text data encoded using UTF-8.
/// @returns the same result as calling @c HCStringContainsSameCodeUnits() on @c self and @c HCStringCreateWithCString(string).
HCBoolean HCStringContainsSameCodeUnitsAsCString(HCStringRef self, const char* string);

#endif /* HCString_h */
