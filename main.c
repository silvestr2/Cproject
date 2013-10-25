#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <string.h>
#include "navigation.h"
#include <math.h>
#include <time.h>
 
 
ships *head=NULL;
shiptime *current_time;

/*
 * This is the main function. It takes no parameters
 * and returns an integer, which represents the exit status
 * of the program. It is responsible for running the program.
 */
int main(void) {
    current_time = (shiptime *)malloc(sizeof(shiptime));
    ships *current_ships = (ships *)malloc(sizeof(ships));
    
        char path[60];
        double time;
        double step;
    
        FILE *file;
        printf("enter file name: ");
        scanf("%s",path);
	file=fopen(path,"r");
	printf("enter the simulation time: ");
   	scanf("%lf",&time);
   	printf("enter the simulation time step: ");
   	scanf(" %lf",&step);
    	fscanf (file, "%d", &current_time->tm_mday);
        fscanf (file, "%d", &current_time->tm_mon);
        fscanf (file, "%d", &current_time->tm_year);
        fscanf (file, "%d", &current_time->tm_hour);
        fscanf (file, "%d", &current_time->tm_min);
        fscanf (file, "%d", &current_time->tm_sec);
        
        while (!feof(file)){
                fscanf (file, "%s", current_ships->ais_id);
                fscanf (file, " %lf", &current_ships->shiplocation.lat);
                fscanf (file, " %lf", &current_ships->shiplocation.lng);
                fscanf (file, " %lf", &current_ships->direction);
                fscanf (file, " %lf", &current_ships->speed);
                insert_ship(create_ship(current_ships->ais_id,current_ships->shiplocation,current_ships->direction,current_ships->speed));
                }
        fclose(file);
       
        current_time->tm_year-=1900;
        current_time->tm_mon-=1;
        
        update_all(time,step);
        
        file=fopen("final.txt","a");
        current_ships=head;
      
        fprintf (file, "%02d ", current_time->tm_mday);
        fprintf (file, "%02d ", current_time->tm_mon);
        fprintf (file, "%d ", current_time->tm_year+1900);
        fprintf (file, "%02d ", current_time->tm_hour);
        fprintf (file, "%02d ", current_time->tm_min);
        fprintf (file, "%02d ", current_time->tm_sec);
	 
        while (current_ships!=NULL){
                fprintf (file, "%s", current_ships->ais_id);
                fprintf (file, " %lf", current_ships->shiplocation.lat);
                fprintf (file, " %lf", current_ships->shiplocation.lng);
                fprintf (file, " %lf", current_ships->direction);
                fprintf (file, " %lf", current_ships->speed); 
                current_ships=current_ships->next;
                }
        fclose(file);  
        
	return (EXIT_SUCCESS);
}
/*
 * This function takes four different parameters and returns a pointer 
 * to the struct ship. The first parameter is a char representing AIS_ID
 * of a ship, the second one represents its location, the third one represents
 * the direction of travel, and the last one represents its speed.
 */
ships *create_ship(char ais_id[],location shiplocation,double direction,double speed){
        ships *new_ship;
        new_ship=(ships *)malloc(sizeof(ships));
        strcpy(new_ship->ais_id,ais_id);
        new_ship->shiplocation=shiplocation;
        new_ship->direction=direction;
        new_ship->speed=speed;
        new_ship->next = NULL;
        
        return new_ship;
}
/*
 * This function takes one parameter and returns nothing. 
 * The parameter represents a pointer to struct ships. 
 * The function is responsible for adding elements to the 
 * linked list at the front of it.
 */
void insert_ship(ships *temporary_ship){
        if (head == NULL){  
                head=temporary_ship;  
                head->next=NULL;  
        }
        else {  
                temporary_ship->next=head;  
                head=temporary_ship;  
        }  
}
/*
 * This function takes two parameters and returns nothing. 
 * The first parameter represents the time of the simulation, 
 * and the other represents its time step. The function is responsible 
 * for updating positions of the ships, and also contains code for 
 * updating the time.
 */
void update_all(double time,double timestep){
       	int i;
       	double howmanytimes;
        char buffer[20];
        time_t updatetime;
        updatetime=mktime(current_time);
        howmanytimes=time*60/timestep;
        ships *current;
        
        for(i=0;i<(int)howmanytimes;i++){
		current=head;
                
                if(current==NULL){
                        printf("The list is empty");
                }
                else{
                        updatetime=updatetime+(timestep*60);
                        current_time=localtime(&updatetime);
                        strftime(buffer,20,"%d %m %Y %H %M %S",current_time);
                        printf("%s\n",buffer);
                
				while((current!=NULL)&&(current->isdead==0)){
                                        double radians=current->direction*M_PI/180;
                                        current->shiplocation.lat= current->shiplocation.lat+(current->speed*cos(radians)*timestep)/3600;
					current->shiplocation.lng= current->shiplocation.lng+(current->speed*sin(radians)*timestep)/(cos(current->shiplocation.lng*M_PI/180))/3600;
                                            
                                        if (((51.667 < current->shiplocation.lat)&&(current->shiplocation.lat < 52.833))&&((-6.667 < current->shiplocation.lng)&&(current->shiplocation.lng < -3.833)))
                                                printf("%-s %7.3f %7.3f %7.3f %7.3f \n", current->ais_id, current->shiplocation.lat, current->shiplocation.lng, current->direction,current->speed);
                                                current=current->next;
                                }
                                compare();
                                printf("\n");
                       }
             }
}
/*
 * This function takes one parameter and returns a pointer to
 * the struct ships. The parameter represents and index of an element
 * in the linked list. The function is responsible for taking an element
 * out of the linked list. 
 */
