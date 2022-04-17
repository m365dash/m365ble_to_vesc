#include <Arduino.h>
#include <SoftwareSerial.h>
// #include <WatchDog.h>

#include "ninebot.c"
#include "m365_register_map.h"

// M365 BLE serial: RX = digital pin 8, TX = digital pin 9
SoftwareSerial portBle(8, 9);

// VESC serial: RX = digital pin 10, TX = digital pin 11
SoftwareSerial portVesc(10, 11);

NinebotPack frame;

ToDash m365_to_display = {.start1=NinebotHeader0, .start2=NinebotHeader1, .len=8, .addr=0x21, .cmd=0x64, .arg=0, .mode=M365_MODE_SPORT};

static uint8_t adc1;
static uint8_t adc2;

void setup() {
    Serial.begin(9600);

    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    portBle.begin(115200); // M365 BLE on 115200 BAUD RATE
    portVesc.begin(19200); // VESC on 19200 BAUD RATE
}

void loop(void) {
    // M365 BLE to VESC

    m365_to_display.speed = 11; // testing speed

    portBle.listen();
    while (portBle.available() > 0) {
        uint8_t data = (uint8_t)portBle.read();
        Serial.print("Data received from controller: " + data);
        if (ninebot_parse(data, &frame) == 0) {
            Serial.print("ninebot packet parsed");

            switch(frame.cmd){
				case 0x64: // request of controller data
					addCRC((uint8_t*)&m365_to_display, m365_to_display.len+6);
                    portBle.write((uint8_t*)&m365_to_display, sizeof(m365_to_display));
				    break;
				case 0x65: // Throttle and brake data
					adc1 = frame.payload[1];
					adc2 = frame.payload[2];
                    Serial.println(adc1);
                    Serial.println(adc2);
					// VescToSTM_timeout_reset();
					// app_check_timer();
				    break;
			}
        }
    }

    

    // VESC data to M365 BLE

    

}
