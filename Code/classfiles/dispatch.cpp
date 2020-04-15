#include "fft.h"
#include "vocoder.h"
#include "dispatch.h"
#include <RtAudio.h>


dispatch::dispatch(fft* fourierPtr, vocoder* vocoderPtr){
    this->fourierObj = fourierPtr;
    this->vocoderObj = vocoderPtr;
};

int dispatch::caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data) {
    dispatch* dispatcher = (dispatch*) data;

    (dispatcher->fourierObj)->executefft((double*) inputBuffer);
    (dispatcher->vocoderObj)->pitchShift_setup((dispatcher->fourierObj)->out);
    (dispatcher->vocoderObj)->pitchShift();
    (dispatcher->fourierObj)->executeInverse_fft((dispatcher->vocoderObj)->FourierTransform);

    inputBuffer = (void*) dispatcher->fourierObj->inverse_out;
    memcpy(outputBuffer, inputBuffer, sizeof(double)*nBufferFrames);
    return 0;
};
