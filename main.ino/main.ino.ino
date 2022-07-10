#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h> 
#include "defines.hpp"
#include "flags.hpp"
#include "timer.hpp"
#include "logic.hpp"

ESP8266WebServer HTTP = ESP8266WebServer(80);
Timer searching_timer;
Interval check_interval;
Interval blinking_interval;



void setup() {
  Serial.begin(9600);
    
  HTTP.begin();
  
  WiFi.softAP(ssid, pass, 1, 0, 1);
  
  HTTP.on("/", [&](){
    HTTP.enableCORS(true);
    HTTP.send(200, "text/html", String(Flags::state));
  });
  
  HTTP.on("/manage-on", [&](){
    HTTP.enableCORS(true);
    Logic::logic.manage_on();
    HTTP.send(200, "text/plain", String(digitalRead(dist_manage)));
  });

  HTTP.on("/manage-off", [&](){
    
    HTTP.enableCORS(true);  
    String working_time_string;
    unsigned long working_time;
    if (HTTP.args()) 
    {
      working_time_string = HTTP.arg("time");
      working_time = working_time_string.toInt();
    }  
    else working_time = 600000;
    
    Logic::logic.manage_off(working_time);
    HTTP.send(200, "text/plain", "");
  });

  HTTP.on("/cancel", [&](){
    HTTP.enableCORS(true);
    
    switch(Flags::state)
    {
      case on_lock:
        Logic::logic.manage_off(0);
        break;
      case on_manage:
        Flags::lower();
        Logic::logic.set_error();
        break;
      case working_online:
        Logic::logic.turn_off();
        break;
      default:
        break;
    }
    HTTP.send(200, "text/plain", "");
  });

  HTTP.on("/state", [&](){
    HTTP.enableCORS(true);
    HTTP.send(200, "application/json", Flags::prepare());
  });

  searching_timer.set(WIFI_SEARCHING_TIME, [](){
    digitalWrite(wifi_ind, LOW);
  });
  
  blinking_interval.set(500, WIFI_SEARCHING_TIME, [](){
    digitalWrite(wifi_ind, !digitalRead(wifi_ind));
  });

  /**
   * For developing only!!!!!!!
   */
  check_interval.set(2000, -1, [](){ Serial.println(Flags::state); });
  
  set_pins();
}

void loop() {
  HTTP.handleClient();
  
  searching_timer.watch();
  blinking_interval.watch();
  check_interval.watch();
  
  if(WiFi.softAPgetStationNum() && (!Flags::found))
  {
    blinking_interval.interrupt();
    searching_timer.interrupt();
    Flags::found = true;
    digitalWrite(wifi_ind, HIGH);
  }
  else if(!WiFi.softAPgetStationNum() && Flags::found)
  {
    Flags::found = false;
    digitalWrite(wifi_ind, LOW);
    Logic::logic.set_error();
  }

  
  
  Logic::logic.watch();
}
