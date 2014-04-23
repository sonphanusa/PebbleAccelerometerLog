#include "pebble.h"
#include "kalman.h"
//#include "pebble_debug.h"

#define	NUM_SAMPLES	25
	
/** Acc x,y,z vector struct
typedef struct __attribute__((__packed__)) AccLogData {
	//uint64_t timestamp;
	int16_t x;
	int16_t y; 
	int16_t z;
} AccLogData;
**/
	
static Window *window;

static TextLayer *top_layer;
static TextLayer *center_layer;
static TextLayer *bottom_layer;

//AccLogData accLogData;
int16_t acc_mag;

DataLoggingSessionRef logging_session;
DataLoggingResult result;

bool running = false;

// Accelerometer data handler, called when enough samples have been received to fill one batch of data
static void accel_data_handler(AccelData *data, uint32_t num_samples) {
	// Create logging session
	logging_session = data_logging_create(1, DATA_LOGGING_INT, sizeof(int16_t), false);
	// Getting acc. x,y,z components with timestamp
	//accLogData.timestamp = data->timestamp;	
	//accLogData.x = data->x;
	//accLogData.y = data->y;
	//accLogData.z = data->z;	
	acc_mag = Kalman_Filter((float) data->x, (float) data->y, (float) data->z);
	// Logging
	result = data_logging_log(logging_session, &acc_mag, num_samples);	
	if (result ==  DATA_LOGGING_SUCCESS) data_logging_finish(logging_session);
}
	
// Toggle accelerometer service and logging
static void toggle_accelerometer() {
	running = !running;	
	if (running) {
		// UI
		text_layer_set_text(center_layer, "Stop ->");
		text_layer_set_text(bottom_layer, "Running...");
		// Accelerometer subscribe
    	accel_data_service_subscribe(NUM_SAMPLES, &accel_data_handler);
		accel_service_set_sampling_rate(ACCEL_SAMPLING_25HZ);		   	 
    } else {
		// Unsubscribe Accelerometer
		accel_data_service_unsubscribe();
		// Finish up logging
		result = data_logging_log(logging_session, &acc_mag, NUM_SAMPLES);	
		if (result !=  DATA_LOGGING_SUCCESS) data_logging_finish(logging_session);		
		// UI
		text_layer_set_text(center_layer, "Resume ->");
		text_layer_set_text(bottom_layer, "");
    }
}
	
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	toggle_accelerometer();
}

/*
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	
}
*/

static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
	//window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
	//window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}	

static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);
	
	top_layer = text_layer_create(GRect(0, 5, bounds.size.w /* width */, 28 /* height */));
	text_layer_set_text(top_layer, "Accelerometer Log");
	text_layer_set_font(top_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(top_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(top_layer));
	
	center_layer = text_layer_create(GRect(0, 50, bounds.size.w /* width */, 30 /* height */));
	text_layer_set_text(center_layer, "Start ->");
	text_layer_set_font(center_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(center_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(center_layer));
	
	bottom_layer = text_layer_create(GRect(0, 110, bounds.size.w /* width */, 30 /* height */));
	text_layer_set_text(bottom_layer, "");
	text_layer_set_font(bottom_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(bottom_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(bottom_layer));
}

static void window_unload(Window *window) {
	text_layer_destroy(top_layer);
	text_layer_destroy(center_layer);
	text_layer_destroy(bottom_layer);
}

static void init(void) {
	window = window_create();
	window_set_click_config_provider(window, click_config_provider);
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});
	window_stack_push(window, false /* No Animated */);
}

static void deinit(void) {  
  	window_destroy(window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}

