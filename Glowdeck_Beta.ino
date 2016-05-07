/*------------------------------------------------------------------------------
 --------------------------------------------------------------------------------
 GLOWDECK v0.9 (12/3/14)
 by Justin Kaufman <justin@glowdeck.com>
 --------------------------------------------------------------------------------
 NOTES: Master file. Contains all global variable declarations, library includes,
 hardware initializations (pin modes/states, interrupts, serial ports, modules),
 setup routine, and Glowdeck's primary master loop. NeoPixel library by Adafruit,
 Pixels library by PD4ML, UTFT library by Karl Henning. Others native Arduino.
 --------------------------------------------------------------------------------
 ------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 --------------------------------------------------------------------------------
 1. INCLUDED LIBRARIES
 --------------------------------------------------------------------------------
 ------------------------------------------------------------------------------*/
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Time.h>
#include <UTFT.h>
#include <Pixels_SPIhw.h>
#include <Pixels_Antialiasing.h>
#include <Pixels_ILI9341.h>
#include <FastLED.h>

/*-------------------------------------------------------------------------------
 --------------------------------------------------------------------------------
 2. PIN DEFINITIONS
 --------------------------------------------------------------------------------
 ------------------------------------------------------------------------------*/
//A. LCD (2.4" TFT ILI9341)
#define LCD_CS      20  //CHIP SELECT
#define LCD_SCLK    13  //SERIAL CLOCK
#define LCD_MOSI    11  //MASTER OUT SLAVE IN
#define LCD_DC      2   //REGISTER SELECT
#define LCD_RSET    5   //RESET
#define LCD_BL      6   //BACKLIGHT PWM

//B. RGB LEDS (WS2812B DRIVER)
#define LED_PIN     23  //DATA IN
#define NUM_LEDS    56  //LED COUNT
#define BRIGHTNESS  200 //LED BRIGHTNESS
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

//C. WIRELESS CHARGER (QI-A6 TRANSMITTER)
#define COILS       12  //CHARGING SENSOR
#define COIL_PW     15  //COIL SWITCH

//D. BUTTONS (MOMENTARY SWITCH)
#define PRESS_L     24  //LEFT BUTTON
#define PRESS_R     25  //RIGHT BUTTON
#define PRESS_F     26  //FRONT BUTTON

//E. AMPLIFIER (MAX98400BETG+T)
#define AMP         22  //AMPLIFIER SWITCH
#define AUDIO_L     3   //AUDIO ANALOG-IN LEFT
#define AUDIO_R     0   //AUDIO ANALOG-IN RIGHT

//F. PHOTORESISTOR (GL5528)
#define AMB_LEVEL   13  //AMBIENT LIGHT SENSOR

//G. WIFI MODULE (USR-WIFI232-T | SERIAL1)
#define WIFI_RESTORE 7  //FACTORY RESTORE
#define WIFI_RESET   8  //RESET SWITCH

//H. BLUETOOTH MODULE (BC127 | SERIAL2)
#define PIO0        27  //VOLUME UP
#define PIO1        28  //VOLUME DOWN
#define PIO2        29  //MUSIC PLAY/PAUSE | CALL ANSWER/END
#define PIO4        30  //MUSIC BACKWARD
#define PIO5        31  //MUSIC FORWARD

//I. MICRO-USB DETECT (DFU MODE)
#define USB_IN      16  //DETECT MICRO-USB PLUG

//J. TIME SYNC MESSAGES FROM SERIAL PORT
#define TIME_HEADER "T" //HEADER TAG FOR SERIAL TIME SYNC MESSAGE

/*-------------------------------------------------------------------------------
 ---------------------------------------------------------------------------------
 3. GLOBAL VARIABLES
 ---------------------------------------------------------------------------------
 -------------------------------------------------------------------------------*/
