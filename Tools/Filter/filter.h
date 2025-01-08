/**
  ******************************************************************************
  * @file           : filter.h
  * @brief          : �˲������������ļ�
  * @attention      : None

  ******************************************************************************
  */


// filter.h
#ifndef FILTER_H
#define FILTER_H

#include <math.h>

// �˲�������ö�ٱ���
typedef enum {
    NOTCH=1,      // �ݲ��˲���
    LOWPASS,   // ��ͨ�˲���
    HIGHPASS,  // ��ͨ�˲���
    BANDPASS,  // ��ͨ�˲���
    BANDSTOP   // �����˲���
} FilterClassType;

// �˲��������ṹ��
// a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
typedef struct filter {
    FilterClassType class;  // �˲�������
    float fs;               // ����Ƶ��
    float notch_cut;        // �ݲ�Ƶ��
    float low_cut;          // ��ͨƵ�� (��ͨ�ʹ����˲���ʱ low_cut �� high_cut ���ʹ��)
    float high_cut;         // ��ͨƵ�� (��ͨ�ʹ����˲���ʱ low_cut �� high_cut ���ʹ��)
    float q;                // Ʒ������Q ��Q = center_freq / bandwith, center_freq: ����Ƶ��; bandwith: ����
    float b[3];             // �˲�������ϵ�� numerator
    float a[3];             // �˲�����ĸϵ�� denominator
    float x[3];             // �����ź� x[n], x[n-1], x[n-2]
    float y[3];             // ����ź� y[n], y[n-1], y[n-2]
}FilterTypeDef;


extern FilterTypeDef filter_nt_data1;

void init_filter(FilterTypeDef *filter, FilterClassType class, float fs,  float notch_cut, float low_cut, float high_cut);
float apply_filter(float input, FilterTypeDef *filter);

#endif



