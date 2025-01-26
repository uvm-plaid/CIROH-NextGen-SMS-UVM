clear;
clc;

%% MATLAB MFCC Calculation and Plot
% Define the file path for the WAV file
filePath = 'rec1.wav';

% Read the audio file
[audioIn, fs] = audioread(filePath);

% Display basic information about the audio file
disp('Audio Information:');
disp(['Sample Rate: ', num2str(fs), ' Hz']);
disp(['Number of Samples: ', num2str(length(audioIn))]);
disp(['Duration: ', num2str(length(audioIn)/fs), ' seconds']);

% Define parameters
frameLength = 512;         % Frame length in samples
hopLength = 512;           % Hop length in samples (50% overlap)
fftPoints = 512;           % Number of FFT points
windowFunction = hann(frameLength); % Hanning window

% Compute MFCCs using MATLAB library
numCoeffs = 13; % Number of MFCC coefficients
[coeffs, ~, ~, ~] = mfcc(audioIn, fs, ...
    NumCoeffs=numCoeffs, ...
    WindowLength=frameLength, ...
    OverlapLength=frameLength - hopLength, ...
    FFTLength=fftPoints, ...
    Window=windowFunction);

% Apply logarithmic scaling to MFCC values
logCoeffs = log10(abs(coeffs) + 1e-6); % Logarithmic scaling with small epsilon for stability

% Plot the MFCCs (MATLAB Computation)
figure;
timeAxisMatlab = 1:464; % Time axis in samples
imagesc(timeAxisMatlab, 1:numCoeffs, logCoeffs');
axis xy; % Flip y-axis
xlabel('Sample Number');
ylabel('MFCC Index');
title('Mel-Frequency Cepstral Coefficients (MATLAB) [Log Scale]');
colorbar;
colormap jet;
caxis([-2, 2]); % Adjust color axis for logarithmic scale visualization

%% MFCC Visualization from MFCC.csv
% Define the path for MFCC.csv
mfccFilePath = 'MFCC.csv';
mfccData = readmatrix(mfccFilePath);

% Validate the dimensions of the data
[numFramesCsv, numCoeffsCsv] = size(mfccData);
disp(['MFCC CSV File: ', num2str(numFramesCsv), ' frames, ', num2str(numCoeffsCsv), ' coefficients.']);

% Apply logarithmic scaling to MFCC values from CSV
logMfccData = log10(abs(mfccData) + 1e-6); % Logarithmic scaling with small epsilon for stability

% Plot the MFCCs from CSV
figure;
timeAxisCsv = 1:464; % Time axis in samples
imagesc(timeAxisCsv, 1:numCoeffsCsv, logMfccData');
axis xy; % Flip y-axis
xlabel('Sample Number');
ylabel('MFCC Index');
title('Mel-Frequency Cepstral Coefficients (MFCCs from CSV) [Log Scale]');
colorbar;
colormap jet;
caxis([-2, 2]); % Adjust color axis for logarithmic scale visualization
