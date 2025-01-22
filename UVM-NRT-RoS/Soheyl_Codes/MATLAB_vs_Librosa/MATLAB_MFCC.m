clear;
clc;

% Define the file path for the WAV file
filePath = 'rec0.wav';

% Read the audio file
[audioIn, fs] = audioread(filePath);

% Display basic information about the audio file
disp('Audio Information:');
disp(['Sample Rate: ', num2str(fs), ' Hz']);
disp(['Number of Samples: ', num2str(length(audioIn))]);
disp(['Duration: ', num2str(length(audioIn)/fs), ' seconds']);

% Define parameters
frameLength = round(0.032 * fs); % Frame length in samples (32 ms window)
hopLength = round(0.016 * fs);   % Hop length in samples (16 ms hop)
fftPoints = 512;                % Number of FFT points
windowFunction = hann(frameLength, 'periodic'); % Hamming window

% Frame the audio signal
numFrames = floor((length(audioIn) - frameLength) / hopLength) + 1;
disp(['Number of Frames: ', num2str(numFrames)]);

% Initialize FFT result storage
fftMagnitudes = zeros(fftPoints / 2 + 1, numFrames);

% Process each frame for FFT
disp('Calculating FFT for each frame...');
for frameIdx = 1:numFrames
    startIdx = (frameIdx - 1) * hopLength + 1;
    endIdx = startIdx + frameLength - 1;
    
    if endIdx > length(audioIn)
        break; % Skip incomplete frame at the end
    end
    
    % Extract frame and apply window
    frame = audioIn(startIdx:endIdx) .* windowFunction;
    
    % Compute FFT and magnitude spectrum
    fftResult = fft(frame, fftPoints);
    fftMagnitudes(:, frameIdx) = abs(fftResult(1:fftPoints / 2 + 1));
end

% Plot the spectrogram
timeVector = (0:numFrames - 1) * hopLength / fs; % Time axis in seconds
frequencyVector = (0:fftPoints / 2) * fs / fftPoints; % Frequency axis in Hz

figure;
imagesc(timeVector, frequencyVector, 20 * log10(fftMagnitudes + 1e-6)); % Log scale for better visualization
axis xy; % Flip y-axis
xlabel('Time (s)');
ylabel('Frequency (Hz)');
title('Spectrogram (STFT)');
colorbar;
colormap jet;
clim([-100, 0]); % Adjust color axis for better contrast

% Compute MFCCs
numCoeffs = 13; % Number of MFCC coefficients
[coeffs, ~, ~, ~] = mfcc(audioIn, fs, ...
    NumCoeffs=numCoeffs, ...
    WindowLength=frameLength, ...
    OverlapLength=frameLength - hopLength, ...
    FFTLength=fftPoints, ...
    Window=windowFunction);

% Plot the MFCCs
figure;
timeAxis = (0:size(coeffs, 1) - 1) * hopLength / fs; % Time axis in seconds
imagesc(timeAxis, 1:numCoeffs, coeffs');
axis xy; % Flip y-axis
xlabel('Time (s)');
ylabel('MFCC Index');
title('Mel-Frequency Cepstral Coefficients (MFCCs)');
colorbar;
colormap jet;
