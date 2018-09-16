

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


#define is_0111 1
#define is_1011 2
#define is_1101 3
#define is_1110 4

/*---------------------------------data structures------------------------------*/

uint8_t dpin[4] = {dpin_0, dpin_1, dpin_2, dpin_3};
/*key matrix*/
uint8_t lower [16][5] = {{0U, 'v', '/', ',', '.'},
                         {0U, 'w', 'j', 'm', 'l' },
                         {0U,  0U, 'f', 0U,  KEY_LEFT_ALT},
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
                         {0U, KEY_LEFT_GUI, KEY_LEFT_CTRL, KEY_UP_ARROW, KEY_LEFT_ARROW },
                         {0U, KEY_BACKSPACE, '`', KEY_DOWN_ARROW, KEY_END}};



uint8_t upper [16][5] = {{0U, 'V', '?', '<', '>'},
                         {0U, 'W', 'J', 'M', 'L' },
                         {0U, 0U, 'F', 0U,KEY_LEFT_ALT },
                         {0U, 0U,'}','+','!'},
                         {0U, '%', '$','#', '@' },
                         {0U, '(' , '*', '&', '^'},
                         {0U, 'X', 'T', KEY_HOME, ')'},
                         {0U, ':', '{', '_', 'Q'},
                         {0U, 'P', 'K', 'Z', 0U},
                         {0U, 'N', 'G', 'C', 'H'},
                         {0U, 'A', 'E', 'S', 'R'},
                         {0U, 'B', 'U', 'D', 'I'},
                         {0U, ' ', KEY_RETURN, 'Y', 'O'},
                         {0U, KEY_RIGHT_ARROW, KEY_TAB, KEY_ESC, KEY_BACKSPACE},
                         {0U, KEY_LEFT_GUI, KEY_LEFT_CTRL, KEY_UP_ARROW, KEY_LEFT_ARROW },
                         {0U, KEY_DELETE, '~', KEY_DOWN_ARROW, KEY_END}};

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
