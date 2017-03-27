#include <pebble.h>
#include "BatterieAnzeige.h"
const GPoint center = {
      .x =35,
      .y =35,
    };
int16_t punkte_gespeichert = 0;


    
void battery_callback(BatteryChargeState state) {
  rad = (TRIG_MAX_ANGLE * state.charge_percent  / 200) + (TRIG_MAX_ANGLE * 3 / 4);
  layer_mark_dirty(s_akku_layer);
}

void akku_update_proc(Layer *layer, GContext *ctx)
{ 
  zeichne_zeiger(ctx,23,1);
  zeichne_zeiger(ctx,12,2);
  zeichne_ziffernblatt(ctx);
}
void zeichne_ziffernblatt(GContext *ctx)
{
  if(punkte_gespeichert == 0)
  {
    for(int i=0;i<11;i++)
    {
      rad_array[i] = (TRIG_MAX_ANGLE * i / 20) + (TRIG_MAX_ANGLE * 3 / 4);
      punkte_ziffernblatt[i] [0] = (int16_t)(sin_lookup(rad_array[i]) * (int32_t)28 / TRIG_MAX_RATIO) + center.x;
      punkte_ziffernblatt[i] [1] = (int16_t)(-cos_lookup(rad_array[i]) * (int32_t)28 / TRIG_MAX_RATIO) + center.y;
      punkte_ziffernblatt[i] [2] = (int16_t)(sin_lookup(rad_array[i]) * (int32_t)31 / TRIG_MAX_RATIO) + center.x;
      punkte_ziffernblatt[i] [3] = (int16_t)(-cos_lookup(rad_array[i]) * (int32_t)31/ TRIG_MAX_RATIO) + center.y;
    }
    punkte_gespeichert = 1;
  }
  for(int i=0;i<11;i++)
  {
    GPoint start = {
      .x = punkte_ziffernblatt[i] [0],
      .y = punkte_ziffernblatt[i] [1],
     };
    GPoint end = {
      .x = punkte_ziffernblatt[i] [2],
      .y = punkte_ziffernblatt[i] [3],
     };
    graphics_context_set_antialiased(ctx, false);
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_context_set_stroke_width(ctx,1);
    graphics_draw_line(ctx,start, end);
  }
}
void zeichne_zeiger(GContext *ctx,int laenge,int dicke)
{
    GPoint hand = {
    .x = (int16_t)(sin_lookup(rad) * (int32_t)laenge / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(rad) * (int32_t)laenge / TRIG_MAX_RATIO) + center.y,
   };
    // Set the stroke width (must be an odd integer value)
    graphics_context_set_stroke_width(ctx,dicke);
    // Disable antialiasing (enabled by default where available)
    graphics_context_set_antialiased(ctx, false);
    // second hand
    graphics_context_set_stroke_color(ctx, GColorWhite);
    graphics_draw_line(ctx,hand, center);
}