import numpy as np
import matplotlib.pyplot as plt

timesig = np.loadtxt('timesig.txt', delimiter = '\n')
spectrumIn = abs(np.loadtxt('spectIn.txt', delimiter = '\n'))
#spectrumOut = abs(np.loadtxt('spectOut.txt', delimiter = '\n'))

pyspec = abs(np.fft.fft(timesig))

x_axis = np.arange(0, 44100, 44100/len(timesig))

plt.figure(1)
plt.plot(timesig)
plt.title('time series signal')

plt.figure(2)
plt.plot(x_axis, spectrumIn)
plt.title('Initial Freq Spectrum')

# plt.figure(3)
# plt.plot(x_axis, spectrumOut)
# plt.title('Shifted Spectrum')

plt.figure(4)
plt.plot(x_axis, pyspec)
plt.title('python spectrum')

plt.show()
