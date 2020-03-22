# Software

All of the software for this project is run on the Raspberry Pi. It involves processing audio input in the format of I2S, performing frequency analysis and shifting the pitch of the sample using a phase-vocoding mechanism.

## FFT

The Fastest Fourier Transform in the West is being used to identify the frequency components of the input signal.

## Phase Vocoding

Phase vocoding involves modifying the signal and playing it back over a longer or shorter time-window in order to lower or raise the pitch.

## Qt GUI

The Qt GUI is being used to display information about the current musical note of the input. It also allows the note, which is to be shifted to, to be selected.
