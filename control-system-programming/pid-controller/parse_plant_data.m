clear all
close all

path = 'C:\Users\Eugene\QtProjects\sem2\build-ControllerUI-Desktop_Qt_5_14_1_MinGW_64_bit-Debug\';
filename = '00_11_12';
string = fileread(strcat(path, filename));
split_string = strsplit(string, '\n');

for i = 1:length(split_string)-1
    temp = strsplit(char(split_string(i)));
    current_pos(i) = str2double(temp{1});
    reference_pos(i) = str2double(temp{2});
end

current_pos = current_pos(~isnan(current_pos));
reference_pos = reference_pos(~isnan(reference_pos));

figure
hold on, grid on
plot(current_pos)

mean_current_pos = mean(current_pos);
mean_reference_pos = mean(reference_pos);
sum_current_pos = sum(current_pos);
sum_reference_pos = sum(reference_pos);

outpFile_current_pos = fopen(strcat(path, 'parsed_', filename, '_current_pos'), 'w');
outpFile_reference_pos = fopen(strcat(path, 'parsed_', filename, '_reference_pos'), 'w');

for i = 1:length(current_pos)
    fprintf(outpFile_current_pos, '%4.2f\n', current_pos(i));
    fprintf(outpFile_reference_pos, '%4.2f\n', reference_pos(i));
end