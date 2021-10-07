// File:        gridlist.cpp
// Date:        2021-05-20 02:56
// Description: Contains partial implementation of GridList class
//              CPSC 221 2021S PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "gridlist.h"

// Creates a PNG of appropriate pixel dimensions according to the GridList's structure
//   and colours each pixel according the each GridNode's Block data.
// The fully coloured PNG is returned.
PNG GridList::Render() const
{
  int blockDim = northwest->data.Dimension();
  int width = (blockDim * dimx);
  int height = (blockDim * dimy);
   
  PNG image; 
  image.resize(width, height); // changes PNG dimensions

  GridNode* curr = northwest;
  int currX = 0;
  int currY = 0;

  // traverses through GridList and renders each block
  while(curr != NULL) {  
    curr->data.Render(image, currY, currX);
    curr = curr->next;

    if ((currX + blockDim) < width) {    // keeps track of curr x-coord of the PNG
      currX += blockDim;  
    } else {
      if ((currY + blockDim) < height) {   
        currX = 0;
        currY += blockDim;
      } else {
        break;
      }
    }
  }
  return image; 
}

// Allocates a new GridNode containing bdata and
//   attaches it to end of this list.
// Be careful of the special case of inserting into an empty list.
void GridList::InsertBack(const Block& bdata)
{
  if (IsEmpty()) {      // case 1: northwest and southeast point to only grid node in the list
    northwest = new GridNode(bdata);;
    southeast = northwest;
  } else {   // case 2: add gridnode to end of gridlist and increment southeast
    GridNode* curr = new GridNode(bdata);
    southeast->next = curr;
    southeast = southeast->next;
  } 
}

// Rearranges the GridNodes in the list (and the pixel data in each GridNode) so that
//   the complete image data has been flipped/mirrored horizontally (across a vertical axis)
// e.g. for the following list:
// NW -> 1 -> 2 -> 3 -> 4
//    -> 5 -> 6 -> 7 -> 8
//    -> 9 ->10 ->11 ->12 <- SE
// The list after flipping horizontally will be:
// NW -> 4 -> 3 -> 2 -> 1
//    -> 8 -> 7 -> 6 -> 5
//    ->12 ->11 ->10 -> 9 <- SE,
//   where the pixel data in each block has also been flipped horizontally
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::FlipHorizontal()
{
  GridNode *curr = northwest;    
  while (curr != NULL) {
    curr->data.FlipHorizontal();  // H flipping every block's data in the list
    curr = curr->next;
  }

  GridList::FlipHPointers();   // calling helper function to reverse the nodes along the rows
}

// Rearranges the GridNodes in the list (and the pixel data in each GridNode) so that
//   the complete image data has been flipped/mirrored vertically (across a horizontal axis)
// e.g. for the following list:
// NW -> 1 -> 2 -> 3 -> 4
//    -> 5 -> 6 -> 7 -> 8
//    -> 9 ->10 ->11 ->12 <- SE
// The list after flipping horizontally will be:
// NW -> 9 ->10 ->11 ->12
//    -> 5 -> 6 -> 7 -> 8
//    -> 1 -> 2 -> 3 -> 4 <- SE,
//   where the pixel data in each block has also been flipped vertically
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::FlipVertical()
{
  if (northwest == NULL || northwest->next == NULL || dimx == 1) return;

  GridNode* curr = northwest;
  GridNode* prev = NULL;
  GridNode* next;

  while(curr != NULL) {  // reversing the entire gridlist 
    next = curr->next; 
    curr->next = prev; 
    curr->data.FlipVertical();   
    prev = curr; 
    curr = next;
  }

  northwest = prev;

  FlipHPointers();   // calling helper function to reverse the nodes along the rows

  southeast = northwest; 

  while (southeast->next != NULL) {   // traversing through the list to set southeast to the last node
    southeast = southeast->next;
  }
}

