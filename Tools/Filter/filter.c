/**
  ******************************************************************************
  * @file           : filter.c
  * @brief          : 滤波器函数功能文件. 包括陷波滤波器, 低通滤波器, 高通滤波器, 带通滤波器.
                      滤波器分子系数: b (numerator)
                      滤波器分母系数: a (denominator)
                      滤波器结构:
                      a(1)*y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(order)*x(n-order)- a(2)*y(n-1) - ... - a(order)*y(n-order)
                      滤波器实现 (a[0] = 1):
                      y[0] = b[0]*x[0] + b[1]*x[1] + ... + b[n-order]*x[n-order] - a[1]*y[1] - ... - a[n-order]*y[n-order]
  * @attention      :
                      主程序使用示例 (仅做参考):
						
						FilterTypeDef filter_nt_data1; // 定义滤波器结构体
						
                        int main(void) {

                            int32_t xn, yn; // 输入输出信号
                            float fs = 500; // 采样频率
                            float notch_cut = 50.0f; // 陷波频率

                            init_filter(&filter_nt_data1, NOTCH, fs, notch_cut, 0.0f, 0.0f); // 滤波器初始化

                            while(1) {

                                   yn = (int32_t)(apply_filter((float)xn, &filter_nt_data1));// 滤波处理

                            }
                            
                            return 0;

                        }

  ******************************************************************************
  */

#include "filter.h"

// Example of defining a structure for processing a notch filter for data1
FilterTypeDef filter_nt_data1;

/**
  * @brief  二阶滤波器初始化函数
  * @note   滤波器的基本公式为:
  *         a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
  *         当a[0] = 1 时:
  *         y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
  * @note   notch filter init example:      init_filter(&filter_nt_data1, NOTCH, fs, notch_cut, 0.0f, 0.0f);
            lowpass filter init example:    init_filter(&filter_lp_data1, LOWPASS, fs, 0.0f, low_cut, 0.0f);
            highpass filter init example:   init_filter(&filter_hp_data1, HIGHPASS, fs, 0.0f, 0.0f, high_cut);
            bandpass filter init example:   init_filter(&filter_bp_data1, BANDPASS, fs, 0.0f, low_cut, high_cut);
            bandstop filter init example:   init_filter(&filter_bs_data1, BANDSTOP, fs, 0.0f, low_cut, high_cut);
  * @param  filter:     滤波器结构体地址
  * @param  class:      滤波器类型
  * @param  fs:         采样频率 (hz)
  * @param  notch_cut:  陷波频率 (滤波器类型不为陷波滤波器时, 此参数不起作用, 可输入0)
  * @param  low_cut:    低通滤波器截止频率 (带通和带组滤波器时 low_cut 和 high_cut 配合使用)
  * @param  high_cut:   高通滤波器截止频率 (带通和带组滤波器时 low_cut 和 high_cut 配合使用)
  * @retval None
  */
