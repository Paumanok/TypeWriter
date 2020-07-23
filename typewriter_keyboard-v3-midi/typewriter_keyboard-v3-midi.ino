#include <frequencyToNote.h>
#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>

//Typewriter keyboard
//matthew smith

//#include <Keyboard.h>
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
  //Keyboard.begin();
  octave_shift = 4;
  
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
    /*possibly use as chord mod, but disable for now
    if(shift){
      Keyboard.press(KEY_LEFT_SHIFT);
    }else{
      Keyboard.release(KEY_LEFT_SHIFT);
    }*/
    handle_press(input, muxOut);
    delay(3);
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
      uint8_t key = lower[muxOut][i+1];
      Serial.println(key);
      
        
 
        
      
        if(kb_mem[muxOut][i] == true){
          
          if( i+1 != input){ //if kb mem=high && key not pressed, set low
            kb_mem[muxOut][i] = false;
            //Keyboard.release(lower[muxOut][i+1]);
            if(key >3){
            noteOff(1, decode_note(key),64);
            MidiUSB.flush();
            }
         }
        }else{
            
          if(i+1 == input){ //if kb_mem=low && key is pressed, set high
            kb_mem[muxOut][i] = true;
            //Keyboard.press(lower[muxOut][i+1]);
            if(key >= 0 && key <=3){
              octave_shifter(key);
            }else{
            noteOn(1, decode_note(key), 64);
            MidiUSB.flush();
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

uint8_t decode_input()
{
  uint8_t input = 0;
  input += (digitalRead(tw_3) == HIGH ? 8 : 0);
  input += (digitalRead(tw_2) == HIGH ? 4 : 0);
  input += (digitalRead(tw_1) == HIGH ? 2 : 0);
  input += (digitalRead(tw_0) == HIGH ? 1 : 0);
  return input_table[input];
}

void octave_shifter(uint8_t key)
{
  switch(key)
        {
          case 0:
            if(octave_shift > 1) octave_shift=octave_shift-1;
            break;
          case 1:
            if(octave_shift < 8) octave_shift++;
            
            break;
          case 2: //velocity keys
            break;
          case 3:
            break;
        }   
}
uint8_t decode_note(uint8_t base_note)
{
  uint8_t final_note;
  if(octave_shift < 4){
    final_note = base_note - ((4-octave_shift)*12);
    }
  else if(octave_shift > 4) {
    final_note = base_note + ((octave_shift-4)*12);
    }
  else{ final_note = base_note;
    } 
  return final_note;
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity){
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
