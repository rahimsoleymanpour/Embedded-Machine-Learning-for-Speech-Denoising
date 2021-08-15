#include "dsk6713.h"
#include "dsk6713_aic23.h"
//#include "C6713dskinit.h"
//#include "DSPF_dp_fir_r2.h"
//-----------

//---------------------------
///
#include "stdlib.h"
#include "math.h"
//#include "bs2700.cof"
#include "LP1.cof"
#include "LP2.cof"
#include "ch1.cof"
#include "ch2.cof"
#include "ch3.cof"
#include "ch4.cof"
#include "ch5.cof"
#include "ch6.cof"
#include "ch7.cof"
#include "ch8.cof"
#include "ch9.cof"
#include "ch10.cof"
#include "ch11.cof"
#include "ch12.cof"
#include "ch13.cof"
#include "ch14.cof"
#include "ch15.cof"
#include "ch16.cof"
//#include "dsplib674x.h"

/*#include "ch16_tmp1.cof"
#include "ch16_tmp2.cof"
#include "ch16_tmp3.cof"
#include "ch16_tmp4.cof"*/



//#include "ch16_tmp1.cof"
//#include "ch16_tmp2.cof"
//#include "ch16_tmp1.cof"
//#include "ch16_tmp1.cof"







// Codec configuration settings
DSK6713_AIC23_Config config = { \
    0x0017,  /* 0 DSK6713_AIC23_LEFTINVOL  Left line input channel volume */ \
    0x0017,  /* 1 DSK6713_AIC23_RIGHTINVOL Right line input channel volume */\
    0x01f9,  /* 2 DSK6713_AIC23_LEFTHPVOL  Left channel headphone volume */  \
    0x01f9,  /* 3 DSK6713_AIC23_RIGHTHPVOL Right channel headphone volume */ \
    0x0011,  /* 4 DSK6713_AIC23_ANAPATH    Analog audio path control */      \
    0x0000,  /* 5 DSK6713_AIC23_DIGPATH    Digital audio path control */     \
    0x0000,  /* 6 DSK6713_AIC23_POWERDOWN  Power down control */             \
    0x0043,  /* 7 DSK6713_AIC23_DIGIF      Digital audio interface format */ \
    0x0001,  /* 8 DSK6713_AIC23_SAMPLERATE Sample rate control */            \
    0x0001   /* 9 DSK6713_AIC23_DIGACT     Digital interface activation */   \
};
//#define N 433786 //buffer size 300 sec

//-------------- Global index for delay array
int GIndex=0;
int jj;
#define Num 252000//buffer size 300 sec
long i;
float buffer_input[Num];
float buffer_output[Num];
float tem_output_ch2[Num];
#pragma DATA_SECTION(buffer_input,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(buffer_output,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(tem_output_ch2,".EXT_RAM") //buffer in ext memory
float yn = 0;				   //initialize filter's output
//---------- General parameter
#define FO_BP_GP 513
float dly_BP_GP[FO_BP_GP];
//----------- Moving Variance parameter
float SX1_lpf1[16],SX1_lpf2[16];
float SX2_lpf1[16],SX2_lpf2[16];

int new_index_lpf1[16],new_index_lpf2[16];
int old_index_lpf1[16],old_index_lpf2[16];




//---------- channel 1 parameter
#define DO_CH1 256
#define MV_length 2400
float output_lpf1_mv_ch1[MV_length];
float output_lpf2_mv_ch1[MV_length];

float dly_CH1_BP[FO_CH1_BP];
float dly_CH1_LP1[FO_CH1_LP1];
float dly_CH1_LP2[FO_CH1_LP1];

