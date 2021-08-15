% function Parameter_DSP
close all, clear all;
word_24s=[1:0.001:1000];
L=24000*16;
mu=-0.0089;
sigma=0.0379;
X_noise=[1:1:240000*6];%wgn(24000*16,1,0.1);%sqrt(sigma)*randn(L,1)+mu;
X_noise= X_noise/20;
% pink_noise = pinknoise(L,1);
BPF_Setting_Brammer4;
SW_MD=2;
Seect_Input=1;

%-------   data path
speech_G =1
noise_G=0.876%0.4396(0db)     0.62(-3dB)    0.876(-6dB)    1.24(-9dB)    
% 0.554 (-2 dB)    0.7 (-4 dB)   1.105 (-8 dB)
noise_G


% ------------- Control Path
% speech_G_cntrl=1;
% noise_G_cntrl=0.62;
%51.24 %0.876 %0.62 %0.4395 %1.24
 speech_G_cntrl =0; noise_G_cntrl=noise_G;

% speech_G_cntrl
% noise_G_cntrl
% path_file ='C:\Users\Biodynamics\OneDrive - University of Connecticut\thesis\Test Material\official test (Jan 2019)\Voice ( speech in quite)\test_1_org.wav'
 path_file='C:\Users\Biodynamics\OneDrive - University of Connecticut\thesis\Test Material\Official test(June 2020)\Speech_key_code\test_9_org.wav'
% path_file='C:\Users\Biodynamics\OneDrive - University of Connecticut\thesis\Test Material\Official Test (Jan 2020)\Speech_key_code\test_5_org.wav'%test_1(-1)_org.wav

%path_file='C:\Users\Biodynamics\OneDrive - University of Connecticut\thesis\Test Material\Official test(Oct 2019) jw\test_1_BEST_TARGET.wav'
% path_file='C:\Users\Biodynamics\OneDrive - University of Connecticut\thesis\Test Material\Official test(Oct 2019) jw1\test_3_org_new_Target_wideband.wav'
%path_file='C:\Users\Biodynamics\OneDrive - University of Connecticut\HPD_Modulation\0093_24k.wav'
% 1.093 SNR (-8) for isolated word
% 0.866 SNR (-6) for isolated word
% 1.50 SNR (-8) for whole pharas e 
% 1.21 SNR (-6) for whole pharase  
% 2.5 Snr (-8) for alarm signal
% 2 SNR (-6) for alarm signal
% 172
G_MV1=1;%1000/8;
added2PR=0;
powerPR=1;
W_G2=10;                            % the gain after modulation
sampling_rate_LPF1=12000;
sampling_rate_LPF2=12000;
sampling_rate=12000;
% powerLog=2;
downsample_value=sampling_rate/sampling_rate_LPF1;
Max_len=3600;%                     % window length of  Moving varinance or Avarage 
MA_LPF1_len =480*1%/downsample_value;
CF_LP1=8;FO_LPF1=512; MSA_LPF1=48; % Lowpass filter 1 in control path
MPR_LPF1=1;                        % Maximum passband filter
CF_LP2=90;FO_LPF2=512;MSA_LPF2=48; % Lowpass filter 2 in control path
MPR_LPF2=1;                        % Maximum passband filter
CF_LP3=16;FO_LPF3=512;MSA_LPF3=48; % Lowpass filter 3 in control path
MPR_LPF3=1;                        % Maximum passband filter


FO_BPF=512;                        % Bandpass filter

delay_cp=(FO_LPF1-FO_LPF2)/2;      % delay_cp = different delay between LPF1 and LPF2
delay_dp=(FO_LPF1/2)+(MA_LPF1_len/2)  %300             % delay_dp = different delay between input signal and  control path

W_LPF1=1;
const=1;%10
ch0=0;%const;
ch1=const;ch2=const;
ch3=const;ch4=const;
ch5=const;ch6=const;
ch7=const;ch8=const;
ch9=const;ch10=const;
ch11=const;ch12=const;
ch13=const;ch14=const;
ch15=const;ch16=const;

