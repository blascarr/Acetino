
#define FAN_PIN 12
#define MAX_DEPTH 2

// ----- Track List ----- //
// Schedule Tracks Definition
struct scheduletrack{
  long timestamp;
  int track;
  scheduletrack( long timeflag, int t ):timestamp(timeflag),track(t){}
};

scheduletrack scheduleList[] = {
  scheduletrack(1000,2),
  scheduletrack(5000,3),
  scheduletrack(10000,4), 
  scheduletrack(15000,5), 
  scheduletrack(20000,6),
  scheduletrack(25000,7),
  scheduletrack(30000,11), 
  scheduletrack(37000,9),
  scheduletrack(40000,8), 
  scheduletrack(50000,10), 
  scheduletrack(55000,1), 
};

#include "DFPlayer_Controller.h"
#include "Fan_Controller.h"

#include "Menu_Controller.h"
#include "Time_Controller.h"

#include "Acetino.h"

clockscheduler clocksystem( mp3 , tempo, scheduleList, sizeof( scheduleList )/sizeof( scheduletrack ) );
Acetino model( clocksystem ,lcd, lpg );

#include "Menu_events.h"

long timestamp = 100;
long oldtime = 0 ;

void setup() {
  model.set_fan_pin( FAN_PIN );
  model.init();
  model.set_nav ( nav );

  //-------------- INPUT- OUTPUT INIT ----------------
  pinMode(encBtn,INPUT_PULLUP);
  encoder.begin();
  
  Serial.println("Clock Menu");
  delay(500);
  out.clear();
}

void loop() {
  
    //Si se ha salido del menu , estamos en estado parado
    if(nav.sleepTask) {
      model.update_screen( out[0] );
    }
    
    //if( millis() - oldtime > timestamp ){
      nav.poll();
      //oldtime = millis();
      //
    //}

    model.update(  );

}
