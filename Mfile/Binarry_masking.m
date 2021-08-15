close all
clear BPF_data data_modulated Data_env_lpf1 data_MV2 data_MV1 path2_MA data2subCh new_BPF_data gain_data2subCH data_data2subCh data2subCh PR PR1;
clear modif_Noisy_segData_PR  modif_Noisy_segData_SNR Decisian_table
Sampling_rate=12000;
Fs_AS=Sampling_rate;
Num_iter=1;
env_fun=2;
File_additive_info="Niosh_SNR(0db)_M4_";%_SSN(-8)_Coff(0.5)_step(0.01)_test9_";%WinLeng(480)_Coff(0.3)_MA_480(2)";
rejected_coff=0.5;
prev_thre=1;
step_prev_thre=0.01;

weight_conv=[0.5 0.5 0.5 0.5 0.5 0.75 0.75 0.75 0.75 0.75 0.75 0.75 0.75 1 1 1 1 1 1 1 1 1 0.5 0.75 1];
BPF_data(:,1)=BPF_ch1;BPF_data(:,2)=BPF_ch2;BPF_data(:,3)=BPF_ch3;BPF_data(:,4)=BPF_ch4;BPF_data(:,5)=BPF_ch5;
BPF_data(:,6)=BPF_ch6;BPF_data(:,7)=BPF_ch7;BPF_data(:,8)=BPF_ch8;BPF_data(:,9)=BPF_ch9;BPF_data(:,10)=BPF_ch10;
BPF_data(:,11)=BPF_ch11;BPF_data(:,12)=BPF_ch12;BPF_data(:,13)=BPF_ch13;BPF_data(:,14)=BPF_ch14;BPF_data(:,15)=BPF_ch15;BPF_data(:,16)=BPF_ch16;

data_BPF_data(:,1)=data_BPF_ch1;data_BPF_data(:,2)=data_BPF_ch2;data_BPF_data(:,3)=data_BPF_ch3;data_BPF_data(:,4)=data_BPF_ch4;data_BPF_data(:,5)=data_BPF_ch5;
data_BPF_data(:,6)=data_BPF_ch6;data_BPF_data(:,7)=data_BPF_ch7;data_BPF_data(:,8)=data_BPF_ch8;data_BPF_data(:,9)=data_BPF_ch9;data_BPF_data(:,10)=data_BPF_ch10;
data_BPF_data(:,11)=data_BPF_ch11;data_BPF_data(:,12)=data_BPF_ch12;data_BPF_data(:,13)=data_BPF_ch13;data_BPF_data(:,14)=data_BPF_ch14;data_BPF_data(:,15)=data_BPF_ch15;data_BPF_data(:,16)=data_BPF_ch16;

Pure_BPF_data(:,1)=Pur_speech_ch1;Pure_BPF_data(:,2)=Pur_speech_ch2;Pure_BPF_data(:,3)=Pur_speech_ch3;Pure_BPF_data(:,4)=Pur_speech_ch4;Pure_BPF_data(:,5)=Pur_speech_ch5;
Pure_BPF_data(:,6)=Pur_speech_ch6;Pure_BPF_data(:,7)=Pur_speech_ch7;Pure_BPF_data(:,8)=Pur_speech_ch8;Pure_BPF_data(:,9)=Pur_speech_ch9;Pure_BPF_data(:,10)=Pur_speech_ch10;
Pure_BPF_data(:,11)=Pur_speech_ch11;Pure_BPF_data(:,12)=Pur_speech_ch12;Pure_BPF_data(:,13)=Pur_speech_ch13;Pure_BPF_data(:,14)=Pur_speech_ch14;Pure_BPF_data(:,15)=Pur_speech_ch15;Pure_BPF_data(:,16)=Pur_speech_ch16;

data2subCh=BPF_data;
data_data2subCh=data_BPF_data;
Sampling_rate=12000;
Fs = Sampling_rate;
filtertype = 'FIR';
Fpass = CF_LP1;
Forder = FO_LPF1;
Rp = MPR_LPF1;
Astop = MSA_LPF1;
LPF1 = dsp.LowpassFilter('SampleRate',Fs,...
                             'FilterType',filtertype,...
                             'PassbandFrequency',Fpass,...
                             'DesignForMinimumOrder',false,...
                             'FilterOrder',Forder,...
                             'PassbandRipple',Rp,...
                             'StopbandAttenuation',Astop);
