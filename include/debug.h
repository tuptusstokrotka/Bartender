#define SERIAL_ENABLE

#ifdef SERIAL_ENABLE
    #define DEBUG_SERIAL        Serial.begin(115200);
    #define DEBUG_PRINT(x)      Serial.print(x)
    #define DEBUG_PRINTLN(x)    Serial.println(x)
#else
    #define DEBUG_SERIAL        ;
    #define DEBUG_PRINT(x)      ;
    #define DEBUG_PRINTLN(x)    ;
#endif
