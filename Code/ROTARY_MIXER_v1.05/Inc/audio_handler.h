/////////////////////////////////////////////////////////////////////////////////////////////////
// Audio processing handler 48000Hz
//	product: ROTARY MIXER
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

float MoogVCF_0(float input_0, float f_0, float fb_0);
float MoogVCF_1(float input_1, float f_1, float fb_1);
float MoogVCF_2(float input_2, float f_2, float fb_2);
float MoogVCF_3(float input_3, float f_3, float fb_3);

float MoogHVCF_0(float input_0h, float f_0h, float fb_0h);
float MoogHVCF_1(float input_1h, float f_1h, float fb_1h);
float MoogHVCF_2(float input_2h, float f_2h, float fb_2h);
float MoogHVCF_3(float input_3h, float f_3h, float fb_3h);


//////////////////////////////////////////////////////////
//
//					DAC1 HANDLER
//
void SAI1_IRQHandler(void)
	{
	//GPIOA->BSRR = 0x00000002;		
	HAL_SAI_IRQHandler(&hsai_BlockA1);
				
	if((CH2IN[0]>32765) || (CH2IN[0]<-32765) || (CH2IN[1]>32765) || (CH2IN[1]<-32765))				//clipping detector
		{
		clip_ch2_cnt = 15;	
		}		
		
	if((CH1IN[0]>32765) || (CH1IN[0]<-32765) || (CH1IN[1]>32765) || (CH1IN[1]<-32765))				//clipping detector
		{
		clip_ch1_cnt = 15;		
		}			
		
	if(VR_LEVEL[VCF0]<128)	//lpf
		{
		CH1POSTFILTER[0] = MoogVCF_0(CH1IN[0], fcutoff_0, resonanse_0);
		CH1POSTFILTER[1] = MoogVCF_1(CH1IN[1], fcutoff_0, resonanse_0);	
		}	
	else if(VR_LEVEL[VCF0]==128)		//disable filter
		{
		CH1POSTFILTER[0] = CH1IN[0];
		CH1POSTFILTER[1] = CH1IN[1];		
		}
	else	//hpf
		{
		CH1POSTFILTER[0] = MoogHVCF_0(CH1IN[0], fcutoff_0h, resonanse_0h);
		CH1POSTFILTER[1] = MoogHVCF_1(CH1IN[1], fcutoff_0h, resonanse_0h);	
		}		
		
	if(VR_LEVEL[VCF1]<128)	//lpf
		{
		CH2POSTFILTER[0] = MoogVCF_2(CH2IN[0], fcutoff_1, resonanse_1);
		CH2POSTFILTER[1] = MoogVCF_3(CH2IN[1], fcutoff_1, resonanse_1);	
		}	
	else if(VR_LEVEL[VCF1]==128)		//disable filter
		{
		CH2POSTFILTER[0] = CH2IN[0];
		CH2POSTFILTER[1] = CH2IN[1];		
		}
	else	//hpf
		{
		CH2POSTFILTER[0] = MoogHVCF_2(CH2IN[0], fcutoff_1h, resonanse_1h);	
		CH2POSTFILTER[1] = MoogHVCF_3(CH2IN[1], fcutoff_1h, resonanse_1h);	
		}
				
	CHECK_CLIP = (CH1POSTFILTER[0]*VR_LIN[VR_LEVEL[VOL0]]+CH2POSTFILTER[0]*VR_LIN[VR_LEVEL[VOL1]]);
	if(CHECK_CLIP>32767.0)
		{
		MASTER_BUS[0] = 32767;
		}	
	else if(CHECK_CLIP<-32768.0)			
		{
		MASTER_BUS[0] = -32768;	
		}
	else
		{
		MASTER_BUS[0] = (int)CHECK_CLIP;	
		}			
	
	CHECK_CLIP = (CH1POSTFILTER[1]*VR_LIN[VR_LEVEL[VOL0]]+CH2POSTFILTER[1]*VR_LIN[VR_LEVEL[VOL1]]);
	if(CHECK_CLIP>32767.0)
		{
		MASTER_BUS[1] = 32767;
		}	
	else if(CHECK_CLIP<-32768.0)			
		{
		MASTER_BUS[1] = -32768;	
		}
	else
		{
		MASTER_BUS[1] = (int)CHECK_CLIP;	
		}		

	if(CUE_NUM==0)				//CH1
		{
		HP_BUS[0] = CH1IN[0];
		HP_BUS[1] = CH1IN[1];	
		}			
	else if(CUE_NUM==1)	//CH1+M
		{
		CHECK_CLIP = (0.5*CH1IN[0]+MASTER_BUS[0]);
		if(CHECK_CLIP>32767.0)
			{
			HP_BUS[0] = 32767;
			}	
		else if(CHECK_CLIP<-32768.0)			
			{
			HP_BUS[0] = -32768;	
			}
		else
			{
			HP_BUS[0] = (int)CHECK_CLIP;	
			}			
		CHECK_CLIP	= (0.5*CH1IN[1]+MASTER_BUS[1]);
		if(CHECK_CLIP>32767.0)
			{
			HP_BUS[1] = 32767;
			}	
		else if(CHECK_CLIP<-32768.0)			
			{
			HP_BUS[1] = -32768;	
			}
		else
			{
			HP_BUS[1] = (int)CHECK_CLIP;	
			}		
		}		
	else if(CUE_NUM==2)	//MASTER
		{
		HP_BUS[0] = MASTER_BUS[0];
		HP_BUS[1] = MASTER_BUS[1];	
		}					
	else if(CUE_NUM==3)	//CH2+M
		{
		CHECK_CLIP = (0.5*CH2IN[0]+MASTER_BUS[0]);
		if(CHECK_CLIP>32767.0)
			{
			HP_BUS[0] = 32767;
			}	
		else if(CHECK_CLIP<-32768.0)			
			{
			HP_BUS[0] = -32768;	
			}
		else
			{
			HP_BUS[0] = (int)CHECK_CLIP;	
			}			
		CHECK_CLIP	= (0.5*CH2IN[1]+MASTER_BUS[1]);
		if(CHECK_CLIP>32767.0)
			{
			HP_BUS[1] = 32767;
			}	
		else if(CHECK_CLIP<-32768.0)			
			{
			HP_BUS[1] = -32768;	
			}
		else
			{
			HP_BUS[1] = (int)CHECK_CLIP;	
			}	
		}
	else			//CH2
		{
		HP_BUS[0] = CH2IN[0];
		HP_BUS[1] = CH2IN[1];	
		}

	if(HP_BUS[0]>0)
		{
		lvl_tempL = HP_BUS[0];	
		}
	else
		{
		lvl_tempL = -HP_BUS[0];	
		}
	if(lvl_tempL>lvl_actualL)
		{
		lvl_actualL = lvl_tempL;	
		}
		
	if(HP_BUS[1]>0)
		{
		lvl_tempR = HP_BUS[1];	
		}
	else
		{
		lvl_tempR = -HP_BUS[1];	
		}
	if(lvl_tempR>lvl_actualR)
		{
		lvl_actualR = lvl_tempR;	
		}	
	HAL_SAI_Transmit_IT(&hsai_BlockA1, MASTER_BUS, 2);	
	//HAL_SAI_Transmit_IT(&hsai_BlockA1, &SINED[2*i], 2);
	//GPIOA->BSRR = 0x00020000;	
	}
	
	
