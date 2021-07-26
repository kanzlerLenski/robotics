clear all
close all

path = 'C:\Users\Eugene\QtProjects\sem2\ControlTest\';
filename = '09_12_03';
string = fileread(strcat(path, filename));
split_string = strsplit(string, '\n');

for i = 1:length(split_string)-1
    temp = strsplit(char(split_string(i)));
    current_pos(i) = str2double(temp{1});
    reference_pos(i) = str2double(temp{2});
end

%current_pos = current_pos(~isnan(current_pos));
%reference_pos = reference_pos(~isnan(reference_pos));
god_pos = ~isnan(current_pos) & ~isnan(reference_pos);
current_pos = current_pos(god_pos);
reference_pos = reference_pos(god_pos);

array_length = length(current_pos);
y = current_pos;

S = zeros(array_length-200, 3);
Ydotdot = zeros(array_length-200, 1);

dt = 0.02;

% w = 1000*5*pi;
% filter_tf = tf(1,[1/w 1]);
% filtered_y = lsim(filter_tf, y, t);

filtered_y = lowpass(current_pos, 0.05);
filtered_ydot = lowpass(diff(filtered_y)/dt, 0.05);
filtered_ydotdot = lowpass(diff(filtered_ydot)/dt, 0.05);

figure
hold on, grid on

subplot(4,1,1)
plot(current_pos)

subplot(4,1,2)
plot(filtered_y)

subplot(4,1,3)
plot(filtered_ydot)

subplot(4,1,4)
plot(filtered_ydotdot)

for i = 1:array_length-200
    S(i, 1:3) = [reference_pos(i) -filtered_ydot(i) -filtered_y(i)];
    Ydotdot(i) = filtered_ydotdot(i);
end

S_inv = pinv(S);
P = S_inv*Ydotdot;

P