#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <vector>

using namespace std;

#include "tube.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}

bool get_symbol_position(char** map,int height, int width, char target,int& row, int& column){
  for (int r = 0; r < height; r++){
    for (int c = 0; c < width; c++){
      if (map[r][c] == target){
        row = r;
        column = c;
        return true;
      }
    }
  }
  row = -1;
  column = -1;
  return false;
}

char get_symbol_for_station_or_line(const char* name){
  char symbol = ' ';
  if (!valid_station_or_line("stations.txt",name,symbol)){
    valid_station_or_line("lines.txt",name,symbol);
  }
  return symbol;
}

/* error handling */
int validate_route(char** map, int height, int width, const char* start, const char* route, char* destination){
  //check start station
  char symbol = ' ';
  if (!valid_station_or_line("stations.txt",start,symbol)){
    return ERROR_START_STATION_INVALID;
  }

  //check route
  char direction[3];
  Direction d;
  int index = 0;
  vector <int> instruction;
  for (int i = 0; i < strlen(route); i++){
    if (isalpha(route[i])){
      direction[index] = route[i];
      index++;
    }else {
      direction[index] = '\0';
      index = 0;
      d = string_to_direction(direction);
      if (d == INVALID_DIRECTION){ return ERROR_INVALID_DIRECTION; }
      instruction.push_back(d);
    }
  }
  direction[index] = '\0';
  d = string_to_direction(direction);
  if (d == INVALID_DIRECTION){ return ERROR_INVALID_DIRECTION; }
  instruction.push_back(d);

  //get coordinates of start station
  int r, c;
  get_symbol_position(map, height, width, symbol, r, c);
  //loop through the checked and saved route vector
  // out of bounds or off track or error line hoping
  for (int i = 0; i < instruction.size(); i++){
    if (instruction[i] == N){
      if (r == 0) { return ERROR_OUT_OF_BOUNDS;}
      if (map[r-1][c] == ' '){ return ERROR_OFF_TRACK; }
      if (map[r-1][c] == ' ')
      r--;
    }
    if (instruction[i] == S){
      if (r+1 >= height){ return ERROR_OUT_OF_BOUNDS; }
      if (map[r+1][c] == ' '){ return ERROR_OFF_TRACK; }
      r++;
    }
    if (instruction[i] == W){
      if (c==0){ return ERROR_OUT_OF_BOUNDS; }
      if (map[r][c-1] == ' '){ return ERROR_OFF_TRACK; }
      c--;
    }
    if (instruction[i] == E){

    }
    if (instruction[i] == NE){

    }
    if (instruction[i] == NW){

    }
    if (instruction[i] == SE){

    }
    if (instruction[i] == SW){

    }

  }





  return ERROR_BACKTRACKING_BETWEEN_STATIONS;
}

bool compare(const char* str1, const char* str2){
  if (*str1 == '\0' && *str2 == '\0'){return true;}
  if (*str1 != *str2){ return false; }
  if (compare(str1+1,str2+1)){ return true; }
  return false;
}

void get_station_tube(const char* line, char* station_tube){
  int length = strlen(line);
  for (int i = 2; i < length; i++){
    *station_tube = line[i];
    station_tube++;
  }

  *station_tube = '\0';
}

bool valid_station_or_line(const char* filename, const char* name, char& symbol){
  ifstream in(filename);
  if(!in){ cout << "Failed opening stations.txt!" << endl; return false; }

  char line[512];
  in.getline(line,512); 
  char stationline[512];
  get_station_tube(line,stationline);
  while (in) {
    if (compare(name,stationline)){
      symbol = line[0];
      return true;
    }
    in.getline(line,512);
    get_station_tube(line,stationline);
  }
  in.close();
  return false;
}

int move_valid(char** map, int height, int width, Direction direction, int& r, int& c, char line, Direction last_direction){
  if (direction == N){
      if (r == 0) { return ERROR_OUT_OF_BOUNDS;}
      if (map[r-1][c] == ' '){ return ERROR_OFF_TRACK; }
      //if changed line
      if (isalnum(map[r][c]) == 0){
        if (map[r-1][c] != line){ return ERROR_LINE_HOPPING_BETWEEN_STATIONS; }
      }
      if (map[r-1][c] != line) 
      if (reverse(direction,last_direction)){ return ERROR_BACKTRACKING_BETWEEN_STATIONS; }
      r--;
    }
    if (direction == S){
      if (r+1 >= height){ return ERROR_OUT_OF_BOUNDS; }
      if (map[r+1][c] == ' '){ return ERROR_OFF_TRACK; }
      r++;
    }
    if (direction == W){
      if (c==0){ return ERROR_OUT_OF_BOUNDS; }
      if (map[r][c-1] == ' '){ return ERROR_OFF_TRACK; }
      c--;
    }
    if (direction == E){

    }
    if (direction== NE){

    }
    if (direction == NW){

    }
    if (direction == SE){

    }
    if (direction == SW){}

}

bool reverse(Direction direction,Direction last_direction){
  if ((last_direction == S && direction == N)|| (last_direction == N && direction == S)){
    return true;
  }
  if ((last_direction == W && direction == E)|| (last_direction == E && direction == W)){
    return true;
  }
  if ((last_direction == NE && direction == SW)|| (last_direction == SW && direction == NE)){
    return true;
  }
  if ((last_direction == NW && direction == SE)|| (last_direction == SE && direction == NW)){
    return true;
  }
  return false;
}