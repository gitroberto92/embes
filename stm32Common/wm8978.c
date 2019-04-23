/* WM8978모듈은 http://51maxgear.taobao.com 에서 구입할수 있다，
본 프로그람의 저작권은 http://51maxgear.taobao.com 에 속하며 수정과 배포는 가능하지만 우의 정보를 유지하십시오
*/
#include "wm8978.h"


static uint16_t WM8978_REGVAL[58]=
{
	0X0000,0X0000,0X0000,0X0000,0X0050,0X0000,0X0140,0X0000,
	0X0000,0X0000,0X0000,0X00FF,0X00FF,0X0000,0X0100,0X00FF,
	0X00FF,0X0000,0X012C,0X002C,0X002C,0X002C,0X002C,0X0000,
	0X0032,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
	0X0038,0X000B,0X0032,0X0000,0X0008,0X000C,0X0093,0X00E9,
	0X0000,0X0000,0X0000,0X0000,0X0003,0X0010,0X0010,0X0100,
	0X0100,0X0002,0X0001,0X0001,0X0039,0X0039,0X0039,0X0039,
	0X0001,0X0001
}; 

uint8_t WM8978_Write_Reg(uint8_t reg,uint16_t val)
{
	HAL_StatusTypeDef res;
	uint8_t RegAddr;
	uint8_t RegValue;
	RegAddr=(reg<<1)|((uint8_t)((val>>8)&0x01));
	RegValue=(uint8_t)val;
	res = HAL_I2C_Mem_Write(&hi2c1, (WM8978_ADDR<<1), RegAddr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)&RegValue, 1, 1000);
	if(res == HAL_OK)
		WM8978_REGVAL[reg]=val;
	else
	{
#ifdef __DEBUG
		printf("wm write reg err\r\n");
#endif
		initSuccess  = 1;
	}
	return (uint8_t)res;
}

uint16_t WM8978_Read_Reg(uint8_t reg)
{  
	return WM8978_REGVAL[reg];
} 

