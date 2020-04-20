import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile as wav

timesig = np.loadtxt('timesig.txt', delimiter = '\n')
spectrumIn = np.loadtxt('spectIn.txt', delimiter = '\n')
realSpectrum = np.loadtxt('real_spect.txt', delimiter = '\n')
spectrumOut = np.loadtxt('spectOut.txt', delimiter = '\n')
inverseOut = np.loadtxt('inverseOut.txt', delimiter = '\n')

pyspec = abs(np.fft.fft(timesig))

x_axis = np.arange(0, 44100, 44100/len(timesig))

plt.figure(1)
plt.plot(timesig)
plt.title('time series signal')

plt.figure(2)
plt.plot(x_axis, spectrumIn)
plt.title('Initial Freq Spectrum')

plt.figure(3)
plt.plot(x_axis, realSpectrum)
plt.title('real Spectrum')

plt.figure(4)
plt.plot(x_axis, spectrumOut)
plt.title('Shifted Spectrum')

plt.figure(5)
plt.plot(inverseOut)
plt.title('Inverse fft')

plt.figure(6)
plt.plot( pyspec)
plt.title('python spectrum')

plt.show()

#write to wavs
def normalize(v):
    norm = np.linalg.norm(v)
    if norm == 0:
       return v
    return v / norm

wav.write('input.wav', 44100, timesig)
wav.write('output.wav', 44100, inverseOut)
