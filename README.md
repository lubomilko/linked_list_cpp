# C++ linked list class template with closed loop detection
A simple C++ class template implementation of a linked list in the **linked_list.h** file with methods providing the following functionalities:
1. Adding an item at the end of the list.
2. Deleting an item with the specified value.
3. Getting the next item value starting from the list head.
4. Forcing a link between the existing items with the intention of creating a loop in the list.

Additionaly, the class provides three methods to detect a closed loop:
1. Hashing algorithm generating addresses of visited items.
2. Counting transitions during list traversal and comparing them to the expected number of items in the list.
3. Floyd's cycle detection algorithm.

The **linked_list_demo.cpp** file contains a demonstration of the linked list class usage.