/*
# python ��֤����(���Ʒ�Ƶ���Ժ���Ƶ��������):
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import freqz

def plot_amplitude_phase(b, a, sample_freq, mark_Hz=None, mark_dB=-3):
    #  �����Ƶ��Ӧ
    frequencies = np.linspace(0, sample_freq / 2, 1000)
    w, h = freqz(b, a, worN=frequencies, fs=sample_freq)
    amplitude = 20 * np.log10(abs(h))
    phase = np.angle(h)
    # ���Ʒ�Ƶ����
    plt.figure(figsize=(8, 6))
    plt.subplot(2, 1, 1)
    plt.plot(frequencies, amplitude)
    plt.title("Filter Frequency Response")
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Amplitude (dB)")
    if mark_Hz is not None:
        # ����ض�Ƶ�ʵ�
        mark_Hz  = mark_Hz  # ���Ƶ��ֵ (Hz)
        mark_Hz_idx = np.argmin(np.abs(frequencies - mark_Hz))  # �ҵ���ӽ�Ŀ��Ƶ�ʵ��λ��
        mark_Hz_freq = frequencies[mark_Hz_idx]  # ��ȡ��Ƶ�ʵ��Ӧ��Ƶ��ֵ
        mark_Hz_amplitude = 20 * np.log10(abs(h[mark_Hz_idx]))  # �������
        plt.plot(mark_Hz, mark_Hz_amplitude, 'go')  # ���Ʊ�ǵ�
        plt.text(mark_Hz, mark_Hz_amplitude, f"({mark_Hz_freq:.2f} Hz, {mark_Hz_amplitude:.2f} dB)", color='red')  # ����ı�
    if mark_dB is not None:
        # ����ض����ȵ�
        mark_dB = mark_dB  # ��Ƿ���ֵ (dB)
        mark_dB_idx = np.argmin(np.abs(amplitude - mark_dB))  # �ҵ���ӽ�Ŀ����ȵ��λ��
        mark_dB_freq = frequencies[mark_dB_idx]  # ��ȡ�÷��ȵ��Ӧ��Ƶ��ֵ
        mark_dB_amplitude = 20 * np.log10(abs(h[mark_dB_idx]))  # �������
        plt.plot(mark_dB_freq, mark_dB_amplitude, 'ro')  # ���Ʊ�ǵ�
        plt.text(mark_dB_freq, mark_dB_amplitude, f"({mark_dB_freq:.2f} Hz, {mark_dB_amplitude:.2f} dB)", color='red')  # ����ı�
    # ������Ƶ����
    plt.subplot(2, 1, 2)
    plt.plot(frequencies, phase)
    plt.title("Filter Phase Response")
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Phase (radians)")
    # ��ͼ
    plt.tight_layout()
    plt.show()

def notch_filter_coefficients(notch_freq, sample_freq, Q=30):
    """
    �����ݲ��˲���ϵ��
    ʱ���ϵ: a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
    Args:
        notch_freq (float): �˲���������Ƶ�� (Hz)
        sample_freq (float): ����Ƶ�� (Hz)
        Q (float): Ʒ������, �������� (Q = center_freq / bandwith, center_freq: ����Ƶ��; bandwith: ����)
    Returns:
        b (numpy.ndarray): �˲�������ϵ�� NUM
        a (numpy.ndarray): �˲�����ĸϵ�� DEN
    """
    w0 = 2.0 * np.pi * notch_freq / sample_freq  # �����һ����Ƶ��
    alpha = np.sin(w0) / (2.0 * Q)  # �����˲����� alpha ����
    b = [1.0, -2.0 * np.cos(w0), 1.0]  # ����ϵ��
    a = [1.0 + alpha, -2.0 * np.cos(w0), 1.0 - alpha]  # ��ĸϵ��
    a0 = a[0]
    b = np.array(b) / a0
    a = np.array(a) / a0
    return np.array(b), np.array(a)


def lowpass_filter_coefficients(cut_freq, sample_freq, Q=0.707):
    """
    �����ͨ�˲���ϵ��
    ʱ���ϵ: a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
    Args:
        cut_freq (float): ��ֹƵ�� (Hz)
        sample_freq (float): ����Ƶ�� (Hz)
        Q (float): Ʒ������, �������� (Q = center_freq / bandwith, center_freq: ����Ƶ��; bandwith: ����)
    Returns:
        b (numpy.ndarray): �˲�������ϵ�� NUM
        a (numpy.ndarray): �˲�����ĸϵ�� DEN
    """
    w0 = 2.0 * np.pi * cut_freq / sample_freq  # �����һ����Ƶ��
    alpha = np.sin(w0) / (2.0 * Q)  # �����˲����� alpha ����
    b = [(1.0 - np.cos(w0)) / 2.0, 1 - np.cos(w0), (1.0 - np.cos(w0)) / 2.0]  # ����ϵ��
    a = [1.0 + alpha, -2.0 * np.cos(w0), 1.0 - alpha]  # ��ĸϵ��
    a0 = a[0]
    b = np.array(b) / a0
    a = np.array(a) / a0
    return np.array(b), np.array(a)


def highpass_filter_coefficients(cut_freq, sample_freq, Q=0.707):
    """
    �����ͨ�˲���ϵ��
    ʱ���ϵ: a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
    Args:
        cut_freq (float): ��ֹƵ�� (Hz)
        sample_freq (float): ����Ƶ�� (Hz)
        Q (float): Ʒ������, �������� (Q = center_freq / bandwith, center_freq: ����Ƶ��; bandwith: ����)
    Returns:
        b (numpy.ndarray): �˲�������ϵ�� NUM
        a (numpy.ndarray): �˲�����ĸϵ�� DEN
    """
    w0 = 2.0 * np.pi * cut_freq / sample_freq  # �����һ����Ƶ��
    alpha = np.sin(w0) / (2.0 * Q)  # �����˲����� alpha ����
    b = [(1.0 + np.cos(w0)) / 2.0, -1.0 - np.cos(w0), (1.0 + np.cos(w0)) / 2.0]  # ����ϵ��
    a = [1.0 + alpha, -2.0 * np.cos(w0), 1.0 - alpha]  # ��ĸϵ��
    a0 = a[0]
    b = np.array(b) / a0
    a = np.array(a) / a0
    return np.array(b), np.array(a)


def bandpass_filter_coefficients(low_cut, high_cut, sample_freq):
    """
    �����ͨ�˲���ϵ�� (Ч����̫����)
    ʱ���ϵ: a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
    Args:
        cut_freq (float): ��ֹƵ�� (Hz)
        sample_freq (float): ����Ƶ�� (Hz)
    Returns:
        b (numpy.ndarray): �˲�������ϵ�� NUM
        a (numpy.ndarray): �˲�����ĸϵ�� DEN
    """
    center_freq = np.sqrt(low_cut * high_cut)  # ��������Ƶ��
    bandwith = high_cut - low_cut  # �������
    Q = center_freq / bandwith  # ����Ʒ������ �������� (Q = center_freq / bandwith, center_freq: ����Ƶ��; bandwith: ����)
    w0 = 2.0 * np.pi * center_freq / sample_freq  # �����һ����Ƶ��
    alpha = np.sin(w0) / (2.0 * Q)  # �����˲����� alpha ����
    b = [alpha, 0.0, -alpha]  # ����ϵ��
    a = [1.0 + alpha, -2.0 * np.cos(w0), 1.0 - alpha]  # ��ĸϵ��
    a0 = a[0]
    b = np.array(b) / a0
    a = np.array(a) / a0
    return np.array(b), np.array(a)


def bandstop_filter_coefficients(low_cut, high_cut, sample_freq):
    """
    �����ͨ�˲���ϵ�� (Ч����̫����)
    ʱ���ϵ: a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
    Args:
        cut_freq (float): ��ֹƵ�� (Hz)
        sample_freq (float): ����Ƶ�� (Hz)
    Returns:
        b (numpy.ndarray): �˲�������ϵ�� NUM
        a (numpy.ndarray): �˲�����ĸϵ�� DEN
    """
    center_freq = np.sqrt(low_cut * high_cut)  # ��������Ƶ��
    bandwith = high_cut - low_cut  # �������
    Q = center_freq / bandwith  # ����Ʒ������ �������� (Q = center_freq / bandwith, center_freq: ����Ƶ��; bandwith: ����)
    w0 = 2.0 * np.pi * center_freq / sample_freq  # �����һ����Ƶ��
    alpha = np.sin(w0) / (2.0 * Q)  # �����˲����� alpha ����
    b = [1.0, -2.0 * np.cos(w0), 1.0]  # ����ϵ��
    a = [1.0 + alpha, -2.0 * np.cos(w0), 1.0 - alpha]  # ��ĸϵ��
    a0 = a[0]
    b = np.array(b) / a0
    a = np.array(a) / a0
    return np.array(b), np.array(a)


freq =100
sample_freq = 2000
b, a = notch_filter_coefficients(freq, sample_freq)
# b, a = lowpass_filter_coefficients(freq, sample_freq)
# b, a = highpass_filter_coefficients(freq, sample_freq)
# b, a = bandpass_filter_coefficients(10, 100, sample_freq)
# b, a = bandstop_filter_coefficients(10, 100, sample_freq)

mark_Hz = freq
mark_dB = -3
print(b)
print(a)
plot_amplitude_phase(b, a, sample_freq, mark_Hz, mark_dB)


*/
