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
       strcpy(buff, "Goedemiddag! "); // в коде нет вызова strcpy там просто mov несколько раз
   }
   else if (gloabal_var == 0)
   {
       strcpy(buff, "Hello "); // в коде нет вызова strcpy там просто mov несколько раз
   }
   strcat(buff, lang);
   puts(buff);
}
 
int main(int argc, char **argv)
{
   char buff[76];
   char *lang;
 
   if (argc != 3)
       return (1);
   memse