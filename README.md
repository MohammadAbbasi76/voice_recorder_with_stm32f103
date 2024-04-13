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
 Let's start with the basics of signal analysis, exploring how signals can be understood in both the time domain and the frequency domain. The time domain representation, shown as a plot of voltage against time, is a concept familiar to many, illustrating how a signal's voltage changes over time.

When we analyze a signal using the fast Fourier transform (FFT), its frequency domain components become apparent. According to Fourier's theorem, any signal can be broken down into individual or multiple frequency components, known as harmonics. For example, let's take a look at a sine wave.

This sine wave consists of two components: a direct current (DC) component and an alternating current (AC) component. In the time domain, it resembles the top part of the diagram. However, when we shift to the frequency domain through FFT analysis, it appears as shown in the middle section of the diagram. Here, we can see a DC component at 0Hz and an AC component at 1KHz on the frequency spectrum. This graphical representation is commonly known as the "Frequency-Domain."

![image](https://github.com/MohammadAbbasi76/voice_recorder_with_stm32f103/assets/104908459/16822c2a-ba7c-446d-b3f3-f05e279bdbec)

Certainly, here's an alternative phrasing:

Now, consider the average voltage (DC component) of this sine wave. To extract this, we pass the signal through a low-pass filter (LPF), removing the high-frequency component (the AC sine wave portion at 1KHz) and retaining only the DC average voltage component.

This is demonstrated in the bottom part of the diagram. After low-pass filtering in the time domain, only the DC voltage component (average value) remains. This phenomenon is also observed in the frequency domain, where the AC component is significantly suppressed, while the magnitude of the DC component remains unchanged. This is the result of low-pass filtering the signal in both the temporal and spectral domains.
3- PWM Signal Characteristics & Filtering

The PWM signal, essentially a square wave, allows for adjustment of its duty cycle over time. Its frequency spectrum displays a wide-band nature due to the infinite number of harmonics it possesses. According to Fourier's theorem, which states that any signal can be represented as a sum of sines and cosines of various frequencies, the square wave exhibits an infinite array of harmonics or sine waves at multiple frequencies. This concept is illustrated in the following diagram.

![image](https://github.com/MohammadAbbasi76/voice_recorder_with_stm32f103/assets/104908459/98c74670-4b11-4d26-9009-094540c5ca5e)


As we add more harmonics, the square wave begins to resemble a "cleaner" square wave. Hence, it exhibits a wide-band spectrum, as mentioned earlier. This point is also visually confirmed in the accompanying diagram.

![image](https://github.com/MohammadAbbasi76/voice_recorder_with_stm32f103/assets/104908459/805fc34c-0395-4aff-b388-3dc5f52d43cd)

### PWM Filtering

The PWM duty cycle represents the average voltage of a given PWM waveform. For instance, a 3.3V PWM signal with a 50% duty cycle can be interpreted as having an average voltage of 1.65V. However, it's crucial to note that the signal isn't a constant DC level; it fluctuates between 0V and 3.3V over time.

Devices like motors and speakers often exhibit characteristics similar to a low-pass filter (LPF). Consequently, a motor's speed will change in response to variations in the PWM duty cycle. However, when constructing a digital-to-analog converter (DAC) using this PWM signal, it's essential to eliminate these PWM alternating current (AC) components or harmonics.

Our goal is to ensure that the DAC output accurately represents the average voltage of the PWM signal with minimal acceptable ripple. To achieve this, we aim for a high PWM frequency and a low cutoff frequency (Fc) for the LPF. This setup results in a stable DC output (the average voltage of the PWM) from the LPF.

![image](https://github.com/MohammadAbbasi76/voice_recorder_with_stm32f103/assets/104908459/04da7dc7-d84b-48b3-b596-a6af1426f394)

Another point to note here which is extremely important is that the way this LFP is setup makes it cut-off any AC signal on the output. The output swing is too slow due to the LFP high time constant effect. This will limit the ability to use the PWM-To-DAC system for generating AC signals like sine waves of whatever.

Therefore, there is always a trade-off between the PWM frequency, the output signals frequency FBW, and the LPF characteristics. And that’s what we’re going to investigate in the next section for design steps and in the next tutorial as well to generate waveforms using this PWM-To-DAC technique.
Designing PWM To DAC System   

### 1- Determine the Desired Resolution

The first step in creating a PWM-To-DAC system is to determine the required DAC resolution. Resolution refers to the number of distinct voltage levels the DAC can have, which is inherited from the PWM. The PWM resolution determines the number of discrete duty cycle levels, thereby defining the DAC resolution.

It's important to note that the PWM resolution specified in the datasheet isn't fixed; it changes during operation based on the FPWM frequency. Higher FPWM frequencies result in lower resolutions. Thus, determining the required DAC resolution sets an upper limit for the FPWM.
As mentioned earlier, a high FPWM is desirable to improve filtering effectiveness and reduce output ripple. However, increasing FPWM compromises resolution. Hence, this initial step is crucial in system design.
A rule of thumb is to choose the FPWM to be some order of magnitudes larger than the desired output signal’s frequency FBW. The larger the factor k the better.

PWM To DAC - STM32 PWM Waveform Generator Sinewave 50Hz etc

And also keep in mind that raising the FPWM will degrade the final DAC’s resolution. As you can see from the formula down below. This indicates that raising the FPWM will decrease its resolution at any value for timer clock FCLK, and therefore the DAC’s resolution decreases as well.
 
### 2- PWM Frequency Selection
A rule of thumb is to choose the FPWM to be some order of magnitudes larger than the desired output signal’s frequency FBW. The larger the factor k the better.

![image](https://github.com/MohammadAbbasi76/voice_recorder_with_stm32f103/assets/104908459/2c2493c5-08b0-443e-88c6-fa170a2698c2)

And also keep in mind that raising the FPWM will degrade the final DAC’s resolution. As you can see from the formula down below. This indicates that raising the FPWM will decrease its resolution at any value for timer clock FCLK, and therefore the DAC’s resolution decreases as well.

![image](https://github.com/MohammadAbbasi76/voice_recorder_with_stm32f103/assets/104908459/6903d476-f915-4767-b2cf-0039f33d1839)

The formula to be used in configuring the timer module in PWM mode to control the PWM output frequency.
![image](https://github.com/MohammadAbbasi76/voice_recorder_with_stm32f103/assets/104908459/0942410b-5812-46f5-8372-122b430c5fe5)
### 3- RC LPF Design

This step is the most critical step in the design process. As it defines the dynamic characteristics of the resulting DAC system at the end. The LPF filter introduces some delay due to the time constant that can limit the DAC’s ability to swing very quickly to follow a specific waveform pattern.

Therefore, you’ve to carefully choose the frequency response parameters for the LPF while designing it. I can divide the resulting DACs using this technique into two categories: static DACs, dynamic DACs. The static ones do have a heavy LPF with a cut-off frequency at 10Hz may be or 1Hz. With a very slow output swing and it’s convenient in some applications where certain voltage levels need to be sent as an output.

On the other hand, dynamic DACs are used to generate analog waveforms like sine, sawtooth, etc. And the LPF should be designed so as to pass the frequency of the desired signal FBW while blocking the FPWM harmonics. And the following formula is used for this case.
![image](https://github.com/MohammadAbbasi76/voice_recorder_with_stm32f103/assets/104908459/8d492aa8-9448-4137-a84c-e1bf49bedadc)

Choose the resistor R depending on the GPIO pin output driving capability and solve for C to find the capacitor value. And you can calculate the attenuation using the following formula. And if the attenuation is not sufficient, you’ll have to increase the k factor.

![image](https://github.com/MohammadAbbasi76/voice_recorder_with_stm32f103/assets/104908459/8f79289e-8cb1-46c2-b0ae-b103b381a327)

you can customize the filter’s design to meet your application’s criteria. You can use a high order filter that has a sharper roll-off so it filters out the PWM signal in a stronger manner.
![image](https://github.com/MohammadAbbasi76/voice_recorder_with_stm32f103/assets/104908459/e8bfe050-a0d5-49b8-937a-6a0436ef9d4e)
###4- DAC Output Voltage Control

Controlling the DAC’s output is an easy process, it’s done by changing the duty cycle and that’s it.
![image](https://github.com/MohammadAbbasi76/voice_recorder_with_stm32f103/assets/104908459/46f5a7ac-e42f-4f48-9709-e273a711727f)

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
