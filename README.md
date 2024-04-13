<p align="center">
  <a ></a>
</p>

# Voice Recorder using Pulse Width Modulation (PWM) and External Flash Memory

This module facilitates to record voice and play it with 

## Table of Contents

* [Introduction](#introduction)
* [PWM Methodology](#pwm-methodology)
* [How to use it](#how-to-use-it)
* [Dependencies](#dependencies)
* [Features](#features)
* [API Reference](#api-reference)


## Introduction
This project aims to create a voice recorder that uses a microcontroller, specifically the STM32F103 model, to generate analog signals. We achieved this by employing Pulse Width Modulation (PWM) techniques instead of utilizing a dedicated Digital-to-Analog Converter (DAC) unit. Additionally, we used external flash memory to store data efficiently.

Our approach offers a cost-effective solution for generating analog waveforms compared to using a DAC. DACs can be costly and may lack the necessary resolution for certain applications.

We demonstrate that PWM-based analog signal generation can effectively create analog voltage levels suitable for driving various analog circuits or devices by modulating the duty cycle of a PWM signal. This approach allows for the generation of arbitrary analog waveforms without the need for a dedicated DAC, which may have limited resolution.

Our solution provides an alternative to DAC-dependent methods and offers a versatile solution that is both cost-effective and capable of generating high-resolution analog waveforms. It addresses the issue of resolution limitations and overcomes the constraints of DACs.

## Microcontroller Limitations
Our setup revolves around a microcontroller ((STM32F103) )with constrained computational capabilities and lacking a built-in DAC unit, necessitating alternative approaches for sound reproduction.

## PWM Methodology
We employ PWM to generate analog-like signals from digital outputs. This involves modulating the duty cycle of a digital signal to approximate analog waveforms, enabling sound synthesis.
The PWM signal is a square waveform with an adjustable duty cycle, meaning the ratio of time the signal is high (on) to the total period of the waveform can be changed. This characteristic gives PWM signals a wide-band spectrum with an infinite number of harmonics.

When converting PWM signals to analog-like waveforms, such as sine waves, filtering becomes crucial. Actuators like motors typically use low-pass filters to interpret PWM duty cycles accurately. However, for applications requiring Digital-to-Analog Conversion (DAC), it's essential to effectively remove PWM's AC components, favoring high PWM frequencies and low cut-off frequencies (Fc) in the low-pass filter (LPF) design to ensure stable DC outputs.

However, optimizing for DAC applications can limit the system's ability to efficiently generate AC signals like sine waves due to the slow response time of the LPF. Achieving the right balance between PWM frequency, output signal frequency, and LPF characteristics is crucial for achieving optimal performance in DAC applications.

In the context of voice recording, the initial step involves buffering the output waves from the microphone using an operational amplifier (op amp). This buffering process ensures that the signals from the microphone are properly conditioned and prepared for further processing or conversion.

### External Flash Memory Integration
To overcome limitations of internal memory, an external flash memory module is utilized, offering expanded storage capacity for recorded audio samples.

## Implementation Strategies

### Sampling Rate and Resolution Optimization
The voice recorder operates at a sampling rate of 25000 samples per second, optimizing fidelity and accuracy of recorded audio through sampling rate and resolution enhancements.

### PWM Signal Generation
PWM signals are meticulously modulated to emulate desired analog waveforms representative of the recorded audio, ensuring faithful reproduction of the audio signal.


### Performance Optimization Techniques
Efficient interrupt-driven programming and memory management techniques enhance real-time performance, ensuring seamless operation of the voice recorder.

## Advancements and Scalability

### Sampling Rate Enhancement
Exploration of methods to increase the sampling rate beyond 25000 samples per second for heightened audio fidelity, necessitating optimizations in computational efficiency and memory access speeds.


## Conclusion

The voice recorder developed using PWM modulation and external flash memory represents a viable solution for microcontroller-based systems. Leveraging sophisticated engineering methodologies and optimization techniques, it achieves real-time audio capture and playback functionality, suitable for diverse applications in embedded audio systems.
