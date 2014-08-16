#include <pebble.h>
 
Window* window;
TextLayer *text_layer;
InverterLayer *inv_layer;
GBitmap *txlogo_bitmap;
BitmapLayer *txlogo_layer;
char buffer[] = "00:00:00";

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
    strftime(buffer, sizeof("00:00:00"), "%H:%M:%S", tick_time);
    text_layer_set_text(text_layer, buffer);
}

void window_load(Window *window)
{
  //Add an image
  txlogo_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TX_LOGO_WHITE2); //RESOURCE_ID is prefix
  
  txlogo_layer = bitmap_layer_create(GRect(0, 0, 144, 168)); //Screen Size is 144x168 pixels
  bitmap_layer_set_bitmap(txlogo_layer, txlogo_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(txlogo_layer));

  
  text_layer = text_layer_create(GRect(0, 125, 144, 40));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
  text_layer_set_text(text_layer, "TablonautiX GmbH");
  
  //Register a tick handler
  struct tm *t;
  time_t temp;
  temp = time(NULL);
  t = localtime(&temp);
  tick_handler(t, SECOND_UNIT); //more battery friendly: MINUTE_UNIT

}
 
void window_unload(Window *window)
{
  text_layer_destroy(text_layer);
  inverter_layer_destroy(inv_layer);
}
 
void init()
{
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
	
  window_set_fullscreen(window, true);
  window_stack_push(window, true);
  
  tick_timer_service_subscribe(SECOND_UNIT, (TickHandler) tick_handler);
}
 
void deinit()
{
  tick_timer_service_unsubscribe();
  window_destroy(window);
}
 
int main(void)
{
  init();
  app_event_loop();
  deinit();
}

