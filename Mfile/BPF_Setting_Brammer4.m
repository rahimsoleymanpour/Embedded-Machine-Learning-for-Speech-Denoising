% start over
% filters #41 to #56

sampling_rate=12000;
FO_BPF=512;                        % Bandpass filter
%% lower frequency band
% 
% CF1_ch1=129; CF2_ch1=191;
% CF1_ch2=202 ; CF2_ch2=278 ;
% CF1_ch3=289 ; CF2_ch3=381;
% CF1_ch4=390; CF2_ch4=500;
% CF1_ch5=510; CF2_ch5=640;
% 
% CF1_ch6=648; CF2_ch6=802;
% CF1_ch7=809; CF2_ch7=991;
% CF1_ch8=992; CF2_ch8=1208;
% CF1_ch9=1208; CF2_ch9=1462;
% 
% CF1_ch10=1466; CF2_ch10=1764;
% CF1_ch11=1764; CF2_ch11=2116;
% CF1_ch12=2118; CF2_ch12=2532;
% CF1_ch13=2532; CF2_ch13=3018;
% 
% CF1_ch14=3019; CF2_ch14=3591;
% CF1_ch15=3593; CF2_ch15=4267;
% CF1_ch16=4269; CF2_ch16=5061;

%% Middle frequency band

% % CF1_ch0=129; CF2_ch0=191;
CF1_ch1=202 ; CF2_ch1=278;
CF1_ch2=289 ; CF2_ch2=381;
CF1_ch3=390; CF2_ch3=500;
CF1_ch4=510; CF2_ch4=640;

% CF1_ch1=129 ; CF2_ch1=381;
% CF1_ch2=390 ; CF2_ch2=640;
% CF1_ch3=648; CF2_ch3=991;
% CF1_ch4=992; CF2_ch4=1462;

CF1_ch5=648; CF2_ch5=802;
CF1_ch6=809; CF2_ch6=991;
CF1_ch7=992; CF2_ch7=1208;
CF1_ch8=1208; CF2_ch8=1462;

CF1_ch9=1466; CF2_ch9=1764;
CF1_ch10=1764; CF2_ch10=2116;
CF1_ch11=2118; CF2_ch11=2532;
CF1_ch12=2532; CF2_ch12=3018;

CF1_ch13=3019; CF2_ch13=3591;
CF1_ch14=3593; CF2_ch14=4267;
CF1_ch15=4269; CF2_ch15=5061;
CF1_ch16=5064; CF2_ch16=5996;

CF2(1)=CF2_ch1;
CF2(2)=CF2_ch2;
CF2(3)=CF2_ch3;
CF2(4)=CF2_ch4;

CF2(5)=CF2_ch5;
CF2(6)=CF2_ch6;
CF2(7)=CF2_ch7;
CF2(8)=CF2_ch8;

CF2(9)=CF2_ch9;
CF2(10)=CF2_ch10;
CF2(11)=CF2_ch11;
CF2(12)=CF2_ch12;

CF2(13)=CF2_ch13;
CF2(14)=CF2_ch14;
CF2(15)=CF2_ch15;
CF2(16)=CF2_ch16;
CF1(1)=CF1_ch1;
CF1(2)=CF1_ch2;
CF1(3)=CF1_ch3;
CF1(4)=CF1_ch4;

CF1(5)=CF1_ch5;
CF1(6)=CF1_ch6;
CF1(7)=CF1_ch7;
CF1(8)=CF1_ch8;

CF1(9)=CF1_ch9;
CF1(10)=CF1_ch10;
CF1(11)=CF1_ch11;
CF1(12)=CF1_ch12;

CF1(13)=CF1_ch13;
CF1(14)=CF1_ch14;
CF1(15)=CF1_ch15;
CF1(16)=CF1_ch16;
% 
 %% upper frequency band
% CF1_ch1=220 ; CF2_ch1=297;
% CF1_ch2=298; CF2_ch2=389;
% CF1_ch3=390; CF2_ch3=496;
% CF1_ch4=497; CF2_ch4=623;
% CF1_ch5=624; CF2_ch5=772;
% CF1_ch6=773; CF2_ch6=947;
% CF1_ch7=948; CF2_ch7=1152;
% CF1_ch8=1153; CF2_ch8=1397;
% CF1_ch9=1398; CF2_ch9=1682;
% CF1_ch10=1683; CF2_ch10=2018;
% CF1_ch11=2019; CF2_ch11=2411;
% CF1_ch12=2412; CF2_ch12=2868;
% CF1_ch13=2869; CF2_ch13=3416;
% CF1_ch14=3417; CF2_ch14=4053;
% CF1_ch15=4054; CF2_ch15=4783;
% CF1_ch16=4783; CF2_ch16=5608;