float delay_CH1_256[DO_CH1];
//---------- channel 2 parameter
#define DO_CH2 256
//float dly_CH2_BP[FO_CH2_BP];
float dly_CH2_LP[FO_CH2_LP];
float delay_CH2_256[DO_CH2];
/*#pragma DATA_SECTION(dly_CH2_BP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(dly_CH2_LP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(delay_CH2_256,".EXT_RAM") //buffer in ext memory*/
//---------- channel 3 parameter
#define DO_CH3 256
//float dly_CH3_BP[FO_CH3_BP];
float dly_CH3_LP[FO_CH3_LP];
float delay_CH3_256[DO_CH3];
// -------------------------------
//---------- channel 4 parameter
#define DO_CH4 256
//float dly_CH4_BP[FO_CH4_BP];
float dly_CH4_LP[FO_CH4_LP];
float delay_CH4_256[DO_CH4];
/*#pragma DATA_SECTION(dly_CH4_BP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(dly_CH4_LP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(delay_CH4_256,".EXT_RAM") //buffer in ext memory*/
// -------------------------------
//---------- channel 5 parameter
#define DO_CH5 256
//float dly_CH5_BP[FO_CH5_BP];
float dly_CH5_LP[FO_CH5_LP];
float delay_CH5_256[DO_CH5];
// -------------------------------
//---------- channel 6 parameter
#define DO_CH6 256
//float dly_CH6_BP[FO_CH6_BP];
float dly_CH6_LP[FO_CH6_LP];
float delay_CH6_256[DO_CH6];
/*#pragma DATA_SECTION(dly_CH6_BP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(dly_CH6_LP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(delay_CH6_256,".EXT_RAM") //buffer in ext memory*/
// -------------------------------
//---------- channel 7 parameter
#define DO_CH7 256
//float dly_CH7_BP[FO_CH7_BP];
float dly_CH7_LP[FO_CH7_LP];
float delay_CH7_256[DO_CH7];
// -------------------------------
//---------- channel 8 parameter
#define DO_CH8 256
//float dly_CH8_BP[FO_CH8_BP];
float dly_CH8_LP[FO_CH8_LP];
float delay_CH8_256[DO_CH8];
/*#pragma DATA_SECTION(dly_CH8_BP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(dly_CH8_LP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(delay_CH8_256,".EXT_RAM") //buffer in ext memory*/
//---------- channel 9 parameter
#define DO_CH9 256
//float dly_CH9_BP[FO_CH9_BP];
float dly_CH9_LP[FO_CH9_LP];
float delay_CH9_256[DO_CH9];
// -------------------------------
//---------- channel 10 parameter
#define DO_CH10 256
//float dly_CH10_BP[FO_CH10_BP];
float dly_CH10_LP[FO_CH10_LP];
float delay_CH10_256[DO_CH10];
/*#pragma DATA_SECTION(dly_CH10_BP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(dly_CH10_LP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(delay_CH10_256,".EXT_RAM") //buffer in ext memory*/
// -------------------------------
//---------- channel 11 parameter
#define DO_CH11 256
//float dly_CH11_BP[FO_CH11_BP];
float dly_CH11_LP[FO_CH11_LP];
float delay_CH11_256[DO_CH11];
// -------------------------------
//---------- channel 12 parameter
#define DO_CH12 256
//float dly_CH12_BP[FO_CH12_BP];
float dly_CH12_LP[FO_CH12_LP];
float delay_CH12_256[DO_CH12];
/*#pragma DATA_SECTION(dly_CH12_BP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(dly_CH12_LP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(delay_CH12_256,".EXT_RAM") //buffer in ext memory*/
//---------- channel 13 parameter
#define DO_CH13 256
//float dly_CH13_BP[FO_CH13_BP];
float dly_CH13_LP[FO_CH13_LP];
float delay_CH13_256[DO_CH13];
// -------------------------------
//---------- channel 14 parameter
#define DO_CH14 256
//float dly_CH14_BP[FO_CH14_BP];
float dly_CH14_LP[FO_CH14_LP];
float delay_CH14_256[DO_CH14];
/*#pragma DATA_SECTION(dly_CH14_BP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(dly_CH14_LP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(delay_CH14_256,".EXT_RAM") //buffer in ext memory*/
// -------------------------------
//---------- channel 15 parameter
#define DO_CH15 256
//float dly_CH15_BP[FO_CH15_BP];
float dly_CH15_LP[FO_CH15_LP];
float delay_CH15_256[DO_CH15];
// -------------------------------
//---------- channel 16 parameter
#define DO_CH16 256
//float dly_CH16_BP[FO_CH16_BP];
float dly_CH16_LP[FO_CH16_LP];
float delay_CH16_256[DO_CH16];
/*#pragma DATA_SECTION(dly_CH16_BP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(dly_CH16_LP,".EXT_RAM") //buffer in ext memory
#pragma DATA_SECTION(delay_CH16_256,".EXT_RAM") //buffer in ext memory*/
// -------------------------------


