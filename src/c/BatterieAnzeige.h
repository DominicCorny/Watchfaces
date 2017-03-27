#pragma once
void battery_callback(BatteryChargeState state);
void akku_update_proc(Layer *layer, GContext *ctx);
//Akku Layer
Layer *s_akku_layer;
int32_t rad;
int32_t rad_array[11];
// [i] [0] start_x [i] [1] start_y ...
int punkte_ziffernblatt[11] [4];
void zeichne_ziffernblatt(GContext *ctx);
void zeichne_zeiger(GContext *ctx,int laenge,int dicke);

static BitmapLayer *s_tank_layer;
static GBitmap *s_tank_bitmap;