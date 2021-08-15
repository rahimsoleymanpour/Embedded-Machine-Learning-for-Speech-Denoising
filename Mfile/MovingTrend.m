function  output=MovingTrend(input,scaling,shifiting, window_mode, len,shift)

temp1=input-movmean(input,len); 
input=temp1;
input= ((input>0).*1.*input)+((input<0).*scaling.*input) ;
if (min(input(3500:end)) < 0)
      level=shifiting*abs(min(input(3500:end)));
     output=input+shifiting*abs(min(input(3500:end)));
      
else 
     output=input;%+0.8*abs(min(killme_data_env_lpf2(3500:end,index)));
end