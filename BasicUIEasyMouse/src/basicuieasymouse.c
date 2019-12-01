#include "basicuieasymouse.h"
#include <sensor.h>
#include <dlog.h> //for logging
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 7500

int sock = 0;

typedef struct appdata {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *label;
} appdata_s;

//You must declare sensor handle and sensor listener handle.
sensor_h sensor;
sensor_listener_h listener;

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

int create_client() {
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        dlog_print(DLOG_ERROR, LOG_TAG, "Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "192.168.50.222", &serv_addr.sin_addr) <= 0) {
        dlog_print(DLOG_ERROR, LOG_TAG, "Invalid address, address not supported");
        return -1;
    }

    if (connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        dlog_print(DLOG_ERROR, LOG_TAG, "Connection Failed");
        return -1;
    }

    char *hello = "Why hello there server :)\n";
    send(sock, hello, strlen(hello), 0);
    dlog_print(DLOG_INFO, LOG_TAG, "Sent the server a hello!!!");
    return 0;
}

static void
create_base_gui(appdata_s *ad)
{
	/* Window */
	/* Create and initialize elm_win.
	   elm_win is mandatory to manipulate window. */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
	eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

	/* Conformant */
	/* Create and initialize elm_conformant.
	   elm_conformant is mandatory for base gui to have proper size
	   when indicator or virtual keypad is visible. */
	ad->conform = elm_conformant_add(ad->win);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	create_client();

	/* Label */
	/* Create an actual view of the base gui.
//	   Modify this part to change the view. */
//	ad->label = elm_label_add(ad->conform);
//	char str[20];
//	str = accelX;
//	sprintf (str, "%f", accelX);
//	char message[100];
//	strcat(message, str);
//	str = accelY;
//	strcat(message, str);
//	str = accelZ;
//	strcat(message, str);
	elm_object_text_set(ad->label, "Hello World");
	evas_object_size_hint_weight_set(ad->label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_content_set(ad->conform, ad->label);

	/* Show window after base gui is set up */
	evas_object_show(ad->win);
}

void send_floats(float accelerometer[]) {
    char destination_buffer[100];
    char number_buffer[50];

    // Vector 1
    gcvt(accelerometer[0], 10, number_buffer);
    strcpy(destination_buffer, number_buffer);
    strcat(destination_buffer, " ");

    // Vector 2
    gcvt(accelerometer[1], 10, number_buffer);
    strcat(destination_buffer, number_buffer);
    strcat(destination_buffer, " ");

    // Vector 3
    gcvt(accelerometer[2], 10, number_buffer);
    strcat(destination_buffer, number_buffer);
    strcat(destination_buffer, " ");



    // Print the values and send
    send(sock, destination_buffer, strlen(destination_buffer), 0);
}


//sensor event callback implementation
void sensor_event_callback(sensor_h sensor, sensor_event_s *event, void *user_data)
{
    sensor_type_e type;
    sensor_get_type(sensor, &type);
    if(type == SENSOR_ACCELEROMETER)
    {
    	send_floats(event->values);
        //dlog_print(DLOG_INFO, LOG_TAG, "Sent accelerometer data");
        //dlog_print(DLOG_INFO, LOG_TAG, "accelerometer: %f, %f, %f", event->values[0], event->values[1], event->values[2]);
    }
}

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	appdata_s *ad = data;

	create_base_gui(ad);

	//Starting sensor listener
		sensor_type_e type = SENSOR_ACCELEROMETER;

		if (sensor_get_default_sensor(type, &sensor) == SENSOR_ERROR_NONE)
		{
		    if (sensor_create_listener(sensor, &listener) == SENSOR_ERROR_NONE
		        && sensor_listener_set_event_cb(listener, 100, sensor_event_callback, NULL) == SENSOR_ERROR_NONE)
		    {
		        if (sensor_listener_start(listener) == SENSOR_ERROR_NONE)
		        {
		            dlog_print(DLOG_INFO, LOG_TAG, "sensor listener started!");

		        }
		    }
		}


	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */



}

static void
app_terminate(void *data)
{
	/* Release all resources. */
	//Stopping sensor listener
	int err = sensor_listener_stop(listener);
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}


int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

	return ret;
}
