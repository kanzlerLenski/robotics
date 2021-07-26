clear all
close all

path = 'C:\Users\Eugene\QtProjects\sem2\build-ControllerUI-Desktop_Qt_5_14_1_MinGW_64_bit-Debug\';
filename = '13_12_02_check';
string = fileread(strcat(path, filename));
split_string = strsplit(string, '\n');

for i = 1:length(split_string)-1
    temp = strsplit(char(split_string(i)));
    current_pos(i) = str2double(temp{1});
    reference_pos(i) = str2double(temp{2});
end

current_pos = current_pos(~isnan(current_pos));
reference_pos = reference_pos(~isnan(reference_pos));

error = reference_pos - current_pos;
t = 60;
time = linspace(0,t,length(error));
plot(time, error)

total_E = sum((reference_pos - current_pos).^2);
mean_E = total_E/length(error);

total_E
mean_E