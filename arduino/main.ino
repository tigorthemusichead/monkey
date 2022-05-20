#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>

#define WIFI_SEARCHING_TIME 60000
#define CONNECTION_LOSS_AFTER 10000
#define WIFI_BLINK_TIME 500
#define ERROR_TIME 500
#define WORKING_RATE 100

//INPUTS

const byte analog_state = 0; // Analog input for checking working status 
const byte working_state = 2;
const byte error_state = 4; // Input for error checking: 0 - error
const byte key_state = 16; // Input for key state checking: 0 - on, 1 - off 

//OUTPUTS

const byte wifi_ind = 14; // Output wifi status led: blink - in search, even - connected
const byte dist_manage = 5; // Output distant mangement led and status: 1 - on, 0 - off
const byte lock_ind = 12; 
const byte error_set = 13;

//WIFI settings

const char *ssid = "MART-250-001"; // wifi name
const char *pass = "12345678"; // wifi password


ESP8266WebServer HTTP(80); // run server

char web[] = R"=====()=====";

//variables

unsigned long timer = 0;
bool connected = 0;
bool distant = 0;
short decsec = 0;

class State
{
  private:
  
  unsigned long start_time = millis();
  unsigned long timer = millis();
  unsigned long error_timer = 0;
  unsigned long blink_timer = 0;
  bool reboot = 0;
  
  void searchWIFI(){
    if((timer - start_time < WIFI_SEARCHING_TIME) && !WiFi.softAPgetStationNum() && !this->is_connected){
      if((millis() - start_time) % 1000 < 500){
        digitalWrite(wifi_ind, 1);
      }
      else{
        digitalWrite(wifi_ind, 0);
      }
    }
    //succesfuly connected
    else if(WiFi.softAPgetStationNum() && !this->is_connected){
      digitalWrite(wifi_ind, 1);
      this->is_connected = 1;
      timer += WIFI_SEARCHING_TIME;
    }
    //lost connection
    else if(!WiFi.softAPgetStationNum() && this->is_connected){
      if(!((this->stage == 0) && this->is_working)){
        this->needs_stop = 1;
        this->reboot = 1;
      }
      digitalWrite(wifi_ind, 0);
      this->is_connected = 0;
    }
    //not found
    else if((timer - start_time > WIFI_SEARCHING_TIME) && !WiFi.softAPgetStationNum()){
      digitalWrite(wifi_ind, 0);
      //Serial.println("led off");
    }
  }

  void checkLOCK(){
    //enter LOCK
    if(!this->key_on && this->on_manage && !this->on_lock){
      this->on_lock = 1;
      digitalWrite(lock_ind, 1);
      this->stage = 1;
    }
    //exit LOCK
    else if((this->key_on && this->on_manage && this->on_lock)||(!this->key_on && !this->on_manage && this->on_lock)){
      this->on_lock = 0;
      digitalWrite(lock_ind, 0);
      this->stage = 2;
      Serial.print(this->stage);
      Serial.println(" - stage");
    }
  }

  void turn_off_seed(){
    if(this->needs_stop && !error_timer){
      if(reboot) this->stage = 0;
      else this->stage = 4;
      
      digitalWrite(error_set, 1);
      digitalWrite(dist_manage, 0);
      
      if(millis() - this->working_timer > 60000)
        this->working_timer = millis() - this->working_timer;
      else
        this->working_timer = 60000;
        
      error_timer = timer;
    }
    else if(timer - error_timer > ERROR_TIME){
      this->needs_stop = 0;
      reboot = 0;
      digitalWrite(error_set, 0);
      error_timer = 0;
    }
  }

  void connection_loss_seed(){
    if((this->state_timer > 0) && (timer - this->state_timer > CONNECTION_LOSS_AFTER)){
      if(!((this->stage == 0) && this->is_working)){
        reboot = 1;
        this->needs_stop = 1;
      }
      state_timer = 0;
      
    }
  }
  
  public:

  unsigned long working_time_limit = 0;
  unsigned long state_timer = 0;
  unsigned long working_timer = 0;
  float voltage;
  bool is_working = 0;
  bool is_connected = 0;
  bool on_lock = 0;
  bool on_error = 0;
  bool on_manage = 0;
  bool key_on = 0;
  bool needs_stop = 0;
  short stage = 0;
  
  void check(){
    if(millis() - timer > WORKING_RATE){
      timer = millis();
      
      this->voltage = analogRead(analog_state);
      this->is_working = !digitalRead(working_state);
      this->on_error = !digitalRead(error_state);
      this->key_on = !digitalRead(key_state);
      this->on_manage = digitalRead(dist_manage);
      
      searchWIFI();
      checkLOCK();
      turn_off_seed();
      connection_loss_seed();
    }
  }
  
  String manage_turn_on(){
    String success = "0";
    if(!this->on_error && !this->key_on && !this->is_working && this->is_connected){
      on_manage = 1;
      digitalWrite(dist_manage, 1);
      success = "1";
      this->stage = 1;
    }
    return success;
  }

  String manage_turn_off(){
    if(!this->on_lock){
      this->stage = 3;
      this->working_timer = millis();
    }
    else{
      this->stage = 0;
      this->on_lock = 0;
      digitalWrite(lock_ind, 0);  
    }
    on_manage = 0;
    digitalWrite(dist_manage, 0);
    return("<h1>Manage is turned off</h1>");
  }
};

State state;

void setup() {
  pinMode(analog_state, INPUT);
  pinMode(working_state, INPUT);
  pinMode(error_state, INPUT);
  pinMode(key_state, INPUT);  
  pinMode(wifi_ind, OUTPUT);
  pinMode(dist_manage, OUTPUT);
  pinMode(lock_ind, OUTPUT);
  pinMode(error_set, OUTPUT);
  Serial.begin(9600);
  WiFi.softAP(ssid, pass, 1, 0, 1);
  HTTP.begin();

  HTTP.on("/", [](){
    HTTP.send(200, "text/html", web);
  });
  
  HTTP.on("/manage-on", [](){
    HTTP.enableCORS(true);
    HTTP.send(200, "text/plain", state.manage_turn_on());
  });
  
  HTTP.on("/manage-off", [](){
    HTTP.enableCORS(true);
    //state.working_time_limit = HTTP.arg("length").toInt();    //  http://192.168.4.1/manage-off?length=5000
    HTTP.send(200, "text/plain", state.manage_turn_off());
  });

  HTTP.on("/off", [](){
    HTTP.enableCORS(true);
    state.needs_stop = 1;
    state.stage = 4;
    if(millis() - state.working_timer > 60000)
      state.working_timer = millis() - state.working_timer;
    else
      state.working_timer = 60000;
    HTTP.send(200, "text/plain", String(state.working_timer));
  });

  HTTP.on("/ready", [](){
    HTTP.enableCORS(true);
    state.stage = 0;
    state.working_timer = 0;
    HTTP.send(200, "text/plain", "ready");
  });

  HTTP.on("/state", [](){
    String stateJson = "{\"stage\":\"" + String(state.stage) + "\",\"voltage\":\"" + String(state.voltage) + "\",\"is_working\":\"" + String(state.is_working) + "\",\"on_lock\":\"" + String(state.on_lock) + "\",\"on_error\":\"" + String(state.on_error) + "\",\"on_manage\":\"" + String(state.on_manage)+"\"}";
    HTTP.enableCORS(true);
    state.state_timer = millis();
    HTTP.send(200, "application/json", stateJson);
  });
  
}

void loop() 
{  
  HTTP.handleClient(); // HTTP handler

  state.check();
  
}
