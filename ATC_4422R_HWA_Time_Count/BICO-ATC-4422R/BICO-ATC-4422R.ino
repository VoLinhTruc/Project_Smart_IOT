#include "Arduino_Slave_RTU_Serial.h"
#include "Bico_Check_Bit_Change.h"

#define DI0 10
#define DI1 9
#define DI2 8
#define DI3 7

#define DO0 12
#define DO1 11
#define DO2 3
#define DO3 2

#define AO0 7
#define AO1 6

#define RTD0 A6
#define RTD1 A7

#define ESP_RESET 4

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
  updateAnalogIn();

  if(millis() - last_esp_polling > ESP_POLLING_TIMEOUT)
  {
    digitalWrite(ESP_RESET, LOW);
    delay(10);
    digitalWrite(ESP_RESET, HIGH);

    last_esp_polling = millis();
  }


  #define INPUT_0_BIT getBitFromArray(slave_discrete_input_contact_list, 0)
  #define COUNTING_VALUE_0 slave_analog_output_holding_register_list[0]
  #define INPUT_0_BIT_IN_HOLDING_REG slave_analog_output_holding_register_list[1]
  #define RUN_TIME_0 slave_analog_output_holding_register_list[2]

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
  slave_analog_output_holding_register_list[6] = analogRead(RTD0);
  slave_analog_output_holding_register_list[7] = analogRead(RTD1);
}


void serialEvent() 
{
  last_esp_polling = millis();
}
