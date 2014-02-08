#include "pebble.h" //Pebble API

/* 
    Your app must include at least one window. The window is
    one of the main aspects of the Pebble UI, and acts as a
	container for the drawable Layers in your UI. There are 
	a variety of different API calls for modifying your app's
	window.
*/
static Window *window; 

/*
    Layers are for displaying text and graphics in your app.
	Layers are the basic building blocks for your UI. You can
	create child layers within a parent layer. There are more
	specific layers you can make sure of, such as TextLayer and
	ImageLayer. 
*/
static Layer *layer;

/*
    Callback function for when the layer is updated.
	Layer is updated whenever it is marked as dirty.
*/
int cordX = 5;
int cordY = 5; 

static void update_layer_callback(Layer *layer, GContext* ctx) {
  graphics_context_set_text_color(ctx, GColorBlack);

  GRect bounds = layer_get_frame(layer);

  graphics_draw_text(ctx,
      "Hello World!",
      fonts_get_system_font(FONT_KEY_FONT_FALLBACK),
      GRect(cordX, cordY, bounds.size.w-10, 100),
      GTextOverflowModeWordWrap,
      GTextAlignmentLeft,
      NULL);

  graphics_draw_text(ctx,
      "Hello World, Again!",
      fonts_get_system_font(FONT_KEY_FONT_FALLBACK),
      GRect(90, 100, bounds.size.w-95, 60),
      GTextOverflowModeWordWrap,
      GTextAlignmentRight,
      NULL);
}

/*
    Though it is not required, having an init() function is
	good practice when developing for Pebble. Inside init(),
	you perform any actions necessary to initialize your app.
	This involves creating your window and any layers you may
	need.
*/

static void up_click_handler(ClickRecognizerRef recognizer, void* context) {
	cordY = cordY - 2;
	layer_mark_dirty(layer);
	
	if (cordY <= 5) {
		cordY = 5;
		vibes_short_pulse();
	}
}

static void down_click_handler(ClickRecognizerRef recognizer, void* context) {
	cordY = cordY + 2;
	layer_mark_dirty(layer);
	
	if (cordY >= 132) {
		cordY = 132;
		vibes_short_pulse();
	}
}

static void select_click_handler(ClickRecognizerRef recognizer, void* context) {
	// Handle select-button clicks
}

static void click_config_provider(void* context) {
  // How quickly the repeated clicks should fire
  const uint16_t repeat_interval_ms = 100;
  // Attach the UP button to the current context
  window_set_click_context(BUTTON_ID_UP, context);
  // Perform the up_click_handler function repeatedly whilst the UP button is held down
  window_single_repeating_click_subscribe(BUTTON_ID_UP, repeat_interval_ms, up_click_handler);

  // Attach the SELECT button to the current context
  window_set_click_context(BUTTON_ID_SELECT, context);
  // Perform the select_click_handler function repeatedly whilst the SELECT button is held down
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);

  // Attach the DOWN button to the current context
  window_set_click_context(BUTTON_ID_DOWN, context);
  // Perform the down_click_handler function repeatedly whilst the DOWN button is held down
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, repeat_interval_ms, down_click_handler);
	
  // You can guess what things look like for the BACK button
}

// Window is loaded whenever it is pushed onto the window stack
static void window_load(Window *window) {
  // Perform any necessary actions when the window is loaded
}

// A window appears whenever it is displayed on the screen
static void window_appear(Window *window) {
  // Perform any necessary actions whenever the window appears
}

// A window disappears it no longer appears on the screen
static void window_disappear(Window *window) {
  // Perform any necessary actions whenever the window disappears
}

// A window is unloaded when it is popped from the window stack
static void window_unload(Window *window) {
  // Perform any necessary actions when the window is unloaded
}

static void init() {
	
  // Creates a window structure
  window = window_create();
  // Push your window onto the screen
  window_stack_push(window, true /* Slide-in animation */); 

  // Obtain the root layer of the window
  Layer *window_layer = window_get_root_layer(window);
  // Get the boundaries of the window
  GRect bounds = layer_get_frame(window_layer);
  // Create a layer structure with specified bounds
  layer = layer_create(bounds);
  // Specify event handler function for layer update
  layer_set_update_proc(layer, update_layer_callback);
  // Make our layer a child of the window's root layer
  layer_add_child(window_layer, layer);
	
	// Set the click handler config for the specified window
  window_set_click_config_provider(window, click_config_provider);
  // Set all the handlers for the specified window
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
	.appear = window_appear,
	.disappear = window_disappear,
    .unload = window_unload,
  });
}

/*
    The deinit() function is not necessary either, but having
	it is good practice. This is where you release any resources,
	such as windows and layers. This function should be called
	in main() right before it returns.
*/
static void deinit() {
  // Must destroy layer when done
  layer_destroy(layer); 
  // Must destroy window when done
  window_destroy(window); 
}

/*
    All apps must include a main function.
*/


int main(void) {
  // Call init() immediately
  init();  
	
  /*
      The app_event_loop() is the entry point for your app, 
	  which allows interaction between your app and the
	  Pebble OS. This function call allows you to subscribe 
	  to event services and implement event handlers.
  */
  app_event_loop();
	
  // Call deinit() before exiting
  deinit();  
}