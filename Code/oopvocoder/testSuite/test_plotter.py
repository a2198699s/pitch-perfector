import numpy as np
import matplotlib.pyplot as plt

timesig = np.loadtxt('timesig.txt', delimiter = '\n')
spectrumIn = np.loadtxt('spectIn.txt', delimiter = '\n')
realSpectrum = np.loadtxt('real_spect.txt', delimiter = '\n')
spectrumOut = np.loadtxt('spectOut.txt', delimiter = '\n')

pyspec = abs(np.fft.fft(timesig))

#x_axis = np.arange(0, 44100, 44100/len(timesig))

plt.figure(1)
plt.plot(timesig)
plt.title('time series signal')

plt.figure(2)
plt.plot(spectrumIn)
plt.title('Initial Freq Spectrum')

plt.figure(3)
plt.plot(realSpectrum)
plt.title('real Spectrum')

plt.figure(4)
plt.plot(spectrumOut)
plt.title('Shifted Spectrum')

plt.figure(5)
plt.plot(pyspec)
plt.title('python spectrum')

plt.show()
