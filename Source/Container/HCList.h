///
/// @file HCList.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 1/21/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///

#ifndef HCList_h
#define HCList_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------

/// Type of @c HCList instances.
extern HCType HCListType;

/// A reference to an @c HCList instance.
typedef struct HCList* HCListRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------

/// An index value indicating an unsuccessful search.
extern const HCInteger HCListNotFound;

/// Object used to track iteration over the contents of a list.
typedef struct HCListIterator {
    HCListRef list;
    HCInteger index;
    HCRef object;
    void* state; // Private
} HCListIterator;
extern const HCListIterator HCListIteratorInvalid;

/// Function operating on a value of a list during a for-each iteration over the list.
typedef void (*HCListForEachFunction)(void* context, HCRef value);

/// Function used to categorize a value of a list during a filter iteration over the list.
typedef HCBoolean (*HCListFilterFunction)(void* context, HCRef value);

/// Function used to map a value of a list to another value during a map iteration over the list.
typedef HCRef (*HCListMapFunction)(void* context, HCRef value);

/// Function used to reduce a value of a list into an aggregate value during a reduce iteration over the list.
typedef HCRef (*HCListReduceFunction)(void* context, HCRef aggregate, HCRef value);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------

/// Creates an empty list.
/// @returns A reference to the created list.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCListRef HCListCreate(void);


/// Creates an empty list with an initial capacity.
/// @param capacity The initial capacity of the created list.
/// @returns A reference to the created list.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCListRef HCListCreateWithCapacity(HCInteger capacity);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if the contents of a list are equal to those of another list.
/// @param self A reference to the list to examine.
/// @param other The other list to evaluate equality against.
/// @returns @c true if @c self and @c other have the same element count and @c HCIsEqual() returns @c true for all elements in @c self when evaluated against the correspondingly indexed element of @c other. Returns @c false otherwise. @c self and @c other may have differing capacities and still evaluate equal.
HCBoolean HCListIsEqual(HCListRef self, HCListRef other);

/// Calculates a hash value for the elements of a list.
/// @param self A reference to the list.
/// @returns A hash value determined from each list element using @c HCHashValue().
HCInteger HCListHashValue(HCListRef self);

