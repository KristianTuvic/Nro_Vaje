% Branje podatkov iz datoteke vozlisca_temperature_dn2.txt
filename_nodes = 'vozlisca_temperature_dn2.txt';
fileID = fopen(filename_nodes, 'r');
fgetl(fileID); 
Nx = str2double(extractAfter(fgetl(fileID), ':'));
Ny = str2double(extractAfter(fgetl(fileID), ':'));
N_nodes = str2double(extractAfter(fgetl(fileID), ':'));
data_nodes = fscanf(fileID, '%f,%f,%f', [3, N_nodes])';
fclose(fileID);

% Branje podatkov iz datoteke celice_dn2.txt
filename_cells = 'celice_dn2.txt';
fileID = fopen(filename_cells, 'r');
fgetl(fileID); 
N_cells = str2double(extractAfter(fgetl(fileID), ':'));
data_cells = fscanf(fileID, '%d,%d,%d,%d', [4, inf])';
fclose(fileID);

% Koordinate in temperature iz vozlišč
x = data_nodes(:, 1);
y = data_nodes(:, 2);
T = data_nodes(:, 3);

% Ciljna točka
target_point = [0.403, 0.503];

%% Metoda 1: scatteredInterpolant
tic;
F_scattered = scatteredInterpolant(x, y, T, 'linear', 'none');
T_scattered = F_scattered(target_point(1), target_point(2));
time_scattered = toc;
fprintf('Temperatura z metodo scatteredInterpolant: %.4f °C (čas: %.6f s)\n', T_scattered, time_scattered);

%% Metoda 2: griddedInterpolant
tic;

% Generacija mreže
x_unique = unique(x);
y_unique = unique(y);
[Y, X] = ndgrid(y_unique, x_unique);
T_grid = reshape(T, Nx, Ny)';
X = X'; Y = Y'; T_grid = T_grid';

F_gridded = griddedInterpolant(X, Y, T_grid, 'linear', 'none');
T_gridded = F_gridded(target_point(1), target_point(2));
time_gridded = toc;
fprintf('Temperatura z metodo griddedInterpolant: %.4f °C (čas: %.6f s)\n', T_gridded, time_gridded);
%% Metoda 3: Bilinearna interpolacija
tic;
for i = 1:N_cells
    pt1 = data_cells(i, 1);
    pt2 = data_cells(i, 2);
    pt3 = data_cells(i, 3);
    pt4 = data_cells(i, 4);
    
    % Koordinate vozlišč celice
    x1 = x(pt1); y1 = y(pt1); T11 = T(pt1);
    x2 = x(pt2); y2 = y(pt2); T21 = T(pt2);
    x3 = x(pt3); y3 = y(pt3); T12 = T(pt3);
    x4 = x(pt4); y4 = y(pt4); T22 = T(pt4);
    
   % Preverimo, ali je točka znotraj celice
    if target_point(1) >= min([x1, x2, x3, x4]) && target_point(1) <= max([x1, x2, x3, x4]) && ...
       target_point(2) >= min([y1, y2, y3, y4]) && target_point(2) <= max([y1, y2, y3, y4])
        % Uporabimo bilinearno interpolacijo
        x_min = min([x1, x2]);
        x_max = max([x1, x2]);
        y_min = min([y1, y3]);
        y_max = max([y1, y3]);
        
        K1 = ((x_max - target_point(1)) / (x_max - x_min)) * T11 + ...
             ((target_point(1) - x_min) / (x_max - x_min)) * T21;
        K2 = ((x_max - target_point(1)) / (x_max - x_min)) * T12 + ...
             ((target_point(1) - x_min) / (x_max - x_min)) * T22;
        T_bilinear = ((y_max - target_point(2)) / (y_max - y_min)) * K1 + ...
                     ((target_point(2) - y_min) / (y_max - y_min)) * K2;
        break;
    end
end
time_bilinear = toc;
fprintf('Temperatura z bilinearno interpolacijo: %.4f °C (čas: %.6f s)\n', T_bilinear, time_bilinear);

%% Metoda 4: Najbližji sosed
tic;
distances = sqrt((x - target_point(1)).^2 + (y - target_point(2)).^2);
[~, idx_nearest] = min(distances);
T_nearest = T(idx_nearest);
time_nearest = toc;
fprintf('Temperatura z metodo najbližjega soseda: %.4f °C (čas: %.6f s)\n', T_nearest, time_nearest);

%% Največja temperatura in lokacija
[max_temp, max_idx] = max(T);
fprintf('Največja temperatura: %.4f °C na koordinatah (%.3f, %.3f)\n', ...
        max_temp, x(max_idx), y(max_idx));

