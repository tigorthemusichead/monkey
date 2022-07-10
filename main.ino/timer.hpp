#pragma once

#include <Arduino.h>

class Timer
{
  private: 
    unsigned long start_time;
    unsigned long waiting_time;
    bool needs_stop = false;
    bool started = false;
    void (*callback)(void);

  public: 
    void set(unsigned long time, void (*function)(void))
    {
      if (!started){
        start_time = millis();
        started = true;
        needs_stop = false;
        callback = function;
        waiting_time = time;
      }
    }
    
    void interrupt()
    {
      needs_stop = true;
      started = false;
    }
    
    void watch()
    {
      if (started && (millis() - start_time > waiting_time) && !needs_stop){
        needs_stop = true;
        started = false;
        callback();
      }
    }
};

class Interval
{
  private:
    unsigned long start_time;
    unsigned long last_time;
    unsigned long waiting_time;
    unsigned long working_time;
    void (*callback)(void);
    bool started = false;
    bool needs_stop = false;

  public:
    void set(unsigned long time, unsigned long w_time, void (*function)(void))
    {
      if(!started){
        start_time = millis();
        last_time = millis();
        started = true;
        needs_stop = false;
        callback = function;
        waiting_time = time;
        working_time = w_time;
      }
    }
    
    void interrupt()
    {
      needs_stop = true;
      started = false;
    }
    
    void watch()
    {
      if (started && (millis() - last_time > waiting_time) && !needs_stop){
        last_time = millis();
        callback();
      }
      if ((millis() - start_time > working_time) && (working_time != -1))
      {
        needs_stop = true;
        started = false;
      }
    }
};