LPF11 = dsp.LowpassFilter('SampleRate',Fs,...
                             'FilterType',filtertype,...
                             'PassbandFrequency',Fpass,...
                             'DesignForMinimumOrder',false,...
                             'FilterOrder',Forder,...
                             'PassbandRipple',Rp,...
                             'StopbandAttenuation',Astop);

  Fs=12000;
  font_size_label=12;
  index_next_quart=12;
  lineWidth_v=2;
  MA_LPF_16_lenght=2;
  movavgWindow_480s = dsp.MovingAverage(MA_LPF_16_lenght);
  movavgWindow_360s = dsp.MovingAverage(360);
  MA_LPF_8_lenght=960;
  movavgWindow_960s = dsp.MovingAverage(MA_LPF_8_lenght);
  
  thed_value=[0.65 0.60 0.55 0.55  ...
                    0.50 0.45 0.45 0.35 ...
                    0.40 0.35 0.32 0.30 ...
                    0.30 0.30 0.25 0.25];
clear Snr_seg modif_Noisy_segData modif_Noisy   

th_shift_level=1.2;%th_shift_level*;
itr=1
%     set(0,'defaultfigurecolor','w');
%     fig=figure('units','normalized','outerposition',[0 0 1 1],'DefaultAxesFontSize',10);
    for index=1:16 
         f=figure('units','normalized','outerposition',[0 0 1 1]);
        % segmnetation
         thd_dB= -30;
         len_seg=480;%12000*0.030;
         len_orgData=length(data_BPF_data);
         len_segData=floor(len_orgData/len_seg);
         clear Noisy_segData Clean_segData
        
         
         %% PR based modulation  
         
         data_BPF_data(1:end,index); data2subCh(1:end,index);
         Data_env(:,index)= abs(data_BPF_data(1:end,index));
         killme_data_env_HHH(:,index,itr)= MovingTrend(Data_env(:,index),0,0,1,6000,6000);    %2-100 Hz
         killme_data_env_2_16Hz(:,index,itr)= movavgWindow_480s(LPF1(killme_data_env_HHH(:,index,itr)));%movavgWindow_360s(movavgWindow_480s(killme_data_env_HHH(:,index,itr)));%%2-16 Hz
         %movavgWindow_960s
         killme_data_env_2_16Hz_afterMA_960(:,index,itr)= movavgWindow_960s(killme_data_env_2_16Hz(:,index,itr));%2Hz_16Hz
%          killme_data_env_2_16Hz_afterMA_960_v2(:,index,itr)=killme_data_env_2_16Hz_afterMA_960(:,index,itr);
         %------------
         kk=[3.45956393179041,3.42426630633186,3.42003284324308,3.40841966421316,3.38257537147348,3.39094407803892,3.37511182847874,3.34557049609671,3.35933029308199,3.34033962101306,3.32962636807698,3.33880529749581,3.32948476700049,3.32109857622621,3.31922384984851,3.31319589410211];
