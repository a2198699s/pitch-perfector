#include <fftw3.h>
#include "voxFilter.h"

VoxFilter::VoxFilter(int samplingRate, float multiplier, int nBufferFrames){
    this->nBufferFrames = nBufferFrames;
    frequencyResolution = samplingRate/nBufferFrames;
    multiplier = multiplier;
    cut100Hz   = 100 / frequencyResolution;
    cut2000Hz  = 2000 / frequencyResolution;
    cut7000Hz  = 7000 / frequencyResolution;
    cut9000Hz  = 9000 / frequencyResolution;
    cut10000Hz = 10000 / frequencyResolution;
}


void VoxFilter::filter(fftw_complex* fourierSpectrum) {

    // 0Hz to 100Hz is 0.
    for (int i=0; i<=cut100Hz; ++i) {
        fourierSpectrum[i][0] = 0;
        fourierSpectrum[i][1] = 0;
    }

    // 100Hz to 2000Hz is kept the same.
    for (int i=cut100Hz; i<=cut2000Hz; ++i) {
        fourierSpectrum[i][0] = fourierSpectrum[i][0] * this->multiplier;
    }

    // 2000Hz to 7000Hz is multipled by 3.
    for (int i=cut2000Hz; i<=cut7000Hz; ++i) {
        fourierSpectrum[i][0] = fourierSpectrum[i][0] * this->multiplier * 3;
    }

    // 7000Hz to 9000Hz is kept the same.
    for (int i=cut7000Hz; i<=cut9000Hz; ++i) {
        fourierSpectrum[i][0] = fourierSpectrum[i][0] * this->multiplier;
    }

    // 9000Hz to 10000Hz is multipled by 3.
    for (int i=cut9000Hz; i<=cut10000Hz; ++i) {
        fourierSpectrum[i][0] = fourierSpectrum[i][0] * this->multiplier * 3;
    }

    // 10000Hz to END is set to 0.
    for (int i=cut10000Hz; i<=nBufferFrames; ++i) {
        fourierSpectrum[i][0] = 0;
        fourierSpectrum[i][1] = 0;
    }
}
