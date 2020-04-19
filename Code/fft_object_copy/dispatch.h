/** \class dispatch
* \brief This class handles the callback functionality from RtAudio.
*
* Dispatch objects are used to collect the processing objects
* used (<fft.h> and <vocoder.h>) and utilise them to process
* the audio data from RtAudio by implementing a method
* compatable with the RtAudio callback format.
*/


#include "fft.h"
#include "vocoder.h"
#include <RtAudio.h>
#include <cstdlib>
#include <cstring>

class dispatch  {
  public:
    fft* fourierObj;
    vocoder* vocoderObj;

    /// Collect pointers to objects to use in processing
    dispatch(fft* fourierPtr, vocoder* vocoderPtr);

    ///Callback function for RtAudio
    static int caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data);
};
