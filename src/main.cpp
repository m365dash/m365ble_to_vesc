#include <Arduino.h>
#include <SoftwareSerial.h>
#include <VescUart.h>
// #include <WatchDog.h>

#include "ninebot.c"
#include "m365_register_map.h"

// M365 BLE serial: RX = digital pin 8, TX = digital pin 9
SoftwareSerial serialBle(8, 9);

int buttonPin = A0;
int pwrPin = A1;
int lightPin = A2;

// VESC serial: RX = digital pin 10, TX = digital pin 11
SoftwareSerial serialVesc(10, 11);
VescUart vescCntrl;

NinebotPack frame;

DashPack dash_pack = {.start1=NinebotHeader0, .start2=NinebotHeader1, .len=8, .addr=0x21, .cmd=0x64, .arg=0, .mode=M365_MODE_SPORT};

static uint8_t adc1;
static uint8_t adc2;

void setup() {
    Serial.begin(9600);

    while (!Serial) {
          ; // wait for serial port to connect. Needed for native USB port only
      }

    Serial.println("Hello USB!");

    serialBle.begin(115200); // M365 BLE on 115200 BAUD RATE
    serialVesc.begin(115200); // VESC on 19200 BAUD RATE

    Serial.println("waiting for vesc");
    while (!serialVesc) { ; }

    Serial.println("Connected VESC");

    vescCntrl.setSerialPort(&serialVesc);
    // vescCntrl.setDebugPort(&Serial); It will cause issues with the Serial output

    pinMode(buttonPin, INPUT);
    pinMode(pwrPin, OUTPUT);
    pinMode(lightPin, OUTPUT);

    delay(1000);
}

bool turned_on;

void loop(void) {
    digitalWrite(pwrPin, HIGH);
    digitalWrite(lightPin, HIGH); // Rear light is always on

    // M365 BLE to VESC

    serialBle.listen();
    
    while (serialBle.available() > 0) {
        uint8_t data = (uint8_t)serialBle.read();
        
        if (ninebot_parse(data, &frame) == 0) {
            switch(frame.cmd){
				case 0x64: // request of controller data
                    // dash_pack.faultcode = random(1, 99);
                    dash_pack.speed = random(0, 99); // testing speed
                    // switch (random(0, 3)) {
                    //     case 0:
                    //         dash_pack.mode = M365_MODE_SLOW;
                    //         break;
                    //     case 1:
                    //         dash_pack.mode = M365_MODE_DRIVE;
                    //         break;
                    //     case 2:
                    //         dash_pack.mode = M365_MODE_SPORT;
                    //         break;
                    //     default:
                    //         break;
                    // }

                    // dash_pack.light = random(0, 2);
                    // dash_pack.battery = random(100);
                    // dash_pack.beep = 0;

					addCRC((uint8_t*)&dash_pack, dash_pack.len+6);
                    serialBle.write((uint8_t*)&dash_pack, sizeof(dash_pack));
				    break;
				case 0x65: // Throttle and brake data
					adc1 = frame.payload[1];
					adc2 = frame.payload[2];
                    Serial.println("adc " + String(adc1, HEX) + " adc2 " + String(adc2, HEX));
				    break;
                default:
                    // todo: implement cmd handling for app connection, not very important, but some might would like to use.

                    Serial.println("unknown cmd: " + String(frame.cmd, HEX));
                    break;
			}
        }
    }

    delay(1);
    // VESC data to M365 BLE


}
