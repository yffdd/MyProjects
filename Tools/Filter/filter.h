/**
  ******************************************************************************
  * @file           : filter.h
  * @brief          : 滤波器函数功能文件
  * @attention      : None

  ******************************************************************************
  */


// filter.h
#ifndef FILTER_H
#define FILTER_H

#include <math.h>

// 滤波器类型枚举变量
typedef enum {
    NOTCH=1,      // 陷波滤波器
    LOWPASS,   // 低通滤波器
    HIGHPASS,  // 高通滤波器
    BANDPASS,  // 带通滤波器
    BANDSTOP   // 带阻滤波器
} FilterClassType;

// 滤波器参数结构体
// a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
typedef struct filter {
    FilterClassType class;  // 滤波器类型
    float fs;               // 采样频率
    float notch_cut;        // 陷波频率
    float low_cut;          // 低通频率 (带通和带组滤波器时 low_cut 和 high_cut 配合使用)
    float high_cut;         // 高通频率 (带通和带组滤波器时 low_cut 和 high_cut 配合使用)
    float q;                // 品质因数Q （Q = center_freq / bandwith, center_freq: 中心频率; bandwith: 带宽）
    float b[3];             // 滤波器分子系数 numerator
    float a[3];             // 滤波器分母系数 denominator
    float x[3];             // 输入信号 x[n], x[n-1], x[n-2]
    float y[3];             // 输出信号 y[n], y[n-1], y[n-2]
}FilterTypeDef;


extern FilterTypeDef filter_nt_data1;

void init_filter(FilterTypeDef *filter, FilterClassType class, float fs,  float notch_cut, float low_cut, float high_cut);
float apply_filter(float input, FilterTypeDef *filter);

#endif



