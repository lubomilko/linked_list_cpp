#ifndef LINKED_LIST_H
#define LINKED_LIST_H


using namespace std;

// *************************************************************************************************
// MACROS
// *************************************************************************************************
// Default array size for hash table that uses open hashing algorithm. 
#define DEFAULT_HASH_TABLE_SIZE 10u

// *************************************************************************************************
// CLASS DECLARATIONS
// *************************************************************************************************
// Forward declaration of LinkedList class so it can be specified as a friend in LinkedListItem class.
// "T" is used as a custom type name, because this letter is commonly used for such purpose and it cannot
// be mistaken for something else, since there is only one template type used.
template <typename T>
class LinkedList;

template <typename T>
// Class representing one item of the linked list.
// Could be a struct instead of class, but class allows hiding the data value and a pointer
// to the next item, so the list cannot be damaged so easily from outside.
class LinkedListItem
{
    // Ensuring that the linked list class has access to the private attributes of the linked list item.
	friend class LinkedList<T>;

    public:
        // Constructor directly setting the data of the list item.
        LinkedListItem(T);
        // Destructor setting the next item pointer to null.
        ~LinkedListItem();

    private:
        // Data held within the list item.
        // Note:
        //    The data type must be comparable using "==" or "!=" operator due to the simplicity of the
        //    linked list implementation. See LinkedList class method GetCurrentPreviousItemPointer for
        //    more details.
        T data;
        // Pointed to the next linked list item.
        LinkedListItem* p_next;
};


// Linked list class for linear lists of data. Each item represented by LinkedListItem object
// consists of data (value) and a pointer to the next item.
template <typename T>
class LinkedList
{
    public:
        // Constructor creating empty list.
        LinkedList();
        // Destructor deleting all list items from memory.
        ~LinkedList();
        // Adds item with specified data at the end of the list.
        void AddItemAtEnd(const T);
        // Deletes first instance of an item having the specified data value.
        void DeleteItem(const T);
        // Get data of the next item through the reference argument.
        // Return value indicates if there are further items to be read, i.e. if the list still continues.
        bool GetNextItem(T &);
        // Moves reading index used by GetNextItem method back to the list head item.
        void ResetReading();
        // Indicates whether item with specified data is present.
        bool IsItemPresent(const T);
        // Returns number of items in the linked list.
        unsigned long GetItemsNum();
        // Forcing a link from one item to another. To be used only for intentional creation of looped list.
        void SetNextReference(const T, const T);
        // Indicates if list is looping using open hashing algorithm.
        bool IsLooping_OpenHash(unsigned int=DEFAULT_HASH_TABLE_SIZE);
        // Indicates if list is looping detection by counting the number of transitions made when traversing the list.
        bool IsLooping_TransitionCount();
        // Indicates if list is looping using Floyd's cycle detection algorithm.
        bool IsLooping_FloydsCycleFind();

    private:
        // Pointers to the linked list head and tail.
        LinkedListItem<T> *p_head;
        LinkedListItem<T> *p_tail;
        // Pointer to the linked list item used by GetNextItem method.
        LinkedListItem<T> *p_listing;
        // Number of items in a list.
        // number_of_items = added_items_number - deleted_items_number
        unsigned long items_num;

        // Utility function getting pointer to the current item with the specified data and also pointer to the previous item. 
        void GetCurrentPreviousItemPointer(const T, LinkedListItem<T> **, LinkedListItem<T> ** = nullptr);
};


// *************************************************************************************************
// CLASS DEFINITIONS
// *************************************************************************************************
// Constructor directly setting the data of the list item.
template <typename T>
LinkedListItem<T>::LinkedListItem(T data)
{
    this->data = data;
    this->p_next = nullptr;
}


// Destructor setting the next item pointer to null.
template <typename T>
LinkedListItem<T>::~LinkedListItem()
{
    // Ensuring that an end of the list is made by setting next pointer to null.
    this->p_next = nullptr;
}


// Constructor creating empty list.
template <typename T>
LinkedList<T>::LinkedList()
{
    // Init all list item pointers to null creating an empty list.
    this->p_head = nullptr;
    this->p_tail = nullptr;
    this->p_listing = nullptr;
    this->items_num = 0u;
}


