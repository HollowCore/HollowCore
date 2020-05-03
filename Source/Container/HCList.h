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

/// Clears a list of contents such that it is emtpy.
/// @param self A reference to the list to modify.
void HCListClear(HCListRef self);

/// Appends an element to the end of a list.
///
/// When the object is added to the list, the list retains the object as part of the operation and will release it when it is removed from the list.
/// The caller is still required to release the object when it finishes with the object.
/// For a call that releases the object after adding it to the list in one operation, see @c HCListAddObjectReleased().
///
/// @param self A reference to the list to modify.
/// @param object The object to append as an element to the list.
void HCListAddObject(HCListRef self, HCRef object);

/// Removes an element from the end of a list.
///
/// When the object is removed from the list, the list releases the object as part of the operation.
/// Since the object is released when it is removed from the list, the caller cannot receive the removed object for other use.
/// For a call that retains the object before removing it from the list and returns it, see @c HCListRemoveObjectRetained().
///
/// If the index does not exist in the list, the list is left unmodified.
///
/// @param self A reference to the list to modify.
void HCListRemoveObject(HCListRef self);

/// Inserts an element into a list at an index.
///
/// When the object is added to the list, the list retains the object as part of the operation and will release it when it is removed from the list.
/// The caller is still required to release the object when it finishes with the object.
/// For a call that releases the object after adding it to the list in one operation, see @c HCListAddObjectReleasedAtIndex().
///
/// @param self A reference to the list to modify.
/// @param index The index in the list the object should occupy.
/// @param object The object to add as an element into the list.
void HCListAddObjectAtIndex(HCListRef self, HCInteger index, HCRef object);

/// Removes an element from a list at an index.
///
/// When the object is removed from the list, the list releases the object as part of the operation.
/// Since the object is released when it is removed from the list, the caller cannot receive the removed object or other use.
/// For a call that retains the object before removing it from the list and returns it, see @c HCListRemoveObjectRetainedAtIndex().
///
/// @param self A reference to the list to modify.
/// @param index The index of the object to remove. If the index does not exist in the list, the list is left unmodified.
void HCListRemoveObjectAtIndex(HCListRef self, HCInteger index);

/// Searches for and removes an object from the start of a list.
/// @param self A reference to the list to modify.
/// @param object The object to search for. Uses @c HCIsEqual() to determine when the object has been found. If no element is equal to @c object, the list is left unmodified.
void HCListRemoveFirstObjectEqualToObject(HCListRef self, HCRef object);

/// Searches for and removes an object from the end of a list.
/// @param self A reference to the list to modify.
/// @param object The object to search for. Uses @c HCIsEqual() to determine when the object has been found. If no element is equal to @c object, the list is left unmodified.
void HCListRemoveLastObjectEqualToObject(HCListRef self, HCRef object);

/// Searches for and removes an object from a list starting at a specified index.
/// @param self A reference to the list to modify.
/// @param searchIndex The index in the list to start looking for the object.
/// @param reverseSearch If @c false, searches forward from @c searchIndex for the object. If @c true, searches backward from @c searchIndex for the object.
/// @param object The object to search for. Uses @c HCIsEqual() to determine when the object has been found. If no element is equal to @c object, the list is left unmodified.
void HCListRemoveObjectEqualToObject(HCListRef self, HCInteger searchIndex, HCBoolean reverseSearch, HCRef object);

/// Searches for and removes all objects equal to an object from a list.
/// @param self A reference to the list to modify.
/// @param object The object to search for. Uses @c HCIsEqual() to determine when the object has been found. If no element is equal to @c object, the list is left unmodified.
void HCListRemoveAllObjectsEqualToObject(HCListRef self, HCRef object);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------

/// Appends an element to the end of a list and then releases it.
///
/// When the object is added to the list, the list releases it after adding it to its element collection.
/// The caller is no longer required to release the object as the list now owns it.
/// For a call that keeps the object retained by both the caller and the list, see @c HCListAddObject().
///
/// @param self A reference to the list to modify.
/// @param object The object to append as an element to the list.
void HCListAddObjectReleased(HCListRef self, HCRef object);

/// Removes an element from the end of a list after retaining it.
///
/// When the object is removed from the list, the list retains it prior to removing it.
/// This allows the caller to receive the removed object for other use, but the caller must later release it.
/// For a call that does not require the caller to release the removed object, see @c HCListRemoveObject().
///
/// @param self A reference to the list to modify.
/// @returns The last object in the list. If the list is empty, the list is left unmodified and the function returns @c NULL.
HCRef HCListRemoveObjectRetained(HCListRef self);

/// Inserts an element into a list at an index.
///
/// When the object is added to the list, the list releases it after adding it to its element collection.
/// The caller is no longer required to release the object as the list now owns it.
/// For a call that keeps the object retained by both the caller and the list, see @c HCListAddObjectAtIndex().
///
/// @param self A reference to the list to modify.
/// @param index The index in the list the object should occupy.
/// @param object The object to insert as an element into the list.
void HCListAddObjectReleasedAtIndex(HCListRef self, HCInteger index, HCRef object);

/// Removes an element from a list at an index after retaining it.
///
/// When the object is removed from the list, the list retains it prior to removing it.
/// This allows the caller to receive the removed object for other use, but the caller must later release it.
/// For a call that does not require the caller to release the removed object, see @c HCListRemoveObjectAtIndex().
///
/// @param self A reference to the list to modify.
/// @param index The index of the object to remove. If the index does not exist in the list, the list is left unmodified.
HCRef HCListRemoveObjectRetainedAtIndex(HCListRef self, HCInteger index);

/// Searches for and removes an object from the start of a list after retaining it.
/// @param self A reference to the list to modify.
/// @param object The object to search for. Uses @c HCIsEqual() to determine when the object has been found. If no element is equal to @c object, the list is left unmodified and the function returns @c NULL.
HCRef HCListRemoveFirstObjectRetainedEqualToObject(HCListRef self, HCRef object);

/// Searches for and removes all objects equal to an object from a list after retaining it.
/// @param self A reference to the list to modify.
/// @param object The object to search for. Uses @c HCIsEqual() to determine when the object has been found. If no element is equal to @c object, the list is left unmodified and the function returns @c NULL.
HCRef HCListRemoveLastObjectRetainedEqualToObject(HCListRef self, HCRef object);

/// Searches for and removes an object from a list starting at a specified index.
/// @param self A reference to the list to modify.
/// @param searchIndex The index in the list to start looking for the object.
/// @param reverseSearch If @c false, searches forward from @c searchIndex for the object. If @c true, searches backward from @c searchIndex for the object.
/// @param object The object to search for. Uses @c HCIsEqual() to determine when the object has been found. If no element is equal to @c object, the list is left unmodified and the function returns @c NULL.
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
