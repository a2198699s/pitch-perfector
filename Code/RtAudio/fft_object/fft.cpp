#include <fftw3.h>
#include <cstring>
#include <RtAudio.h>
#include "fft.h"
#include "vocoder.h"


fft::fft(int nBufferFrames) {
    in = (double *) fftw_malloc(sizeof(double)*nBufferFrames);
    out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*nBufferFrames);
    my_plan = fftw_plan_dft_r2c_1d(nBufferFrames, in, out, FFTW_MEASURE);

    inverse_in = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*nBufferFrames);
    inverse_out = (double *) fftw_malloc(sizeof(double)*nBufferFrames);
    inverse_plan = fftw_plan_dft_c2r_1d(nBufferFrames, inverse_in, inverse_out, FFTW_MEASURE);

    this->nBufferFrames = nBufferFrames; //need to set as object variable or it wont exist outwith this function!
};

void fft::executefft(double *inputBuffer) {
    memcpy(in, inputBuffer, sizeof(double)*nBufferFrames );
    fftw_execute(my_plan);
};

void fft::executeInverse_fft(fftw_complex* fourierSpectrum){
    memcpy(inverse_in, fourierSpectrum, sizeof(fftw_complex)*nBufferFrames);
    fftw_execute(inverse_plan);
};


Dispatch::Dispatch(fft* fourier_obj){
    fourierPtr = fourier_obj;
    // vocodePtr = vocoder_obj;
};

// Static declared in header.
int Dispatch::caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data) {
    Dispatch* dispatchPtr = (Dispatch*) data;
    fft *fourier = dispatchPtr->fourierPtr;
    // Vocoder* vocode = dispatchPtr->vocodePtr;

    double *input = (double *) inputBuffer;
    fourier->executefft(input);
    fourier->executeInverse_fft(fourier->out);
    memcpy(outputBuffer, fourier->inverse_out, sizeof(double)*nBufferFrames);

    // vocode->pitchShift_setup(fourier->out);
    // vocode->pitchShift();

    // fourier->executeInverse_fft(fourier->out);

    //return audio to output buffer for playback
    // memcpy(outputBuffer, fourier->inverse_out, sizeof(double)*nBufferFrames);
    return 0;
};