// Destructor deleting all list items from memory.
template <typename T>
LinkedList<T>::~LinkedList()
{
    LinkedListItem<T> *p_current;   // Pointer used for traversing the list.
    LinkedListItem<T> *p_next;      // Temporary pointer to the next item.


    p_current = this->p_head;

    // Loop through the list until the end of the list is found, i.e. null pointer.
    while(p_current != nullptr)
    {
        // Save next item to temporary pointer.
        p_next = p_current->p_next;
        // Delete the current item. It must be ensured (in the list item destructor)
        // that the next pointer of the deleted item is set to null, otherwise this
        // while loop cannot end if the list is looped.
        // Note:
        //    Beware that all the items cut-out after the list loop (if there is one)
        //    will remain undeleted in memory, because no mechanism is implemented to
        //    find them!
        delete p_current;
        // Move to the next item.
        p_current = p_next;
    }
}


template <typename T>
// Adds item with specified data at the end of the list.
void LinkedList<T>::AddItemAtEnd(const T data)
{
    // Allocate memory for new item.
    LinkedListItem<T> *p_new = new LinkedListItem<T>(data);

    // Check if list is empty, i.e. head pointer is pointing to null.
    if(this->p_head == nullptr)
    {
        // Add first item of the list.
        this->p_head = p_new;
        this->p_tail = p_new;
        this->p_listing = this->p_head;
    }
    else
    {
        // Add new item to the end of the list.
        // Tail pointer is used to avoid traversing the whole list to
        // find the tail every time the new item is added at the end.
        this->p_tail->p_next = p_new;
        this->p_tail = p_new;
    }

    // Increment number of items in list.
    // Beware of possible overflow if there are more than 2^32 - 1 items. Loop detection counting
    // the number of transitions would stop working.
    this->items_num++;
}


// Deletes first instance of an item having the specified data value.
// The next point of the item before the deleted item is pointed to the item right after the
// deleted item to keep the list continuos.
template <typename T>
void LinkedList<T>::DeleteItem(const T data_to_delete)
{
    LinkedListItem<T> *p_current = nullptr;
    LinkedListItem<T> *p_previous = nullptr;

    // Get pointer to the current and previous item for the specified data value.
    this->GetCurrentPreviousItemPointer(data_to_delete, &p_current, &p_previous);

    // If current item having specified data value was found.
    if(p_current != nullptr)
    {
        // If previous item before the one with specified data was found.
        if(p_previous != nullptr)
        {
            // Connect previous item to the item after current item, so the
            // current item can be deleted.
            p_previous->p_next = p_current->p_next;

            // If current item being removed is the tail of the list.
            if(p_current == this->p_tail)
            {
                // Move the tail to the previous item.
                this->p_tail = p_previous;
            }
        }
        else
        {
            // There is no previous item, which means the first list item is being
            // deleted, so connect head item to the item right after the current item.
            this->p_head = p_current->p_next;
        }

        // Delete the item with specified data value.
        delete p_current;

        // Decrement number of items in the list while avoiding overflow.
        if(this->items_num > 0u)
        {
            this->items_num--;
        }
    } // end if(p_current != nullptr)
}


// Get data of the next item through the reference argument. Starts at the head of the list and
// moves to the next item after each call. Automatically keeps returning the last item data after
// the end of the list is reached until the reading is reset to the beginning by the ResetReading method.
// Return value indicates if there are further items to be read, i.e. if the list still continues.
template <typename T>
bool LinkedList<T>::GetNextItem(T &data)
{
    bool list_continues = false;    // Flag indicating whether there are further items to be read.

    // this->p_listing pointer is used for iterating over the list items.
    // The listing pointer jumps to the next item during each call of this method and stops at the end item.
    if(this->p_listing != nullptr)
    {
        // Set data value pointed to by the listing pointer.
        data = this->p_listing->data;
        // Move the listing pointer to the next item unless it's already at the end.
        if(this->p_listing->p_next != nullptr)
        {
            this->p_listing = this->p_listing->p_next;
            list_continues = true;
        }
    }

    return list_continues;
}


// Moves reading index used by GetNextItem method back to the list head item.
template <typename T>
void LinkedList<T>::ResetReading()
{
    // Set listing pointer used by the GetNextItem method to the head item of the list.
    this->p_listing = this->p_head;
}


// Indicates whether item with specified data is present.
template <typename T>
bool LinkedList<T>::IsItemPresent(const T data)
{
    LinkedListItem<T> *p_current = nullptr;
    bool item_present = false;

    // Get pointer to the item with specified data value.
    this->GetCurrentPreviousItemPointer(data, &p_current);

    if(p_current != nullptr)
    {
        item_present = true;
    }

    return item_present;
}


// Returns number of items in the linked list.
template <typename T>
unsigned long LinkedList<T>::GetItemsNum()
{
    return this->items_num;
}


