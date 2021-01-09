

class Acetino{
  public:

    uint8_t FANPIN;

    //---------------- Time Events- Update ---------------
    long debugstamp = 500;
    long debugtime = 0 ;
    long playtime = 0 ;
    long playstamp = 500;

    int volume = 20;

    clockscheduler &scheduler;
    LiquidCrystal &lcd;
    LcdProgressBar &lpg;
    navRoot *nav;
    
    Acetino( clockscheduler &schedule ):scheduler( schedule ) {
    }

    Acetino( clockscheduler &schedule,LiquidCrystal &liquidcrystal,LcdProgressBar &progressbar ): scheduler( schedule ), lcd( liquidcrystal ), lpg( progressbar ){
    
    }

    void init(){
      pinMode( FANPIN, OUTPUT );
      Serial.begin(9600);
      while(!Serial);
      lcd.begin(20,4);
      lpg.setMinValue(0);
      lpg.setMaxValue(100);
      
      //-------------- TIMER INITIALIZATION --------------
      scheduler.timer.stop();

      //-------------- MP3 INITIALIZATION ----------------
      if(  mp3Serial() ){
        Serial.println("Success");
      }else{
        Serial.println("Fail MP3");
      }
        scheduler.player.volume(volume);
      }

    void update(){

      if( millis() - playtime > playstamp ){
          scheduler.update( );
          playtime = millis();
      }
  
      flush();
    }

    void set_fan_pin ( uint8_t pin ){
      FANPIN = pin;
    }

    bool mp3Serial(){
      return scheduler.player.begin( Serial );
    }

    void flush(){
      if (Serial.available()) {
        Serial.flush();
      }
    }

    void update_screen( menuOut& out ){
      if( millis() - debugtime > debugstamp ){
        scheduler.update_screen( out, lpg );
        debugtime = millis();
      }
    }

    void set_nav( navRoot& navigation ){
      nav = &navigation;
    }

    void stopFan( bool fanvalue){
      digitalWrite( FAN_PIN, fanvalue );
    }

};