/// Prints a list to a stream.
/// @param self A reference to the list.
/// @param stream The stream to which the list and its elements should be printed. Uses @c HCPrint() to print each element in the list.
void HCListPrint(HCListRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Content
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a list contains no elements.
/// @param self A reference to the list.
/// @returns @c true if a list contains no elements. Its capacity may still be larger than zero.
HCBoolean HCListIsEmpty(HCListRef self);

/// Determines the number of elements contained in the list.
/// @param self A reference to the list.
/// @returns The list element count.
HCInteger HCListCount(HCListRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Searching
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if an index indexes an element in a list.
/// @param self A reference to the list.
/// @param index The index the list should be inspected for.
/// @returns @c true if @c index is in the range @code [0, HCListCount()) @endcode. Otherwise returns @c false. A return value of @c true, indicates that a call to @c HCListObjectAtIndex() will succeed.
HCBoolean HCListContainsIndex(HCListRef self, HCInteger index);

/// Determines if an object is in a list.
/// @param self A reference to the list.
/// @param object The object to search for.
/// @returns @c true if an element in the list returned @c true to a call to @c HCIsEqual() with @c object. Otherwise returns @c false.
HCBoolean HCListContainsObject(HCListRef self, HCRef object);

/// Searches a list for the first occurance of an object.
/// @param self A reference to the list.
/// @param object The object to search for.
/// @returns The index of the first element in the list that returns @c true to a call to @c HCIsEqual() with @c object. If no objects are equal,  returns @c HCListNotFound.
HCInteger HCListFirstIndexOfObject(HCListRef self, HCRef object);

/// Searches a list for the last occurance of an object.
/// @param self A reference to the list.
/// @param object The object to search for.
/// @returns The index of the last element in the list that returns @c true to a call to @c HCIsEqual() with @c object. If no objects are equal,  returns @c HCListNotFound.
HCInteger HCListLastIndexOfObject(HCListRef self, HCRef object);

/// Searches a list for for an object with a specified start index.
/// @param self A reference to the list.
/// @param searchIndex The index from which searching should begin. Must be in the range @code [0, HCListCount()) @endcode.
///
/// @param object The object to search for.
/// @returns The index of the last element in the list that returns @c true to a call to @c HCIsEqual() with @c object. If no objects are equal,  returns @c HCListNotFound.
HCInteger HCListIndexOfObject(HCListRef self, HCInteger searchIndex, HCBoolean reverseSearch, HCRef object);

/// Obtains the first object in a list.
/// @param self A reference to the list.
/// @returns The element found at the first index in the list. If the list is empty, returns @c NULL.
HCRef HCListFirstObject(HCListRef self);

/// Obtains the last object in a list.
/// @param self A reference to the list.
/// @returns The element found at the last index in the list. If the list is empty, returns @c NULL.
HCRef HCListLastObject(HCListRef self);

/// Obtains a object in a list.
/// @param self A reference to the list.
/// @param index The index of the desired list element. Should be in the range @code [0, HCListCount()) @endcode.
/// @returns The element found at the first index in the list. If the list does not contain @c index, returns @c NULL.
HCRef HCListObjectAtIndex(HCListRef self, HCInteger index);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCListClear(HCListRef self);

void HCListAddObject(HCListRef self, HCRef object);
void HCListRemoveObject(HCListRef self);
void HCListAddObjectAtIndex(HCListRef self, HCInteger index, HCRef object);
void HCListRemoveObjectAtIndex(HCListRef self, HCInteger index);

void HCListRemoveFirstObjectEqualToObject(HCListRef self, HCRef object);
void HCListRemoveLastObjectEqualToObject(HCListRef self, HCRef object);
void HCListRemoveAllObjectsEqualToObject(HCListRef self, HCRef object);
void HCListRemoveObjectEqualToObject(HCListRef self, HCInteger searchIndex, HCBoolean reverseSearch, HCRef object);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCListAddObjectReleased(HCListRef self, HCRef object);
HCRef HCListRemoveObjectRetained(HCListRef self);
void HCListAddObjectReleasedAtIndex(HCListRef self, HCInteger index, HCRef object);
HCRef HCListRemoveObjectRetainedAtIndex(HCListRef self, HCInteger index);

HCRef HCListRemoveFirstObjectRetainedEqualToObject(HCListRef self, HCRef object);
HCRef HCListRemoveLastObjectRetainedEqualToObject(HCListRef self, HCRef object);
HCRef HCListRemoveObjectRetainedEqualToObject(HCListRef self, HCInteger searchIndex, HCBoolean reverseSearch, HCRef object);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Iteration
//----------------------------------------------------------------------------------------------------------------------------------
HCListIterator HCListIterationBegin(HCListRef self);
HCListIterator HCListIterationBeginAtLast(HCListRef self);
HCListIterator HCListIterationBeginAtIndex(HCListRef self, HCInteger index);
void HCListIterationPrevious(HCListIterator* iterator);
void HCListIterationNext(HCListIterator* iterator);
void HCListIterationEnd(HCListIterator* iterator);
HCBoolean HCListIterationHasBegun(HCListIterator* iterator);
HCBoolean HCListIterationHasPrevious(HCListIterator* iterator);
HCBoolean HCListIterationHasNext(HCListIterator* iterator);
HCBoolean HCListIterationHasEnded(HCListIterator* iterator);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Iteration Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCListForEach(HCListRef self, HCListForEachFunction forEachFunction, void* context);
HCListRef HCListFilterRetained(HCListRef self, HCListFilterFunction isIncluded, void* context);
HCListRef HCListMapRetained(HCListRef self, HCListMapFunction transform, void* context);
HCRef HCListReduceRetained(HCListRef self, HCRef initialValue, HCListReduceFunction nextPartialResult, void* context);

#endif /* HCList_h */
