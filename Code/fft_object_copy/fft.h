/** \class fft
* \brief Class for performing fourier transforms using FFTW3
*
* Allows for the set-up of a fftw3 plan (the most time consuming
* part of the process) when the class instantiated, allowing the
* performance of forward and inverse fast fourier transforms with
* minimal computational cost inside the RtAudio Callback when the
* methods are called.
*/


#include <RtAudio.h>
#include <fftw3.h>
#include "voxFilter.h"

#pragma once

class fft {
  public:
    int nBufferFrames;
    double* in;
    fftw_complex *out;
    fftw_complex* inverse_in;
    double* inverse_out;
    fftw_plan my_plan;
    fftw_plan inverse_plan;

    /// Construct the fftw3 plans for the object
    fft(int nBufferFrames, int samplingRate);

    /** Execute the forward fourier transform and update
    * generate the frequncy spectrum for the data input
    */
    void executefft(double* inputBuffer);

    /** Execute the inverse fourier transform to reconstruct
    * a time-series signal from the input frequency spectrum.
    * Data is then scaled back to the appropriate size.
    */
    void executeInverse_fft(fftw_complex* fourierSpectrum);
};
