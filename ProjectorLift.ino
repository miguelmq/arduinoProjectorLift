/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

#define pinRF 10
#define IRMP_INPUT_PIN 3
#define IRMP_PROTOCOL_NAMES 1
#include <irmpSelectMain15Protocols.h>
#include <irmp.c.h>
IRMP_DATA irmp_data;

// Choose the IR protocol of your remote. See the other example for this.
//CNec IRLremote;
//CPanasonic IRLremote;
//CHashIR IRLremote;
//#define IRLremote Sony12

#define pinLed LED_BUILTIN

void setup() {
  // Start serial debug output
  while (!Serial);
  Serial.begin(9600);
  Serial.println(F("Startup"));
  
  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(pinRF);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);

  mySwitch.setRepeatTransmit(100);

  // Optional set pulse length.
  // mySwitch.setPulseLength(320);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);

  // init IR receiver
  irmp_init();
  // Set LED to output
  irmp_irsnd_LEDFeedback(true);
}

void loop() {
  // Check if we are currently receiving data
  //if (!IRLremote.receiving()) {
    // Run code that disables interrupts, such as some led strips
  //}

  // Check if new IR protocol data is available
  if (irmp_get_data(&irmp_data)){
    /*
     * Skip repetitions of command
     */
    if (!(irmp_data.flags & IRMP_FLAG_REPETITION))
    {
      /*
       * Here data is available and is no repetition -> evaluate IR command
       */
      switch (irmp_data.command)
      {
        case 0x1AA:
          digitalWrite(LED_BUILTIN, HIGH); // will be set to low by IR feedback / irmp_LEDFeedback()
          Serial.println("Lift Down");
          mySwitch.send(12041060, 24);
          delay(4000);
          break;
        case 0x1A6:
          digitalWrite(LED_BUILTIN, HIGH); // will be set to low by IR feedback / irmp_LEDFeedback()
          Serial.println("Lift Up");
          mySwitch.send(12041058, 24);
          delay(4000);
          break;
        default:
          break;
      }
      irmp_result_print(&irmp_data);
    }
  }
}