void init_filter(FilterTypeDef *filter, FilterClassType class, float fs,  float notch_cut, float low_cut, float high_cut) {

    float w0;       // 归一化角频率
    float alpha;    // alpha 参数
    float a0;       // 滤波器分母系数 a0, 通过 a0 将系数 filter->a[0]归一化为1方便计算
    filter->class = class;
    filter->fs = fs;
    filter->notch_cut = notch_cut;
    filter->low_cut = low_cut;
    filter->high_cut = high_cut;
    // Initialisation x and y
    filter->x[0] = 0.0f;
    filter->x[1] = 0.0f;
    filter->x[2] = 0.0f;
    filter->y[0] = 0.0f;
    filter->y[1] = 0.0f;
    filter->y[2] = 0.0f;
    
    // Initialisation of the notch filter structure
    if(filter->class == NOTCH) {
        filter->q = 30.0f;
        w0 = 2.0f * 3.14159265f * filter->notch_cut / filter->fs;
        alpha = sinf(w0) / (2.0f / filter->q);
        // compute b and a
        filter->b[0] = 1.0f;
        filter->b[1] = -2.0 * cosf(w0);
        filter->b[2] = 1.0f;
        filter->a[0] = 1.0f + alpha;
        filter->a[1] = -2.0f * cosf(w0);
        filter->a[2] = 1.0f - alpha;
        // uniformization
        a0 = filter->a[0];
        filter->b[0] = filter->b[0] / a0;
        filter->b[1] = filter->b[1] / a0;
        filter->b[2] = filter->b[2] / a0;
        filter->a[0] = filter->a[0] / a0;
        filter->a[1] = filter->a[1] / a0;
        filter->a[2] = filter->a[2] / a0;
    }
    // Initialisation of the lowpass filter structure
    else if(filter->class == LOWPASS) {
        filter->q = 0.707f;
        w0 = 2.0f * 3.14159265f * filter->low_cut / filter->fs;
        alpha = sinf(w0) / (2.0f / filter->q);
        // compute b and a
        filter->b[0] = (1.0f - cosf(w0)) / 2.0f;
        filter->b[1] = 1.0f - cosf(w0);
        filter->b[2] = (1.0f - cosf(w0)) / 2.0f;
        filter->a[0] = 1.0f + alpha;
        filter->a[1] = -2.0f * cosf(w0);
        filter->a[2] = 1.0f - alpha;
        // uniformization
        a0 = filter->a[0];
        filter->b[0] = filter->b[0] / a0;
        filter->b[1] = filter->b[1] / a0;
        filter->b[2] = filter->b[2] / a0;
        filter->a[0] = filter->a[0] / a0;
        filter->a[1] = filter->a[1] / a0;
        filter->a[2] = filter->a[2] / a0;
    }
    // Initialisation of the highpass filter structure
    else if(filter->class == HIGHPASS) {
        filter->q = 0.707f;
        w0 = 2.0f * 3.14159265f * filter->high_cut / filter->fs;
        alpha = sinf(w0) / (2.0f / filter->q);
        // compute b and a
        filter->b[0] = (1.0f + cosf(w0)) / 2.0f;
        filter->b[1] = -1.0f - cosf(w0);
        filter->b[2] = (1.0f + cosf(w0)) / 2.0f;
        filter->a[0] = 1.0f + alpha;
        filter->a[1] = -2.0f * cosf(w0);
        filter->a[2] = 1.0f - alpha;
        // uniformization
        a0 = filter->a[0];
        filter->b[0] = filter->b[0] / a0;
        filter->b[1] = filter->b[1] / a0;
        filter->b[2] = filter->b[2] / a0;
        filter->a[0] = filter->a[0] / a0;
        filter->a[1] = filter->a[1] / a0;
        filter->a[2] = filter->a[2] / a0;
    }
    // Initialisation of the bandpass filter structure
    else if(filter->class == BANDPASS) {
        float center_freq = sqrtf(filter->low_cut * filter->high_cut);
        float bandwith = filter->high_cut - filter->low_cut;
        filter->q = center_freq / bandwith;
        w0 = 2.0f * 3.14159265f * center_freq / filter->fs;
        alpha = sinf(w0) / (2.0f / filter->q);
        // compute b and a
        filter->b[0] = alpha;
        filter->b[1] = 0.0f;
        filter->b[2] = -alpha;
        filter->a[0] = 1.0f + alpha;
        filter->a[1] = -2.0f * cosf(w0);
        filter->a[2] = 1.0f - alpha;
        // uniformization
        a0 = filter->a[0];
        filter->b[0] = filter->b[0] / a0;
        filter->b[1] = filter->b[1] / a0;
        filter->b[2] = filter->b[2] / a0;
        filter->a[0] = filter->a[0] / a0;
        filter->a[1] = filter->a[1] / a0;
        filter->a[2] = filter->a[2] / a0;
    }
    // Initialisation of the bandstop filter structure
    else if(filter->class == BANDSTOP) {
        float center_freq = sqrtf(filter->low_cut * filter->high_cut);
        float bandwith = filter->high_cut - filter->low_cut;
        filter->q = center_freq / bandwith;
        w0 = 2.0f * 3.14159265f * center_freq / filter->fs;
        alpha = sinf(w0) / (2.0f / filter->q);
        // compute b and a
        filter->b[0] = 1.0f;
        filter->b[1] = -2.0f * cosf(w0);
        filter->b[2] = 1.0f;
        filter->a[0] = 1.0f + alpha;
        filter->a[1] = -2.0f * cosf(w0);
        filter->a[2] = 1.0f - alpha;
        // uniformization
        a0 = filter->a[0];
        filter->b[0] = filter->b[0] / a0;
        filter->b[1] = filter->b[1] / a0;
        filter->b[2] = filter->b[2] / a0;
        filter->a[0] = filter->a[0] / a0;
        filter->a[1] = filter->a[1] / a0;
        filter->a[2] = filter->a[2] / a0;
    }
}


/**
  * @brief  二阶滤波器应用函数
  * @note   滤波器的基本公式为:
  *         a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
  *         当a[0] = 1 时:
  *         y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
  * @param  input:      当前时刻输入值
  * @param  filter:     滤波器结构体地址
  * @retval output：    当前时刻输出值
  */
float apply_filter(float input, FilterTypeDef *filter) {
    // update x[n]
    filter->x[0] = input;
    // compute output (update y[n])
    // y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
    filter->y[0] = filter->b[0] * filter->x[0] + filter->b[1] * filter->x[1] + filter->b[2] * filter->x[2] \
                                               - filter->a[1] * filter->y[1] - filter->a[2] * filter->y[2];
    // update x and y
    filter->x[2] = filter->x[1];
    filter->x[1] = filter->x[0];
    filter->y[2] = filter->y[1];
    filter->y[1] = filter->y[0];
    // return output
    return filter->y[0];
}

