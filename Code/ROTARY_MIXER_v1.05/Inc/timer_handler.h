/////////////////////////////////////////////////////////////////////////////////////////////////
// 	timer handler
//	product: Rotary Mixer
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdint.h"

	
///////////////////////////////////////////////
//
//			TIMER 300Hz
//	
void TIM2_IRQHandler(void)
	{
	//HAL_GPIO_TogglePin(PCB_LED_GPIO_Port, PCB_LED_Pin);	
	if(TIMER_DIV<4)
		{
		TIMER_DIV++;	
		if(TIMER_DIV==1)
			{
			HAL_ADC_Start(&hadc1);	
			HAL_ADC_Start(&hadc2);	
			HAL_ADC_Start(&hadc3);					
			}	
		else if(TIMER_DIV==2)
			{
			ADC_TEMP[VOL0] = 3*ADC_TEMP[VOL0] + HAL_ADC_GetValue(&hadc1);	
			ADC_TEMP[VOL1] = 3*ADC_TEMP[VOL1] + HAL_ADC_GetValue(&hadc2);		
			ADC_TEMP[CUES] = 3*ADC_TEMP[CUES] + HAL_ADC_GetValue(&hadc3);	
			ADC_TEMP[VOL0]>>=2;
			ADC_TEMP[VOL1]>>=2;
			ADC_TEMP[CUES]>>=2;				
			ADC1_SELECT_CH(POT_VCF0);	
			ADC2_SELECT_CH(POT_VCF1);
			ADC3_SELECT_CH(POT_QFAC);
			/////////////////VOL0	
			if(ADC_TEMP[VOL0]>(ADC_center[VOL0]+ADC_HYSTERESIS) || (ADC_HYSTERESIS+ADC_TEMP[VOL0])<ADC_center[VOL0] || (ADC_TEMP[VOL0]<ADC_MIN))			
				{																																							
				ADC_center[VOL0] = ADC_TEMP[VOL0];			
				if(ADC_center[VOL0]<ADC_MIN)
					{
					VR_LEVEL[VOL0] = 0;	
					}
				else if(ADC_center[VOL0]>ADC_MAX)
					{
					VR_LEVEL[VOL0] = 255;	
					}
				else
					{
					adc_tmp_mux = (ADC_center[VOL0]-ADC_MIN)<<4; 	
					VR_LEVEL[VOL0] = adc_tmp_mux/mux_divider;	
					}	
				}

			/////////////////VOL1	
			if(ADC_TEMP[VOL1]>(ADC_center[VOL1]+ADC_HYSTERESIS) || (ADC_HYSTERESIS+ADC_TEMP[VOL1])<ADC_center[VOL1] || (ADC_TEMP[VOL1]<ADC_MIN))			
				{																																							
				ADC_center[VOL1] = ADC_TEMP[VOL1];	
				if(ADC_center[VOL1]<ADC_MIN)
					{
					VR_LEVEL[VOL1] = 0;	
					}
				else if(ADC_center[VOL1]>ADC_MAX)
					{
					VR_LEVEL[VOL1] = 255;	
					}
				else
					{
					adc_tmp_mux = (ADC_center[VOL1]-ADC_MIN)<<4; 	
					VR_LEVEL[VOL1] = adc_tmp_mux/mux_divider;		
					}		
				}		
		
				
			/////////////////CUES	
			if(ADC_TEMP[CUES]>(ADC_center[CUES]+ADC_HYSTERESIS) || (ADC_HYSTERESIS+ADC_TEMP[CUES])<ADC_center[CUES] || (ADC_TEMP[CUES]<ADC_MIN))			
				{																																							
				ADC_center[CUES] = ADC_TEMP[CUES];		
				if(ADC_center[CUES]<ADC_MIN)
					{
					VR_LEVEL[CUES] = 0;	
					}
				else if(ADC_center[CUES]>ADC_MAX)
					{
					VR_LEVEL[CUES] = 255;	
					}
				else
					{
					adc_tmp_mux = (ADC_center[CUES]-ADC_MIN)<<4; 	
					VR_LEVEL[CUES] = adc_tmp_mux/mux_divider;			
					}						
				}		
		
			}	
		else if(TIMER_DIV==3)
			{				
			if(clip_ch1_cnt>0)				//clipping detector
				{
				clip_ch1_cnt--;	
				if(clip_ch1_cnt==14)
					{
					GPIOB->BSRR = 0x00000008;	
					}
				else if(clip_ch1_cnt==1)
					{
					GPIOB->BSRR = 0x00080000;	
					}
				}
			if(clip_ch2_cnt>0)				//clipping detector
				{
				clip_ch2_cnt--;	
				if(clip_ch2_cnt==14)
					{
					GPIOD->BSRR = 0x00000080;
					}
				else if(clip_ch2_cnt==1)
					{
					GPIOD->BSRR = 0x00800000;	
					}
				}		
			HAL_ADC_Start(&hadc1);	
			HAL_ADC_Start(&hadc2);	
			HAL_ADC_Start(&hadc3);					
			}	
		else if(TIMER_DIV==4)
			{
			ADC_TEMP[VCF0] = 3*ADC_TEMP[VCF0] + HAL_ADC_GetValue(&hadc1);	
			ADC_TEMP[VCF1] = 3*ADC_TEMP[VCF1] + HAL_ADC_GetValue(&hadc2);		
			ADC_TEMP[QFAC] = 3*ADC_TEMP[QFAC] + HAL_ADC_GetValue(&hadc3);		
			ADC_TEMP[VCF0]>>=2;
			ADC_TEMP[VCF1]>>=2;
			ADC_TEMP[QFAC]>>=2;	
			ADC1_SELECT_CH(POT_VOL0);	
			ADC2_SELECT_CH(POT_VOL1);
			ADC3_SELECT_CH(POT_CUES);
			/////////////////VCF0	
			if(ADC_TEMP[VCF0]>(ADC_center[VCF0]+ADC_HYSTERESIS) || (ADC_HYSTERESIS+ADC_TEMP[VCF0])<ADC_center[VCF0] || (ADC_TEMP[VCF0]<ADC_MIN))			
				{																																							
				ADC_center[VCF0] = ADC_TEMP[VCF0];	
				if(ADC_center[VCF0]<ADC_MIN)
					{
					VR_LEVEL[VCF0] = 0;	
					}
				else if(ADC_center[VCF0]<1641)		//lpf	
					{
					adc_tmp_mux = (ADC_center[VCF0]-ADC_MIN)<<4; 	
					VR_LEVEL[VCF0] = adc_tmp_mux/198;			
					}
				else if(ADC_center[VCF0]<2153)		//center	
					{
					VR_LEVEL[VCF0] = 128;	
					}	
				else if(ADC_center[VCF0]<ADC_MAX)		//hpf	
					{
					adc_tmp_mux = (ADC_center[VCF0]-2153)<<4; 	
					VR_LEVEL[VCF0] = 129+(adc_tmp_mux/243);		
					}	
				else
					{
					VR_LEVEL[VCF0] = 255;	
					}																
				}		
	
			/////////////////VCF1	
			if(ADC_TEMP[VCF1]>(ADC_center[VCF1]+ADC_HYSTERESIS) || (ADC_HYSTERESIS+ADC_TEMP[VCF1])<ADC_center[VCF1] || (ADC_TEMP[VCF1]<ADC_MIN))			
				{																																							
				ADC_center[VCF1] = ADC_TEMP[VCF1];					
				if(ADC_center[VCF1]<ADC_MIN)
					{
					VR_LEVEL[VCF1] = 0;	
					}
				else if(ADC_center[VCF1]<1641)		//lpf	
					{
					adc_tmp_mux = (ADC_center[VCF1]-ADC_MIN)<<4; 	
					VR_LEVEL[VCF1] = adc_tmp_mux/198;			
					}
				else if(ADC_center[VCF1]<2153)		//center	
					{
					VR_LEVEL[VCF1] = 128;	
					}	
				else if(ADC_center[VCF1]<ADC_MAX)		//hpf	
					{
					adc_tmp_mux = (ADC_center[VCF1]-2153)<<4; 	
					VR_LEVEL[VCF1] = 129+(adc_tmp_mux/243);		
					}	
				else
					{
					VR_LEVEL[VCF1] = 255;	
					}			
				}		

			/////////////////QFAC	
			if(ADC_TEMP[QFAC]>(ADC_center[QFAC]+ADC_HYSTERESIS) || (ADC_HYSTERESIS+ADC_TEMP[QFAC])<ADC_center[QFAC] || (ADC_TEMP[QFAC]<ADC_MIN))			
				{																																							
				ADC_center[QFAC] = ADC_TEMP[QFAC];		
				if(ADC_center[QFAC]<ADC_MIN)
					{
					VR_LEVEL[QFAC] = 0;	
					}
				else if(ADC_center[QFAC]>ADC_MAX)
					{
					VR_LEVEL[QFAC] = 255;	
					}
				else
					{
					adc_tmp_mux = (ADC_center[QFAC]-ADC_MIN)<<4; 	
					VR_LEVEL[QFAC] = adc_tmp_mux/mux_divider;			
					}	
				}		
		
			}		
		}
	else
		{	
		currcrc = VR_LEVEL[0] +	VR_LEVEL[1] +	VR_LEVEL[2] +	VR_LEVEL[3] +	VR_LEVEL[4] +	VR_LEVEL[5];
		TIMER_DIV = 0;	
			
		if(lvl_actualL>=actualL_temp)
			{
			actualL_temp = lvl_actualL;	
			}
		else
			{
			if(actualL_temp>255)
				{
				actualL_temp-=256;	
				}
			else
				{
				actualL_temp = 0;	
				}				
			}
		lvl_actualL = 0;	
		if(lvl_actualR>=actualR_temp)
			{
			actualR_temp = lvl_actualR;	
			}
		else
			{
			if(actualR_temp>255)
				{
				actualR_temp-=256;	
				}
			else
				{
				actualR_temp = 0;	
				}					
			}
		lvl_actualR = 0;					
		TIM3->CCR2 = actualL_temp>>7;
		TIM3->CCR1 = actualR_temp>>7;				
		}
	HAL_TIM_IRQHandler(&htim2);		
	}

///////////////////////////////////////////////
//
//  UART HANDLER	
//	
void UART5_IRQHandler(void)
	{

		
  HAL_UART_IRQHandler(&huart5);
	}





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



