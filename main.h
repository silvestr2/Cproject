#ifndef _MAIN_H_
#define _MAIN_H_
#include "navigation.h"
#include <time.h>

/*
 * This is the structure for time that is
 * a part of <time.h> header file.
 */
typedef struct tm shiptime;

/*
 * This is the structure for a ship that contains of
 * six elements: char representing AIS_ID of a ship,
 * reference to the struct location from the "navigation.h"
 * header, two doubles representing the direction of travel
 * and the ship's speed, integer boolean value that indicates
 * if the ship has sunk or not and also a pointer to the next element
 * in the linked list.
 */
typedef struct ships{
		char ais_id[21];
		location shiplocation;
		double direction;
		double speed;
                int isdead;
		struct ships *next;
	}ships;
        
/*
 * This function takes four different parameters and returns a pointer 
 * to the struct ship. The first parameter is a char representing AIS_ID
 * of a ship, the second one represents its location, the third one represents
 * the direction of travel, and the last one represents its speed.
 */		
ships *create_ship(char ais_id[], location shiplocation ,double direction,double speed);
/*
 * This function takes one parameter and returns nothing. 
 * The parameter represents a pointer to struct ships. 
 * The function is responsible for adding elements to the 
 * linked list at the front of it.
 */
void insert_ship(ships *temporary_ship);
/*
 * This function takes two parameters and returns nothing. 
 * The first parameter represents the time of the simulation, 
 * and the other represents its time step. The function is responsible 
 * for updating positions of the ships, and also contains code for 
 * updating the time.
 */
void update_all(double time,double timestep);
/*
 * This function takes no parameters and returns nothing.
 * It is responsible for managing any significant events 
 * that occur during the simulation.
 */
void compare();
/*
 * This function takes no parameters and returns
 * an integer representing the length of the linked list. 
 * This function gives us the length of the linked list.
 */
int length();
/*
 * This function takes one parameter and returns a pointer to
 * the struct ships. The parameter represents and index of an element
 * in the linked list. The function is responsible for taking an element
 * out of the linked list. 
 */
ships* getship(int index);
	
#endif
