// #include <Arduino.h>
// #include <VescUart.h>

// #define DEBUG 1
// #define SERIAL_BAUD 921600 // debug baudrate

// #define BAUD_RATE_SMARTESC_XIAOMI 115200 // default baudrate of UART on SmartESC

// // pin layout
// #define PIN_SERIAL_ESP_TO_CNTRL 1 // TX -> RX controller
// #define PIN_SERIAL_CNTRL_TO_ESP 0 // RX <- TX controller

// // send orders every 40ms
// #define TIME_SEND 40

// char print_buffer[500];

// // VESC
// VescUart vescCntrl;

// // last order send time
// unsigned long iTimeSend = 0;

// void setup() {
//     Serial.begin(SERIAL_BAUD);
//     Serial.println("M365BLE_TO_VESC v1.0");

//     // hwSerCntrl.begin(BAUD_RATE_SMARTESC_XIAOMI);

//     // while (!hwSerCntrl) { ; }

//     // Define serial port of VESC
//     // vescCntrl.setSerialPort(&hwSerCntrl);
// }

// void loop(void) {
//     unsigned long timeNow = millis();

//     // Avoid delay
//     if (iTimeSend > timeNow)
//         return;
//     iTimeSend = timeNow + TIME_SEND;

//     // The valueY is used to control the speed, where 127 is the middle = no current
//     uint8_t value;
//     value = 127; // no current

//     #if DEBUG
//         Serial.println("nunchuckValue = " + (String)value);
//     #endif

//     // Call the function setNunchuc kValues to send the current nunchuck values to the VESC
//     vescCntrl.nunchuck.valueY = value;
//     vescCntrl.setNunchuckValues();
// }