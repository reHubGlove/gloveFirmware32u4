#include <SPI.h>
#include <boards.h>
#include <RBL_nRF8001.h>

byte pitch      = 10;   // 175-10  = 165
byte roll       = 10;   // 175-10  = 165
byte yow        = 10;   // 175-10  = 165

int thumb       = 513;  // 513-300 = 213
int thumbrol    = 513;  // 513-300 = 213
int index       = 500;  // 500-270 = 230
int middle      = 515;  // 515-280 = 235
int ring        = 480;  // 480-210 = 270
int little      = 525;  // 525-295 = 230

int thumbpad    = 513;  // 513-300 = 213
int indexpad    = 500;  // 500-270 = 230
int middlepad   = 515;  // 515-280 = 235
int ringpad     = 480;  // 480-210 = 270
int littlepad   = 525;  // 525-295 = 230

void setup() {
  ble_set_name("LeftGlove");
  ble_begin();
}

void loop() {
  ble_do_events();

  if(ble_connected()){

    pitch      = random(0,165);
    roll       = random(0,165);
    yow        = random(0,165);
    
    thumb      = random(0,230);
    thumbrol   = random(0,230);
    index      = random(0,230);
    middle     = random(0,230);
    ring       = random(0,230);
    little     = random(0,230);

    thumbpad   = random(0,230);
    indexpad   = random(0,230);
    middlepad  = random(0,230);
    ringpad    = random(0,230);
    littlepad  = random(0,230);


    /**************************************************
    
    ble_printInt( pitch    ); ble_write(',');
    ble_printInt( roll     ); ble_write(',');
    ble_printInt( yow      ); ble_write('');

    /**************************************************/
   
    unsigned char myData[15] = {'l',pitch,roll,yow,thumb,thumbrol,index,middle,ring,little,thumbpad,indexpad,middlepad,ringpad,littlepad};
    ble_write_bytes( myData,15 );
    
    /**************************************************/

    pinMode(13, true);
    digitalWrite(13, true);
    delay(100);
    
  }
  else{
    pinMode(13, false);
    digitalWrite(13, false);
  }

}

void ble_print(char *str){
  char i = 0;
  while(char c = str[i]){
    ble_write(str[i]);
    i++;
  }
}

char buf[12];
void ble_printInt(int num){
  ble_print(itoa(num, buf, 10));
}
