#include "Arduino_Master_RTU_Soft_Serial.h"
#include "Arduino_Slave_RTU_Serial.h"
#include "Bico_Check_Bit_Change.h"

#define DI0 7 
#define DI1 6 
#define DI2 5 
#define DI3 4 

#define DO0 14 // A0
#define DO1 15 // A1
#define DO2 16 // A2
#define DO3 17 // A3

#define AI0 A6
#define AI1 A7

#define AO0 10
#define AO1 9

#define ESP_RESET A5

Arduino_Master_RTU_Soft_Serial master(2,3);
Arduino_Slave_RTU_Serial slave;

#define ESP_POLLING_TIMEOUT 7000
long last_esp_polling = 0;

void setup() {
  // put your setup code here, to run once:
  master.begin(9600);
  slave.begin(9600);
  IOSetup();
  pinMode(ESP_RESET, OUTPUT);
  digitalWrite(ESP_RESET, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  slave.fullHanlde();
  IOUpdate();
  analogUpdate();

  if(millis() - last_esp_polling > ESP_POLLING_TIMEOUT)
  {
    digitalWrite(ESP_RESET, LOW);
    delay(10);
    digitalWrite(ESP_RESET, HIGH);

    last_esp_polling = millis();
  }



  #define POLL_INTERVAL 1000  // 1 giay poll mot lan
  static uint32_t last_time = millis();
  if((millis() - last_time) > POLL_INTERVAL)
  {
    #define PERIPHERAL_ID 1 // ID cua Slave
    #define START_ADDRESS 0 // Dia chi thanh ghi dau tien
    #define NUM_OF_REG 3  // So thanh ghi can doc
    
    master.RTUSoftSerialReadInputRegistersRequest(PERIPHERAL_ID, 1000, NUM_OF_REG);  // poll
    
    #define DELAY_DEPEND_ON_PERIPHERAL 50 // in millis
    delay(DELAY_DEPEND_ON_PERIPHERAL); // cho slave phan hoi lai
  
    if(master.available())
    {
      register_t temp_reg[NUM_OF_REG];  // tao mang tam thoi
      master.RTUSoftSerialGetReadInputRegistersValue(temp_reg); // lay gia tri cac thanh ghi vua nhan duoc tu slave dua vao cac thanh ghi tam thoi
  
      for(uint8_t i = 0; i < NUM_OF_REG; i++)
      {
        slave_analog_output_holding_register_list[1 + i] = temp_reg[i];
      }
    }

    //--------------------------------------
    delay(100);
    //--------------------------------------
    
    master.RTUSoftSerialReadHoldingRegistersRequest(PERIPHERAL_ID, 0, NUM_OF_REG);  // poll
    
    delay(DELAY_DEPEND_ON_PERIPHERAL); // cho slave phan hoi lai
  
    if(master.available())
    {
      register_t temp_reg[NUM_OF_REG];  // tao mang tam thoi
      master.RTUSoftSerialGetReadHoldingRegistersValue(temp_reg); // lay gia tri cac thanh ghi vua nhan duoc tu slave dua vao cac thanh ghi tam thoi
  
      for(uint8_t i = 0; i < NUM_OF_REG; i++)
      {
        slave_analog_output_holding_register_list[10 + i] = temp_reg[i];
      }
    }

    last_time = millis();
  }


  static register_t last_setting_value;
  if(slave_analog_output_holding_register_list[14] != last_setting_value)
  {
    master.RTUSoftSerialWriteSingleRegisterRequest(PERIPHERAL_ID, 0, 1);
    last_setting_value = slave_analog_output_holding_register_list[14];
  }

  
  slave_analog_output_holding_register_list[4] = map(slave_analog_output_holding_register_list[6], 0, 1024, 0, 1024);
  slave_analog_output_holding_register_list[5] = map(slave_analog_output_holding_register_list[7], 0, 1024, 0, 1024);


  #define INPUT_1_BIT getBitFromArray(slave_discrete_input_contact_list, 1)
  #define INPUT_1_BIT_IN_HOLDING_REG slave_analog_output_holding_register_list[8]
  #define RUN_TIME_1 slave_analog_output_holding_register_list[9]

  static uint32_t last_active_1;
  static Bico_Check_Bit_Change input_1_bit(INPUT_1_BIT);
  uint8_t changing_type_1 = input_1_bit.isChanging(INPUT_1_BIT);
  if(changing_type_1 == RISING)
  {
    delay(50);
    last_active_1 = millis();
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
  
//  static long last_time = millis();
//  if(millis() - last_time > 10)
//  {
//    slave_analog_output_holding_register_list[0]+=2;
//    slave_analog_output_holding_register_list[1]+=3;
//    slave_analog_output_holding_register_list[2]+=4;
//    slave_analog_output_holding_register_list[3]+=5;
//    slave_analog_output_holding_register_list[4]+=6;
//    slave_analog_output_holding_register_list[5]+=2;
//    slave_analog_output_holding_register_list[6]+=3;
//    slave_analog_output_holding_register_list[7]+=4;
//    slave_analog_output_holding_register_list[8]+=5;
//    slave_analog_output_holding_register_list[9]+=6;
//    last_time = millis();
//  }

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

void analogUpdate()
{
  slave_analog_output_holding_register_list[6] = analogRead(AI0);
  slave_analog_output_holding_register_list[7] = analogRead(AI1);
}

void serialEvent() 
{
  last_esp_polling = millis();
}
