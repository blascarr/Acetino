/////////////////////////////////////////////////////////////////////////
// MENU DEFINITION

int clockCtrl=LOW;
int fanCtrl=LOW;

result stopClock(eventMask e,navNode& nav,prompt& item) {
  model.scheduler.timer.stop();
  model.scheduler.player.stop();
  return proceed;
}

result resumeClock(eventMask e,navNode& nav,prompt& item) {
  model.scheduler.timer.resume();
  return proceed;
}

result playMP3(eventMask e,navNode& nav,prompt& item) {
  //Detect SD Card
  model.scheduler.player.play(track);
  return proceed;
}

result resetMP3(eventMask e,navNode& nav,prompt& item) {
  model.scheduler.player.reset();
  return proceed;
}

result stopMP3(eventMask e,navNode& nav,prompt& item) {
  model.scheduler.player.stop();
  return proceed;
}

result randomTime(eventMask e,navNode& nav,prompt& item) {
  model.scheduler.player.randomAll();
  return proceed;
}

result fanEvent(eventMask e,navNode& nav,prompt& item) {
  digitalWrite(FAN_PIN, fanCtrl);
  return proceed;
}

result setVolume (eventMask e,navNode& nav,prompt& item) {
  model.scheduler.player.volume( volume );
  return proceed;
}

TOGGLE(fanCtrl,fanToggle,"FAN: ",doNothing,noEvent,noStyle
  ,VALUE("ON",HIGH,fanEvent,exitEvent)
  ,VALUE("OFF",LOW,fanEvent,exitEvent)
);


TOGGLE(clockCtrl,pauseclock,"Pause: ",doNothing,noEvent,noStyle
  ,VALUE("Pause",HIGH,stopClock,enterEvent)
  ,VALUE("Resume",LOW,resumeClock,enterEvent)
);

TOGGLE(clocksystem.type,setType,"Type: ",doNothing,noEvent,noStyle
  ,VALUE("Alarm",clockscheduler::clock,doNothing,enterEvent)
  ,VALUE("Countdown",clockscheduler::countDown,doNothing,enterEvent)
);

MENU(testMenu,"Test menu",doNothing,anyEvent,wrapStyle
  ,FIELD(track,"Play Track","",1,100,1,1,playMP3,exitEvent ,wrapStyle)
  ,OP("Stop MP3",stopMP3,exitEvent)
  ,OP("Reset MP3",resetMP3,exitEvent)
  ,OP("Random Time",randomTime,enterEvent)
  ,EXIT("<Back")
)

MENU(settingsMenu,"Settings menu",doNothing,anyEvent,wrapStyle
  ,FIELD(volume,"Volume","",0,30,5,1,setVolume,exitEvent ,wrapStyle)
  ,OP("Stop MP3",stopMP3,enterEvent)
  ,SUBMENU(setType)
  ,SUBMENU(fanToggle)
  ,EXIT("<Back")
)

void clockStart() {
  model.scheduler.timer.restart();
  clocksystem.init();
  //mainMenu[4].enabled=disabledStatus;
  model.nav->idleOn();
  
  fanCtrl=HIGH; 
  digitalWrite( FAN_PIN, fanCtrl);
  
  //print EndClock
  out[0].setCursor( 13, 0 );
  clocksystem.printAlarm( out[0], clocksystem.clock_time, "" );
  clocksystem.toWork();
}

//Set static Functions for ClockScheduler Task
void clockStop() {
  model.scheduler.timer.stop();
  model.scheduler.player.stop();
  //mainMenu[4].enabled=enabledStatus;

  clocksystem.toIdle();
  fanCtrl=LOW; 
  digitalWrite( FAN_PIN, fanCtrl);
}

result restartClock(eventMask e,navNode& nav,prompt& item) {
  model.scheduler.timer.restart();
  return proceed;
}

result addTime(eventMask e,navNode& nav,prompt& item) {
  model.scheduler.timer.add(addSeconds*1000);
  return proceed;
}

//menu with list of alarms
MENU(mainMenu,"Main Menu",doNothing,noEvent,wrapStyle
  ,EXIT("Info")
  ,FIELD(clocksystem.clock_time.s,"Set Seconds","",0,59,5,1,doNothing,exitEvent,wrapStyle)
  ,FIELD(clocksystem.clock_time.m,"Set Minutes","",0,59,1,0,doNothing,exitEvent,wrapStyle)
  ,FIELD(clocksystem.clock_time.h,"Set Hours","",0,23,1,0,doNothing,exitEvent,wrapStyle)
  ,OP("START",clockStart,enterEvent)
  ,OP("STOP",clockStop,enterEvent)
  ,SUBMENU(settingsMenu)
  ,SUBMENU(testMenu)
);

NAVROOT(nav,mainMenu,MAX_DEPTH,in,out);//the navigation root object
