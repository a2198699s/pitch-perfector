/*
    This class is used to filter Vocals. It roughly mimics the
    frequency response of an SM58 Dynamic Vocal Microphone.
*/

#ifndef VOXFILTER_H
#define VOXFILTER_H

#include <fftw3.h>

class VoxFilter {
    public:
        VoxFilter(int samplingRate, float multiplier);
        void filter(fftw_complex* fourierSpectrum);

        int samplingRate;
        float multiplier;

    private:
        int cut100Hz;
        int cut2000Hz;
        int cut7000Hz;
        int cut9000Hz;
        int cut10000Hz;
        float frequencyResolution;
};

#endif
