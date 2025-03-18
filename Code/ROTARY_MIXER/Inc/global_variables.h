/////////////////////////////////////////////////////////////////////////////////////////////////
// 	global variables
//	product: Rotary Mixer
//
//
//
//
//	Author: Anatska Andrei
//
/////////////////////////////////////////////////////////////////////////////////////////////////

					
int16_t MASTER_BUS[2];
uint8_t devider = 0;
int16_t CH2IN[2];
int16_t CH1IN[2];
float CH1POSTFILTER[2];
float CH2POSTFILTER[2];
int16_t HP_BUS[2];
uint8_t BUTTON_K1_pressed = 0;
uint8_t BUTTON_K2_pressed = 0;
uint8_t TIMER_DIV = 0;
uint8_t i = 0;
int16_t currcrc, prewcrc;


uint8_t CUE_NUM = 0;


/* ADCs and VR ---------------------------------------------------------*/			
uint8_t VR_LEVEL[6] = {0};	
uint8_t previous_VR_LEVEL[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};	
uint32_t ADC_TEMP[6];
uint16_t ADC_center[6];	
uint32_t adc_tmp_mux;

#define	ADC_HYSTERESIS 	21		

#define POT_VCF0 ADC_CHANNEL_3
#define POT_VCF1 ADC_CHANNEL_5
#define POT_VOL0 ADC_CHANNEL_4
#define POT_VOL1 ADC_CHANNEL_7
#define POT_QFAC ADC_CHANNEL_0
#define POT_CUES ADC_CHANNEL_1

#define ADC_MAX			4071
#define ADC_MIN			62
#define mux_divider	251

#define VCF0 0
#define VCF1 1
#define VOL0 2
#define VOL1 3
#define QFAC 4
#define CUES 5

uint32_t temp_time;
char U_TX_DATA[64] = {0};	

uint8_t clip_ch1_cnt = 0;
uint8_t clip_ch2_cnt = 0;

///////////////////////////////////////LEVEL METER
//uint8_t previous_levelL = 0;
//uint8_t previous_levelR = 0;
//uint8_t previous_peak_L = 0;
//uint8_t previous_peak_R = 0;
uint16_t  lvl_tempL = 0;
uint16_t  lvl_tempR = 0;
uint16_t  lvl_actualL = 0;
uint16_t  lvl_actualR = 0;
uint16_t actualL_temp = 0;
uint16_t actualR_temp = 0;
//uint16_t integr_timeL = 0;						//	times for level meter
//uint16_t integr_time_peakL = 0;				//
//uint16_t integr_timeR = 0;						//
//uint16_t integr_time_peakR = 0;				//
//uint16_t time_clip_L = 0;							//
//uint16_t time_clip_R = 0;							//
//uint8_t count_clip_sample_LH = 0;
//uint8_t count_clip_sample_LL = 0;
//uint8_t count_clip_sample_RH = 0;
//uint8_t count_clip_sample_RL = 0;
//uint8_t CLIPPING_L = 0;
//uint8_t CLIPPING_R = 0;
//uint8_t need_unshow_clip_R = 0;
//uint8_t need_unshow_clip_L = 0;







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				
																																									