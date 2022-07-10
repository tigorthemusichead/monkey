#pragma once

#define WIFI_SEARCHING_TIME 60000
#define CONNECTION_LOSS_AFTER 10000
#define WIFI_BLINK_TIME 500
#define ERROR_TIME 500
#define WORKING_RATE 100
#define MINIMAL_PAUSE_TIME 60000

//INPUTS

const byte analog_state = 0; // Analog input for checking working status 
const byte working_state = 2;
const byte error_state = 4; // Input for error checking: 0 - error
const byte key_state = 16; // Input for key state checking: 0 - on, 1 - off 

#define voltage analogRead(analog_state)
#define is_working !digitalRead(working_state)
#define is_on_error !digitalRead(error_state)
#define key_is_on !digitalRead(key_state)

//OUTPUTS

const byte wifi_ind = 14; // Output wifi status led: blink - in search, even - connected
const byte dist_manage = 5; // Output distant mangement led and status: 1 - on, 0 - off
const byte lock_ind = 12; 
const byte error_set = 13;

static void set_pins()
{
  pinMode(wifi_ind, OUTPUT);
  pinMode(dist_manage, OUTPUT);
  pinMode(lock_ind, OUTPUT);
  pinMode(error_set, OUTPUT);
}

//WIFI settings

static const char *ssid = "MART-250-001"; // wifi name
static const char *pass = "12345678"; // wifi password