ships* getship(int index){
    int i;
    ships *currentpointer=head;
    
    for(i=0;i<length();i++){
        if(i<index){
            currentpointer=currentpointer->next;
        }else break;
    }
    return currentpointer;
}
/*
 * This function takes no parameters and returns
 * an integer representing the length of the linked list. 
 * This function gives us the length of the linked list.
 */
int length(){
    int i=0;
    ships *currentpointer=head;
    
    while(currentpointer->next != NULL){
        currentpointer=currentpointer->next;
        i++;
    }
    return i;
}
/*
 * This function takes no parameters and returns nothing.
 * It is responsible for managing any significant events 
 * that occur during the simulation.
 */
void compare(){
    FILE *file;
    file=fopen("eventslog.txt","a");
    int i;
    int j;
    ships *temporary1,*temporary2;
    
    char buffer2[20];
    strftime(buffer2,20,"%d %m %Y %H %M %S",current_time);
                      //  printf("%s\n",buffer2);
    
    for(i=0;i<length()-1;i++){
        for(j=i+1;j<length();j++){
            temporary1=getship(i);
            temporary2=getship(j);
            double space = great_circle(temporary1->shiplocation,temporary2->shiplocation);
            
            if ((51.667 < temporary1->shiplocation.lat)&&(temporary1->shiplocation.lat < 52.833)&&(-6.667 < temporary1->shiplocation.lng)&&(temporary1->shiplocation.lng < -3.833)&&(51.667 < temporary2->shiplocation.lat)&&(temporary2->shiplocation.lat < 52.833)&&(-6.667 < temporary2->shiplocation.lng)&&(temporary2->shiplocation.lng < -3.833)&&(temporary1->isdead==0)&&(temporary2->isdead==0)){
           
                if(space<0.25){
                        temporary1->isdead=1;
                        temporary2->isdead=1;
                        printf(" *collision: %s and %s* \n",temporary1->ais_id,temporary2->ais_id);
                        fprintf(file," *collision: %s and %s(%s)* \n",temporary1->ais_id,temporary2->ais_id,buffer2);
                }
                else if(space<2){
                        printf(" *risk of collision: %s and %s* \n",temporary1->ais_id,temporary2->ais_id);
                        fprintf(file," *risk of collision: %s and %s(%s)* \n",temporary1->ais_id,temporary2->ais_id,buffer2);
                }
           
           }
           if(((temporary1->shiplocation.lat==51.667)&&(temporary1->direction==0.000)) || ((temporary1->shiplocation.lat==52.833)&&(temporary1->direction==180.000))||((temporary1->shiplocation.lng==-3.833)&&(temporary1->direction==90.000)) || ((temporary1->shiplocation.lng==-6.667)&&(temporary1->direction==270.000))){
               printf(" *ship %s went into the area* \n",temporary1->ais_id);
               fprintf(file," *ship %s went into the area(%s)* \n",temporary1->ais_id, buffer2);
           
           }
           if(((temporary2->shiplocation.lat==51.667)&&(temporary2->direction==0.000)) || ((temporary2->shiplocation.lat==52.833)&&(temporary2->direction==180.000))||((temporary2->shiplocation.lng==-3.833)&&(temporary2->direction==90.000)) || ((temporary2->shiplocation.lng==-6.667)&&(temporary2->direction==270.000))){
               printf(" *ship %s went into the area* \n",temporary2->ais_id);
               fprintf(file," *ship %s went into the area(%s)* \n",temporary2->ais_id,buffer2);
           
           }
           if(((temporary1->shiplocation.lat==51.667)&&(temporary1->direction==180.000)) || ((temporary1->shiplocation.lat==52.833)&&(temporary1->direction==0.000))||       ((temporary1->shiplocation.lng==-3.833)&&(temporary1->direction==270.000)) || ((temporary1->shiplocation.lng==-6.667)&&(temporary1->direction==90.000))){
               printf(" *ship %s went beyond the area* \n",temporary1->ais_id);
               fprintf(file," *ship %s went beyond the area(%s)* \n",temporary1->ais_id,buffer2);
           
           }
           if(((temporary2->shiplocation.lat==51.667)&&(temporary2->direction==180.000)) || ((temporary2->shiplocation.lat==52.833)&&(temporary2->direction==0.000))||       ((temporary2->shiplocation.lng==-3.833)&&(temporary2->direction==270.000)) || ((temporary2->shiplocation.lng==-6.667)&&(temporary2->direction==90.000))){
               printf(" *ship %s went beyond the area* \n",temporary2->ais_id);
               fprintf(file," *ship %s went beyond the area(%s)* \n",temporary2->ais_id,buffer2);
           
           }
        }
    } fclose(file); 
}