//////////////////////////////////////////////////////////
//
//					DAC2 HANDLER (HEADPHONES)
//
void SAI2_IRQHandler(void)
	{
  HAL_SAI_IRQHandler(&hsai_BlockA2);
	HAL_SAI_Transmit_IT(&hsai_BlockA2, HP_BUS, 2);		
	//HAL_SAI_Transmit_IT(&hsai_BlockA2, &SINED[2*i], 2);	
	}	

	
	
//////////////////////////////////////////////////////////
//
//					ADCs HANDLER
//	
void SAI3_IRQHandler(void)
	{
	HAL_SAI_IRQHandler(&hsai_BlockA3);	
	HAL_SAI_Receive_IT(&hsai_BlockA3, CH2IN, 2);
	HAL_SAI_IRQHandler(&hsai_BlockB3);	
	HAL_SAI_Receive_IT(&hsai_BlockB3, CH1IN, 2);	
	}	
	
	
	
/////////////////////////////////	
//
// filter 
//		
float MoogVCF_0(float input_0, float f_0, float fb_0)
	{	
  input_0 -= out4_0 * fb_0;
  input_0 *= 0.35013 * (f_0*f_0)*(f_0*f_0);		
  out1_0 = input_0 + 0.3 * in1_0 + (1 - f_0) * out1_0; // Pole 1
  in1_0  = input_0;
  out2_0 = out1_0 + 0.3 * in2_0 + (1 - f_0) * out2_0;  // Pole 2
  in2_0  = out1_0;
  out3_0 = out2_0 + 0.3 * in3_0 + (1 - f_0) * out3_0;  // Pole 3
  in3_0  = out2_0;
  out4_0 = out3_0 + 0.3 * in4_0 + (1 - f_0) * out4_0;  // Pole 4
  in4_0  = out3_0;
	return TRIM_FILTER[VR_LEVEL[QFAC]]*out4_0;	
	};	

