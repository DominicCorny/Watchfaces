#include <pebble.h>
#include "textmanagement.h"

static const char wochenTage[7] [2] = { {'S','o'},
                             {'M','o'},
                             {'D','i'},
                             {'M','i'},
                             {'D','o'},
                             {'F','r'},
                             {'S','a'}};


//Datum Handling
void update_date(TextLayer *s_date_layer)
{
  //Variablen
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char date_buffer[10];
  static char ausgabe[15];
  
  //Code
  strftime(date_buffer, sizeof(date_buffer),"%d/%m/%y", tick_time);
  
  for(int i=0;i<2;i++)
  {
        ausgabe[i] = wochenTage[tick_time->tm_wday] [i];
  }
  ausgabe[2] = '.';
  ausgabe[3] = ' ';
  ausgabe[4] = ' ';
  for(int j=0;j<10;j++)
  {
        ausgabe[j+5] = date_buffer[j];
  }
  text_layer_set_text(s_date_layer, ausgabe);
}
//zeit Handling
void update_time(TextLayer *s_date_layer, TextLayer *s_time_layer) 
{
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[6];
  strftime(s_buffer, sizeof(s_buffer),"%H:%M", tick_time);
  
  if(s_buffer[0] == '0' && s_buffer[1] == '0' && s_buffer [3] == '0' && s_buffer[4] == '0')
    update_date(s_date_layer);
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
  
}
