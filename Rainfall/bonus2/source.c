#include <string.h>
#include <stdio.h>
#include <stdlib.h>
 
int gloabal_var = 0;
 
void greetuser(char lang[])
{
   char buff[72];
 
   if (gloabal_var == 1)
   {
       strcpy(buff, "Hyvää päivää "); // в коде нет вызова strcpy там просто mov несколько раз
   }
   else if (gloabal_var == 2)
   {
       strcpy(buff, "Goedemiddag! "); // 