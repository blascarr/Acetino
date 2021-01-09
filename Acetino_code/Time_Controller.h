#include <Chrono.h> 

// Chrono /////////////////////////////////////////
Chrono tempo;
long tick = 1000;
long timeelapsed = tick;
int addSeconds = 10;

//-------Class Clock Schedule
class clockscheduler{
  public:

    struct chrono_time{
      long h,m,s;
    };
    scheduletrack * list;
    int size;

    int current_track=0;
    
    chrono_time clock_time;
    chrono_time current_time;
    chrono_time next_time;

    typedef void ( clockscheduler::*_f_execution )( menuOut& out, LcdProgressBar& lcd_pg );
    _f_execution p_update = &clockscheduler::executePrint ;
    
    Chrono & timer;
    DFRobotDFPlayerMini & player;
    enum {clock,countDown} type=clock;
    
    clockscheduler( DFRobotDFPlayerMini &mp3, Chrono &tempo, scheduletrack * schedulerpointer, int schedulersize ):player(mp3), timer( tempo ), list( schedulerpointer ), size( schedulersize ){
      next_time.s = list[ current_track ].timestamp/1000;
    }

    void init(){
      current_track=0;
      next_time.s = list[ current_track ].timestamp/1000;
    }
    
    void update_screen( menuOut& out, LcdProgressBar& lcd_pg ){
      (this->*p_update)(out, lcd_pg);
    }

    void executePrint( menuOut& out, LcdProgressBar& lcd_pg ){
      // Print Chrono Progress
      printclock( out, 0 );

      //Paint DFPlayer Status
      statemp3( out, 2);
      
      //Paint Progress Bar
      progressbar( out, lcd_pg, 3 );

      //Hay que cambiar si el estado del sistema esta parado que no ejecute nada
      //Update tempo for activation and next track
      if( timer.elapsed() >= list[ current_track ].timestamp ){
        player.play( list[ current_track ].track );
        current_track++;
        //Print only when change
        if( current_track < size ){
          next_time.s = list[ current_track ].timestamp/1000;
          //If clock is extra 60 sec formt hh:mm:ss
          printNext( out, 1 );
        }else{
          
        }
      }else {
        //Print once the first track status
        printNext( out, 1 );
      }
    }
    
    void idle(  menuOut& out, LcdProgressBar& lcd_pg  ){
    
    }

    void toIdle(){
      p_update = &clockscheduler::idle;
    }
    void toWork(){
      p_update = &clockscheduler::executePrint;
    }
    
    void update(  ){
      if( timer.elapsed() >= ( chronoToSeconds( clock_time )*1000 ) ){
        timer.stop();
        digitalWrite( FAN_PIN, false );
        init();
        //Establecer todo al estado inicial
        //mainMenu[4].enabled=enabledStatus;
      }

    }

    void printclock( menuOut& out, int row ){
      out.setCursor( 0,row );
      current_time.s = tempo.elapsed()/1000;
      current_time.h = current_time.s/(60*60);
      current_time.s -= current_time.h*60*60;
      current_time.m = current_time.s/60;
      current_time.s -= current_time.m*60;
      
      printAlarm( out, current_time, "C: " );
    }

    void printNext( menuOut& out , int row ){
      out.setCursor( 0, row );
      printAlarm( out, next_time, "Next: " );
    }

    void statemp3( menuOut& out, int row ){
      
      if (player.available()) {
        out.setCursor( 0, row );
        out.print("                  ");
        out.setCursor( 0, row );
        int state_type = player.readType();
        char* msg;
        switch (state_type) {
          case TimeOut:
            msg = "Time Out!";
          break;
          case WrongStack:
            msg = "Stack Wrong!";
          break;
          case DFPlayerCardInserted:
            msg = "Card Inserted!";
          break;
          case DFPlayerCardRemoved:
            msg = "Card Removed!";
          break;
          case DFPlayerCardOnline:
            msg = "Card Online!";
          break;
          case DFPlayerUSBInserted:
            msg = "USB Inserted!";
          break;
          case DFPlayerUSBRemoved:
            msg = "USB Removed!";
          break;
          case DFPlayerPlayFinished:
            msg = "Song Finished!";
          break;
          case DFPlayerError:
            msg = "ERROR!";
            break;
          default:
            break;
        }
        out.print( msg );
      }
      
    }
    
    void progressbar( menuOut& out, LcdProgressBar& lcd_pg, int row ){
      out.setCursor( 0, row );
      long total_sec = chronoToSeconds( clock_time )*1000;
      int progress_value = ( ( total_sec - tempo.elapsed() )*100/total_sec);
      lcd_pg.draw( 100 - progress_value );
    }
    
    void printAlarm( menuOut& out, chrono_time ct, char* msg ){
      int len = 12;
      switch(type) {
            case clock:
              
              out.print( msg );
              len-=out.print(ct.h);
              len-=out.printRaw(F(":"),len);
              if (ct.m<10) len-=out.print("0");
              len-=out.print(ct.m);
              len-=out.printRaw(F(":"),len);
              if (ct.s<10) len-=out.print("0");
              len-=out.print(ct.s);
              break;
            // Reloj Marcha atras
            case countDown:
              out.print( msg );
              
              if (ct.h<10) len-=out.print("Next ");
              len-=out.print(ct.h);
              len-=out.printRaw(F(":"),len);
              if (ct.m<10) len-=out.print("0");
              len-=out.print(ct.m);
              len-=out.printRaw(F(":"),len);
              if (ct.s<10) len-=out.print("0");
              len-=out.print(ct.s);
              break;
      }
    }

    long chronoToSeconds( chrono_time ct ){
      return ( ct.h*3600+ct.m*60+ct.s );
    }

};
