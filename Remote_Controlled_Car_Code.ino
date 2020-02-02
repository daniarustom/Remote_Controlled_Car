/*
  Example sketch for the PS3 Bluetooth library - developed by Kristian Lauszus
  For more information visit my blog: http://blog.tkjelectronics.dk/ or
  send me an e-mail:  kristianl@tkjelectronics.com
*/

#include <usbhub.h> //This is the library for the bluetooth dongle
#include <PS3BT.h> //This is the library for the PS3 controller

// Satisfy the IDE, which needs to see the include statment in the ino too.
// the following code is necessary for the USB shield, nothing to change here
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb; // Create a USB Hub instance
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb); // Create a bluetooth dongle instance
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

// The following variable determines if you will use the accelerotmeters
// in the controller
bool printAngle;

// The following are the variables for the controller
bool Triangle = 0;
bool Square = 0;
bool Circle = 0;
bool Cross = 0;
bool Up = 0;
bool Down = 0;
bool Left = 0;
bool Right = 0;
bool Left_Trigger_Button_1 = 0;
bool Right_Trigger_Button_1 = 0;
bool Left_Joystick_Button_3 = 0;
bool Right_Joystick_Button_3 = 0;
int Left_X;
int Left_Y;
int Right_X;
int Right_Y;
int Left_Trigger_Analog;
int Right_Trigger_Analog;
int Roll_Angle;
int Pitch_Angle;
int motor1 = 5;
int motor2 = 6;
int red1 = 2;
int red2 = 3;

void setup() {
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(red1, OUTPUT);
  pinMode(red2, OUTPUT);
  Serial.begin(9600);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));
}

