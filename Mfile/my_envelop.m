function data_output=my_envelop(input_data,length, env_fun)
if env_fun==1
    data_output=envelope(input_data,length);
elseif env_fun==2
    data_output=abs(input_data);
end