short data_out;



//#pragma DATA_SECTION(buffer,".EXT_RAM") //buffer in ext memory
float EV(float inData, float dly[],float filter_cof[], short filter_order, float delay_256[], short delay_order)
{
	float abs_inData;
	if (inData < 0)
		abs_inData=inData*(-1);//abs();
	else
		abs_inData=inData;

	//------------------- Start :lowpass filter -------------------
	short ii;
	dly[0] = abs_inData;     	 //newest input @ top of buffer
	float yn = 0;
	//initialize filter output
#pragma MUST_ITERATE(513,513,8)
#pragma UNROLL(8)
	for (ii = 0; ii< filter_order; ii++)
	   yn +=(filter_cof[ii]*dly[ii]);//y(n) += h(LP#,i)*x(n-i)
#pragma MUST_ITERATE(513,513,8)
#pragma UNROLL(8)
	for (ii = filter_order-1; ii > 0; ii--)        //starting @ bottom of buffer
	   dly[ii] = dly[ii-1];
	//------------------- End :lowpass filter -------------------
   float out_data= yn*delay_256[0];
    short j=0;
#pragma MUST_ITERATE(256,256,4)
#pragma UNROLL(4)

    for (j=0; j<delay_order-1; j++)
    {
    	delay_256[j]=delay_256[j+1];
    }
    delay_256[255]=inData;
	return out_data;
   // return yn;
}

float fir_smpl_circle_f32_EV( float sample, float* buffer_LPF1, const float* coeffs_LPF1, short order_LPF1,float* buffer_LPF2,
		     const float* coeffs_LPF2, short order_LPF2,float delay_256[], short delay_order, int* state_LPF1, int* state_LPF2,
			 float output_lpf1_mv[], float output_lpf2_mv[],float* SX1_lpf1, float* SX1_lpf2,float* SX2_lpf1, float* SX2_lpf2,
			 int* new_index_lpf1,int* new_index_lpf2, int* old_index_lpf1,int* old_index_lpf2)
                          /*(float inData, float dly[],float filter_cof[], short filter_order, float delay_256[], short delay_order)*/
{
 float out_data,accu_LPF1 = 0.0f,accu_LPF2 = 0.0f;

 //---------- Low pass filter 1 in control pass
 short j,i = order_LPF1 - 1;
 buffer_LPF1[*state_LPF1] = sample;
 if ( ++( *state_LPF1 ) >= order_LPF1 )
 {
     *state_LPF1 = 0;
 }
 for ( ; i >= 0; --i ) {
	 accu_LPF1 += buffer_LPF1[*state_LPF1] * coeffs_LPF1[i];
	 if ( ++( *state_LPF1 ) >= order_LPF1 )
		 *state_LPF1 = 0;
 }


 //----------- Low pass filter 2 in Control pass
 i = order_LPF2 - 1;
  buffer_LPF2[*state_LPF2] = sample;
  if ( ++( *state_LPF2 ) >= order_LPF2)
  {
      *state_LPF2 = 0;
  }
  for ( ; i >= 0; --i ) {
 	 accu_LPF2+= buffer_LPF2[*state_LPF2] * coeffs_LPF2[i];
 	 if ( ++( *state_LPF2) >= order_LPF2)
 		 *state_LPF2 = 0;
  }
  // --------------------- Moving Varience for LPF1
  (*SX1_lpf1)=(*SX1_lpf1)+sample-output_lpf1_mv[*old_index_lpf1];
  (*SX2_lpf1)=(*SX2_lpf1)+sample*sample-(output_lpf1_mv[*old_index_lpf1]*output_lpf1_mv[*old_index_lpf1]);
  float N=MV_length;
  float mv_lpf1=(N * (*SX2_lpf1) - ((*SX1_lpf1) * (*SX1_lpf1))) / (N * (N - 1));
  if ( ++( *old_index_lpf1 ) >= MV_length )
  {
      *old_index_lpf1 = 0;
  }
  if ( ++( *new_index_lpf1 ) >= MV_length )
    {
        *new_index_lpf1 = 0;
    }
  output_lpf1_mv[*new_index_lpf1]=accu_LPF1;
  // --------------------- Moving Varience for LPF2
   (*SX1_lpf2)=(*SX1_lpf2)+sample-output_lpf2_mv[*old_index_lpf2];
   (*SX2_lpf2)=(*SX2_lpf2)+sample*sample-(output_lpf2_mv[*old_index_lpf2]*output_lpf2_mv[*old_index_lpf2]);
  // float N=MV_length;
   float mv_lpf2=(N * (*SX2_lpf2) - ((*SX1_lpf2) * (*SX1_lpf2))) / (N * (N - 1));
   if ( ++( *old_index_lpf2 ) >= MV_length )
   {
       *old_index_lpf2 = 0;
   }
   if ( ++( *new_index_lpf2 ) >= MV_length )
     {
         *new_index_lpf2 = 0;
     }
   output_lpf1_mv[*new_index_lpf2]=accu_LPF2;

	//------------------- End :lowpass filter -------------------
out_data= accu_LPF1*(mv_lpf1/mv_lpf2)*delay_256[0];
/*#pragma MUST_ITERATE(256,256,4)
#pragma UNROLL(4)*/
 for (j=0; j<delay_order-1; j++)
 {
 	delay_256[j]=delay_256[j+1];
 }
 delay_256[255]=sample;
	return out_data;
// return accu;
}

