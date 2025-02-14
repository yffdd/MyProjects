/**
  ******************************************************************************
  * @file           : filter.c
  * @brief          : �˲������������ļ�. �����ݲ��˲���, ��ͨ�˲���, ��ͨ�˲���, ��ͨ�˲���.
                      �˲�������ϵ��: b (numerator)
                      �˲�����ĸϵ��: a (denominator)
                      �˲����ṹ:
                      a(1)*y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(order)*x(n-order)- a(2)*y(n-1) - ... - a(order)*y(n-order)
                      �˲���ʵ�� (a[0] = 1):
                      y[0] = b[0]*x[0] + b[1]*x[1] + ... + b[n-order]*x[n-order] - a[1]*y[1] - ... - a[n-order]*y[n-order]
  * @attention      :
                      ������ʹ��ʾ�� (�����ο�):
						
						FilterTypeDef filter_nt_data1; // �����˲����ṹ��
						
                        int main(void) {

                            int32_t xn, yn; // ��������ź�
                            float fs = 500; // ����Ƶ��
                            float notch_cut = 50.0f; // �ݲ�Ƶ��

                            init_filter(&filter_nt_data1, NOTCH, fs, notch_cut, 0.0f, 0.0f); // �˲�����ʼ��

                            while(1) {

                                   yn = (int32_t)(apply_filter((float)xn, &filter_nt_data1));// �˲�����

                            }
                            
                            return 0;

                        }

  ******************************************************************************
  */

#include "filter.h"

// Example of defining a structure for processing a notch filter for data1
FilterTypeDef filter_nt_data1;

/**
  * @brief  �����˲�����ʼ������
  * @note   �˲����Ļ�����ʽΪ:
  *         a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
  *         ��a[0] = 1 ʱ:
  *         y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
  * @note   notch filter init example:      init_filter(&filter_nt_data1, NOTCH, fs, notch_cut, 0.0f, 0.0f);
            lowpass filter init example:    init_filter(&filter_lp_data1, LOWPASS, fs, 0.0f, low_cut, 0.0f);
            highpass filter init example:   init_filter(&filter_hp_data1, HIGHPASS, fs, 0.0f, 0.0f, high_cut);
            bandpass filter init example:   init_filter(&filter_bp_data1, BANDPASS, fs, 0.0f, low_cut, high_cut);
            bandstop filter init example:   init_filter(&filter_bs_data1, BANDSTOP, fs, 0.0f, low_cut, high_cut);
  * @param  filter:     �˲����ṹ���ַ
  * @param  class:      �˲�������
  * @param  fs:         ����Ƶ�� (hz)
  * @param  notch_cut:  �ݲ�Ƶ�� (�˲������Ͳ�Ϊ�ݲ��˲���ʱ, �˲�����������, ������0)
  * @param  low_cut:    ��ͨ�˲�����ֹƵ�� (��ͨ�ʹ����˲���ʱ low_cut �� high_cut ���ʹ��)
  * @param  high_cut:   ��ͨ�˲�����ֹƵ�� (��ͨ�ʹ����˲���ʱ low_cut �� high_cut ���ʹ��)
  * @retval None
  */
void init_filter(FilterTypeDef *filter, FilterClassType class, float fs,  float notch_cut, float low_cut, float high_cut) {

    float w0;       // ��һ����Ƶ��
    float alpha;    // alpha ����
    float a0;       // �˲�����ĸϵ�� a0, ͨ�� a0 ��ϵ�� filter->a[0]��һ��Ϊ1�������
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
  * @brief  �����˲���Ӧ�ú���
  * @note   �˲����Ļ�����ʽΪ:
  *         a[0] * y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
  *         ��a[0] = 1 ʱ:
  *         y[n] = b[0] * x[n] + b[1] * x[n-1]  + b[2] * x[n-2] - a[1] * y[n-1] - a[2] * y[n-2]
  * @param  input:      ��ǰʱ������ֵ
  * @param  filter:     �˲����ṹ���ַ
  * @retval output��    ��ǰʱ�����ֵ
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