//A. SYSTEM
char udid[7];           //STORES EEPROM BYTES 1-6
int serial_debug = 0;   //ECHO MESSAGES OVER SERIAL PORT
int startup = 1;        //FIRST TIME THROUGH PRIMARY LOOP (0 = NO; 1 = YES)

//B. DISPLAY
int bright = 0;         //BACKLIGHT PWM 0-100 (0 = OFF; 100 = FULL BRIGHTNESS)
int autobright = 0;     //AUTO ADJUST BACKLIGHT (0 = OFF; 1 = ON);

//C. LEDS
CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
int led_sw = 1;           //LED MASTER SWITCH (0 = OFF; 1 = ON)
//int led[57][3];         //COLOR STORAGE ARRAY (led[1-56][0=R/1=G/2=B]=[0-255])
int msync = 0;          //MUSIC SYNCING LEDS (0 = OFF; 1 = ON)

//D. BLUETOOTH
int bluetooth = 1;      //BLUETOOTH MASTER SWITCH (0 = OFF; 1 = ON)
int ble_conn = 0;        //BLE LINK OPEN (0 = NO; 1 = YES)
int btc_conn = 0;        //BTC LINK OPEN (0 = NO; 1 = YES)
String ble_addr = "";        //BLE ADDRESS OF PAIRED DEVICE
String btc_addr = "";         //BT CLASSIC ADDRESS OF PAIRED DEVICE

//E. AUDIO
String song = "";
String artist = "";
String track_status = "";

//E. WIFI
int wifi = 1;           //WIFI MASTER SWITCH (0 = OFF; 1 = ON)
String ssid = "";       //NETWORK SSID
String code = "0";      //SSID PASSWORD
int wiconn = 0;         //0:NO CONNECTION, 1:CONNECTED TO WIFI
String wissid = "";
String wicode = "";
int ssid_count = -1;
String ssid_scan[25][5]; //0: SSID, 1: AUTHENTICATION TYPE, 2: ENCRYPTION METHOD, 3: RSSI, 4: PASSWORD

//F. PRESS
int fpress = 0;
int fhold = 1;
int lpress = 2;
int lhold = 3;
int rpress = 4;
int rhold = 5;

//F. COILS
int coils = 1;          //COIL MASTER SWITCH 0/1 (0 = OFF; 1 = ON)
int smart = 0;          //SMART CHARGING 0/1

//G. AUDIO
int volume = -1;         //AUDIO Vs
int level = -1;
//int volume = 5;         //BC127 AUDIO LEVEL (0 = MUTE; 14 = MAX LOUDNESS)
//int level = -1;         //LOCAL GLOWDECK AUDIO LEVEL (-1 = NO BT AUDIO SOURCE CONNECTED; 0 = MUTE; 14 = MAX LOUDNESS)

//H. PRESS SYSTEM
volatile boolean flagFront = 0; volatile boolean flagLeft = 0; volatile boolean flagRight = 0;
volatile unsigned long last_micros_f; volatile unsigned long last_micros_l; volatile unsigned long last_micros_r;
uint8_t f_settle = 0; uint8_t l_settle = 0; uint8_t r_settle = 0;
uint8_t frontVal = 0; uint8_t leftVal = 0; uint8_t rightVal = 0;

//I. AMBIENT LIGHT SENSOR
int ambient = 50;

//J. CLOCK
int clock_set = 0;
char* daynames[]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
int hh,mm,ss,dow,dd,mon,yyyy;
String ampm = "PM";
int days = 0;
int date = 0;
int months = 0;
int years = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;
int hoursLast = 0;
int minutesLast = 0;
int monthsLast = 0;
int dateLast = 0;

//K. GUI ASSETS
extern prog_uchar glowdeck_icons20a[5598] PROGMEM;
extern prog_uchar AvenirLTStd35Light16a[6055] PROGMEM;
extern prog_uchar AvenirNextUltraLight65a[21861] PROGMEM;
extern prog_uchar OpenSansLight14ba[5723] PROGMEM;
extern prog_uchar OpenSansLight14a[6094] PROGMEM;

