///
/// @file HCObject.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 12/28/18
/// @copyright © 2020 HollowCore Contributors. MIT License.
///
/// @brief The base type for all @c HollowCore objects.
///

#ifndef HCObject_h
#define HCObject_h

#include "HCCore.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------

/// Type of all @c HCObject instances.
extern HCType HCObjectType;

/// Function providing object equality comparison.
typedef HCBoolean (*HCObjectIsEqualFunction)(HCRef self, HCRef other);

/// Function providing object hash value computation.
typedef HCInteger (*HCObjectHashValueFunction)(HCRef self);

/// Function printing an object to a stream.
typedef void (*HCObjectPrintFunction)(HCRef self, FILE* stream);

/// Function that deallocates an object.
typedef void (*HCObjectDestoryFunction)(HCRef self);

/// Type data associated with all @c NSObject instances.
typedef const struct HCObjectTypeData {
    HCTypeData base;
    HCObjectIsEqualFunction isEqual;
    HCObjectHashValueFunction hashValue;
    HCObjectPrintFunction print;
    HCObjectDestoryFunction destroy;
} HCObjectTypeData;

/// A reference to an @c HCObject instance.
typedef struct HCObject* HCObjectRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type Query
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a type is the same as another type.
/// @param type The type to examine.
/// @param other The type to check @a type against.
/// @returns @c true if @c other is of the same type as @c type.
HCBoolean HCTypeIsOfType(HCType type, HCType other);

/// Determines if a type is an ancestor of another type.
/// @param type The type to examine.
/// @param other The type to check @a type against.
/// @returns @c true if @c other is an ancestor of @c type.
HCBoolean HCTypeHasAncestor(HCType type, HCType other);

/// Determines if a type is the same as another type or is a descendant of that type.
/// @param type The type to examine.
/// @param other The type to check @a type against.
/// @returns @c true if @c other is of the same type as @c type or @c other is an ancestor of @c type.
HCBoolean HCTypeIsOfKind(HCType type, HCType other);

/// Determines the type of an object instance.
/// @param object The object whose type should be queried.
/// @returns The type of @c object.
HCType HCObjectTypeOf(HCRef object);

/// Determines the type name of an object instance.
/// @param object The object whose type should be queried.
/// @returns The name of the type of @c object.
HCTypeName HCObjectTypeName(HCRef object);

/// Determines the ancestor type of the type of an object instance.
/// @param object The object whose type should be queried.
/// @returns The ancestor type of the type of @c object.
HCType HCObjectTypeAncestor(HCRef object);

/// Determines if the type of an object instance has the passed type as an ancestor.
/// @param object The object whose type should be queried.
/// @param type The type to compare against.
/// @returns @c true if @c type is an ancestor of the type of @c object.
HCBoolean HCObjectHasAncestor(HCRef object, HCType type);

/// Determines if the type of an object instance is the passed type.
/// @param object The object whose type should be queried.
/// @returns @c true if the type of @c object is @c type.
HCBoolean HCObjectIsOfType(HCRef object, HCType type);

/// Determines if the type of an object instance is the passed type or is a descendant of that type.
/// @param object The object whose type should be queried.
/// @returns @c true if @c type is an ancestor of the type of @c object or they are the same type.
HCBoolean HCObjectIsOfKind(HCRef object, HCType type);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Management
//----------------------------------------------------------------------------------------------------------------------------------

/// Increments the reference count of an object instance.
///
/// When the number of calls to @c HCRelease() matches the number of calls to @c HCRetain(), one final call to @c HCRelease() deallocates the object.
/// By calling @c HCRetain() whenever an @c HCRef is assigned to an object and @c HCRelease() is called whenever an @c HCRef will go out of scope, the referenced object will be deallocated when all references to the object are deactivated.
///
/// @param self The object whose reference count should be increased.
/// @returns @c self so that the call may be chained with an assignment operator.
HCRef HCRetain(HCRef self);

/// Decrements the reference count of an object instance.
///
/// When the number of calls to @c HCRelease() matches the number of calls to @c HCRetain(), one final call to @c HCRelease() deallocates the object.
/// By calling @c HCRetain() whenever an @c HCRef is assigned to an object and @c HCRelease() is called whenever an @c HCRef will go out of scope, the referenced object will be deallocated when all references to the object are deactivated.
///
/// @param self The object whose reference count should be decremented.
void HCRelease(HCRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if an object is equal to another object.
///
/// Uses the @c isEqual parameter of the type of @c self to compare @c self against @c other for equality.
///
/// @param self The object to examine.
/// @param other The object to compare against.
/// @returns The result of calling the @c isEqual function of the type of @c self on @c self and @c other.
HCBoolean HCIsEqual(HCRef self, HCRef other);

/// Determines a hash value representing an object and its content.
///
/// Uses the @c hashValue parameter of the object type to query the object for its hash value.
///
/// @param self The object to examine.
/// @returns The result of calling the @c hashValue function of the type of @c self on @c self.
HCInteger HCHashValue(HCRef self);

/// Prints an object to a stream.
///
/// Uses the @c print parameter of the object type to print the object to a stream.
///
/// @param self The object to examine.
/// @param stream The stream to which @c self should be printed.
void HCPrint(HCRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
// NOTE: Instances of HCObject should be created using sub-types

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a pair of references reference the same object instance.
/// @param self The object to examine.
/// @param other An object to compare against.
/// @returns @c true if @c self references the same object as @c other.
HCBoolean HCObjectIsEqual(HCObjectRef self, HCObjectRef other);

/// Provides a hash value for an object using only the object's reference.
/// @param self The object reference to hash.
/// @returns A hash value resulting from only examining the value @c self.
HCInteger HCObjectHashValue(HCObjectRef self);

/// Prints the type information of an object's type and a text representation of its reference.
/// @param self The object to print.
/// @param stream The stream to which the object's type information should be printed.
void HCObjectPrint(HCObjectRef self, FILE* stream);

#endif /* HCObject_h */
