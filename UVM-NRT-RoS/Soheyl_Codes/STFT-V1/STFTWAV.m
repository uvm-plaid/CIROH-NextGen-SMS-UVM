clear;
close all;
clc;

% Parameters
audioFilePath = 'rec1.wav';  % Path to the audio file
frame_size = 512;           % Number of samples per frame
overlap = frame_size/2;                % No overlap
n_fft = 512;                % FFT size for MATLAB FFT calculation
fs = 24000;                 % Sampling frequency (Hz)
arduino_fft_file = 'AUDIO3.csv'; % Arduino FFT data file

%% Spectrogram from Audio File
% Read the audio file
if ~isfile(audioFilePath)
    error('The file %s does not exist.', audioFilePath);
end
[audio_data, fs] = audioread(audioFilePath);
audio_data = audio_data(:, 1); % Use first channel if stereo

% Calculate the spectrogram
[S, F, T] = spectrogram(audio_data, hamming(frame_size), overlap, n_fft, fs);

% Convert magnitude to dB scale
S_dB = 20 * log10(abs(S));

% Plot the spectrogram
figure;
imagesc(T, F, S_dB);
axis xy; % Flip y-axis for correct orientation
colormap jet;
colorbar;
caxis([-100, 0]); % Limit the color map to -100 to 0 dB
title('Spectrogram of Signal - MATLAB FFTs');
xlabel('Time (s)');
ylabel('Frequency (Hz)');

%% Spectrogram from Arduino FFTs
% Read Arduino FFT data
if ~isfile(arduino_fft_file)
    error('The file %s does not exist.', arduino_fft_file);
end
arduino_data = readmatrix(arduino_fft_file);

% Extract frequency bins (first row) and FFT values (remaining rows)
arduino_frequencies = arduino_data(1, 1:255); % Frequency bins
arduino_magnitudes = arduino_data(2:464, 1:255); % FFT values (64 rows, 255 columns)

% Generate time axis for Arduino FFTs
num_frames_arduino = size(arduino_magnitudes, 1);
time_axis_arduino = (0:(num_frames_arduino - 1)) * (frame_size / fs);

% Convert magnitudes to dB scale
arduino_magnitudes_dB = 20 * log10(arduino_magnitudes) - 45; % adjust the magnitudes

% Plot the Arduino FFT spectrogram
figure;
imagesc(time_axis_arduino, arduino_frequencies, arduino_magnitudes_dB.');
axis xy; % Flip y-axis for correct orientation
colormap jet;
colorbar;
caxis([-100, 0]); % Limit the color map to -100 to 0 dB
title('Spectrogram of Signal - Arduino FFTs');
xlabel('Time (s)');
ylabel('Frequency (Hz)');
