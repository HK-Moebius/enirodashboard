
// SUPPORTED CARS
#define CAR_KIA_ENIRO_2020_64     0
#define CAR_HYUNDAI_KONA_2020_64  1
#define CAR_HYUNDAI_IONIQ_2018    2
#define CAR_KIA_ENIRO_2020_39     3
#define CAR_HYUNDAI_KONA_2020_39  4
#define CAR_RENAULT_ZOE           5
#define CAR_DEBUG_OBD2_KIA        999

// SCREENS
#define SCREEN_BLANK  0
#define SCREEN_AUTO   1
#define SCREEN_DASH   2
#define SCREEN_SPEED  3
#define SCREEN_CELLS  4
#define SCREEN_CHARGING 5
#define SCREEN_SOC10  6
#define SCREEN_DEBUG  7

// Commands loop
uint16_t commandQueueCount;
uint16_t commandQueueLoopFrom;
String commandQueue[300];
String responseRow;
String responseRowMerged;
uint16_t commandQueueIndex;
bool canSendNextAtCommand = false;
String commandRequest = "";
String currentAtshRequest = "";

// Structure with realtime values
typedef struct {
  time_t currentTime; 
  time_t chargingStartTime; 
  time_t automatickShutdownTimer; 
  bool ignitionOn;
  bool ignitionOnPrevious;
  bool forwardDriveMode;
  bool reverseDriveMode;
  bool parkModeOrNeutral;
  bool headLights;
  bool dayLights;
  bool brakeLights;
  uint8_t lightInfo;
  uint8_t brakeLightInfo;
  uint8_t espState;
  float batteryTotalAvailableKWh; 
  float speedKmh;
  float motorRpm;
  float odoKm;
  float socPerc;
  float socPercPrevious;
  float sohPerc;
  float cumulativeEnergyChargedKWh;
  float cumulativeEnergyChargedKWhStart;
  float cumulativeEnergyDischargedKWh;
  float cumulativeEnergyDischargedKWhStart;
  float availableChargePower; // max regen
  float availableDischargePower; // max power
  float isolationResistanceKOhm;
  float batPowerAmp;
  float batPowerKw;
  float batPowerKwh100;
  float batVoltage;
  float batCellMinV;
  float batCellMaxV;
  float batTempC;
  float batHeaterC;
  float batInletC;
  float batFanStatus;
  float batFanFeedbackHz;
  float batMinC;
  float batMaxC;
  uint16_t batModuleTempCount;
  float batModuleTempC[12];
  float coolingWaterTempC;
  float coolantTemp1C;
  float coolantTemp2C;
  float bmsUnknownTempA;
  float bmsUnknownTempB;
  float bmsUnknownTempC;
  float bmsUnknownTempD;
  float auxPerc;
  float auxCurrentAmp;
  float auxVoltage;
  float indoorTemperature;
  float outdoorTemperature;
  float tireFrontLeftTempC;
  float tireFrontLeftPressureBar;
  float tireFrontRightTempC;
  float tireFrontRightPressureBar;
  float tireRearLeftTempC;
  float tireRearLeftPressureBar;
  float tireRearRightTempC;
  float tireRearRightPressureBar;
  uint16_t cellCount;
  float cellVoltage[98]; // 1..98 has index 0..97
  // Screen - charging graph
  float chargingGraphMinKw[101]; // 0..100% .. Min power Kw
  float chargingGraphMaxKw[101]; // 0..100% .. Max power Kw
  float chargingGraphBatMinTempC[101]; // 0..100% .. Min bat.temp in.C
  float chargingGraphBatMaxTempC[101]; // 0..100% .. Max bat.temp in.C
  float chargingGraphHeaterTempC[101]; // 0..100% .. Heater temp in.C
  float chargingGraphWaterCoolantTempC[101]; // 0..100% .. Heater temp in.C
  // Screen - consumption info
  float soc10ced[11]; // 0..10 (5%, 10%, 20%, 30%, 40%).. (never discharged soc% to 0)
  float soc10cec[11]; // 0..10 (5%, 10%, 20%, 30%, 40%)..
  float soc10odo[11]; // odo history
  time_t soc10time[11]; // time for avg speed
  // additional
  /*
  uint8_t bmsIgnition;
  uint8_t bmsMainRelay;
  uint8_t highVoltageCharging;
  float inverterCapacitorVoltage;
  float normalChargePort;
  float rapidChargePort;
  float operationTimeHours;*/
} PARAMS_STRUC;

// Setting stored to flash
typedef struct {
  byte initFlag; // 183 value
  byte settingsVersion; // current 4
  uint16_t carType; // 0 - Kia eNiro 2020, 1 - Hyundai Kona 2020, 2 - Hyudai Ioniq 2018
  char obdMacAddress[20];
  char serviceUUID[40];
  char charTxUUID[40];
  char charRxUUID[40];
  byte displayRotation; // 0 portrait, 1 landscape, 2.., 3..
  char distanceUnit; // k - kilometers
  char temperatureUnit; // c - celsius
  char pressureUnit; // b - bar
  // version 2
  byte defaultScreen; // 1 .. 6
  byte lcdBrightness; // 0 - auto, 1 .. 100%
  byte debugScreen; // 0 - off, 1 - on
  byte predrawnChargingGraphs; // 0 - off, 1 - on
  byte wifiEnable; // 0 off 1 on used for NTP datetime sync
  char wifiSsid[32]; 
  char wifiPassword[32];
  byte sdcardAutoRecord; // 0 off 1 on
  
} SETTINGS_STRUC;

PARAMS_STRUC params;     // Realtime sensor values
SETTINGS_STRUC settings, tmpSettings; // Settings stored into flash

/**
  Hex to dec (1-2 byte values, signed/unsigned)
  For 4 byte change int to long and add part for signed numbers
*/
float hexToDec(String hexString, byte bytes = 2, bool signedNum = true) {

  unsigned int decValue = 0;
  unsigned int nextInt;

  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;
  }

  // Unsigned - do nothing
  if (!signedNum) {
    return decValue;
  }
  // Signed for 1, 2 bytes
  if (bytes == 1) {
    return (decValue > 127 ? (float)decValue - 256.0 : decValue);
  }
  return (decValue > 32767 ? (float)decValue - 65536.0 : decValue);
}
