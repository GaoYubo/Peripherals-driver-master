/***********************************************************************************************************************
* History: 
* <author>    <time>      <version >       <desc>
* Gaoyubo     2018.10.09    V1.0            creat
*
* Description:  ADC get system temprature 
***********************************************************************************************************************/
#include "system_temprature.h"

/***********************************************************************************************************************
* Function:     void system_temprature_Init(void)
*
* Scope:        public
*
* Description:  Initialize ADC1 
*
* Arguments:
*
* Return:
***********************************************************************************************************************/
void system_temprature_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  //模拟输入
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  //下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);//复位ADC1
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);//停止复位ADC1
    
    ADC_TempSensorVrefintCmd(ENABLE);//使能内部温度传感器
    
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInit(&ADC_CommonInitStructure);
    
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_Cmd(ADC1, ENABLE);
}


u16 Get_Adc(u8 ch)
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );
    
	ADC_SoftwareStartConv(ADC1);
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));

	return ADC_GetConversionValue(ADC1);
}


u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		for(int i=0;i<200;i++);
	}
	return temp_val/times;
} 

short getCPUtemperature(void)
{
    u32 adcx; short result;
    double temperate;
    adcx=Get_Adc_Average(ADC_Channel_16,20);
    temperate=(float)adcx*(3.3/4096);//电压值
    temperate=(temperate-0.76)/0.0025+25;
    result=temperate*=10;
    return result;
}
