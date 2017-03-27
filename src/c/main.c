#include <pebble.h>
#include "VektorZeichnungen.h"
#include "textmanagement.h"
#include "BatterieAnzeige.h"
#include "Wetter.h"
//230 zeilen

//Window and TEXTlayer
static TextLayer *s_time_layer;
static Window *s_main_window;
//Datum
static TextLayer *s_date_layer;
//Zeitstrahl
static Layer *s_canvas_layer;

//Zeit Handling
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) 
{
  if((tick_time->tm_sec+3) % 6 == 0)
    layer_mark_dirty(s_canvas_layer);
  if(tick_time->tm_sec == 0)
    update_time(s_date_layer, s_time_layer);
}

//Methoden Erstellen Löschen
static void main_window_load(Window *window) 
{
  // Create GBitmap
s_tank_bitmap = gbitmap_create_with_resource(RESOURCE_ID_tank_image);

// Create BitmapLayer to display the GBitmap
s_tank_layer = bitmap_layer_create(GRect(86,145,47,22));

// Set the bitmap onto the layer and add to the window
bitmap_layer_set_bitmap(s_tank_layer, s_tank_bitmap);
  
  
  
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create canvas layer
  s_canvas_layer = layer_create(GRect(7, 85,130,3));
  // Create Akku layer
  s_akku_layer = layer_create(GRect(74,128,70,40));
  // Assign the custom drawing procedure
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  //Batterie
  battery_state_service_subscribe(battery_callback);
  layer_set_update_proc(s_akku_layer, akku_update_proc);
  
  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(8,30, bounds.size.w, 50));
  
  //Datum Layer
  s_date_layer = text_layer_create(
      GRect(0,12, bounds.size.w, 25));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_background_color(s_date_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorClear);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  //wetter zeug
  s_weather_layer = text_layer_create(
  GRect(0,90, bounds.size.w, 25));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  text_layer_set_text(s_weather_layer, "Loading...");
  text_layer_set_font(s_weather_layer,fonts_get_system_font(FONT_KEY_GOTHIC_18));
  
  // "Rendering"
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  layer_add_child(window_get_root_layer(window), s_canvas_layer);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_tank_layer));
  layer_add_child(window_get_root_layer(window), s_akku_layer);
  
}

static void main_window_unload(Window *window) 
{
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_layer);
  layer_destroy(s_canvas_layer);
  layer_destroy(s_akku_layer);
  gbitmap_destroy(s_tank_bitmap);
  bitmap_layer_destroy(s_tank_layer);
  text_layer_destroy(s_weather_layer);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) 
{

}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() 
{
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  
  //Hintergrundfarbe
  window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  //Zeit Zeug
  update_time(s_date_layer, s_time_layer);
  update_date(s_date_layer);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  battery_callback(battery_state_service_peek());
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  const int inbox_size = 128;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);
}

static void deinit() 
{
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
