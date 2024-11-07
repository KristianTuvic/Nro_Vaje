%1.Naloga
filename = 'naloga1_1.txt';
delimiterIn=';';
headerlinesIn=2;
podatki = importdata(filename, delimiterIn, headerlinesIn);
podatki;
t= podatki.data(:,1);

%2.naloga
fid = fopen('naloga1_2.txt', 'r');

prva_vrstica = fgetl(fid);

parts = strsplit(prva_vrstica, ':');
numValues = str2double(strtrim(parts{2}));

P = zeros(numValues,1);

for i = 1:numValues
    valueLine = fgetl(fid);
    P(i) = str2double(strtrim(valueLine));
end

fclose(fid);

%graf P(t)
figure;
plot(t, P, '-o');
xlabel('t [s]');
ylabel('P [W]');
title('graf P(t)');
grid on;

%3.naloga
%lastna implementacija trapezne metode
integral_value = 0;
for i = 1:length(t) - 1
     delta_t = t(i+1) - t(i);
     integral_value = integral_value + (P(i) + P(i+1)) / 2 * delta_t;
end
disp(['Integral izračunan s trapezno metodo: ', num2str(integral_value)]);

%uporaba funkcije trapz
integral_trapz = trapz(t, P);
disp(['Integral izračunan s funkcijo trapz: ', num2str(integral_trapz)]);