//L. STREAMS
int streams = 0;        //STREAMS MASTER SWITCH (0 = OFF; 1 = ON)
int privacy = 0;        //PRIVACY MODE (0 = NONE; 1 = PROXIMITY; 2 = CHARGE)
int scrolls = 0;        //SCROLL STREAMS (0 = OFF; 1 = ON)
int mstream = 1;
int night = 0;          //NIGHT MODE (0 = OFF; 1 = ON)
String streams_id = "";
String streams_tk = "";
String bt_name = "";

//M. TIMERS
unsigned long waitBegin = millis(); unsigned long waitMax = 5000;
unsigned long timerStart; unsigned long timerMax = 10000;

//N. BUFFERS
uint8_t buffer1[150]; 
uint8_t buffer2[150];
uint8_t loop_count = 0;

//O. SERIAL COMM SWITCH
uint8_t serial_tx = 2;   //2 FOR SENDING SERIAL COMMANDS TO BC127; 1 FOR SENDING SERIAL COMMANDS TO USR

/*-------------------------------------------------------------------------------
 ---------------------------------------------------------------------------------
 5. INITIALIZE HARDWARE
 ---------------------------------------------------------------------------------
 -------------------------------------------------------------------------------*/
//A. LCD
UTFT utft(ILI9341_S5P, LCD_MOSI, LCD_SCLK, LCD_CS, LCD_RSET, LCD_DC); Pixels pxs(320, 240);

/*-------------------------------------------------------------------------------
 ---------------------------------------------------------------------------------
 6. SETUP ROUTINE
 ---------------------------------------------------------------------------------
 -------------------------------------------------------------------------------*/
void setup()
{
  /*-------------------------------------------------------------------------------
  ---------------------------------------------------------------------------------
  7. SETUP ROUTINE
  ---------------------------------------------------------------------------------
  ------------------------------------------------------------------------------*/
  //A. MICRO-USB
  init_usb();
  
  //B. STATIC MEMORY
  init_eeprom();
  
  //A. RTC
  init_clock();
  
  //C. LCD BACKLIGHT DIMMER
  init_lcd();

  //B. COILS SWITCH/MONITOR
  init_coils();
  
  //D. BUTTONS
  init_press();

  //A. AUDIO AMPLIFIER SWITCH
  init_audio();

  //E. BLUETOOTH AUDIO GPIOS
  init_bc127();

  //H. INITIALIZE SERIAL MONITORS
  init_serial();

  //G. LCD SPLASH
  lcd_splash(); //!!!!MAKE THIS DISPLAY SPLASH SCREEN
  
  //K. PLSCO TONE
  play_tone(2);
  
  //L. BTC DISCOVERABLE
  btc_discoverable();
  
  //F. LEDS
  init_leds();
  
  //L. WIFI
  init_usr();
  
  //M. BLE ENABLE FOR IPHONE
  ble_advertise(); delay(100);
  
  //I. ATTACH INTERRUPTS
  attachInterrupt(PRESS_F, ISR_F, FALLING); attachInterrupt(PRESS_L, ISR_L, FALLING); attachInterrupt(PRESS_R, ISR_R, FALLING);
  
  //L. BTC DISCOVERABLE
  btc_discoverable();
  
  debug("Glowdeck v1.2 - Initialized", 0);
}

/*-------------------------------------------------------------------------------
 --------------------------------------------------------------------------------
 8. PRIMARY LOOP
 --------------------------------------------------------------------------------
 ------------------------------------------------------------------------------*/
void loop()
{
  serial_stream();
  glowdeck_sync();
}

/*-------------------------------------------------------------------------------
 --------------------------------------------------------------------------------
 9. FUNCTIONS
 --------------------------------------------------------------------------------
 ------------------------------------------------------------------------------*/
void glowdeck_sync()
{
  if (((loop_count++)%3) == 0)
  { 
    update_clock();
    //update_audio();
  }
}


