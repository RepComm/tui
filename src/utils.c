
#ifndef UTILS_C
#define UTILS_C

#include <string.h>
#include <stdbool.h>

/**Concat a bunch of strings together
 * 
 * count - number of strings in strs
 * strs - an array of strings
 * joiner - a string to insert in between each string in strs
 */
char * string_concat (char** strs, int start, int count, char* joiner) {
  //total length of chars in the result (not including null terminator)
  int totalLength = 0;

  //joiner info
  bool useJoiner = joiner != 0;
  int joinerLength;
  if (useJoiner) joinerLength = strlen(joiner);

  //reusable loop string
  char * current;
  //reusable loop string's length
  int currentLength = 0;
  //lengths for each string in strs
  int strslengths[count];

  //loop through strs
  for (int i=start; i<count; i++) {
    current = strs[i];
    //calculate the length
    currentLength = strlen(current);

    //copy into list of lengths
    strslengths[i] = currentLength;
    //add to total
    totalLength += currentLength;

    //if a joiner is specified, add it for each item except the last one
    if (useJoiner && i != count-1) totalLength += joinerLength;
  }

  //the resulting string
  //make sure to add one for the null terminator
  char * result = malloc(totalLength + 1);
  //marker for the loop when copying chars
  int offset = 0;

  //loop through strs again
  for (int i=start; i<count; i++) {
    current = strs[i]; //current string
    currentLength = strslengths[i]; //current length

    //loop through the current string
    for (int j=0; j<currentLength; j++) {
      //copy the contents into result in the right spot
      //offset marks the result starting offset where current will be copied to
      //j marks the individual character of the current string
      result[offset + j] = current[j];
    }
    offset += currentLength;
    
    //copy in joiner data as well (except for after last string)
    if (useJoiner && i != count-1) {
      for (int j=0; j<joinerLength; j++) {
        result[offset + j] = joiner[j];
      }
      offset += joinerLength;
    }
  }
  //arrays start at 0, so using totalLength as the index here gives the last index
  //which we reserved for a null terminator
  //lets explicitly set it to null ffs
  result[totalLength] = 0;

  //we're done
  return result;
}

char * string_copy (char * src, int start, int width) {
  char * result = malloc(width + 1); //remember null terminator
  strncpy(result, src+start, width);
  result[width] = 0;
  return result;
}

char * string_copy_const (const char * src, int start, int width) {
  char * result = malloc(width + 1); //remember null terminator
  strncpy(result, src+start, width);
  result[width] = 0;
  return result;
}

bool char_in_string (char* haystack, char needle) {
  int ind = 0;
  char current = haystack[ind];
  while (current != 0) {
    if (current == needle) return true;
    ind ++;
    current = haystack[ind];
  }
  return false;
}

void nibble_to_hex (char nibble, char * out, int offset) {
  if (nibble == 0) out[offset] = '0';
  switch (nibble) {
    case 0: out[offset] = '0'; break;
    case 1: out[offset] = '1'; break;
    case 2: out[offset] = '2'; break;
    case 3: out[offset] = '3'; break;
    case 4: out[offset] = '4'; break;
    case 5: out[offset] = '5'; break;
    case 6: out[offset] = '6'; break;
    case 7: out[offset] = '7'; break;
    case 8: out[offset] = '8'; break;
    case 9: out[offset] = '9'; break;
    case 10: out[offset] = 'a'; break;
    case 11: out[offset] = 'b'; break;
    case 12: out[offset] = 'c'; break;
    case 13: out[offset] = 'd'; break;
    case 14: out[offset] = 'e'; break;
    case 15: out[offset] = 'f'; break;
  }
  return;
}

/**Write a byte into hex format (2 bytes) into char[offset] out
 * 
 */
void byte_to_hex (char byte, char * out, int offset) {
  nibble_to_hex(byte & 0b11110000, out, offset);
  nibble_to_hex(byte & 0b00001111, out, offset+1);
}

/**Convert a float into its hex format
 */
void _32bit_to_hex (char * value, char * out, int offset) {
  byte_to_hex(value[0], out, 0); //0, 1
  byte_to_hex(value[1], out, 2); //2, 3
  byte_to_hex(value[2], out, 4); //4, 5
  byte_to_hex(value[3], out, 6); //6, 7
}

#endif
