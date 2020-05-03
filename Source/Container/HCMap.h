///
/// @file HCMap.h
/// @ingroup HollowCore
///
/// @author Matt Stoker
/// @date 2/2/19
/// @copyright © 2020 HollowCore Contributors. MIT License.
///
/// @brief Collection of unordered, exclusive keys associated with objects.
///

#ifndef HCMap_h
#define HCMap_h

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------

/// Type of @c HCMap instances.
extern HCType HCMapType;

/// A reference to an @c HCMap instance.
typedef struct HCMap* HCMapRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Other Definitions
//----------------------------------------------------------------------------------------------------------------------------------

/// An index value indicating an unsuccessful search.
extern const HCInteger HCMapNotFound;

/// Structure used to track iteration over the contents of a map.
typedef struct HCMapIterator {
    #define HCMapIteratorStateSizeStatic (4 * sizeof(HCInteger))
    
    /// The map over which the iterator is configured to iterate.
    HCMapRef map;
    
    /// The current index of iteration.
    HCInteger index;
    
    /// A reference to the object at the current index of iteration.
    HCRef object;
    
    /// A reference to the key at the current index of iteration.
    HCRef key;
    
    /// Private state for tracking iteration.
    HCByte state[HCMapIteratorStateSizeStatic];
} HCMapIterator;

/// An iterator not configured to iterate over a map.
extern const HCMapIterator HCMapIteratorInvalid;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------

/// Creates an empty map.
/// @returns A reference to the created map.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCMapRef HCMapCreate(void);

/// Creates an empty map with an initial capacity.
/// @param capacity The initial capacity of the created map.
/// @returns A reference to the created map.
///     When finished with the reference, call @c HCRelease() on the reference to decrement the referenced object's reference count and destroy it when all references to are released.
HCMapRef HCMapCreateWithCapacity(HCInteger capacity);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if the contents of a map are equal to those of another map.
/// @param self A reference to the map to examine.
/// @param other The other map to evaluate equality against.
/// @returns @c true if @c self and @c other have the same key count and @c HCIsEqual() returns @c true for all keys and their assocaited objects in @c self when evaluated against the correspond key / object pair of @c other. Returns @c false otherwise. @c self and @c other may have differing capacities and still evaluate equal.
HCBoolean HCMapIsEqual(HCMapRef self, HCMapRef other);

/// Calculates a hash value for the key object pairs of a map.
/// @param self A reference to the map.
/// @returns A hash value determined from each map key and associated object using @c HCHashValue().
HCInteger HCMapHashValue(HCMapRef self);

