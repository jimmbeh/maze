// Include Arduino standard library to access standard functions
#include <Arduino.h>
// Include EEPROM to access it internal functions
#include <EEPROM.h>

// Reset memory in EEPROM
void memoryReset(){
    for(int i = 0; i < EEPROM.length(); i++){
        EEPROM.write(i, 0);
    }
}

// Write memory to EEPROM
void memoryWrite(String input){
    for(int i = 0; i < int(input.length()); i++){
        EEPROM.write(i, input[i]);
    }
}

// Read and Return all value from EEPROM
String memoryRead(){
    String buffer = "";
    for(int i = 0; i < EEPROM.length(); i++){
        if(EEPROM.read(i) != 0){
            buffer += char(EEPROM.read(i));
        }else{
            break;
        }
    }
    return buffer;
}

// How to use?
// memoryReset(); to reset the existing memory
// memoryWrite(input message); to store value in EEPROM
// memoryRead(); to reset values from existing memory