float BPF(float inData, float dly[],const float filter_cof[], short filter_order){

	short ii=0;
	dly[0] = inData;     	 //newest input @ top of buffer
	float yn = 0;
/*#pragma MUST_ITERATE(513,513,4)
#pragma UNROLL(4)*/
	for (ii = 0; ii< filter_order; ii++)
		yn += (filter_cof[ii]*dly[ii]);  //y(n) += h(i)* x(n-i)
//#pragma MUST_ITERATE(513,513,4)
	for (ii = filter_order-1; ii > 0; ii--)  //starting @ end of buffer
	    dly[ii] = dly[ii-1];      //update delays with data move

	return yn;

}
float fir_smpl_circle_f32( float sample, float* buffer, const float* coeffs, short order , int* state )
                       /*( float inData, float dly[],float filter_cof[], short filter_order)*/
{
 /*float accu = 0.0f;
 short i = order - 1;
 buffer[*state] = sample;
 if ( ++( *state ) >= order ) {
 *state = 0;
 }
 for ( ; i >= 0; --i ) {
 accu += buffer[*state] * coeffs[i];
 if ( ++( *state ) >= order )
 *state = 0;
 }
 return accu;*/
	 float accu = 0.0f;
	 int b= (*state);
	 short i = order - 1;
	 buffer[b] = sample;
	 /*if ( ++( *state ) >= order ) {
		 *state = 0;
	 }*/
	 for (i=0; i < order; i++ ) {
		 accu += buffer[b] * coeffs[i];
		 b++;
		 if ( b >= order )
			 b=0;
	 }
	 return accu;
}
//**********************version 2**********************************************

