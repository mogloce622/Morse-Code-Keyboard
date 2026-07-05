#include "DigiKeyboard.h"

#define BIT_PIEZO 1
#define BIT_KEY   2

const unsigned char dashlength  = 9;
const unsigned char spacelength = 45;

static unsigned char symbolBuffer[16];
static unsigned char *symbol;
static char keyToType = 0;

static unsigned int longPressCounter = 0;
static bool specialMode = false;

void setup() {
    pinMode(BIT_PIEZO, OUTPUT);
    pinMode(BIT_KEY, INPUT_PULLUP);
    digitalWrite(BIT_PIEZO, LOW);

    symbol = &symbolBuffer[sizeof(symbolBuffer)];
    while (symbol != &symbolBuffer[0]) {
        *--symbol = 0;
    }
}

void typechar(char key) {
    keyToType = key;
}

void morsePoll() {
    static unsigned char up = 0;
    static unsigned char down = 0;
    static bool dashStarted = false;

    if (digitalRead(BIT_KEY) == LOW) {
        digitalWrite(BIT_PIEZO, HIGH);
        down++;
        longPressCounter++;
        up = 0;

        if (specialMode) {
            if (down >= dashlength) {
                dashStarted = true;
                typechar(45);
            }
        }
    } 
    else {
        digitalWrite(BIT_PIEZO, LOW);

        if (longPressCounter > 625) {
            specialMode = !specialMode;
            down = 0;
            dashStarted = false;
            longPressCounter = 0;
            return;
        }

        longPressCounter = 0;

        if (specialMode) {
            if (down > 0 && !dashStarted) {
                typechar(55);
            }
            down = 0;
            dashStarted = false;
            return;
        }

        if (down > 0) {
            if (down >= spacelength) {
                *symbol = 0;
                while (symbol != &symbolBuffer[0]) { *--symbol = 0; }
            } else if (down < dashlength) {
                *symbol++ = '.';
            } else {
                *symbol++ = '-';
            }
            down = 0;
        }

        if (up++ == 255) up = 255;

        if (up == 1 + dashlength) {
            if (symbolBuffer[0] == '.') {
                if (symbolBuffer[1] == '.') {
                    if (symbolBuffer[2] == '.') {
                        if (symbolBuffer[3] == '.') {
                            if (symbolBuffer[4] == '.') {
                                if (symbolBuffer[5] == 0) typechar(KEY_5);
                            } else if (symbolBuffer[4] == '-') {
                                if (symbolBuffer[5] == 0) typechar(46);
                            } else { typechar(KEY_H); }
                        } else if (symbolBuffer[3] == '-') {
                            if (symbolBuffer[4] == '-') {
                                if (symbolBuffer[5] == 0) typechar(KEY_3);
                            } else { typechar(KEY_V); }
                        } else { typechar(KEY_S); }
                    } else if (symbolBuffer[2] == '-') {
                        if (symbolBuffer[3] == '.') {
                            if (symbolBuffer[4] == 0) typechar(KEY_F);
                        } else if (symbolBuffer[3] == '-') {
                            if (symbolBuffer[4] == '-' && symbolBuffer[5] == 0) typechar(KEY_2);
                        } else { typechar(KEY_U); }
                    } else { typechar(KEY_I); }
                } else if (symbolBuffer[1] == '-') {
                    if (symbolBuffer[2] == '.') {
                        if (symbolBuffer[3] == '.') {
                            if (symbolBuffer[4] == 0) typechar(KEY_L);
                        } else if (symbolBuffer[3] == '-') {
                            if (symbolBuffer[4] == '.' && symbolBuffer[5] == '-' && symbolBuffer[6] == 0) typechar(45);
                        } else { typechar(KEY_R); }
                    } else if (symbolBuffer[2] == '-') {
                        if (symbolBuffer[3] == '.') {
                            if (symbolBuffer[4] == 0) typechar(KEY_P);
                        } else if (symbolBuffer[3] == '-') {
                            if (symbolBuffer[4] == 0) typechar(KEY_J);
                            else if (symbolBuffer[4] == '-' && symbolBuffer[5] == 0) typechar(KEY_1);
                        } else { typechar(KEY_W); }
                    } else { typechar(KEY_A); }
                } else { typechar(KEY_E); }
            } else if (symbolBuffer[0] == '-') {
                if (symbolBuffer[1] == '.') {
                    if (symbolBuffer[2] == '.') {
                        if (symbolBuffer[3] == '.') {
                            if (symbolBuffer[4] == '.') {
                                if (symbolBuffer[5] == 0) typechar(KEY_6);
                            } else if (symbolBuffer[4] == '-') {
                                if (symbolBuffer[5] == 0) typechar(46);
                            } else { typechar(KEY_B); }
                        } else if (symbolBuffer[3] == '-') {
                            if (symbolBuffer[4] == '.') {
                                if (symbolBuffer[5] == 0) typechar(56);
                            } else { typechar(KEY_X); }
                        } else { typechar(KEY_D); }
                    } else if (symbolBuffer[2] == '-') {
                        if (symbolBuffer[3] == '.') {
                            if (symbolBuffer[4] == 0) typechar(KEY_C);
                        } else if (symbolBuffer[3] == '-') {
                            if (symbolBuffer[4] == 0) typechar(KEY_Y);
                        } else { typechar(KEY_K); }
                    } else { typechar(KEY_N); }
                } else if (symbolBuffer[1] == '-') {
                    if (symbolBuffer[2] == '.') {
                        if (symbolBuffer[3] == '.') {
                            if (symbolBuffer[4] == 0) typechar(KEY_Z);
                            else if (symbolBuffer[4] == '.' && symbolBuffer[5] == 0) typechar(KEY_7);
                            else if (symbolBuffer[4] == '-' && symbolBuffer[5] == '-' && symbolBuffer[6] == 0) typechar(54);
                        } else if (symbolBuffer[3] == '-') {
                            if (symbolBuffer[4] == 0) typechar(KEY_Q);
                        } else { typechar(KEY_G); }
                    } else if (symbolBuffer[2] == '-') {
                        if (symbolBuffer[3] == '.') {
                            if (symbolBuffer[4] == '.' && symbolBuffer[5] == 0) typechar(KEY_8);
                        } else if (symbolBuffer[3] == '-') {
                            if (symbolBuffer[4] == '-' && symbolBuffer[5] == 0) typechar(KEY_0);
                            else if (symbolBuffer[4] == '.' && symbolBuffer[5] == 0) typechar(KEY_9);
                        } else { typechar(KEY_O); }
                    } else { typechar(KEY_M); }
                } else { typechar(KEY_T); }
            }

            *symbol = 0;
            while (symbol != &symbolBuffer[0]) { *--symbol = 0; }
        }

        if (up == spacelength) {
            typechar(KEY_SPACE);
        }
    }
}

void loop() {
    DigiKeyboard.sendKeyStroke(0);
    morsePoll();
    
    if (keyToType != 0) {
        DigiKeyboard.sendKeyStroke(keyToType);
        keyToType = 0;
    }
    
    DigiKeyboard.delay(16);
}