/////////////////////////////////	
//
// filter 
//		
float MoogVCF_1(float input_1, float f_1, float fb_1)
	{
  input_1 -= out4_1 * fb_1;
  input_1 *= 0.35013 * (f_1*f_1)*(f_1*f_1);		
  out1_1 = input_1 + 0.3 * in1_1 + (1 - f_1) * out1_1; // Pole 1
  in1_1  = input_1;
  out2_1 = out1_1 + 0.3 * in2_1 + (1 - f_1) * out2_1;  // Pole 2
  in2_1  = out1_1;
  out3_1 = out2_1 + 0.3 * in3_1 + (1 - f_1) * out3_1;  // Pole 3
  in3_1  = out2_1;
  out4_1 = out3_1 + 0.3 * in4_1 + (1 - f_1) * out4_1;  // Pole 4
  in4_1  = out3_1;
	return TRIM_FILTER[VR_LEVEL[QFAC]]*out4_1;	
	};		

/////////////////////////////////	
//
// filter 
//		
float MoogVCF_2(float input_2, float f_2, float fb_2)
	{
  input_2 -= out4_2 * fb_2;
  input_2 *= 0.35013 * (f_2*f_2)*(f_2*f_2);		
  out1_2 = input_2 + 0.3 * in1_2 + (1 - f_2) * out1_2; // Pole 1
  in1_2  = input_2;
  out2_2 = out1_2 + 0.3 * in2_2 + (1 - f_2) * out2_2;  // Pole 2
  in2_2  = out1_2;
  out3_2 = out2_2 + 0.3 * in3_2 + (1 - f_2) * out3_2;  // Pole 3
  in3_2  = out2_2;
  out4_2 = out3_2 + 0.3 * in4_2 + (1 - f_2) * out4_2;  // Pole 4
  in4_2  = out3_2;
	return TRIM_FILTER[VR_LEVEL[QFAC]]*out4_2;	
	};			

/////////////////////////////////	
//
// filter 
//		
float MoogVCF_3(float input_3, float f_3, float fb_3)
	{
  input_3 -= out4_3 * fb_3;
  input_3 *= 0.35013 * (f_3*f_3)*(f_3*f_3);		
  out1_3 = input_3 + 0.3 * in1_3 + (1 - f_3) * out1_3; // Pole 1
  in1_3  = input_3;
  out2_3 = out1_3 + 0.3 * in2_3 + (1 - f_3) * out2_3;  // Pole 2
  in2_3  = out1_3;
  out3_3 = out2_3 + 0.3 * in3_3 + (1 - f_3) * out3_3;  // Pole 3
  in3_3  = out2_3;
  out4_3 = out3_3 + 0.3 * in4_3 + (1 - f_3) * out4_3;  // Pole 4
  in4_3  = out3_3;
	return TRIM_FILTER[VR_LEVEL[QFAC]]*out4_3;	
	};			
	
	
///////////////////////////////////////////////////////////////HPF FILTERS////////////////////////////////////////////////////