float EV2(int gindex, float inData[], float dly1[],const float filter_cof1[], short filter_order1,  float delay_256_1[], short delay_order1,
		 float dly2[],const float filter_cof2[],const short filter_order2, float delay_256_2[],const short delay_order2){
	float abs_inData[2];
	if (inData[0] < 0)
		abs_inData[0]=inData[0]*(-1);//abs();
	else
		abs_inData[0]=inData[0];
	if (inData[1] < 0)
			abs_inData[1]=inData[1]*(-1);//abs();
		else
			abs_inData[1]=inData[1];

	//------------------- Start :lowpass filter -------------------
	int ii;
	dly1[0] = abs_inData[0];     	 //newest input @ top of buffer
	dly2[0] = abs_inData[1];     	 //newest input @ top of buffer
	float yn1 = 0;
	float yn2 = 0;
	//initialize filter output
#pragma MUST_ITERATE(513,513,4)
#pragma UNROLL(2)
	for (ii = 0; ii< filter_order1; ii++)
	{

		yn1 +=(filter_cof1[ii]*dly1[ii]);//y(n) += h(LP#,i)*x(n-i)
	    yn2 +=(filter_cof2[ii]*dly2[ii]);//y(n) += h(LP#,i)*x(n-i)
	}
#pragma MUST_ITERATE(513,513,8)
#pragma UNROLL(4)
	for (ii = filter_order1-1; ii > 0; ii--)        //starting @ bottom of buffer
	{
		dly1[ii] = dly1[ii-1];
	    dly2[ii] = dly2[ii-1];
	}
	//------------------- End :lowpass filter -------------------
   float out_data[2];
   out_data[0]= yn1*delay_256_1[0];
   out_data[1]= yn2*delay_256_2[0];

    short j=0;
#pragma MUST_ITERATE(255,255,8)
#pragma UNROLL(4)
    for (j=0; j<delay_order1-1; j++)
    {
    	delay_256_1[j]=delay_256_1[j+1];
    	delay_256_2[j]=delay_256_2[j+1];
    }
    delay_256_1[255]=inData[0];
    delay_256_2[255]=inData[1];

	return (out_data[0]+out_data[1]);
   // return yn;
}
void BPF2(float * restrict dly1 ,const float * restrict filter_cof1, short filter_order1,
		               const float * restrict filter_cof2, short filter_order2,
		                 float * restrict yn){

	short ii=0;
	//dly1[0] = inData;     	 //newest input @ top of buffer
	//dly2[0] = inData;
	//float yn[2];
	//yn[0] = fircasmfunc(dly1,filter_cof1,filter_order1);
	//yn[0] = fircasmfunc(dly1,filter_cof1,filter_order1);

	yn[0]=0; yn[1] = 0;
#pragma MUST_ITERATE(513,513,8)
#pragma UNROLL(4)
	for (ii = 0; ii< filter_order1; ii++)
	{	yn[0] += (filter_cof1[ii]*dly1[ii]);  //y(n) += h(i)* x(n-i)
    	yn[1] += (filter_cof2[ii]*dly1[ii]);  //y(n) += h(i)* x(n-i)
	}

}

// -------------------------------------------------------


