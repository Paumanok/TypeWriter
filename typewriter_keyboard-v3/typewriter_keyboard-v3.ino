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
  Serial.begin(9600);
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
    input = decode_input();
    shift = digitalRead(tw_shift);

    /*hold shift press while shift signal is high*/
    if(shift){
      Keyboard.press(KEY_LEFT_SHIFT);
    }else{
      Keyboard.release(KEY_LEFT_SHIFT);
    }

    handle_press(input, muxOut);
  }
}

/*
 * determine if its single press or multi press on channel
 * and dispatch handle_keys
 */
void handle_press(int input, uint8_t muxOut)
{
  if(input <=4)
  {
     handle_keys(input, muxOut);
  }
  else if(input > 4)
  {
     handle_keys(combo_table[input-5][0], muxOut);
     handle_keys(combo_table[input-5][1], muxOut); 
  }
}

/*
 * handle key press and memory check
 */
void handle_keys(int input, uint8_t muxOut)
{
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

uint8_t decode_input()
{
  uint8_t input = 0;
  input += (digitalRead(tw_3) == HIGH ? 8 : 0);
  input += (digitalRead(tw_2) == HIGH ? 4 : 0);
  input += (digitalRead(tw_1) == HIGH ? 2 : 0);
  input += (digitalRead(tw_0) == HIGH ? 1 : 0);
  return input_table[input];
}