/*
# python 验证代码(绘制幅频特性和相频特性曲线):
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import freqz

def plot_amplitude_phase(b, a, sample_freq, mark_Hz=None, mark_dB=-3):
    #  计算幅频响应
    frequencies = np.linspace(0, sample_freq / 2, 1000)
    w, h = freqz(b, a, worN=frequencies, fs=sample_freq)
    amplitude = 20 * np.log10(abs(h))
    phase = np.angle(h)
    # 绘制幅频特性
    plt.figure(figsize=(8, 6))
    plt.subplot(2, 1, 1)
    plt.plot(frequencies, amplitude)
    plt.title("Filter Frequency Response")
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Amplitude (dB)")
    if mark_Hz is not None:
        # 标记特定频率点
        mark_Hz  = mark_Hz  # 标记频率值 (Hz)
        mark_Hz_idx = np.argmin(np.abs(frequencies - mark_Hz))  # 找到最接近目标频率点的位置
        mark_Hz_freq = frequencies[mark_Hz_idx]  # 获取该频率点对应的频率值
        mark_Hz_amplitude = 20 * np.log10(abs(h[mark_Hz_idx]))  # 计算幅度
        plt.plot(mark_Hz, mark_Hz_amplitude, 'go')  # 绘制标记点
        plt.text(mark_Hz, mark_Hz_amplitude, f"({mark_Hz_freq:.2f} Hz, {mark_Hz_amplitude:.2f} dB)", color='red')  # 添加文本
    if mark_dB is not None:
        # 标记特定幅度点
        mark_dB = mark_dB  # 标记幅度值 (dB)
        mark_dB_idx = np.argmin(np.abs(amplitude - mark_dB))  # 找到最接近目标幅度点的位置
        mark_dB_freq = frequencies[mark_dB_idx]  # 获取该幅度点对应的频率值
        mark_dB_amplitude = 20 * np.log10(abs(h[mark_dB_idx]))  # 计算幅度
        plt.plot(mark_dB_freq, mark_dB_amplitude, 'ro')  # 绘制标记点
        plt.text(mark_dB_freq, mark_dB_amplitude, f"({mark_dB_freq:.2f} Hz, {mark_dB_amplitude:.2f} dB)", color='red')  # 添加文本
    # 绘制相频特性
    plt.subplot(2, 1, 2)
    plt.plot(frequencies, phase)
    plt.title("Filter Phase Response")
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Phase (radians)")
    # 绘图
    plt.tight_layout()
    plt.show()

def notch_filter_coefficients(notch_freq, sample_freq, Q=30):
    """
    计算陷波滤波器系数
    时域关系: a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
    Args:
        notch_freq (float): 滤波器的中心频率 (Hz)
        sample_freq (float): 采样频率 (Hz)
        Q (float): 品质因数, 决定带宽 (Q = center_freq / bandwith, center_freq: 中心频率; bandwith: 带宽)
    Returns:
        b (numpy.ndarray): 滤波器分子系数 NUM
        a (numpy.ndarray): 滤波器分母系数 DEN
    """
    w0 = 2.0 * np.pi * notch_freq / sample_freq  # 计算归一化角频率
    alpha = np.sin(w0) / (2.0 * Q)  # 计算滤波器的 alpha 参数
    b = [1.0, -2.0 * np.cos(w0), 1.0]  # 分子系数
    a = [1.0 + alpha, -2.0 * np.cos(w0), 1.0 - alpha]  # 分母系数
    a0 = a[0]
    b = np.array(b) / a0
    a = np.array(a) / a0
    return np.array(b), np.array(a)


def lowpass_filter_coefficients(cut_freq, sample_freq, Q=0.707):
    """
    计算低通滤波器系数
    时域关系: a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
    Args:
        cut_freq (float): 截止频率 (Hz)
        sample_freq (float): 采样频率 (Hz)
        Q (float): 品质因数, 决定带宽 (Q = center_freq / bandwith, center_freq: 中心频率; bandwith: 带宽)
    Returns:
        b (numpy.ndarray): 滤波器分子系数 NUM
        a (numpy.ndarray): 滤波器分母系数 DEN
    """
    w0 = 2.0 * np.pi * cut_freq / sample_freq  # 计算归一化角频率
    alpha = np.sin(w0) / (2.0 * Q)  # 计算滤波器的 alpha 参数
    b = [(1.0 - np.cos(w0)) / 2.0, 1 - np.cos(w0), (1.0 - np.cos(w0)) / 2.0]  # 分子系数
    a = [1.0 + alpha, -2.0 * np.cos(w0), 1.0 - alpha]  # 分母系数
    a0 = a[0]
    b = np.array(b) / a0
    a = np.array(a) / a0
    return np.array(b), np.array(a)


def highpass_filter_coefficients(cut_freq, sample_freq, Q=0.707):
    """
    计算高通滤波器系数
    时域关系: a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
    Args:
        cut_freq (float): 截止频率 (Hz)
        sample_freq (float): 采样频率 (Hz)
        Q (float): 品质因数, 决定带宽 (Q = center_freq / bandwith, center_freq: 中心频率; bandwith: 带宽)
    Returns:
        b (numpy.ndarray): 滤波器分子系数 NUM
        a (numpy.ndarray): 滤波器分母系数 DEN
    """
    w0 = 2.0 * np.pi * cut_freq / sample_freq  # 计算归一化角频率
    alpha = np.sin(w0) / (2.0 * Q)  # 计算滤波器的 alpha 参数
    b = [(1.0 + np.cos(w0)) / 2.0, -1.0 - np.cos(w0), (1.0 + np.cos(w0)) / 2.0]  # 分子系数
    a = [1.0 + alpha, -2.0 * np.cos(w0), 1.0 - alpha]  # 分母系数
    a0 = a[0]
    b = np.array(b) / a0
    a = np.array(a) / a0
    return np.array(b), np.array(a)


def bandpass_filter_coefficients(low_cut, high_cut, sample_freq):
    """
    计算带通滤波器系数 (效果不太理想)
    时域关系: a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
    Args:
        cut_freq (float): 截止频率 (Hz)
        sample_freq (float): 采样频率 (Hz)
    Returns:
        b (numpy.ndarray): 滤波器分子系数 NUM
        a (numpy.ndarray): 滤波器分母系数 DEN
    """
    center_freq = np.sqrt(low_cut * high_cut)  # 计算中心频率
    bandwith = high_cut - low_cut  # 计算带宽
    Q = center_freq / bandwith  # 计算品质因数 决定带宽 (Q = center_freq / bandwith, center_freq: 中心频率; bandwith: 带宽)
    w0 = 2.0 * np.pi * center_freq / sample_freq  # 计算归一化角频率
    alpha = np.sin(w0) / (2.0 * Q)  # 计算滤波器的 alpha 参数
    b = [alpha, 0.0, -alpha]  # 分子系数
    a = [1.0 + alpha, -2.0 * np.cos(w0), 1.0 - alpha]  # 分母系数
    a0 = a[0]
    b = np.array(b) / a0
    a = np.array(a) / a0
    return np.array(b), np.array(a)


def bandstop_filter_coefficients(low_cut, high_cut, sample_freq):
    """
    计算带通滤波器系数 (效果不太理想)
    时域关系: a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
    Args:
        cut_freq (float): 截止频率 (Hz)
        sample_freq (float): 采样频率 (Hz)
    Returns:
        b (numpy.ndarray): 滤波器分子系数 NUM
        a (numpy.ndarray): 滤波器分母系数 DEN
    """
    center_freq = np.sqrt(low_cut * high_cut)  # 计算中心频率
    bandwith = high_cut - low_cut  # 计算带宽
    Q = center_freq / bandwith  # 计算品质因数 决定带宽 (Q = center_freq / bandwith, center_freq: 中心频率; bandwith: 带宽)
    w0 = 2.0 * np.pi * center_freq / sample_freq  # 计算归一化角频率
    alpha = np.sin(w0) / (2.0 * Q)  # 计算滤波器的 alpha 参数
    b = [1.0, -2.0 * np.cos(w0), 1.0]  # 分子系数
    a = [1.0 + alpha, -2.0 * np.cos(w0), 1.0 - alpha]  # 分母系数
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