void intiatial_channel(short FO_BP, float dly_LP[],int FO_LP,float delay_256[],int DO_CH)
{
		int p;
			/*for(p=0;p<FO_BP;p++)
				{
					dly_BP[p]= 0;
				}*/
			for(p=0;p<FO_LP;p++)
				{
					dly_LP[p]= 0;
				}
			for(p=0;p<DO_CH;p++)
				{
				    delay_256[p]= 0;
				}
			return;
}
void main()
{
	DSK6713_AIC23_CodecHandle hCodec;
//	comm_intr();
	DSK6713_init();
	hCodec = DSK6713_AIC23_openCodec(0, &config);
	// Set  frequency to 24khz
	DSK6713_AIC23_setFreq(hCodec, DSK6713_AIC23_FREQ_24KHZ);
	// Initialize filter
	intiatial_channel(FO_CH1_BP,dly_CH1_LP1,FO_CH1_LP1,delay_CH1_256,DO_CH1);
	intiatial_channel(FO_CH2_BP,dly_CH2_LP,FO_CH2_LP,delay_CH2_256,DO_CH2);
	intiatial_channel(FO_CH3_BP,dly_CH3_LP,FO_CH3_LP,delay_CH3_256,DO_CH3);
	intiatial_channel(FO_CH4_BP,dly_CH4_LP,FO_CH4_LP,delay_CH4_256,DO_CH4);
	intiatial_channel(FO_CH5_BP,dly_CH5_LP,FO_CH5_LP,delay_CH5_256,DO_CH5);
	intiatial_channel(FO_CH6_BP,dly_CH6_LP,FO_CH6_LP,delay_CH6_256,DO_CH6);
	intiatial_channel(FO_CH7_BP,dly_CH7_LP,FO_CH7_LP,delay_CH7_256,DO_CH7);
	intiatial_channel(FO_CH8_BP,dly_CH8_LP,FO_CH8_LP,delay_CH8_256,DO_CH8);
	intiatial_channel(FO_CH9_BP,dly_CH9_LP,FO_CH9_LP,delay_CH9_256,DO_CH9);
	intiatial_channel(FO_CH10_BP,dly_CH10_LP,FO_CH10_LP,delay_CH10_256,DO_CH10);
	intiatial_channel(FO_CH11_BP,dly_CH11_LP,FO_CH11_LP,delay_CH11_256,DO_CH11);
	intiatial_channel(FO_CH12_BP,dly_CH12_LP,FO_CH12_LP,delay_CH12_256,DO_CH12);
	intiatial_channel(FO_CH13_BP,dly_CH13_LP,FO_CH13_LP,delay_CH13_256,DO_CH13);
	intiatial_channel(FO_CH14_BP,dly_CH14_LP,FO_CH14_LP,delay_CH14_256,DO_CH14);
	intiatial_channel(FO_CH16_BP,dly_CH16_LP,FO_CH16_LP,delay_CH16_256,DO_CH16);
	intiatial_channel(FO_CH15_BP,dly_CH15_LP,FO_CH15_LP,delay_CH15_256,DO_CH15);

   //------------------ modify
	int kk;
	for(kk=0;kk<FO_BP_GP;kk++)
	{
		dly_BP_GP[kk]=0;
	}
   //-------------------end : modification ------------
	float sum,yn,yn1,yn2;
	float in_filter;
	Int16 OUT_L, OUT_R;
	float BPF_out_2[2];
    float outBbuf[3];
    int i_OutBuf;
	Uint32 IN_L;
	Int16 tmpp;
    for(i_OutBuf=0;i_OutBuf<3;i_OutBuf++)
    {
    	outBbuf[i_OutBuf]=0;
    }
    int state=0;
    int *GI_filter;// global index for filter
    *GI_filter=0;
	for (i=0;i<Num;i++)
	{
       //===================       State 0 : filtering
		if (state==0)
		{
			while (!DSK6713_AIC23_read(hCodec, &IN_L));
			 //Feeding the input directly to output you can add effects here
			OUT_L = IN_L;
			OUT_R = IN_L;
			sum=0;
			in_filter= buffer_input[i];//*0.5;
			sum=0;

			 //-------------------  channel 1
		    yn1=fir_smpl_circle_f32(in_filter,dly_CH1_BP, FC_CH1_BP, FO_CH1_BP,GI_filter);
		     buffer_output[i]=yn1;

			yn=fir_smpl_circle_f32_EV(yn1, dly_CH1_LP1, FC_CH1_LP1, FO_CH1_LP1,dly_CH1_LP2, FC_CH1_LP2, FO_CH1_LP2,
					                  delay_CH1_256, DO_CH1,GI_filter, GI_filter,
						              output_lpf1_mv_ch1, output_lpf2_mv_ch1,&SX1_lpf1[0], &SX1_lpf2[0],&SX2_lpf1[0],
									  &SX2_lpf2[0],&new_index_lpf1[0],&new_index_lpf2[0],&old_index_lpf1[0],&old_index_lpf2[0]);

			//yn2=BPF(in_filter,dly_CH1_LP1, FC_CH1_BP, FO_CH1_BP);
			tem_output_ch2[i]=yn;
			(*GI_filter)++;
			if ( ++( *GI_filter ) >= FO_CH1_BP ){
			 *GI_filter = 0;
			 }

		//	dly_BP_GP[0] = in_filter;     	 //newest input @ top of buffer

		/*	BPF2(dly_BP_GP,FC_CH1_BP, FO_CH1_BP,FC_CH2_BP, FO_CH2_BP,BPF_out_2);
			yn=EV2(i,BPF_out_2, dly_CH1_LP1,FC_CH1_LP1,FO_CH1_LP1, delay_CH1_256,DO_CH1, dly_CH1_LP1,FC_CH1_LP1,FO_CH1_LP1, delay_CH2_256,DO_CH2);*/
			/*sum=sum+yn;

			BPF2( dly_BP_GP,FC_CH3_BP, FO_CH3_BP,FC_CH4_BP, FO_CH4_BP,BPF_out_2);
			yn=EV2(i,BPF_out_2, dly_CH3_LP,FC_CH3_LP,FO_CH3_LP, delay_CH3_256,DO_CH3, dly_CH4_LP,FC_CH4_LP,FO_CH4_LP, delay_CH4_256,DO_CH4);
			sum=sum+yn;

			 BPF2(dly_BP_GP,FC_CH5_BP, FO_CH5_BP,FC_CH6_BP, FO_CH6_BP,BPF_out_2);
			 yn=EV2(i,BPF_out_2, dly_CH5_LP,FC_CH5_LP,FO_CH5_LP, delay_CH5_256,DO_CH5, dly_CH6_LP,FC_CH6_LP,FO_CH6_LP, delay_CH6_256,DO_CH6);
			 sum=sum+yn;
			 BPF2( dly_BP_GP,FC_CH7_BP, FO_CH7_BP,FC_CH8_BP, FO_CH8_BP,BPF_out_2);
			yn=EV2(i,BPF_out_2,dly_CH7_LP,FC_CH7_LP,FO_CH7_LP, delay_CH7_256,DO_CH7, dly_CH8_LP,FC_CH8_LP,FO_CH8_LP, delay_CH8_256,DO_CH8);
			 sum=sum+yn;
			 BPF2( dly_BP_GP,FC_CH9_BP, FO_CH9_BP,FC_CH10_BP, FO_CH10_BP,BPF_out_2);
			 yn=EV2(i,BPF_out_2,dly_CH9_LP,FC_CH9_LP,FO_CH9_LP, delay_CH9_256,DO_CH9, dly_CH10_LP,FC_CH10_LP,FO_CH10_LP, delay_CH10_256,DO_CH10);
			 sum=sum+yn;
			 BPF2(dly_BP_GP,FC_CH11_BP, FO_CH11_BP,FC_CH12_BP, FO_CH12_BP,BPF_out_2);
			 yn=EV2(i,BPF_out_2, dly_CH11_LP,FC_CH11_LP,FO_CH11_LP, delay_CH11_256,DO_CH11, dly_CH12_LP,FC_CH12_LP,FO_CH12_LP, delay_CH12_256,DO_CH12);
			 sum=sum+yn;
			 BPF2(dly_BP_GP,FC_CH13_BP, FO_CH13_BP,FC_CH14_BP, FO_CH14_BP,BPF_out_2);
			 yn=EV2(i,BPF_out_2, dly_CH13_LP,FC_CH13_LP,FO_CH13_LP, delay_CH13_256,DO_CH13, dly_CH14_LP,FC_CH14_LP,FO_CH14_LP, delay_CH14_256,DO_CH14);
			 sum=sum+yn;

			 BPF2(dly_BP_GP,FC_CH15_BP, FO_CH15_BP,FC_CH16_BP, FO_CH16_BP,BPF_out_2);
			 yn=EV2(i,BPF_out_2,dly_CH15_LP,FC_CH15_LP,FO_CH15_LP, delay_CH15_256,DO_CH15, dly_CH16_LP,FC_CH16_LP,FO_CH16_LP, delay_CH16_256,DO_CH16);
			 sum=sum+yn;*/

			 outBbuf[1]=in_filter;//sum*10 ;
			 //buffer_output[i]=outBbuf[1];
			 state = 1;
			 outBbuf[2]=(outBbuf[0]+outBbuf[1])/2;
			 tmpp= (Int16)(outBbuf[1]*32768.0f);
			 while (!DSK6713_AIC23_write(hCodec,/*tmpp*/OUT_L));
			 while (!DSK6713_AIC23_write(hCodec,/*tmpp*/OUT_R));

             int ii;
             #pragma MUST_ITERATE(513,513,4)
             #pragma UNROLL(4)
			 for (ii = FO_BP_GP-1; ii > 0; ii--)  //starting @ end of buffer
			 dly_BP_GP[ii] = dly_BP_GP[ii-1];

		}
		else if (state==1)
		{
			 state = 0;
			 //buffer_output[i]=outBbuf[2];
			 outBbuf[0]=outBbuf[1];
			 tmpp= (Int16)(outBbuf[2]*32768.0f);
			 //DSK6713_AIC23_write(hCodec,OUT_L/*tmpp*/);
		}

	}

}
