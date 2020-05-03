///
/// @file HCData.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 1/19/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///
/// @brief Object that manages a byte buffer.
///

#ifndef HCData_h
#define HCData_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------

/// Type of @c HCData instances.
extern HCType HCDataType;

/// A reference to an @c HCData instance.
typedef struct HCData* HCDataRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------

/// Creates an empty data object.
/// @returns A reference to the created data object.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCDataRef HCDataCreate(void);

/// Creates a data object initially populated with the contents of a byte buffer.
/// @param size The size in bytes of @c bytes to be copied into the data object.
/// @param bytes Content to be copied to the byte buffer. Must point to a buffer of at least @c size bytes.
/// @returns A reference to the created data object.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCDataRef HCDataCreateWithBytes(HCInteger size, const HCByte* bytes);

/// Creates a data object initially populated with the contents of a boolean value.
/// @param value The boolean value to be copied into the data object.
/// @returns A reference to the created data object.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCDataRef HCDataCreateWithBoolean(HCBoolean value);

/// Creates a data object initially populated with the contents of an integer value.
/// @param value The integer value to be copied into the data object.
/// @returns A reference to the created data object.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCDataRef HCDataCreateWithInteger(HCInteger value);

/// Creates a data object initially populated with the contents of a real value.
/// @param value The real value to be copied into the data object.
/// @returns A reference to the created data object.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCDataRef HCDataCreateWithReal(HCReal value);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if the contents of a data object are exactly equal to those of another data object.
/// @param self A reference to the data object to examine.
/// @param other The other data object to evaluate equality against.
/// @returns @c true if @c self and @c other contain exactly the same bytes and have the same size.
HCBoolean HCDataIsEqual(HCDataRef self, HCDataRef other);

/// Calculates a hash value for a data object.
/// @param self A reference to the data object.
/// @returns A hash value determined using only the content of the data object.
HCInteger HCDataHashValue(HCDataRef self);

/// Prints a data object to a stream.
/// @param self A reference to the data object.
/// @param stream The stream to which the data object and content should be printed.
void HCDataPrint(HCDataRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a data object contains no bytes.
/// @param self A reference to the data object.
/// @returns @c true if @c HCDataSize() is @c 0, or @c false otherwise.
HCBoolean HCDataIsEmpty(HCDataRef self);

/// Obtains the size of a data object's contents.
/// @param self A reference to the data object.
/// @returns The size in bytes of the contents of the data object.
HCInteger HCDataSize(HCDataRef self);

/// Obtains a data object's contents.
/// @param self A reference to the data object.
/// @returns A pointer to a buffer containing the data object's contents. The buffer is of size @c HCDataSize(). The buffer should be considered read-only, and is valid until the data object is next modified or released.
const HCByte* HCDataBytes(HCDataRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a data object contains a boolean value.
/// @param self A reference to the data object.
/// @returns @c true if the size of the data object's content is equal to @c sizeof(HCBoolean) and contains @c 0 or @c 1. Otherwise returns @c false.
HCBoolean HCDataIsBoolean(HCDataRef self);

/// Interprets the contents of a data object as a boolean value.
/// @param self A reference to the data object.
/// @returns @c false if the first @c sizeof(HCBoolean) bytes of the data object are @c 0. Otherwise returns @c true.
HCBoolean HCDataAsBoolean(HCDataRef self);

/// Determines if a data object contains an integer value.
/// @param self A reference to the data object.
/// @returns @c true if the size of the data object's content is equal to @c sizeof(HCInteger). Otherwise returns @c false.
HCBoolean HCDataIsInteger(HCDataRef self);

/// Interprets the contents of a data object as an integer value.
/// @param self A reference to the data object.
/// @returns The first @c sizeof(HCInteger) bytes of the data object interpreted as an integer value.
HCInteger HCDataAsInteger(HCDataRef self);

/// Determines if a data object contains a real value.
/// @param self A reference to the data object.
/// @returns @c true if the size of the data object's content is equal to @c sizeof(HCReal) and is not an invalid real value according to @c HCRealIsInvalid(). Otherwise returns @c false.
HCBoolean HCDataIsReal(HCDataRef self);

/// Interprets the contents of a data object as a real value.
/// @param self A reference to the data object.
/// @returns The first @c sizeof(HCReal) bytes of the data object interpreted as a real value. If @c HCRealIsInvalid() returns @c true for these bytes, returns @c HCRealInvalid.
HCReal HCDataAsReal(HCDataRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------

/// Changes the value of a region of a data object's content.
/// @param self A reference to the data object to modify.
/// @param location Offset in bytes of the location to update in the data object's content. Must be between @c 0 and @code HCDataSize() - size @endcode.
/// @param size The size in bytes of the region to be changed in the data object's content.
/// @param bytes The content that should be copied into the region @code [location, location + size) @endcode. Must point to a buffer of at least @c size bytes. Specifying a buffer within the data object's content is a valid operation, but passing a region overlaps the destination region results in undefined behavior.
void HCDataChangeBytes(HCDataRef self, HCInteger location, HCInteger size, const HCByte* bytes);

/// Clears a data object contents such that it is emtpy.
/// @param self A reference to the data object to modify.
void HCDataClear(HCDataRef self);

/// Appends bytes to the end of a data object's contents.
/// @param self A reference to the data object to modify.
/// @param size The size in bytes of the additional data to append to the data object's contents.
/// @param bytes The content that should be copied to fill the data object's appended content. Must point to a buffer of at least @c size bytes.
void HCDataAddBytes(HCDataRef self, HCInteger size, const HCByte* bytes);

/// Removes bytes from the end of a data object's contents.
/// @param self A reference to the data object to modify.
/// @param size The size in bytes of the content area  to the remove from the data object's contents.
void HCDataRemoveBytes(HCDataRef self, HCInteger size);

/// Appends a boolean value to a data object's contents.
///
/// Functions as if @code HCDataAddBytes(self, sizeof(HCBoolean), (HCByte*)&value) @endcode were called.
///
/// @param self A reference to the data object to modify.
/// @param value The boolean value to append to the data object's contents.
void HCDataAddBoolean(HCDataRef self, HCBoolean value);

/// Removes a boolean from the end of a data object's contents.
///
/// Functions as if @code HCDataRemoveBytes(self, sizeof(HCBoolean)) @endcode were called.
///
/// @param self A reference to the data object to modify.
void HCDataRemoveBoolean(HCDataRef self);

/// Appends an integer value to a data object's contents.
///
/// Functions as if @code HCDataAddBytes(self, sizeof(HCInteger), (HCByte*)&value) @endcode were called.
///
/// @param self A reference to the data object to modify.
/// @param value The integer value to append to the data object's contents.
void HCDataAddInteger(HCDataRef self, HCInteger value);

/// Removes an integer from the end of a data object's contents.
///
/// Functions as if @code HCDataRemoveBytes(self, sizeof(HCInteger)) @endcode were called.
///
/// @param self A reference to the data object to modify.
void HCDataRemoveInteger(HCDataRef self);

/// Appends a real value to a data object's contents.
///
/// Functions as if @code HCDataAddBytes(self, sizeof(HCReal), (HCByte*)&value) @endcode were called.
///
/// @param self A reference to the data object to modify.
/// @param value The integer value to append to the data object's contents.
void HCDataAddReal(HCDataRef self, HCReal value);

/// Removes a real from the end of a data object's contents.
///
/// Functions as if @code HCDataRemoveBytes(self, sizeof(HCReal)) @endcode were called.
///
/// @param self A reference to the data object to modify.
void HCDataRemoveReal(HCDataRef self);

#endif /* HCData_h */
