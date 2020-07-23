

/*dmux bit macros*/
//important one
#define dmux(x, n) ((x & (1<<n)) >> n) 
//these just elaborate on the generic one above
#define dmux_0(x) x & 0b0001
#define dmux_1(x) (x & 0b0010) >> 1
#define dmux_2(x) (x & 0b0100) >> 2
#define dmux_3(x) (x & 0b1000) >> 3

/*----------These pin definitions are for arduino pro-micro-------------*/
/*dmux pin macros*/
#define dpin_0 10
#define dpin_1 16
#define dpin_2 14
#define dpin_3 15

/*typewriter input pin macros*/
#define tw_0 6
#define tw_1 5
#define tw_2 4
#define tw_3 3
#define tw_shift 2

/*typewriter output code*/
#define if_0111  !digitalRead(tw_3) && digitalRead(tw_2)  && digitalRead(tw_1)  && digitalRead(tw_0)
#define if_1011  digitalRead(tw_3)  && !digitalRead(tw_2) && digitalRead(tw_1)  && digitalRead(tw_0)
#define if_1101  digitalRead(tw_3)  && digitalRead(tw_2)  && !digitalRead(tw_1) && digitalRead(tw_0)
#define if_1110  digitalRead(tw_3)  && digitalRead(tw_2)  && digitalRead(tw_1)  && !digitalRead(tw_0)
/*multi-key*/

//still zero indexed for no-op press
#define is_0111 1
#define is_1011 2
#define is_1101 3
#define is_1110 4
//0011 5 1&2
//0101 6 1&3
//0110 7 1&4
//1001 8 2&3
//1010 9 2&4
//1100 10 3&4

uint8_t input_table[16] = { 0,0,0,5,0,6,7,1,0,8,9,2,10,3,4,0};

uint8_t combo_table[6][2] = {{1,2}, {1,3}, {1,4}, {2,3}, {2,4}, {3,4}};
/*
 * brain storm
 * when enum < =4, use single table
 * when greater, point to table of key combos and press both
 */

int octave_shift;
int velocity = 64;
/*---------------------------------data structures------------------------------*/

uint8_t dpin[4] = {dpin_0, dpin_1, dpin_2, dpin_3};

uint8_t lower [16][5] = {{0U, 65, 76, 72, 74},
                         {0U, 74, 70, 71, 73},
                         {0U, 0U, 0, 0U, 0U},
                         {0U, 0U, 0U, 0U, 0U},
                         {0U, 78, 0U, 75, 73},
                         {0U, 85 ,0U, 82, 80},
                         {0U, 62, 79, 0U, 87},
                         {0U, 75, 0U, 0U, 72},
                         {0U, 88, 1, 60, 0U},
                         {0U, 69, 66, 64, 68},
                         {0U, 0U, 76, 61, 77},
                         {0U, 67, 83, 63, 84},
                         {0U, 0U, 0U, 81, 86},
                         {0U,  3, 0U, 0U, 0U},
                         {0U, 0U, 0U,  1,  0},
                         {0U, 0U, 0U,  2, 0U}};
/*
uint8_t lower [16][5] = {{0U, pitchE4, 0U, pitchB4, pitchC5},
                         {0U, 36, pitchA4b, pitchA4, 0U },
                         {0U,  0U, pitchE4b, 0U,  0U},
                         {0U,  0U, 0U,  0U,0U},
                         {0U, 47, 40,38,39 },
                         {0U, 0U ,0U, 0U, 48},
                         {0U, pitchC4, 47, 0U, 0U},
                         {0U, 0U, 0U, 0U, 35},
                         {0U, 0U, 71, pitchB3, 0U},
                         {0U, pitchG4, 0U, pitchD4, pitchG4b},
                         {0U, 0U, 42, 0U, 44},
                         {0U, pitchF4, 41, pitchD4b, 43},
                         {0U, 0U, 0U, 45, 0U},
                         {0U, 0U, 1, 0U, 0U},
                         {0U, 0U, 0U, 0U, 0U },
                         {0U, 0U, 0U, 0U, 0U}};
 */                        
/*key matrix*/
/*uint8_t lower [16][5] = {{0U, 'v', '/', ',', '.'},
                         {0U, 'w', 'j', 'm', 'l' },
                         {0U,  0U, 'f', 0U,  KEY_LEFT_GUI},
                         {0U,  '1', ']',  '=',0U},
                         {0U, '5', '4','3','2' },
                         {0U, '9' ,'8', '7', '6'},
                         {0U, 'x', 't', KEY_HOME, '0'},
                         {0U, ';', '[', '-', 'q'},
                         {0U, 'p', 'k', 'z', '\''},
                         {0U, 'n', 'g', 'c', 'h'},
                         {0U, 'a', 'e', 's', 'r'},
                         {0U, 'b', 'u', 'd', 'i'},
                         {0U, ' ', KEY_RETURN, 'y', 'o'},
                         {0U, KEY_RIGHT_ARROW, KEY_TAB, KEY_ESC, KEY_DELETE},
                         {0U, KEY_LEFT_ALT, KEY_LEFT_CTRL, KEY_UP_ARROW, KEY_LEFT_ARROW },
                         {0U, KEY_BACKSPACE, '`', KEY_DOWN_ARROW, KEY_END}};

*/
/*
uint8_t upper [16][5] = {{0U, 'V', '?', '<', '>'},
                         {0U, 'W', 'J', 'M', 'L' },
                         {0U, 0U, 'F', 0U,KEY_LEFT_GUI },
                         {0U, 0U,'}','+','!'},
                         {0U, '%', '$','#', '@' },
                         {0U, '(' , '*', '&', '^'},
                         {0U, 'X', 'T', KEY_END, ')'},
                         {0U, ':', '{', '_', 'Q'},
                         {0U, 'P', 'K', 'Z', 0U},
                         {0U, 'N', 'G', 'C', 'H'},
                         {0U, 'A', 'E', 'S', 'R'},
                         {0U, 'B', 'U', 'D', 'I'},
                         {0U, ' ', KEY_RETURN, 'Y', 'O'},
                         {0U, KEY_RIGHT_ARROW, KEY_TAB, KEY_ESC, KEY_BACKSPACE},
                         {0U, KEY_LEFT_ALT, KEY_LEFT_CTRL, KEY_UP_ARROW, KEY_LEFT_ARROW },
                         {0U, KEY_DELETE, '~', KEY_DOWN_ARROW, KEY_END}};
*/
bool kb_mem [16][4] = {{false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false}};
