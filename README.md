# C++ linked list class template with closed loop detection
A simple implementation of linked list in C++ class template in **linked_list.h** file with methods allowing following basic functionalities:
1. Adding item at the end of the list.
2. Deleting item with the specified value.
3. Getting next item value starting from list head.
4. Forcing a link between existing items with the intention of creating a loop in the list.

Additionaly the class provides three methods to detect closed loop:
1. Open hashing algorithm for visited item addresses.
2. Counting transitions during list traversal and comparing them to the expected number of items in the list.
3. Floyd's cycle detection algorithm.

The **linked_list_demo.cpp** file contains a demonstration of the linked list class usage.
