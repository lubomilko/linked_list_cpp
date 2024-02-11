/*
Dynamic linked list C++ class template demo application.

Copyright (C) 2024 Lubomir Milko
This file is part of C++ linked list implementation
<https://github.com/lubomilko/linked_list_cpp>.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <string>
#include "linked_list.h"


using namespace std;

// *************************************************************************************************
// FUNCTION DECLARATIONS
// *************************************************************************************************
// Prints data from all linked list items.
template <typename T>
void PrintLinkedListData(LinkedList<T> &, unsigned long=100u);
// Performs all available loop detections and prints results.
template <typename T>
void PerformLoopDetections(LinkedList<T> &);
// Linked list usage demonstration with integer data type.
void DemonstrateLinkedList_int();
// Linked list usage demonstration with string data type.
void DemonstrateLinkedList_str();

// *************************************************************************************************
// FUNCTION DEFINITIONS
// *************************************************************************************************
// Prints data from all items of the specified linked list to the console.
// Maximum amount of printed data values can be limited by the max_printed_data_num argument to
// avoid endless printing of a looped list.
template <typename T>
void PrintLinkedListData(LinkedList<T> &linked_list, unsigned long max_printed_data_num)
{
   T data;
   bool list_continues = true;
   unsigned long printed_data_counter = 0u;


   cout << "List data: ";
   // Reset reading of the list to the head item.
   linked_list.ResetReading();
   // If the list is not empty.
   if(linked_list.GetItemsNum() > 0)
   {
      // Loop until the en of the list is detected or until the maximum amount of printed data values is reached.
      while( (list_continues) && (printed_data_counter < max_printed_data_num) )
      {
         // Get list item data value.
         list_continues = linked_list.GetNextItem(data);
         // Print the data value.
         cout << data;

         // If there are still unprinted data values, then add separator in the print-out.
         if(list_continues)
         {
            cout << "; ";
         }

         printed_data_counter++;
      }
      cout << endl;
   }
   else
   {
      cout << "List is empty.";
   }   

   if(printed_data_counter >= max_printed_data_num)
   {
      cout << "WARNING! Maximum amount of printed data reached. Either the list is longer or it is looping." << endl;
   }
}


// Performs all available loop detections and prints results to the console.
template <typename T>
void PerformLoopDetections(LinkedList<T> &linked_list)
{
   bool a_results[3];   // Array for loop detection algorithms results.
   string a_loop_detect_algo_name[3] =
   {
      "transitions counting ",
      "Floyd's cycle finding",
      "open hash table      "
   };

   // Perform all available loop detections.
   a_results[0] = linked_list.IsLooping_TransitionCount();
   a_results[1] = linked_list.IsLooping_FloydsCycleFind();
   a_results[2] = linked_list.IsLooping_OpenHash();

   // Print all results to the console.
   for(int i = 0; i < 3; i++)
   {
      cout << "Loop detection using " << a_loop_detect_algo_name[i] << ": ";
      if(a_results[i])
      {
         cout << "WARNING! Looping detected!" << endl;
      }
      else
      {
         cout << "OK. No looping." << endl;
      }
   }
}


// Linked list usage demonstration with integer data type.
void DemonstrateLinkedList_int()
{
   // Create linked list with integer data type values.
   LinkedList<int> int_linked_list;

   //--- Add some items to the list and print it to the console together with the loop detection results.
   cout << endl << "Generating simple linked list holding integer values 0-9." << endl;
   for(int i = 0; i < 10; i++)
   {
      int_linked_list.AddItemAtEnd(i);
   }
   PrintLinkedListData<int>(int_linked_list);
   PerformLoopDetections<int>(int_linked_list);
   //---

   //--- Remove some items from the the list and print everything.
   cout << endl << "Removing values 0, 3, 7, 9." << endl;
   int_linked_list.DeleteItem(0);
   int_linked_list.DeleteItem(3);
   int_linked_list.DeleteItem(7);
   int_linked_list.DeleteItem(9);
   PrintLinkedListData<int>(int_linked_list);
   PerformLoopDetections<int>(int_linked_list);
   //---

   //--- Add more items to the the list and print everything.
   cout << endl << "Adding values 11, 12, 13." << endl;
   int_linked_list.AddItemAtEnd(11);
   int_linked_list.AddItemAtEnd(12);
   int_linked_list.AddItemAtEnd(13);
   PrintLinkedListData<int>(int_linked_list);
   PerformLoopDetections<int>(int_linked_list);
   //---

   //--- Create a loop in the linked list by forcefully setting the next pointer of
   // one existing item to another existing item. Print the list and loop detection results.
   cout << endl << "Forcing link from value 12 to value 4." << endl;
   int_linked_list.SetNextReference(12, 4);
   PrintLinkedListData<int>(int_linked_list, 20);
   PerformLoopDetections<int>(int_linked_list);
   //---
}


// Linked list usage demonstration with string data type.
void DemonstrateLinkedList_str()
{
   // Create linked list with string data type values.
   LinkedList<string> str_linked_list;
   string a_colors[5] = {"red", "green", "blue", "white", "black"};

   //--- Add some items to the list and print it to the console together with the loop detection results.
   cout << endl << "Generating linked list with 5 string color values." << endl;
   for(int i = 0; i < 5; i++)
   {
      str_linked_list.AddItemAtEnd(a_colors[i]);
   }
   PrintLinkedListData<string>(str_linked_list);
   PerformLoopDetections<string>(str_linked_list);
   //---

   //--- Remove some items from the the list and print everything.
   cout << endl << "Removing green." << endl;
   str_linked_list.DeleteItem("green");
   PrintLinkedListData<string>(str_linked_list);
   PerformLoopDetections<string>(str_linked_list);
   //---

   //--- Create a loop in the linked list by forcefully setting the next pointer of
   // one existing item to another existing item. Print the list and loop detection results.
   cout << endl << "Forcing link from white to red." << endl;
   str_linked_list.SetNextReference("white", "red");
   PrintLinkedListData<string>(str_linked_list, 10);
   PerformLoopDetections<string>(str_linked_list);
   //---
}


int main()
{
   //--- Perform functions demonstrating linked list usage.
   DemonstrateLinkedList_int();
   DemonstrateLinkedList_str();
   //---

   // Wait for any keyboard input to prevent automatic closing of the Windows console print-out.
   getchar();
}