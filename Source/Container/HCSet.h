///
/// @file HCSet.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 1/23/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///
/// @brief Collection of unordered, exclusive object elements.
///

#ifndef HCSet_h
#define HCSet_h

#include "../Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------

/// Type of @c HCSet instances.
extern HCType HCSetType;

/// A reference to an @c HCSet instance.
typedef struct HCSet* HCSetRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------

/// An index value indicating an unsuccessful search.
extern const HCInteger HCSetNotFound;

/// Structure used to track iteration over the contents of a set.
typedef struct HCSetIterator {
    /// The set over which the iterator is configured to iterate.
    HCSetRef set;
    
    /// The current index of iteration.
    HCInteger index;
    
    /// A reference to the object at the current index of iteration.
    HCRef object;
    
    /// Private state for tracking iteration.
    void* state;
} HCSetIterator;

/// An iterator not configured to iterate over a set.
extern const HCSetIterator HCSetIteratorInvalid;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------

/// Creates an empty set.
/// @returns A reference to the created set.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCSetRef HCSetCreate(void);

/// Creates an empty set with an initial capacity.
/// @param capacity The initial capacity of the created set.
/// @returns A reference to the created set.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCSetRef HCSetCreateWithCapacity(HCInteger capacity);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if the contents of a set are equal to those of another set.
/// @param self A reference to the set to examine.
/// @param other The other set to evaluate equality against.
/// @returns @c true if @c self and @c other have the same element count and @c HCIsEqual() returns @c true for all elements in @c self when evaluated against the correspondingly indexed element of @c other. Returns @c false otherwise. @c self and @c other may have differing capacities and still evaluate equal.
HCBoolean HCSetIsEqual(HCSetRef self, HCSetRef other);

/// Calculates a hash value for the elements of a set.
/// @param self A reference to the set.
/// @returns A hash value determined from each set element using @c HCHashValue().
HCInteger HCSetHashValue(HCSetRef self);

