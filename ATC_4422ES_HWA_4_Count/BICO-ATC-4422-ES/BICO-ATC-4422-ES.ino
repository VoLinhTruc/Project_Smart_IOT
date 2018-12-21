#include "Arduino_Slave_RTU_Serial.h"
#include "Bico_Check_Bit_Change.h"

#define DI0 5
#define DI1 4
#define DI2 3
#define DI3 2

#define DO0 9 // A0
#define DO1 8 // A1
#define DO2 7 // A2
#define DO3 6 // A3

//#define AI0 A6
//#define AI1 A7
//
//#define AO0 10
//#define AO1 9

#define ESP_RESET 13

Arduino_Slave_RTU_Serial modbus;

#define ESP_POLLING_TIMEOUT 5000
long last_esp_polling = 0;

void setup() {
  // put your setup code here, to run once:
  modbus.begin(9600);
  IOSetup();
  pinMode(ESP_RESET, OUTPUT);
  digitalWrite(ESP_RESET, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  modbus.fullHanlde();
  IOUpdate();
//  updateAnalogIn();

  if(millis() - last_esp_polling > ESP_POLLING_TIMEOUT)
  {
    digitalWrite(ESP_RESET, LOW);
    delay(10);
    digitalWrite(ESP_RESET, HIGH);

    last_esp_polling = millis();
  }


  #define INPUT_0_BIT getBitFromArray(slave_discrete_input_contact_list, 0)
  #define INPUT_1_BIT getBitFromArray(slave_discrete_input_contact_list, 1)
  #define INPUT_2_BIT getBitFromArray(slave_discrete_input_contact_list, 2)
  #define INPUT_3_BIT getBitFromArray(slave_discrete_input_contact_list, 3)

  #define INPUT_0_BIT_IN_HOLDING_REG slave_analog_output_holding_register_list[1]
  #define INPUT_1_BIT_IN_HOLDING_REG slave_analog_output_holding_register_list[2]
  #define INPUT_2_BIT_IN_HOLDING_REG slave_analog_output_holding_register_list[3]
  #define INPUT_3_BIT_IN_HOLDING_REG slave_analog_output_holding_register_list[4]

  #define RUN_TIME_0 slave_analog_output_holding_register_list[5]
  #define RUN_TIME_1 slave_analog_output_holding_register_list[6]
  #define RUN_TIME_2 slave_analog_output_holding_register_list[7]
  #define RUN_TIME_3 slave_analog_output_holding_register_list[8]
  
  #define COUNTING_VALUE_0 slave_analog_output_holding_register_list[9]
  #define COUNTING_VALUE_1 slave_analog_output_holding_register_list[10]
  #define COUNTING_VALUE_2 slave_analog_output_holding_register_list[11]
  #define COUNTING_VALUE_3 slave_analog_output_holding_register_list[12]

  
  static uint32_t last_active_0;
  static Bico_Check_Bit_Change input_0_bit(INPUT_0_BIT);
  uint8_t changing_type_0 = input_0_bit.isChanging(INPUT_0_BIT);
  if(changing_type_0 == RISING)
  {
    delay(50);
    last_active_0 = millis();
    COUNTING_VALUE_0++;
  }
  if(INPUT_0_BIT == HIGH)
  {
    RUN_TIME_0 = millis() - last_active_0;
  }
  if(changing_type_0 == FALLING)
  {
    delay(50);
  }
  INPUT_0_BIT_IN_HOLDING_REG = (register_t)INPUT_0_BIT;


  static uint32_t last_active_1;
  static Bico_Check_Bit_Change input_1_bit(INPUT_1_BIT);
  uint8_t changing_type_1 = input_1_bit.isChanging(INPUT_1_BIT);
  if(changing_type_1 == RISING)
  {
    delay(50);
    last_active_1 = millis();
    COUNTING_VALUE_1++;
  }
  if(INPUT_1_BIT == HIGH)
  {
    RUN_TIME_1 = millis() - last_active_1;
  }
  if(changing_type_1 == FALLING)
  {
    delay(50);
  }
  INPUT_1_BIT_IN_HOLDING_REG = (register_t)INPUT_1_BIT;


  static uint32_t last_active_2;
  static Bico_Check_Bit_Change input_2_bit(INPUT_2_BIT);
  uint8_t changing_type_2 = input_2_bit.isChanging(INPUT_2_BIT);
  if(changing_type_2 == RISING)
  {
    delay(50);
    last_active_2 = millis();
    COUNTING_VALUE_2++;
  }
  if(INPUT_2_BIT == HIGH)
  {
    RUN_TIME_2 = millis() - last_active_2;
  }
  if(changing_type_2 == FALLING)
  {
    delay(50);
  }
  INPUT_2_BIT_IN_HOLDING_REG = (register_t)INPUT_2_BIT;


  static uint32_t last_active_3;
  static Bico_Check_Bit_Change input_3_bit(INPUT_3_BIT);
  uint8_t changing_type_3 = input_3_bit.isChanging(INPUT_3_BIT);
  if(changing_type_3 == RISING)
  {
    delay(50);
    last_active_3 = millis();
    COUNTING_VALUE_3++;
  }
  if(INPUT_3_BIT == HIGH)
  {
    RUN_TIME_3 = millis() - last_active_3;
  }
  if(changing_type_3 == FALLING)
  {
    delay(50);
  }
  INPUT_3_BIT_IN_HOLDING_REG = (register_t)INPUT_3_BIT;
}


























void IOSetup()
{
  pinMode(DI0, INPUT);
  pinMode(DI1, INPUT);
  pinMode(DI2, INPUT);
  pinMode(DI3, INPUT);

  pinMode(DO0, OUTPUT);
  pinMode(DO1, OUTPUT);
  pinMode(DO2, OUTPUT);
  pinMode(DO3, OUTPUT);
}

void IOUpdate()
{
  setBitToArray(slave_discrete_input_contact_list, 0, digitalRead(DI0));
  setBitToArray(slave_discrete_input_contact_list, 1, digitalRead(DI1));
  setBitToArray(slave_discrete_input_contact_list, 2, digitalRead(DI2));
  setBitToArray(slave_discrete_input_contact_list, 3, digitalRead(DI3));

  digitalWrite(DO0, getBitFromArray(slave_discrete_output_coil_list, 0));
  digitalWrite(DO1, getBitFromArray(slave_discrete_output_coil_list, 1));
  digitalWrite(DO2, getBitFromArray(slave_discrete_output_coil_list, 2));
  digitalWrite(DO3, getBitFromArray(slave_discrete_output_coil_list, 3));
}

void updateAnalogIn()
{
//  slave_analog_output_holding_register_list[6] = analogRead(RTD0);
//  slave_analog_output_holding_register_list[7] = analogRead(RTD1);
}


void serialEvent() 
{
  last_esp_polling = millis();
}
