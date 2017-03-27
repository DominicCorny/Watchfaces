#include <pebble.h>
#include "VektorZeichnungen.h"

//Vektorzeichnungen
void canvas_update_proc(Layer *layer, GContext *ctx) 
{
  //Variablen
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, GRect(0, 0,13*((tick_time->tm_sec+3)/6), 3), 0, GCornerNone);
}