/// Prints a set to a stream.
/// @param self A reference to the set.
/// @param stream The stream to which the set and its elements should be printed. Uses @c HCPrint() to print each element in the set.
void HCSetPrint(HCSetRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Content
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a set contains no elements.
/// @param self A reference to the set.
/// @returns @c true if a set contains no elements. Its capacity may still be larger than zero.
HCBoolean HCSetIsEmpty(HCSetRef self);

/// Determines the number of elements contained in the set.
/// @param self A reference to the set.
/// @returns The set element count.
HCInteger HCSetCount(HCSetRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Searching
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if an object is in a set.
/// @param self A reference to the set.
/// @param object The object to search for.
/// @returns @c true if an element in the set returned @c true to a call to @c HCIsEqual() with @c object. Otherwise returns @c false.
HCBoolean HCSetContainsObject(HCSetRef self, HCRef object);

/// Obtains the object at the first iteration index in a set.
/// @param self A reference to the set.
/// @returns The element found at the first iteration index in the set. This function will return the same object when called unless the set is modified. If the set is empty, returns @c NULL.
HCRef HCSetFirstObject(HCSetRef self);

/// Obtains the object at the last iteration index in a set.
/// @param self A reference to the set.
/// @returns The element found at the last iteration index in the set. This function will return the same object when called unless the set is modified. If the set is empty, returns @c NULL.
HCRef HCSetLastObject(HCSetRef self);

/// Obtains a object in a set at a specified iteration index.
/// @param self A reference to the set.
/// @param index The iteration index of the desired set element. Should be in the range @code [0, HCSetCount()) @endcode.
/// @returns The element found at the iteration index in the set. If the set does not contain an object at iteration index @c index, returns @c NULL.
HCRef HCSetObjectAtIterationIndex(HCSetRef self, HCInteger index);

/// Finds the object equal to a specified object in a set.
/// @param self A reference to the set.
/// @param object The object to compare against in the search.
/// @returns A reference to the object in the set equal to @c object by @c HCIsEqual() if one exists. Otherwise returns @c NULL.
HCRef HCSetObjectEqualToObject(HCSetRef self, HCRef object);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------

/// Clears a set of contents such that it is emtpy.
/// @param self A reference to the set to modify.
void HCSetClear(HCSetRef self);

/// Adds an element to a set.
///
/// When the object is added to the set, the set retains the object as part of the operation and will release it when it is removed from the set.
/// The caller is still required to release the object when it finishes with the object.
/// For a call that releases the object after adding it to the set in one operation, see @c HCSetAddObjectReleased().
///
/// @param self A reference to the set to modify.
/// @param object The object to add as an element to the set.
void HCSetAddObject(HCSetRef self, HCRef object);

/// Removes an element from a set.
///
/// When the object is removed from the set, the set releases the object as part of the operation.
/// Since the object is released when it is removed from the set, the caller cannot receive the removed object for other use.
/// For a call that retains the object before removing it from the set and returns it, see @c HCSetRemoveObjectRetained().
///
/// If the object does not exist in the set, the set is left unmodified.
///
/// @param self A reference to the set to modify.
void HCSetRemoveObject(HCSetRef self, HCRef object);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------

/// Adds an element to a set and then releases it.
///
/// When the object is added to the set, the set releases it after adding it to its element collection.
/// The caller is no longer required to release the object and the set has retained it.
/// For a call that keeps the object retained by both the caller and the set, see @c HCSetAddObject().
///
/// @param self A reference to the set to modify.
/// @param object The object to append as an element to the set.
void HCSetAddObjectReleased(HCSetRef self, HCRef object);

/// Removes an element from a set after retaining it.
///
/// When the object is removed from the set, the set retains it prior to removing it.
/// This allows the caller to receive the removed object for other use, but the caller must later release it.
/// For a call that does not require the caller to release the removed object, see @c HCSetRemoveObject().
///
/// @param self A reference to the set to modify.
/// @returns The object equal to @c object in the set. If the set does not contain @c object, the set is left unmodified and the function returns @c NULL.
HCRef HCSetRemoveObjectRetained(HCSetRef self, HCRef object);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Iteration
//----------------------------------------------------------------------------------------------------------------------------------

/// Initiates an iteration over a set's elements.
///
/// Iteration over the set is controlled using the returned iterator and can be queried for liveness using @c HCSetIterationHasBegun() and @c HCSetIterationHasEnded().
/// The iterator tracks iteration over the set elements and provides the current index of iteration and its assoicated object reference.
/// Use @c HCSetIterationHasNext() to determine if there are more elements to iterate over.
/// Calling @c HCSetInterationNext() moves to the next element in the set.
/// If set iteration proceeds past the end of the set, the resources used by the iterator are deallocated and the iterator marked as finished, which can be queried using @c HCSetIterationHasEnded().
/// If iteration over the set elements must be terminated before reaching the end of the set, call @c HCSetIterationEnd() to mark the iteration complete.
///
/// Iteration over a set @c set using a @c for loop should use the following form:
/// @code
/// for (HCSetIterator i = HCSetIterationBegin(set, startIndex); !HCSetIterationHasEnded(&i); HCSetIterationNext(&i)) { /*...*/ }
/// @endcode
///
/// @param self A reference to the set.
/// @returns An iterator configured to track the iteration.
HCSetIterator HCSetIterationBegin(HCSetRef self);

/// Moves forward one element during a set iteration.
///
/// If the iterator is referencing the last element in the set when the function is called, iteration is ended as if @c HCSetIterationEnd() were called.
///
/// @param iterator An iterator.
void HCSetIterationNext(HCSetIterator* iterator);

/// Ends iteration over a set by marking the iteration ended.
///
/// This function should be called on an iterator if iteration over a set's elements is terminated before reaching the end of the set.
///
/// @param iterator An iterator.
void HCSetIterationEnd(HCSetIterator* iterator);

/// Determines if iteration using an iterator has begun.
/// @param iterator An iterator.
/// @returns @c true if the iterator was returned from a call to @c HCSetIterationBeginAtIndex() or a related call. Will continue to return @c true even after iteration ends. Returns @c false if the iterator was not created in this way.
HCBoolean HCSetIterationHasBegun(HCSetIterator* iterator);

/// Determines if there is an element following the current element in the set referenced by the iterator.
/// @param iterator An iterator.
/// @returns @c true if a call to @c HCSetIterationNext() will result in @c iterator referencing a valid element.
HCBoolean HCSetIterationHasNext(HCSetIterator* iterator);

/// Determines if the iterator is referencing a valid element in its referenced set.
/// @param iterator An iterator.
/// @returns @c true if iteration has been started on a set and has not yet reached the end of the set or had @c HCSetIterationEnd() called on the iterator. Returns @c false if iteration has not begun or if the iterator is currently referencing a valid element in its referenced set.
HCBoolean HCSetIterationHasEnded(HCSetIterator* iterator);

#endif /* HCSet_h */