uint8_t WM8978_Init(void)
{
	//I2C버스는 이 함수를 호출하기전에 초기화를 완료해야한다.
	uint8_t Res;
	Res=WM8978_Write_Reg(0,0);		//소프트 리셋 WM8978
	if(Res)
		return 1;										//명령전송 실패 ,WM8978 오류
	WM8978_Write_Reg (1,0x1B); 		// R1, MICEN 설정 (MIC를 사용), BIASEN을 1로 설정 (시뮬레이터), VMIDSEL를 [0:1]로 설정된다 : 11 (5K); 
	WM8978_Write_Reg (2,0x1B0); 	// R2, ROUT1, LOUT1 출력 enable(헤드폰 동작가능) BOOSTENR, BOOSTENL 활성화
	WM8978_Write_Reg (3,0x6C); 		// R3, LOUT2, ROUT2 출력 허용 (MIX 작동), RMIX, LMIX 활성화
	WM8978_Write_Reg (6,0); 		// R6, MCLK는 외부에서 제공됩니다.
	WM8978_Write_Reg (43,1 << 4); // R43, INVROUT2 reverse, 구동 스피커
	WM8978_Write_Reg (47,1 << 8); // R47 설정, PGABOOSTL,  왼쪽 채널 MIC가 20 배 증가.
	WM8978_Write_Reg (48,1 << 8); // R48 설정, PGABOOSTR, 오른쪽 채널 MIC의 20 배 증가
	WM8978_Write_Reg (49,1 << 1); // R49, TSDEN, 개방 과열 보호
	WM8978_Write_Reg (10,1 << 3); // R10, SOFTMUTE 꺼짐, 128x 샘플링, 최상의 SNR
	WM8978_Write_Reg (14,1 << 3); // R14, ADC 128x 샘플 속도

	WM8978_I2S_Cfg (2,0); // I2S 인터페이스 모드에서, 데이터 비트는 재생 장치를 이용하지 않고 제공 될 필요는 없다 
	// 재생설정
	WM8978_ADDA_Cfg (1,0); // DAC 켜기
	WM8978_Input_Cfg (0,0,0); // 입력 채널을 닫습니다.
	WM8978_Output_Cfg (1,0); // DAC 출력 켜기
	// 녹음 설정
	// WM8978_ADDA_Cfg (0,1); // ADC 열기
	// WM8978_Input_Cfg (1,1,0); // 입력 채널 열기 (MIC & LINE IN)
	// WM8978_Output_Cfg (0,1); // BYPASS 출력 열기
	// WM8978_MIC_Gain (46); // MIC 게인 설정
	WM8978_HPvol_Set (25,25);
	WM8978_SPKvol_Set (50);

	
	return 0;
}
//WM8978 DAC/ADC설정
//adcen:adc사용(1)/끄기(0)
//dacen:dac사용(1)/끄기(0)
void WM8978_ADDA_Cfg(uint8_t dacen,uint8_t adcen)
{
	uint16_t regval;
	regval=WM8978_Read_Reg(3);							//R3읽기
	if(dacen)
		regval|=3<<0;									//R3최하위 2비드를 1로 설정하여,DACR&DACL를 켠다.
	else 
		regval&=~(3<<0);								//R3의 최하위 2비트를 0으로 설정하여 DACR&DACL를 끈다.
	WM8978_Write_Reg(3,regval);							//R3설정
	regval=WM8978_Read_Reg(2);							//R2읽기
	if(adcen)
		regval|=3<<0;			        				//R2의 최하위 2비트를 1로 설정하여 ADCR&ADCL를 켠다.
	else 
		regval&=~(3<<0);				  				//R2의 최하위 두비트를 0으로 설정하여 ADCR&ADCL를 끈다.
	WM8978_Write_Reg(2,regval);							//R2설정
}
//WM8978 입력채널구성
//micen:MIC켜기(1)/끄기(0)
//lineinen:Line In켜기(1)/끄기(0)
//auxen:aux켜기(1)/끄기(0) 
void WM8978_Input_Cfg(uint8_t micen,uint8_t lineinen,uint8_t auxen)
{
	uint16_t regval;  
	regval=WM8978_Read_Reg(2);							//R2읽기
	if(micen)
		regval|=3<<2;									//INPPGAENR,INPPGAENL(MIC의 PGA증폭)켜기
	else 
		regval&=~(3<<2);								//INPPGAENR,INPPGAENL끄기.
 	WM8978_Write_Reg(2,regval);							//R2설정 
	regval=WM8978_Read_Reg(44);							//R44얻기
	if(micen)
		regval|=3<<4|3<<0;										//LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA 켜기.
	else 
		regval&=~(3<<4|3<<0);									//LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA 끄기.
	WM8978_Write_Reg(44,regval);						//R44 설정
	if(lineinen)
		WM8978_LINEIN_Gain(5);							//LINE IN 0dB 증폭
	else 
		WM8978_LINEIN_Gain(0);								//LINE IN 끄기
	if(auxen)
		WM8978_AUX_Gain(7);										//AUX 6dB 증폭
	else 
		WM8978_AUX_Gain(0);										//AUX입력끄기
}
//WM8978 MIC증폭도 설정(BOOST의 20dB을 포함하지 않는다,MIC-->ADC입력부분의 증폭)
//gain:0~63,对应-12dB~35.25dB,0.75dB/Step
void WM8978_MIC_Gain(uint8_t gain)
{
	gain&=0X3F;
	WM8978_Write_Reg(45,gain);							//R45,좌측채널의 PGA설정 
	WM8978_Write_Reg(46,gain|1<<8);					//R46,우측채널의 PGA설정
}
//WM8978 L2/R2(Line In역시)증폭도 설정(L2/R2-->ADC입력부분 증폭)
//gain:0~7,0 채널이 금지되였음을 나타낸다,1~7,증폭도: -12dB~6dB,3dB/Step
void WM8978_LINEIN_Gain(uint8_t gain)
{
	uint16_t regval;
	gain&=0X07;
	regval=WM8978_Read_Reg(47);							//R47얻기
	regval&=~(7<<4);									//원복삭제설정 
 	WM8978_Write_Reg(47,regval|gain<<4);		//R47설정
	regval=WM8978_Read_Reg(48);					//R48얻기
	regval&=~(7<<4);							//清除原来的설정 
 	WM8978_Write_Reg(48,regval|gain<<4);		//설정R48
} 
//WM8978 AUXR,AUXL(PWM 주파수 부분)강조설정설정(AUXR/L-->ADC입력부분의 강조)
//gain:0~7,0통로가 금지되였음,1~7: -12dB~6dB,3dB/Step
void WM8978_AUX_Gain(uint8_t gain)
{
	uint16_t regval;
	gain&=0X07;
	regval=WM8978_Read_Reg(47);					//R47얻기
	regval&=~(7<<0);							//清除原来的설정 
 	WM8978_Write_Reg(47,regval|gain<<0);		//R47설정
	regval=WM8978_Read_Reg(48);					//R48얻기
	regval&=~(7<<0);							//清除原来的설정 
 	WM8978_Write_Reg(48,regval|gain<<0);		//R48설정
}  