void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {//-> This is where the code starts,
    //if there is connectivity the controller
    //starts reading buttons

    // The "Play Station" button disconnects the controller
    if (PS3.getButtonClick(PS)) {
      PS3.disconnect();//This command disconnects the controller
      Serial.print(F("\r\nPS Controller Disconnected"));
    }

    // The select button helps you print the status of the controller
    // (Battery level, rumble on/off, connection-bluetooth, connection
    // to power supply)
    if (PS3.getButtonClick(SELECT)) {
      Serial.print(F("\r\nSelect - "));
      PS3.printStatusString(); // This line prints the controller status
    }

    // Analog button values can be read from almost all buttons
    if (PS3.getAnalogButton(L2) || PS3.getAnalogButton(R2)) {
      Left_Trigger_Analog = PS3.getAnalogButton(L2);
      Right_Trigger_Analog = PS3.getAnalogButton(R2);
      Serial.print(F("\r\nL2: "));
      Serial.print(Left_Trigger_Analog);
      Serial.print(F("\tR2: "));
      Serial.print(Right_Trigger_Analog);
    }

    // You can read the right side buttons (star, and circle and all that)
    if (PS3.getButtonClick(TRIANGLE)) {
      Triangle = !Triangle;
      Serial.print(F("\r\nTraingle = "));
      Serial.print(Triangle);
    }
    if (PS3.getButtonClick(CIRCLE)) {
      Circle = !Circle;
      Serial.print(F("\r\nCircle = "));
      Serial.print(Circle);
    }
    if (PS3.getButtonClick(CROSS)) {
      Cross = !Cross;
      Serial.print(F("\r\nCross = "));
      Serial.print(Cross);
    }
    if (PS3.getButtonClick(SQUARE)) {
      Square = !Square;
      Serial.print(F("\r\nSquare = "));
      Serial.print(Square);
    }

    // You can read the left side arrows
    if (PS3.getButtonClick(UP)) {
      Up = !Up;
      Serial.print(F("\r\nUp = "));
      Serial.print(Up);
    }
    if (PS3.getButtonClick(RIGHT)) {
      Right = !Right;
      Serial.print(F("\r\nRight = "));
      Serial.print(Right);
    }
    if (PS3.getButtonClick(DOWN)) {
      Down = !Down;
      Serial.print(F("\r\nDown = "));
      Serial.print(Down);
    }
    if (PS3.getButtonClick(LEFT)) {
      Left = !Left;
      Serial.print(F("\r\nLeft = "));
      Serial.print(Left);
    }

    // The top trigger buttons can be read
    if (PS3.getButtonClick(L1)) {
      Left_Trigger_Button_1 = !Left_Trigger_Button_1;
      Serial.print(F("\r\nL1 = "));
      Serial.print(Left_Trigger_Button_1);
    }
    if (PS3.getButtonClick(R1)) {
      Right_Trigger_Button_1 = !Right_Trigger_Button_1;
      Serial.print(F("\r\nR1 = "));
      Serial.print(Right_Trigger_Button_1);
    }

    // The joysticks can also serve as buttons
    if (PS3.getButtonClick(L3)) {
      Left_Joystick_Button_3 = !Left_Joystick_Button_3;
      Serial.print(F("\r\nL3 = "));
      Serial.print(Left_Joystick_Button_3);
    }
    if (PS3.getButtonClick(R3)) {
      Right_Joystick_Button_3 = !Right_Joystick_Button_3;
      Serial.print(F("\r\nR3 = "));
      Serial.print(Right_Joystick_Button_3);
    }

    // The start button makes it switch between tilt/button control
    if (PS3.getButtonClick(START)) {
      Serial.print(F("\r\nStart - This toggles on/off for the angles"));
      printAngle = !printAngle;
    }

#if 1 // Set this to 1 in order to see the angle of the controller
    if (printAngle) {
      // First we collect the controller values in variables
      Pitch_Angle = PS3.getAngle(Pitch);
      Roll_Angle = PS3.getAngle(Roll);

      // Then we trim the pitch within our desired values
      //if ((Pitch_Angle >= 90) && (Pitch_Angle <= 270)) {
      //  Pitch_Angle = map(Pitch_Angle, 90, 270, -100, 100);
      //} else {
      //  Pitch_Angle = 0;
      //}

      // Then we trim the roll within our desired values
      //if ((Roll_Angle >= 90) && (Roll_Angle <= 270)) {
      //  Roll_Angle = map(Roll_Angle, 90, 270, -100, 100);
      //} else {
      //  Roll_Angle = 0;
      //}

      // Finally we print the angles
      Serial.print(F("\r\nPitch: "));
      Serial.print(Pitch_Angle);
      Serial.print(F("\r Roll: "));
      Serial.print(Roll_Angle);


      //********************fix this part, only a few variables... right now its reading from the controller over and over
    } else {      // The joysticks are read only read if you are not reading from the accelerometers
      Left_X = PS3.getAnalogHat(LeftHatX);
      Left_Y = PS3.getAnalogHat(LeftHatY);
      Right_X = PS3.getAnalogHat(RightHatX);
      Right_Y = PS3.getAnalogHat(RightHatY);
      if (Left_X > 137 || Left_X < 117 || Left_Y > 137 || Left_Y < 117 ||
          Right_X > 137 || Right_X < 117 || Right_Y > 137 || Right_Y < 117) {
        Serial.print(F("\r\nLeftHatX: "));
        Serial.print(Left_X);
        Serial.print(F("\tLeftHatY: "));
        Serial.print(Left_Y);
        Serial.print(F("\tRightHatX: "));
        Serial.print(Right_X);
        Serial.print(F("\tRightHatY: "));
        Serial.print(Right_Y);
      }

    }
#endif

    /*
      // It is posisble to make the controller rumble
      PS3.setRumbleOn(RumbleLow);
      PS3.setRumbleOn(RumbleHigh);
    */

    /*
      //this is how you control the controller LEDs
      if (PS3.PS3Connected) {
        PS3.setLedOff();
        PS3.setLedOn(LED4);
      }
      if (PS3.PS3Connected) {
        PS3.setLedOff();
        PS3.setLedOn(LED1);
      }
      if (PS3.PS3Connected) {
        PS3.setLedOff();
        PS3.setLedOn(LED2);
      }
      if (PS3.PS3Connected) {
        PS3.setLedOff();
        PS3.setLedOn(LED3);
      }
    */




    //----------------------------------------------------------------------------------------------------------------------------
    // Place code that only happens when there is connectivity here.... that is a good idea in case you lose signal of the controller
    if (Left_Y > 127.5){
      Left_Y = 0;
    } else {
      Left_Y = map(Left_Y, 127.5, 0, 0, 255);
    }

    if (Right_X < 127.5){
      analogWrite(motor2, Left_Y + map(Right_X,127.5,0,0,50));
      analogWrite(motor1, Left_Y - map(Right_X,127.5,0,0,50));
    } else if (Right_X > 127.5){
      analogWrite(motor1, Left_Y + map(Right_X, 127.5, 255, 0, 50));
      analogWrite(motor2, Left_Y - map(Right_X, 127.5, 255, 0, 50));
    } else {
      analogWrite(motor1, Left_Y);
      analogWrite(motor2, Left_Y);
    }

    if (Triangle == HIGH){
      digitalWrite(red1, HIGH);
    } else{
      digitalWrite(red1, LOW);
    }

    if (Square == HIGH){
      digitalWrite(red2, HIGH);
    } else {
      digitalWrite(red2, LOW);
    }
    
    /* The following are the variable names for reference
            bool Triangle=0;
            bool Square=0;
            bool Circle=0;
            bool Cross=0;
            bool Up=0;
            bool Down=0;
            bool Left=0;
            bool Right=0;
            bool Left_Trigger_Button_1=0;
            bool Right_Trigger_Button_1=0;
            bool Left_Joystick_Button_3=0;
            bool Right_Joystick_Button_3=0;
            int Left_X;
            int Left_Y;
            int Right_X;
            int Right_Y;
            int Left_Trigger_Analog;
            int Right_Trigger_Analog;
            int Roll_Angle;
            int Pitch_Angle;
    */
    //----------------------------------------------------------------------------------------------------------------------------
  }//<- this loop closes the connectivity loop... stuff outside of this loop will happen even if the controller is not connected
  





  //***************************************************************************************************************************
  // Place code that needs to happen regardless of controller connectivity inside of here... turning off the motors i.e. !!!!!
  //***************************************************************************************************************************
}