%          kk=[0.565213272274138,0.308621793131223,0.532248086502706,0.0292854978819075,0.462781995959974,0.311621439707605,0.338900303064810,0.444073374057415,0.188889809012863,0.216542915233553,0.413426158181195,0.0696118058329577,0.336614420659047,0.251494426251778,0.439447360548415,0.343926676568582];
%          kk=[0.268749229257512,0.273142105265277,0.275397109621096,0.277750389350853,0.280743559830874,0.280876706968178,0.282656005031967,0.285877516668580,0.285251451293331,0.287311358798486,0.288603652118438,0.287988234148770,0.289075589061905,0.290055480756754,0.290380672868002,0.291046135764545];
         killme_1_100Hz= Data_env(:,index);%MovingTrend(Data_env(:,index),0,0,1,500,500); %
         clear data_delayed;
         delay_dpp_MV=(512+MA_LPF_16_lenght)/2;
         data_delayed=[zeros(1,delay_dpp_MV) killme_1_100Hz'];
         killme_1_100Hz=data_delayed(1:length(data_delayed)-(delay_dpp_MV))';
         killme_data_env_0_100Hz_afterMA_960(:,index,itr) = movavgWindow_960s(killme_1_100Hz)./kk(index);%0-64Hz
         PR_value=killme_data_env_2_16Hz_afterMA_960(:,index,itr)./killme_data_env_0_100Hz_afterMA_960(:,index,itr);
%          PR_value_All(:,index)=killme_data_env_2_16Hz(:,index,itr)./killme_data_env_0_64Hz(:,index,itr);
         %          kkk(index)=1./mean(PR_value(1500:end))
         clear data_delayed;
         delay_dpp_MV=(512+MA_LPF_8_lenght+MA_LPF_16_lenght)/2;
         data_delayed=[PR_value' zeros(1,delay_dpp_MV)];         
         PR_value=data_delayed(delay_dpp_MV+1:length(data_delayed))';  
%          PR_value=movavgWindow_960s(PR_value);
%          PR_value_All(:,index)=PR_value;
         the_pp=thed_value(index)+th_shift_level;
         PR_value;
         
%          p1=plot(PR_value);
%          hold on;
%          p2=plot(data_BPF_data(1:end,index))
%          ylim([-0.5 3])
% % %          Decisian_table(:,index)= PR_value > the_pp;
% % %          
% % %          PR_value= ((PR_value > the_pp).*1)+ ((PR_value < the_pp).*rejected_coff);
% % %          if ( index > 1)
% % %             tem_decision= (2.*Decisian_table(:,index))+Decisian_table(:,index-1); 
% % %             A1=tem_decision.*0;
% % %             A2=tem_decision.*0;
% % %             A3=tem_decision.*0;
% % %             A1(tem_decision ==0) =rejected_coff;
% % %             A2(tem_decision ==1) =rejected_coff*2;
% % %             A3(tem_decision ==2) =1;
% % %             A3(tem_decision ==3) =1;
% % %             PR_value=A1+A2+A3;
% % %          end
%          PR_value= ((PR_value > the_pp).*1)+ ((PR_value < the_pp).*rejected_coff);
        %%
        %Forward
         switch_step=1;
         for index_samples=1:length(PR_value)
             if PR_value(index_samples)< (thed_value(index)+th_shift_level)*1             
                 if prev_thre <= rejected_coff
                        PPR_value(index_samples)=rejected_coff;
                     else
                         PPR_value(index_samples)=prev_thre;
                         prev_thre=prev_thre-step_prev_thre;
                     end
             else   
                 PPR_value(index_samples)=1;%PR_seg(seg_index,index);               
                 prev_thre=1-step_prev_thre;
             end  
         end
        %%
        %BackWard
%         switch_step=1;
%          for index_samples=1:length(PR_value)
%              if PR_value(index_samples) > (thed_value(index)+th_shift_level)*1 
%                  Back_value=1- step_prev_thre;
%                  Back_index=index_samples-1;
%                  while (Back_value > rejected_coff) && (Back_index > 1) && (PPR_value(Back_index)== rejected_coff)
%                     PPR_value(Back_index)=Back_value;
%                     Back_value= Back_value -step_prev_thre;
%                     Back_index=Back_index-1;                     
%                  end                 
%              end  
%          end
         PR_value_All(:,index)=data_BPF_data(1:end,index).*PPR_value';
         
         %%
         clear data_delayed;
         delay_dpp_MV=(512+MA_LPF_8_lenght+MA_LPF_16_lenght)/2;
         data_delayed=[killme_data_env_0_100Hz_afterMA_960(:,index,itr)' zeros(1,delay_dpp_MV)];         
         killme_data_env_0_100Hz_afterMA_960(:,index,itr)=data_delayed(delay_dpp_MV+1:length(data_delayed))';  
         
         clear data_delayed;
         delay_dpp_MV=(512+MA_LPF_8_lenght+MA_LPF_16_lenght)/2;
         data_delayed=[killme_data_env_2_16Hz_afterMA_960(:,index,itr)' zeros(1,delay_dpp_MV)];         
         killme_data_env_2_16Hz_afterMA_960(:,index,itr)=data_delayed(delay_dpp_MV+1:length(data_delayed))';  
         
         SegPR=reshape(PR_value,[len_seg,len_segData,1]);                
         segData_0_64Hz = reshape(((killme_data_env_0_100Hz_afterMA_960(:,index,itr))),[len_seg,len_segData,1]);
         segData_2_16Hz = reshape(((killme_data_env_2_16Hz_afterMA_960(1:end,index))),[len_seg,len_segData,1]);
         segData_0_64Hz=segData_0_64Hz';
         segData_2_16Hz=segData_2_16Hz';
         SegPR=SegPR';         
         %%
         Noisy_segData=reshape(((data_BPF_data(1:end,index))),[len_seg,len_segData,1]);
         Noisy_segData=Noisy_segData';
         NoiseMod_segData=reshape(((data2subCh(1:end,index))),[len_seg,len_segData,1]);
         CleanMod_segData=reshape(((Pure_BPF_data(1:end,index))),[len_seg,len_segData,1]);
         NoiseMod_segData=NoiseMod_segData';
         CleanMod_segData=CleanMod_segData';         
         for seg_index=1:len_segData
            modul_cleanData= (abs(CleanMod_segData(seg_index,:)));
            modul_noisyData= (abs(NoiseMod_segData(seg_index,:)));           
            Snr_seg(seg_index,index)=snr(modul_cleanData+0.000000000001,modul_noisyData);            
            %% PR value
            %movavgWindow_960s
            PR_seg_withHamming(seg_index,index)= mean((segData_2_16Hz(seg_index,:).*hamming(480)))*1/(1*mean((segData_0_64Hz(seg_index,:).*hamming(480))));
            PR_seg(seg_index,index)=mean(segData_2_16Hz(seg_index,:))/mean(segData_0_64Hz(seg_index,:));%mean(SegPR(seg_index,:));%
        
    
            sw_coff=1;

            %% PR value 
            if PR_seg(seg_index,index)< (thed_value(index)+th_shift_level)*1
%                  modif_Noisy_segData_PR(seg_index,:)= Noisy_segData(seg_index,:).*rejected_coff;
              
                 if prev_thre <= rejected_coff
                         modif_Noisy_segData_PR(seg_index,:)= 1.* Noisy_segData(seg_index,:).*rejected_coff;
                         PR_decision(seg_index,index)=rejected_coff;%rejected_coff;
                     else
                         modif_Noisy_segData_PR(seg_index,:)= 1.*Noisy_segData(seg_index,:).*prev_thre;
                         PR_decision(seg_index,index)=prev_thre;%rejected_coff;
                         prev_thre=prev_thre-step_prev_thre;
                         step_prev_thre=step_prev_thre*1;
                     end
             else   
                 modif_Noisy_segData_PR(seg_index,:)= 1'.*Noisy_segData(seg_index,:).*1;%PR_seg(seg_index,index);               
                 prev_thre=1-step_prev_thre;
%                  step_prev_thre=0.5
                 PR_decision(seg_index,index)=1;
            end  
                    
             %% SNR    
%             else
             if Snr_seg(seg_index,index)< -5
                 modif_Noisy_segData_SNR(seg_index,:)= 1'.*Noisy_segData(seg_index,:).*rejected_coff;
                 SNR_decision(seg_index,index)=rejected_coff;

             else
                 modif_Noisy_segData_SNR(seg_index,:)= 1'.*Noisy_segData(seg_index,:).*1;
                 SNR_decision(seg_index,index)=1;
             end  
%            end
             
         end
          %BackWard
%          for seg_index=1:len_segData
%              if PR_seg(seg_index,index)> (thed_value(index)+th_shift_level)*1
%                  Back_value=1- step_prev_thre;
%                  Back_index=seg_index-1;
%                  while (Back_value > rejected_coff) && (Back_index > 1) && (SNR_decision(Back_index,index)== rejected_coff)
%                     SNR_decision(seg_index,index)=Back_value;
%                     modif_Noisy_segData_PR(seg_index,:)= 1.*Noisy_segData(seg_index,:).*Back_value;
%                     Back_value= Back_value -step_prev_thre;
%                     Back_index=Back_index-1;                     
%                  end   
%          end
%     end
         modif_Noisy_PR(:,index) =reshape( modif_Noisy_segData_PR',[],1);
         modif_Noisy_SNR(:,index)=reshape( modif_Noisy_segData_SNR',[],1);
%        modif_Noisy(:,index)=killme_data_env_2_16Hz(:,index,itr);%.* modif_Noisy(:,index);
         p=plot(PR_decision(:,index)+1);% PR_seg(:,index));%   
         p.LineWidth=2;
         hold on
%          p=plot(PR_seg_withHamming(:,index)+1)    
%          p.LineWidth=2;
%          hold on
         p=plot(SNR_decision(:,index));%Snr_seg(:,index)/50);
         p.LineWidth=2;        

         ylim([-3 3])
         xlim([0 len_segData])
         hold on
         p=plot((thed_value(index)+th_shift_level)*ones(1,length(Snr_seg)));%
         p.LineWidth=1;
         hold on
         p=plot(-5.0*ones(1,length(Snr_seg))/50);
         p.LineWidth=2;       
         
         title(char("SNR of Sub-band"+num2str(index)));
         legend({'r_i',... 
         'SNR divded divided by 50',...
         'Threshod for r_i',...
         'Threshod for SNR'         
         },'Location','best')
%          hold on
%          p=plot((thed_value(index)+th_shift_level)*0.7*ones(1,length(Snr_seg)));%
%          p.LineWidth=3;
       
    end   
    
    processed_output_PR=sum(modif_Noisy_PR,2)*1.5;
    processed_output_SNR=sum(modif_Noisy_SNR,2)*1.5;
    processed_output_PP=sum(PR_value_All,2)*1.5;
    Unprocessed_output=sum(data_BPF_data,2)*1.5;
    
    f=figure('units','normalized','outerposition',[0 0 1 1]);
    plot(processed_output_PR)
    f=figure('units','normalized','outerposition',[0 0 1 1]);
    plot(processed_output_SNR)
    f=figure('units','normalized','outerposition',[0 0 1 1]);
    plot(processed_output_PP)
    f=figure('units','normalized','outerposition',[0 0 1 1]);
    plot(Unprocessed_output)
    

    filename="C:\Users\Biodynamics\OneDrive - University of Connecticut\Masking_SNR_modulation\AudioFile\Window_processed_"+File_additive_info+".wav"; % c:\Rahim\Mycourse\code\HPD_Modulation\results\
    audiowrite(char(filename), processed_output_PR(3500:end), 12000, 'BitsPerSample',16);
    
    filename="C:\Users\Biodynamics\OneDrive - University of Connecticut\Masking_SNR_modulation\AudioFile\SNR(-5dBT)_processed_"+File_additive_info+".wav"; % c:\Rahim\Mycourse\code\HPD_Modulation\results\
    audiowrite(char(filename), processed_output_SNR(3500:end), 12000, 'BitsPerSample',16);
    
    filename="C:\Users\Biodynamics\OneDrive - University of Connecticut\Masking_SNR_modulation\AudioFile\Unprocessed_"+File_additive_info+".wav"; % c:\Rahim\Mycourse\code\HPD_Modulation\results\
    audiowrite(char(filename), Unprocessed_output(3500:end), 12000, 'BitsPerSample',16);
    
    filename="C:\Users\Biodynamics\OneDrive - University of Connecticut\Masking_SNR_modulation\AudioFile\Sample_by_sample_"+File_additive_info+".wav"; % c:\Rahim\Mycourse\code\HPD_Modulation\results\
    audiowrite(char(filename), processed_output_PP(3500:end), 12000, 'BitsPerSample',16);
%%
%     f=figure('units','normalized','outerposition',[0 0 1 1]);
%     [pxx,w] = pwelch(processed_output(3500:end),500,300,500,12000,'power');
%     dB = pow2db(pxx);
%     plot(w,dB)
%     xlabel('f \ procsssed')
%     ylabel('Power (dB)')
%     ylim([-80 -30])
% 
%     f=figure('units','normalized','outerposition',[0 0 1 1]);
%     [pxx,w] = pwelch(Unprocessed_output(3500:end),500,300,500,12000,'power');
%     dB = pow2db(pxx);
%     plot(w,dB)
%     xlabel('f \ Unprocsssed')
%     ylabel('Power (dB)')
%     ylim([-80 -30])