// Uses the average luminance of each GridNode's block, and sends an
//   ASCII character to a return string.
// The characters in the string, if printed, must match the structural
//   dimensions of the GridList (i.e. insert line breaks at appropriate intervals).
// Use the Lum2ASCII private function for mapping luminance levels to
//   ASCII characters, and the += operator to append a character to
//   an existing string.
string GridList::PrintASCIIfy() const
{
  GridNode* curr = northwest;
  int count = 0;
  double lum;
  std::string out = "";

  while (curr->next != NULL && curr != NULL) {  // traversing through list to convert each block's data to a string
    if (count == dimx - 1) {  // edge case: need to have a new line to form a new row
      lum = curr->data.AvgLuminance();
      out.push_back(Lum2ASCII(lum));
      out.append("\n");
      curr = curr->next;
      count = 0; 
    } else { // traversing through the row as normal
      lum = curr->data.AvgLuminance();  
      out.push_back(Lum2ASCII(lum));
      curr = curr->next;
      count++;
    }
    if (curr == NULL) break;
  }
  return out;
}

// Deallocates any dynamic memory associated with this list
//   and re-initializes this list to an empty state
void GridList::Clear()
{
  if (northwest == NULL) {  // case 1: empty gridlist, nothing to do
    return;
  } else { // case 2: gridlist is not empty
    GridNode* curr = northwest;
    GridNode* next = NULL;

    while (curr != NULL) {  // traversing through list to clear each node
      next = curr->next;
      delete curr;
      curr = NULL;
      curr = next;
    }
    northwest = NULL;  // restoring list to empty state
    southeast = NULL;
    dimx = 0;
    dimy = 0;
  }
}

// Allocates new nodes into this list as copies of all nodes from otherlist
void GridList::Copy(const GridList& otherlist)
{
  if (otherlist.northwest == NULL) {  // case 1: empty list, nothing to do
    return;
  }

  // case 2: list is not empty
  // setting dimx and dimy based on otherlist's dimensions
  dimx = otherlist.dimx;  
  dimy = otherlist.dimy;

  GridNode* temp = otherlist.northwest;
  northwest = NULL; // making sure northwest and southeast don't already point to something
  southeast = NULL;
  
  while (temp != NULL) {  // inserting each item in temp at the back of this gridlist
    InsertBack(temp->data);
    temp = temp->next;
  } 
}

// IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE ADDED BELOW
//

// Private helper function that reverses the nodes along rows of dimx size
void GridList::FlipHPointers() 
{
  if (northwest == NULL || northwest->next == NULL || dimx == 1) return;

  // setting pointers
  GridNode* curr = northwest; // current node in list
  GridNode* prev = NULL; // previous node in list
  GridNode* next; // next node in list
  GridNode* headPtr = NULL; // points to the new head of the reversed list
  GridNode* tailPtr1 = NULL; // points to the tails of the reversed rows
  GridNode *tailPtr2 = northwest; // points to the tails of the reversed rows
  int count = 1; // to determine where in the row we are

  while (curr != NULL && curr->next != NULL) {
    while (count <=  dimx && curr != NULL) { // flips nodes in the row 
      next = curr->next; 
      curr->next = prev; 
      prev = curr; 
      curr = next;
      count++;
    }

    // edge case
    if (headPtr == NULL) { // sets head pointer to the new head of the gridlist in very first row
      headPtr = prev;
    }
    if (tailPtr1 != NULL) { // links tail pointer 1 to the end of the previous row
      tailPtr1->next = prev;
    }
    tailPtr2->next = curr; // links tail pointer 2 to the current edge node
    tailPtr1 = tailPtr2; // links tail pointer 1 to the last edge node
    tailPtr2 = curr;  // sets tail pointer 2 to the current edge node
    prev = NULL;
    count = 1; // resets count to 1 so function can traverse through a full row again
  }
  northwest = headPtr;
  southeast = northwest;
  while (southeast->next != NULL) { // traverses through the finished list to find southeast
    southeast = southeast->next;
  }
}