/// Prints a map to a stream.
/// @param self A reference to the map.
/// @param stream The stream to which the map and its keys and objects should be printed. Uses @c HCPrint() to print each key and associated object in the map.
void HCMapPrint(HCMapRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Content
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a map contains no keys.
/// @param self A reference to the map.
/// @returns @c true if a map contains no keys. Its capacity may still be larger than zero.
HCBoolean HCMapIsEmpty(HCMapRef self);

/// Determines the number of keys contained in the map.
/// @param self A reference to the map.
/// @returns The map key count. Objects are associated with each key.
HCInteger HCMapCount(HCMapRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Searching
//----------------------------------------------------------------------------------------------------------------------------------

/// Determines if a key is in a map.
/// @param self A reference to the map.
/// @param key The key to search for.
/// @returns @c true if a key in the map returned @c true to a call to @c HCIsEqual() with @c key. Otherwise returns @c false.
HCBoolean HCMapContainsKey(HCMapRef self, HCRef key);

/// Obtains the key at the first iteration index in a map.
/// @param self A reference to the map.
/// @returns The key found at the first iteration index in the map. This function will return the same key when called unless the map is modified. If the map is empty, returns @c NULL.
HCRef HCMapFirstKey(HCMapRef self);

/// Obtains the key at the last iteration index in a map.
/// @param self A reference to the map.
/// @returns The key found at the last iteration index in the map. This function will return the same key when called unless the map is modified. If the map is empty, returns @c NULL.
HCRef HCMapLastKey(HCMapRef self);

/// Obtains a key in a map at a specified iteration index.
/// @param self A reference to the map.
/// @param index The iteration index of the desired map key. Should be in the range @code [0, HCMapCount()) @endcode.
/// @returns The key found at the iteration index in the map. If the map does not contain a key at iteration index @c index, returns @c NULL.
HCRef HCMapKeyAtIterationIndex(HCMapRef self, HCInteger index);

/// Determines if an object is in a map.
/// @param self A reference to the map.
/// @param object The object to search for.
/// @returns @c true if an object in the map returned @c true to a call to @c HCIsEqual() with @c object. Otherwise returns @c false.
HCBoolean HCMapContainsObject(HCMapRef self, HCRef object);

/// Obtains the object associated with the key at the first iteration index in a map.
/// @param self A reference to the map.
/// @returns The object associated with the key at the first iteration index in the map. This function will return the same object when called unless the map is modified. If the map is empty, returns @c NULL.
HCRef HCMapFirstObject(HCMapRef self);

/// Obtains the object associated with the key at the last iteration index in a map.
/// @param self A reference to the map.
/// @returns The object associated with the key at the last iteration index in the map. This function will return the same object when called unless the map is modified. If the map is empty, returns @c NULL.
HCRef HCMapLastObject(HCMapRef self);

/// Obtains the object associated with the key in a map at a specified iteration index.
/// @param self A reference to the map.
/// @param index The iteration index of the key associated with the desired map object. Should be in the range @code [0, HCMapCount()) @endcode.
/// @returns The object associated with the key at the iteration index in the map. If the map does not contain a key at iteration index @c index, returns @c NULL.
HCRef HCMapObjectAtIterationIndex(HCMapRef self, HCInteger index);

/// Obtains the object associated with a key in a map.
/// @param self A reference to the map.
/// @param key The key to search for.
/// @returns A reference to the object in the map associated with the key equal to @c key by @c HCIsEqual() if one exists. Otherwise returns @c NULL.
HCRef HCMapObjectForKey(HCMapRef self, HCRef key);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------

/// Clears a map of contents such that it is emtpy.
/// @param self A reference to the map to modify.
void HCMapClear(HCMapRef self);

/// Adds an object to a map associated with a key.
///
/// When the key and object are added to the map, the map retains the key and object as part of the operation and will release them when they are removed from the map.
/// The caller is still required to release the key and object when it finishes with the object.
/// For a call that releases the key and/or object after adding them to the map in one operation, see @c HCMapAddObjectReleasedForKey() and @c HCMapAddObjectReleasedForKeyReleased().
///
/// @param self A reference to the map to modify.
/// @param key The key to which @c object should be associated to in the map. If there is already an object assocated with @c key in the map, that object is released and removed from the map.
/// @param object The object to assocate with @c key.
void HCMapAddObjectForKey(HCMapRef self, HCRef key, HCRef object);

/// Removes an object from a map along with its associated key.
///
/// When the key and object are removed from the map, the map releases the key and object as part of the operation.
/// Since the object is released when it is removed from the map, the caller cannot receive the removed object for other use.
/// For a call that retains the key and/or object before removing it from the map and returns the object, see @c HCMapRemoveObjectRetainedForKey() and @c HCMapRemoveObjectRetainedForKeyReleased().
///
/// If the key does not exist in the map, the map is left unmodified.
///
/// @param self A reference to the map to modify.
/// @param key The key to remove along with its associated object.
void HCMapRemoveObjectForKey(HCMapRef self, HCRef key);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Memory Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------

/// Adds an object to a map associated with a key then releases the object.
///
/// When the object is added to the map, the map releases it after adding its key to its collection and associating the object with it.
/// The caller is no longer required to release the object and the map has retained it. The caller is still required to release the key.
/// For a call that keeps the object retained by both the caller and the map, see @c HCMapAddObjectForKey().
///
/// @param self A reference to the map to modify.
/// @param key The key to which @c object should be associated to in the map. If there is already an object assocated with @c key in the map, that object is released and removed from the map.
/// @param object The object to assocate with @c key.
void HCMapAddObjectReleasedForKey(HCMapRef self, HCRef key, HCRef object);

/// Adds an object to a map associated with a key then retains the object and the key.
///
/// When the object is added to the map, the map releases it and its key after adding its key to its collection and associating the object with it.
/// The caller is no longer required to release the object or the key and the map has retained them.
/// For a call that keeps the object and key retained by both the caller and the map, see @c HCMapAddObjectForKey().
///
/// @param self A reference to the map to modify.
/// @param key The key to which @c object should be associated to in the map. If there is already an object assocated with @c key in the map, that object is released and removed from the map.
/// @param object The object to assocate with @c key.
void HCMapAddObjectReleasedForKeyReleased(HCMapRef self, HCRef key, HCRef object);

/// Removes an object from a map along with its associated key after retaining the object.
///
/// When the object is removed from the map, the map retains it prior to removing it.
/// This allows the caller to receive the removed object for other use, but the caller must later release it. The caller is still responsible to release the key.
/// For a call that does not require the caller to release the removed object, see @c HCMapRemoveObject().
///
/// @param self A reference to the map to modify.
/// @param key The key to remove along with its associated object.
/// @returns The object associated with they key equal to @c key in the map. If the map does not contain @c key, the map is left unmodified and the function returns @c NULL.
HCRef HCMapRemoveObjectRetainedForKey(HCMapRef self, HCRef key);

/// Removes an object from a map along with its associated key after retaining the object and releasing the key.
///
/// When the object is removed from the map, the map retains it prior to removing it.
/// This allows the caller to receive the removed object for other use, but the caller must later release it. The caller is no longer responsible to release the key.
/// For a call that does not require the caller to release the removed object, see @c HCMapRemoveObject().
///
/// @param self A reference to the map to modify.
/// @param key The key to remove along with its associated object.
/// @returns The object associated with they key equal to @c key in the map. If the map does not contain @c key, the map is left unmodified and the function returns @c NULL.
HCRef HCMapRemoveObjectRetainedForKeyReleased(HCMapRef self, HCRef key);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Null-Terminated String Convenience Operations
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMapContainsCStringKey(HCMapRef self, const char* key);
HCRef HCMapObjectForCStringKey(HCMapRef self, const char* key);
void HCMapAddObjectForCStringKey(HCMapRef self, const char* key, HCRef object);
void HCMapRemoveObjectForCStringKey(HCMapRef self, const char* key);
void HCMapAddObjectReleasedForCStringKey(HCMapRef self, const char* key, HCRef object);
HCRef HCMapRemoveObjectRetainedForCStringKey(HCMapRef self, const char* key);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Iteration
//----------------------------------------------------------------------------------------------------------------------------------

/// Initiates an iteration over a map's keys and associated objects.
///
/// Iteration over the map is controlled using the returned iterator and can be queried for liveness using @c HCMapIterationHasBegun() and @c HCMapIterationHasEnded().
/// The iterator tracks iteration over the map keys and provides the current index of iteration and its assoicated key and object references.
/// Use @c HCMapIterationHasNext() to determine if there are more keys to iterate over.
/// Calling @c HCMapInterationNext() moves to the next key in the map.
/// If map iteration proceeds past the end of the map, the resources used by the iterator are deallocated and the iterator marked as finished, which can be queried using @c HCMapIterationHasEnded().
/// If iteration over the map keys must be terminated before reaching the end of the map, call @c HCMapIterationEnd() to mark the iteration complete.
///
/// Iteration over a map @c map using a @c for loop should use the following form:
/// @code
/// for (HCMapIterator i = HCMapIterationBegin(map, startIndex); !HCMapIterationHasEnded(&i); HCMapIterationNext(&i)) { /*...*/ }
/// @endcode
///
/// @param self A reference to the map.
/// @returns An iterator configured to track the iteration.
HCMapIterator HCMapIterationBegin(HCMapRef self);

/// Moves forward one key during a map iteration.
///
/// If the iterator is referencing the last key in the map when the function is called, iteration is ended as if @c HCMapIterationEnd() were called.
///
/// @param iterator An iterator.
void HCMapIterationNext(HCMapIterator* iterator);

/// Ends iteration over a map by marking the iteration ended.
///
/// This function should be called on an iterator if iteration over a map's keys is terminated before reaching the end of the map.
///
/// @param iterator An iterator.
void HCMapIterationEnd(HCMapIterator* iterator);

/// Determines if iteration using an iterator has begun.
/// @param iterator An iterator.
/// @returns @c true if the iterator was returned from a call to @c HCMapIterationBegin() or a related call. Will continue to return @c true even after iteration ends. Returns @c false if the iterator was not created in this way.
HCBoolean HCMapIterationHasBegun(HCMapIterator* iterator);

/// Determines if there is a key following the current key in the map referenced by the iterator.
/// @param iterator An iterator.
/// @returns @c true if a call to @c HCMapIterationNext() will result in @c iterator referencing a valid key.
HCBoolean HCMapIterationHasNext(HCMapIterator* iterator);

/// Determines if the iterator is referencing a valid key in its referenced map.
/// @param iterator An iterator.
/// @returns @c true if iteration has been started on a map and has not yet reached the end of the map or had @c HCMapIterationEnd() called on the iterator. Returns @c false if iteration has not begun or if the iterator is currently referencing a valid key in its referenced map.
HCBoolean HCMapIterationHasEnded(HCMapIterator* iterator);

#endif /* HCMap_h */
