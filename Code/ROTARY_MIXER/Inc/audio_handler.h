/////////////////////////////////////////////////////////////////////////////////////////////////
// Audio processing handler 44100Hz
//	product: XDJX
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
		
		
//	if(i<19)
//		{
//		i++;	
//		}	
//	else
//		{
//		i = 0;	
//		}		
	
	if((VR_LEVEL[VCF0])<128)
		{
		CH1POSTFILTER[0] = MoogVCF_0(CH1IN[0], fcutoff_0, resonanse_0);
		CH1POSTFILTER[1] = MoogVCF_1(CH1IN[1], fcutoff_0, resonanse_0);	
		}	
	else
		{
		CH1POSTFILTER[0] = CH1IN[0];
		CH1POSTFILTER[1] = CH1IN[1];		
		}		
		
	if((VR_LEVEL[VCF1])<128)
		{
		CH2POSTFILTER[0] = MoogVCF_2(CH2IN[0], fcutoff_1, resonanse_1);
		CH2POSTFILTER[1] = MoogVCF_3(CH2IN[1], fcutoff_1, resonanse_1);	
		}	
	else
		{
		CH2POSTFILTER[0] = CH2IN[0];
		CH2POSTFILTER[1] = CH2IN[1];		
		}
		
	MASTER_BUS[0] = (int)(CH1POSTFILTER[0]*VR_LIN[VR_LEVEL[VOL0]]+CH2POSTFILTER[0]*VR_LIN[VR_LEVEL[VOL1]]);
	MASTER_BUS[1] = (int)(CH1POSTFILTER[1]*VR_LIN[VR_LEVEL[VOL0]]+CH2POSTFILTER[1]*VR_LIN[VR_LEVEL[VOL1]]);
		
	if(CUE_NUM==0)				//CH1
		{
		HP_BUS[0] = CH1IN[0];
		HP_BUS[1] = CH1IN[1];	
		}			
	else if(CUE_NUM==1)	//CH1+M
		{
		HP_BUS[0] = (int)(0.5*CH1IN[0]+MASTER_BUS[0]);
		HP_BUS[1] = (int)(0.5*CH1IN[1]+MASTER_BUS[1]);
		}		
	else if(CUE_NUM==2)	//MASTER
		{
		HP_BUS[0] = MASTER_BUS[0];
		HP_BUS[1] = MASTER_BUS[1];	
		}					
	else if(CUE_NUM==3)	//CH2+M
		{
		HP_BUS[0] = (int)(0.5*CH2IN[0]+MASTER_BUS[0]);
		HP_BUS[1] = (int)(0.5*CH2IN[1]+MASTER_BUS[1]);
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
	//tmpin_0 = input_0;	
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
  //return tmpin_0-(2.5*out4_0);
	return 2.2*out4_0;	
	}	

/////////////////////////////////	
//
// filter 
//		
float MoogVCF_1(float input_1, float f_1, float fb_1)
	{
	//tmpin_1 = input_1;	
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
  //return tmpin_1-(2.5*out4_1);
	return 2.2*out4_1;	
	}		

/////////////////////////////////	
//
// filter 
//		
float MoogVCF_2(float input_2, float f_2, float fb_2)
	{
	//tmpin_2 = input_2;	
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
  //return tmpin_2-(2.5*out4_2);
	return 2.2*out4_2;	
	}			

/////////////////////////////////	
//
// filter 
//		
float MoogVCF_3(float input_3, float f_3, float fb_3)
	{
	//tmpin_3 = input_3;	
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
  //return tmpin_3-(2.5*out4_3);
	return 2.2*out4_3;	
	}			
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



