
//Typewriter keyboard
//matthew smith

#include <Keyboard.h>
#include "defines.h"



void setup() {
  // put your setup code here, to run once:
  pinMode(dpin_0, OUTPUT);
  pinMode(dpin_1, OUTPUT);
  pinMode(dpin_2, OUTPUT);
  pinMode(dpin_3, OUTPUT);

  pinMode(tw_0, INPUT);
  pinMode(tw_1, INPUT);
  pinMode(tw_2, INPUT);
  pinMode(tw_3, INPUT);
  pinMode(tw_shift, INPUT);
  //Serial.begin(9600);
  Keyboard.begin();
}

void loop() {
  int input;
  bool shift;
  uint8_t key;
  
  /*keyboard strobe loop*/
  for(uint8_t muxOut = 0; muxOut < 16; muxOut++)
  {
    set_demux(muxOut);
    input = determine_input();
    shift = digitalRead(tw_shift);

    /*hold shift press while shift signal is high*/
    if(shift){
      Keyboard.press(KEY_LEFT_SHIFT);
    }else{
      Keyboard.release(KEY_LEFT_SHIFT);
    }

    
    for(int i = 0; i < 4; i++){
      if(kb_mem[muxOut][i] == true){

        if( i+1 != input){ //if kb mem=high && key not pressed, set low
          kb_mem[muxOut][i] = false;
          Keyboard.release(lower[muxOut][i+1]);
       }
      }else{
          
        if(i+1 == input){ //if kb_mem=low && key is pressed, set high
          kb_mem[muxOut][i] = true;
          Keyboard.press(lower[muxOut][i+1]);
        }
      }
    }  
  }
}

/*
 * translate 4 bit int input into 4 output pins
 * iterate over each pin, set the output via dmux macro
 */
void set_demux(uint8_t out_val)
{
  for(int i = 0; i < 4; i++)
  {
    digitalWrite(dpin[i], dmux(out_val,i));
  }
}

/*
 * Decode 4 pin input using if_xxxx macro and return is_xxxx macro
 * to translate to key matrix table
 */
int determine_input()
{
  int input = 0;
  if(if_0111) input = is_0111;
  else if(if_1011) input = is_1011;
  else if(if_1101) input = is_1101;
  else if(if_1110) input = is_1110;
  else input = 0U;

  return input;
}
