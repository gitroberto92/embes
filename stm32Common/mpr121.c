#ifdef STM32F767xx
#include "stm32f7xx_hal.h"
#else
#include "stm32f4xx_hal.h"
#endif

#include "main.h"
#include "mpr121.h"
#include "stdbool.h"
#include "engine_main.h"



/// PB5-SCL
///PB6-SDA
///PA15-IRQ

//touch_shield
#define touch_shield 0xB4  //Addr 은 접지. mpr121 주소의 7비트는 0x5b 이고 최후의 비트를 0으로(왼쪽 비트밀기)하면 8비트의 주소는 0xB4

// I2C의 핸들이 기판마다 다를수 있다.
#ifdef STM32F446xx
#define MPR121HI2C hi2c2 
#endif
#ifdef STM32F411xE
#define MPR121HI2C hi2c3 
#endif
#ifdef STM32F767xx
#define MPR121HI2C hi2c2 
#endif

int write_i2c(unsigned char device,unsigned char addr,unsigned char dat)
{
	HAL_StatusTypeDef res;
	uint8_t RegAddr;
	uint8_t RegValue;
	RegAddr=addr;
	RegValue=(uint8_t)dat;
	res = HAL_I2C_Mem_Write(&MPR121HI2C, device, RegAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&RegValue, 1, 1000);
	res = HAL_I2C_IsDeviceReady(&MPR121HI2C, device, 2, 1000);
	if(res == HAL_OK){
		return 0;
	}else{
#ifdef __DEBUG
		printf("write i2c error with mpr121: addr:%d, dat:%d, res:%d\r\n", addr, dat, res);
#endif
		initSuccess = 1;
	}
	return (int)res;
}


unsigned char read_i2c(unsigned char device,unsigned char addr)
{
	unsigned char dat = 0;
	HAL_StatusTypeDef res;
	res = HAL_I2C_Mem_Read(&MPR121HI2C, device, addr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&dat, 1, 1000);
	if(res != HAL_OK)
	{
#ifdef __DEBUG
		printf("i2c read error(%d)\r\n", res);
#endif
	}
  
	return dat;
}

unsigned int prev_dat = 0;


int get_key(void)
{
	unsigned int  dat = 0;
	
 	dat=read_i2c(touch_shield,0x01)<<8;
	dat=dat| read_i2c(touch_shield,0x00);

	if(prev_dat != dat){
		printf("input data: %d\r\n", dat);
	}
	prev_dat = dat;
	return dat;
}

void mpr121_init(void)
{  // Section A
   //This group controls filtering when data is > baseline.
  write_i2c(touch_shield,MHD_R, 0x01);
  write_i2c(touch_shield,NHD_R, 0x01);
  write_i2c(touch_shield,NCL_R, 0x00);
  write_i2c(touch_shield,FDL_R, 0x00);

//  // Section B
  // This group controls filtering when data is < baseline.
  write_i2c(touch_shield,MHD_F, 0x01);
  write_i2c(touch_shield,NHD_F, 0x01);
  write_i2c(touch_shield,NCL_F, 0xFF);
  write_i2c(touch_shield,FDL_F, 0x02);
//  
//  // Section C
//  // This group sets touch and release thresholds for each electrode
  write_i2c(touch_shield,ELE0_T, TOU_THRESH);
  write_i2c(touch_shield,ELE0_R, REL_THRESH);
  write_i2c(touch_shield,ELE1_T, TOU_THRESH);
  write_i2c(touch_shield,ELE1_R, REL_THRESH);
  write_i2c(touch_shield,ELE2_T, TOU_THRESH);
  write_i2c(touch_shield,ELE2_R, REL_THRESH);
  write_i2c(touch_shield,ELE3_T, TOU_THRESH);
  write_i2c(touch_shield,ELE3_R, REL_THRESH);
  write_i2c(touch_shield,ELE4_T, TOU_THRESH);
  write_i2c(touch_shield,ELE4_R, REL_THRESH);
  write_i2c(touch_shield,ELE5_T, TOU_THRESH);
  write_i2c(touch_shield,ELE5_R, REL_THRESH);
  write_i2c(touch_shield,ELE6_T, TOU_THRESH);
  write_i2c(touch_shield,ELE6_R, REL_THRESH);
  write_i2c(touch_shield,ELE7_T, TOU_THRESH);
  write_i2c(touch_shield,ELE7_R, REL_THRESH);
  write_i2c(touch_shield,ELE8_T, TOU_THRESH);
  write_i2c(touch_shield,ELE8_R, REL_THRESH);
  write_i2c(touch_shield,ELE9_T, TOU_THRESH);
  write_i2c(touch_shield,ELE9_R, REL_THRESH);
  write_i2c(touch_shield,ELE10_T, TOU_THRESH);
  write_i2c(touch_shield,ELE10_R, REL_THRESH);
  write_i2c(touch_shield,ELE11_T, TOU_THRESH);
  write_i2c(touch_shield,ELE11_R, REL_THRESH);
  
//  // Section D
//  // Set the Filter Configuration
//  // Set ESI2
  write_i2c(touch_shield,FIL_CFG, 0x04);
//  
//  // Section E
//  // Electrode Configuration
//  // Enable 6 Electrodes and set to run mode
//  // Set ELE_CFG to 0x00 to return to standby mode
  write_i2c(touch_shield,ELE_CFG, 0x0C);	// Enables all 12 Electrodes
  //write_i2c(touch_shield,ELE_CFG, 0x06);		// Enable first 6 electrodes
  
  // Section F
  // Enable Auto Config and auto Reconfig
  /*write_i2c(touch_shield,ATO_CFG0, 0x0B);
  write_i2c(touch_shield,ATO_CFGU, 0xC9);	// USL = (Vdd-0.7)/vdd*256 = 0xC9 @3.3V   write_i2c(touch_shield,ATO_CFGL, 0x82);	// LSL = 0.65*USL = 0x82 @3.3V
  write_i2c(touch_shield,ATO_CFGT, 0xB5);*/	// Target = 0.9*USL = 0xB5 @3.3V
}

int key = 0xFFFF;
int prev_key = 0xFFFF;

void TestMpr121(void)
{
  unsigned char key = 0xFF;
  unsigned char prev_key = 0xFF;
  HAL_StatusTypeDef res = HAL_I2C_IsDeviceReady(&MPR121HI2C, touch_shield, 2, 1000);
  if(res != HAL_OK)
  {
#ifdef __DEBUG
    printf("I2C3 did not ready[%d]!\r\n", res);
#endif
		return;
  }
  else
  {
#ifdef __DEBUG
    printf("I2C3 device ready ok[%X]\r\n", touch_shield);
#endif
  }
  
	while(1){
    key=get_key();	
		if(key < 12 && key != prev_key){
#ifdef __DEBUG
			printf("CHAR = %c\r\n",'0'+key);
#endif
			prev_key = key;
		}
	}  
}

void handleKeyEvent(void)
{
	key=get_key();
	if(key != prev_key){ // 전상태의 건반하고 다를때에만 건반사건을 처리한다.
#ifdef __DEBUG
		printf("Voice count = %d\r\n", voiceCount);
#endif
		prev_key = key;
		if(settingMode){ // 설정방식일때
			if(key == 2048){ // Rec파일등록부 바꾸기
				main_finish();
				changeDirectory();
				NVIC_SystemReset();
			}
			if (key == 1024) { // 음조바꾸기
				changeKeySet();
			}
		}else{ // 설정방식이 아닐때
			raiseKeyEvent(key, 0);
		}
	}
}
