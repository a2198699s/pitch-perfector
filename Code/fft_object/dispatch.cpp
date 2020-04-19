#include "fft.h"
#include "vocoder.h"
#include "dispatch.h"
#include <RtAudio.h>
#include <cstdlib>
#include <cstring>


dispatch::dispatch(fft* fourierPtr, Vocoder* vocoderPtr){
    fourierObj = fourierPtr;
    vocoderObj = vocoderPtr;
};

int dispatch::caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data) {
    dispatch* dispatcher = (dispatch*) data;
    
    (dispatcher->fourierObj)->executefft((double*) inputBuffer);
    (dispatcher->vocoderObj)->setFourierSpectrum((dispatcher->fourierObj)->out);
    (dispatcher->vocoderObj)->pitchShift();
    (dispatcher->fourierObj)->executeInverse_fft((dispatcher->vocoderObj)->fourierSpectrum);
    dispatcher->currentNote = (dispatcher->vocoderObj)->currentNote;

    memcpy(outputBuffer, (dispatcher->fourierObj)->inverse_out, sizeof(double)*AUDIO_BUFFER_SIZE);

    return 0;
    
};