// Forcing a link from one item to another. To be used only for intentional creation of looped list.
// Next pointer of the item with specified "data" value will be set to point to the item with specified
// "next_data" value.
template <typename T>
void LinkedList<T>::SetNextReference(const T data, const T next_data)
{
    LinkedListItem<T> *p_current = nullptr;
    LinkedListItem<T> *p_next = nullptr;

    // Get pointers to the items with specified data values.
    this->GetCurrentPreviousItemPointer(data, &p_current);
    this->GetCurrentPreviousItemPointer(next_data, &p_next);

    // If pointers to both items were found.
    if( (p_current != nullptr) && (p_next != nullptr) )
    {
        // Link the two items.
        p_current->p_next = p_next;
    }
}


// Indicates if list is looping using open hashing algorithm. Open hashing solves key conflicts
// by having a linked list for each key and if key already has a value assigned, then it simply
// stores the value into the next linked list item for the corresponding key. Following example
// illustrates open hashing array table with size 3 and filled input values 1, 2, 4, 3, 6:
//
// key (list array index) = input_value % list_size
// calculated keys (array indexes): 1 % 3 = 1; 2 % 3 = 2; 4 % 3 = 1; 3 % 3 = 0; 6 % 3 = 0
// 
// linked list array[0-2]           linked list items
// linked_list[0]            =>     3 -> 6 -> null
// linked_list[1]            =>     1 -> 4 -> null
// linked_list[2]            =>     2 -> null
//
// Then if value 4 is searched a key 4 % 3 = 1 is calculated for it and then the linked list
// with index 1 (linked_list[1]) is searched for value 4. 
//
// See https://www.log2base2.com/algorithms/searching/open-hashing.html for more information.
// The hash table is created using new LinkedList object. This greatly simplifies the hashing
// algorithm implementation. Hash table array size can be specified through the input argument.
// The hash table then further grows with the amount of traversed list items.
//
// Note: 
//    Simpler solution that was considered, but rejected in favor of trying something more
//    sophisticated :) :
//    No hashing. Traversing through the list and saving visited addresses into
//    a simple array. Array would be searched before storing new address to see whether the
//    address is not already there. If so, then the loop is detected.
template <typename T>
bool LinkedList<T>::IsLooping_OpenHash(unsigned int hash_table_size)
{
    bool loop_found = false;
    LinkedListItem<T> *p_current = nullptr;         // Pointer used for traversing the list.
    LinkedList<uintptr_t>* pa_hash_table = nullptr; // Hash array - pointer to array of linked lists.
    uintptr_t item_address;


    if(hash_table_size < 1)
    {
        // Set default hash table size if zero size is specified for some reason (probably misunderstanding or mistake).
        hash_table_size = DEFAULT_HASH_TABLE_SIZE;
    }

    // If there is at least one item pointing to some other item in the list (otherwise it doesn't make sense to detect loops).
    if( (this->p_head != nullptr) && (this->p_head->p_next != nullptr) )
    {
        // Allocate memory for the hash table.
        pa_hash_table = new LinkedList<uintptr_t>[hash_table_size];

        // Set current pointer to the item pointed to by head's next pointer.
        p_current = this->p_head->p_next;
        // Cast address of the current item pointer to the numeric value.
        item_address = reinterpret_cast<uintptr_t>(p_current);
        // Loop through list items while the end of the list is not found (null pointer) or the visited address is not already
        // present in the hash table, i.e. it was already visited before, meaning there is a loop.
        while( (p_current != nullptr) && (pa_hash_table[item_address % hash_table_size].IsItemPresent(item_address) == false) )
        {
            // Add current pointer address to the hash table.
            pa_hash_table[item_address % hash_table_size].AddItemAtEnd(item_address);
            // Move to the next item.
            p_current = p_current->p_next;
            // Cast address of the current item pointer to the numeric value.
            item_address = reinterpret_cast<uintptr_t>(p_current);
        }

        // If the while loop above finished with non-null current item pointer, then it means that the end of the
        // list was not found and it therefore loops.
        if(p_current != nullptr)
        {
            loop_found = true;
        }

        delete[] pa_hash_table;
    } // end if( (this->p_head != nullptr) && (this->p_head->p_next != nullptr) )

    return loop_found;
}


