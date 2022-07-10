#pragma once

#include <ArduinoJson.h>

enum State
{
  working_offline,
  initial,
  on_lock,
  on_manage,
  working_online,
  pause
};

namespace Flags
{
  bool found = false; 
  bool is_turned_on = false;
  bool off_query = false;
  bool is_waiting = false;
  bool manage_on = false;
  bool manage_off = false;
  bool lock_on = false;
  bool lock_off = false;
  State state = initial;
  
  void lower()
  {
    is_turned_on = false;
    off_query = false;
    is_waiting = false;
    manage_on = false;
    manage_off = false;
    lock_on = false;
    lock_off = false;
    state = initial;
  }

  String prepare()
  {
    StaticJsonDocument<200> response;
    String response_string;
    
    response["state"] = state;
    response["voltage"] = voltage;
    
    serializeJson(response, response_string);
    return(response_string);
  }
};