/////////////////////////////////	
//
// filter 
//		
float MoogHVCF_0(float input_0h, float f_0h, float fb_0h)
	{
	tmpin_0 = input_0h;	
  input_0h -= out4_0h * fb_0h;
  input_0h *= 0.35013 * (f_0h*f_0h)*(f_0h*f_0h);		
  out1_0h = input_0h + 0.3 * in1_0h + (1 - f_0h) * out1_0h; // Pole 1
  in1_0h  = input_0h;
  out2_0h = out1_0h + 0.3 * in2_0h + (1 - f_0h) * out2_0h;  // Pole 2
  in2_0h  = out1_0h;
  out3_0h = out2_0h + 0.3 * in3_0h + (1 - f_0h) * out3_0h;  // Pole 3
  in3_0h  = out2_0h;
  out4_0h = out3_0h + 0.3 * in4_0h + (1 - f_0h) * out4_0h;  // Pole 4
  in4_0h  = out3_0h;
  return tmpin_0-(TRIM_FILTER[VR_LEVEL[QFAC]]*out4_0h);
	};	

/////////////////////////////////	
//
// filter 
//		
float MoogHVCF_1(float input_1h, float f_1h, float fb_1h)
	{
	tmpin_1 = input_1h;	
  input_1h -= out4_1h * fb_1h;
  input_1h *= 0.35013 * (f_1h*f_1h)*(f_1h*f_1h);		
  out1_1h = input_1h + 0.3 * in1_1h + (1 - f_1h) * out1_1h; // Pole 1
  in1_1h  = input_1h;
  out2_1h = out1_1h + 0.3 * in2_1h + (1 - f_1h) * out2_1h;  // Pole 2
  in2_1h  = out1_1h;
  out3_1h = out2_1h + 0.3 * in3_1h + (1 - f_1h) * out3_1h;  // Pole 3
  in3_1h  = out2_1h;
  out4_1h = out3_1h + 0.3 * in4_1h + (1 - f_1h) * out4_1h;  // Pole 4
  in4_1h  = out3_1h;
  return tmpin_1-(TRIM_FILTER[VR_LEVEL[QFAC]]*out4_1h);
	};		

/////////////////////////////////	
//
// filter 
//		
float MoogHVCF_2(float input_2h, float f_2h, float fb_2h)
	{
	tmpin_2 = input_2h;	
  input_2h -= out4_2h * fb_2h;
  input_2h *= 0.35013 * (f_2h*f_2h)*(f_2h*f_2h);		
  out1_2h = input_2h + 0.3 * in1_2h + (1 - f_2h) * out1_2h; // Pole 1
  in1_2h  = input_2h;
  out2_2h = out1_2h + 0.3 * in2_2h + (1 - f_2h) * out2_2h;  // Pole 2
  in2_2h  = out1_2h;
  out3_2h = out2_2h + 0.3 * in3_2h + (1 - f_2h) * out3_2h;  // Pole 3
  in3_2h  = out2_2h;
  out4_2h = out3_2h + 0.3 * in4_2h + (1 - f_2h) * out4_2h;  // Pole 4
  in4_2h  = out3_2h;
  return tmpin_2-(TRIM_FILTER[VR_LEVEL[QFAC]]*out4_2h);
	};			

/////////////////////////////////	
//
// filter 
//		
float MoogHVCF_3(float input_3h, float f_3h, float fb_3h)
	{
	tmpin_3 = input_3h;	
  input_3h -= out4_3h * fb_3h;
  input_3h *= 0.35013 * (f_3h*f_3h)*(f_3h*f_3h);		
  out1_3h = input_3h + 0.3 * in1_3h+ (1 - f_3h) * out1_3h; // Pole 1
  in1_3h  = input_3h;
  out2_3h = out1_3h + 0.3 * in2_3h + (1 - f_3h) * out2_3h;  // Pole 2
  in2_3h  = out1_3h;
  out3_3h = out2_3h + 0.3 * in3_3h + (1 - f_3h) * out3_3h;  // Pole 3
  in3_3h  = out2_3h;
  out4_3h = out3_3h + 0.3 * in4_3h + (1 - f_3h) * out4_3h;  // Pole 4
  in4_3h  = out3_3h;
  return tmpin_3-(TRIM_FILTER[VR_LEVEL[QFAC]]*out4_3h);
	};		
	
	
	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