// Indicates if list is looping detection by counting the number of transitions made when traversing the list.
// This is a bonus looping detection algorithm, that works only if the list items are addded or removing by the
// assigned methods (AddItemAtEnd, DeleteItem). Despite limitations, its advantage is simplicity and very small
// memory requirements not rising with the list size.
template <typename T>
bool LinkedList<T>::IsLooping_TransitionCount()
{
    bool loop_found = false;
    unsigned long transitions_num = 1u;     // Counter of transitions between visited list items.
    LinkedListItem<T> *p_current = nullptr; // Pointer used for traversing the list.


    // If there is at least one item pointing to some other item in the list (otherwise it doesn't make sense to detect loops).
    if( (this->p_head != nullptr) && (this->p_head->p_next != nullptr) )
    {
        // Set current pointer to the item pointed to by head's next pointer.
        p_current = this->p_head->p_next;
        // Loop through list items while the end of the list is not found (null pointer) or the 
        // number of transitions between visited list items is above the total number of all list items.
        while( (p_current != nullptr) && (transitions_num <= this->items_num) )
        {
            p_current = p_current->p_next;
            transitions_num++;
        }

        // If the while loop above finished with non-null current item pointer, then it means that the end of the
        // list was not found and it therefore loops.
        if(p_current != nullptr)
        {
            loop_found = true;
        }
    }

    return loop_found;
}


// Indicates if list is looping using Floyd's cycle detection algorithm. There are two pointers traversing the
// list together. One pointer is "slow" and moves normally from one item to the next item. The second pointer is
// "fast" and it always jumps over the one item, i.e. it makes two transitions at once. If there is a loop in the
// list, then the fast pointer eventually catches up the slow pointer and they will have the same address. If
// there is no loop, then the fast pointer simply reaches the end of the loop (null pointer) first and the algorithm
// can stop.
// See https://www.codingninjas.com/blog/2020/09/09/floyds-cycle-detection-algorithm/ for more information.
template <typename T>
bool LinkedList<T>::IsLooping_FloydsCycleFind()
{
    bool loop_found = false;
    LinkedListItem<T> *p_slow = this->p_head;   // Slow pointer moving in list from one item to another.
    LinkedListItem<T> *p_fast = this->p_head;   // Fast pointer moving by two items at once.


    // Loop while the loop is not found (slow and fast pointer different) or the end of the list is not found.
    while( (loop_found == false) && (p_slow != nullptr) && (p_fast != nullptr) && (p_fast->p_next != nullptr) )
    {
        // Move slow pointer by one item and fast pointer by two items.
        p_slow = p_slow->p_next;
        p_fast = p_fast->p_next->p_next;
        // If fast pointer catches slow pointer, then loop must exist.
        if(p_slow == p_fast)
        {
            loop_found = true;
        }
    }

    return loop_found;
}


// Utility function getting pointer to the current item with the specified data and also pointer to the previous item.
// If there is no pointer argument specified for the previous item, then only the pointer to the current item is searched.
// The pointers are returned through pointer to pointer arguments, because the function is returning pointers through pointers
// as opposed to returning values through pointers.
//
// Note:
//    The method works by directly comparing the input data value to the data value of each list item. It is therefore necessary
//    to use only the data types that can be directly compared, otherwise the program won't even compile.
//    A more complex solution supporting any kind of custom defined data type (e.g. class) would require also implementation of a 
//    customizable comparison function provided to the linked list class.
template <typename T>
void LinkedList<T>::GetCurrentPreviousItemPointer(const T data, LinkedListItem<T> **pp_current_item, LinkedListItem<T> **pp_previous_item)
{
    unsigned long transitions_num = 1u; // Counter for number of transitions in the list.


    // If the return pointer for the current item is provided.
    if(pp_current_item != nullptr)
    {
        // If the return pointer for the previous item is provided.
        if(pp_previous_item != nullptr)
        {
            // Initialize previous item pointer to null.
            *pp_previous_item = nullptr;
        }

        // Initialize current item pointer to the head pointer.
        *pp_current_item = this->p_head;
        
        // Loop through the list items until the end of the list is detected (null pointer) or the specified data is found or the loop is detected.
        // Loop detection is done in a very simple way by comparing whether the number of transitions between items is smaller than the number of items.
        // This is done to avoid endless while loop when the list is looped and doesn't contain the specified data value.
        while( (*pp_current_item != nullptr) && ((*pp_current_item)->data != data) && (transitions_num <= this->items_num) )
        {
            // If the return pointer for the previous item is provided.
            if(pp_previous_item != nullptr)
            {
                // Set previous pointer to the current pointer.
                *pp_previous_item = *pp_current_item;
            }

            // Move current pointer to the next item.
            *pp_current_item = (*pp_current_item)->p_next;
            // Increment transitions already done.
            transitions_num++;
        }

        // if number of transitions performed is over the number of list items, then the list is probably looped and
        // doesn't contain the specified data value, so set current and previous pointer to null.
        if(transitions_num > this->items_num)
        {
            *pp_current_item = nullptr;
            if(pp_previous_item != nullptr)
            {
                *pp_previous_item = nullptr;
            }
        }
    } // end if(pp_current_item != nullptr)
}


#endif // #ifndef LINKED_LIST_H