//WM8978 출력구성 
//dacen:DAC출력(재생)켜기(1)/끄기(0)
//bpsen:Bypass출력(록음,MIC,LINE IN,AUX등 포함)켜기(1)/끄기(0)
void WM8978_Output_Cfg(uint8_t dacen,uint8_t bpsen)
{
	uint16_t regval=0;
	if(dacen)
		regval|=1<<0;													//DAC출력사용가능
	if(bpsen)
	{
		regval|=1<<1;													//BYPASS사용가능
		regval|=5<<2;													//0dB증폭
	} 
	WM8978_Write_Reg(50,regval);						//R50설정
	WM8978_Write_Reg(51,regval);						//R51설정 
}
//헤드폰 좌우 통로음량설정
//voll:좌측음량(0~63)
//volr:우측음량(0~63)
void WM8978_HPvol_Set(uint8_t voll,uint8_t volr)
{
	voll&=0X3F;
	volr&=0X3F;												//제한된 범위
	if(voll==0)voll|=1<<6;									//볼륨이 0인경우 ,직접mute
	if(volr==0)volr|=1<<6;									//볼륨이 0인경우 ,직접mute
	WM8978_Write_Reg(52,voll);						//R52,헤드폰좌측음량설정
	WM8978_Write_Reg(53,volr|(1<<8));				//R53,헤드폰우측음량설정,동기화업데이트(HPVU=1)
}
//스피커음량설정
//voll:좌측채널음량(0~63)
void WM8978_SPKvol_Set(uint8_t volx)
{
	volx&=0X3F;//제한범위
	if(volx==0)volx|=1<<6;								//음량이0일때 직접mute 
 	WM8978_Write_Reg(54,volx);							//R54,스피커 좌측통로 음량설정
	WM8978_Write_Reg(55,volx|(1<<8));				//R55,스피커 우측통로 음량설정,동기화업데이트(SPKVU=1)
}
//I2S작업방식설정
//fmt:0,LSB(오른쪽정렬);1,MSB(외쪽정렬);2,필립스표준I2S;3,PCM/DSP;
//len:0,16bit;1,20bit;2,24bit;3,32bit;
void WM8978_I2S_Cfg(uint8_t fmt,uint8_t len)
{
	fmt&=0x03;
	len&=0x03;								//제한범위
	WM8978_Write_Reg(4,(fmt<<3)|(len<<5));	//R4,WM8978동작방식설정	
}
