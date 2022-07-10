#pragma once

#include <Arduino.h>
#include "timer.hpp"
#include "flags.hpp"
#include "defines.hpp"

namespace Logic
{
  class Logic
  {
    public:
    
      Timer error_timer;
      Timer working_timer;
      Timer pause_timer;

      unsigned long start_working_time;
    
      void watch()
      {
        error_timer.watch();
        working_timer.watch();
        pause_timer.watch();
        key_handler();
        
        //turn off watcher
        
        if(Flags::off_query)
        {
          Flags::off_query = false;
          this->turn_off();
        }
        
      }
      
      void set_error()
      {
        digitalWrite(error_set, HIGH);
        digitalWrite(dist_manage, LOW);
        Serial.println("error set");
        error_timer.set(ERROR_TIME, [](){
          digitalWrite(error_set, LOW);
          Serial.println("error removed");
          //Flags::lower();
        });
      }

      void manage_on()
      {
        if(!is_on_error && !key_is_on && !is_working)
        {
          Flags::state = on_lock;
          Flags::manage_on = true;
          digitalWrite(dist_manage, HIGH);
          Flags::lock_on = true;
          digitalWrite(lock_ind, HIGH);
        }
      }

      void manage_off(unsigned long working_time)
      {
        if(Flags::lock_on)
        {
          Flags::state = initial;
          Flags::lock_on = false;
          digitalWrite(lock_ind, LOW);
        }
        else
        {
          Flags::state = working_online;
          Flags::is_turned_on = true;
          start_working_time = millis();
          working_timer.set(working_time, [](){ Flags::off_query = true; });
        }
        Flags::manage_on = false;
        digitalWrite(dist_manage, LOW);
      }

      void turn_off()
      {
        working_timer.interrupt();
        this->set_error();
        Flags::state = pause;
        pause_timer.set(max(millis() - start_working_time, (unsigned long)(MINIMAL_PAUSE_TIME)), Flags::lower);
      }
      
    private:

      
      
    
      void key_handler()
      {
        if(key_is_on && Flags::manage_on && Flags::lock_on)
        {
          Flags::lock_on = false;
          Flags::lock_off = true;
          Flags::state = on_manage;
          digitalWrite(lock_ind, LOW);
        }
        else if(!key_is_on && Flags::manage_on && Flags::lock_off)
        {
          Flags::lock_off = false;
          Flags::lock_on = true;
          Flags::state = on_lock;
          digitalWrite(lock_ind, HIGH);
        }
      }

  };  
  
  Logic logic;
};
