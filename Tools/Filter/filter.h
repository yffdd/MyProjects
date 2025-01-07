/**
  ******************************************************************************
  * @file           : filter.h
  * @brief          : 滤波器函数功能文件. 包括陷波滤波器, 低通滤波器, 高通滤波器, 带通滤波器.
  * @attention      : None

  ******************************************************************************
  */


#ifndef __FILTER_H
#define __FILTER_H

#include "main.h"
#include "math.h"
#include <stdlib.h>
#include "usart.h"

/**
  * @brief  是否使用MATLAB
  * @param  1: 使用
  * @param  0: 不使用
  */
#define MATLAB					1


#define F_PI					3.141593		// 圆周率
#define FILTER_Q				0.707           // 滤波品质因数


extern float sample_freq;                       // 采样频率 (Hz)
extern float notch_freq;                        // 陷波滤波器 陷波频率 (Hz)
extern float lowpass_freq;                      // 低通滤波器 低截止频率 (Hz)
extern float highpass_freq;                     // 高通滤波器 高截止频率 (Hz)


/**
  * @brief 滤波器输入输出延迟线结构体
  */
typedef struct {
    float x1, x2; // 输入延迟线
    float y1, y2; // 输出延迟线
}filter_dl;


extern filter_dl nt_filter;		                // 陷波滤波器结构体
extern filter_dl lp_filter;		                // 低通滤波器结构体
extern filter_dl hp_filter;		                // 高通滤波器结构体


extern float nt_a[3];                           // 陷波滤波器分母系数
extern float nt_b[3];                           // 陷波滤波器分子系数

extern float lp_a[3];                           // 低通滤波器分母系数
extern float lp_b[3];                           // 低通滤波器分子系数

extern float hp_a[3];                           // 高通滤波器分母系数
extern float hp_b[3];                           // 高通滤波器分子系数


void Filter_Coe_Init(float freq);// 滤波器系数初始化
void Notch_Filter_Init(float *nt_a, float *nt_b, float freq);// 陷波滤波器系数初始化
void Lowpass_Filter_Init(float *lp_a, float *lp_b, float freq);// 低通滤波器系数初始化
void Highpass_Filter_Init(float *hp_a, float *hp_b, float freq);// 高通滤波器系数初始化
float Notch_Filter(float signal);// 陷波滤波器处理函数
float Lowpass_Filter(float signal);// 低通滤波器处理函数
float Highpass_Filter(float signal);// 高通滤波器处理函数



/******************************************************************************************/
#if MATLAB

/* 滤波器阶数必须为偶数, 由于位宽限制目前只能使用2阶, 阶数过高会导致数据溢出, 经过测试4阶高通和4阶低通可用. (如果输出满量程或输出0, 则是数据溢出)*/
#define F_ORDER					2               // 滤波器阶数


/* MATLAB IIR滤波采样频率, 可选250Hz, 500Hz, 1000Hz, 2000Hz. (如果想使用其它采样频率, 需自己重新生产系数) */
#define MATLAB_FS		        500            // 滤波器采样频率


extern float lp_den[F_ORDER+1];                 // 低通滤波器分母系数
extern float lp_num[F_ORDER+1];                 // 低通滤波器分子系数
extern float lp_xn[F_ORDER+1];                  // 低通滤波器输入数组
extern float lp_yn[F_ORDER+1];                  // 低通滤波器输出数组

extern float hp_den[F_ORDER+1];                 // 高通滤波器分母系数
extern float hp_num[F_ORDER+1];                 // 高通滤波器分子系数
extern float hp_xn[F_ORDER+1];                  // 高通滤波器输入数组
extern float hp_yn[F_ORDER+1];                  // 高通滤波器输出数组

extern float bp_den[F_ORDER+1];                 // 带通滤波器分母系数
extern float bp_num[F_ORDER+1];                 // 带通滤波器分子系数
extern float bp_xn[F_ORDER+1];                  // 带通滤波器输入数组
extern float bp_yn[F_ORDER+1];                  // 带通滤波器输出数组

extern float bs_den[F_ORDER+1];                 // 带阻滤波器分母系数
extern float bs_num[F_ORDER+1];                 // 带阻滤波器分子系数
extern float bs_xn[F_ORDER+1];                  // 带阻滤波器输入数组
extern float bs_yn[F_ORDER+1];                  // 带阻滤波器输出数组


#define LP_FILTER               0               //低通滤波器
#define HP_FILTER               1               //高通滤波器
#define BP_FILTER               2               //带通滤波器
#define BS_FILTER               3               //带阻滤波器


float MATLAB_Fliter(uint8_t type, float xn);// MATLAB滤波函数
float MATLAB_IIR_Model(float *Num, float *Den, float *xnReg, float *ynReg, float xn);// IIR滤波器功能模块函数
void MATLAB_IIR_Coe_Init(void);// MATLAB系数初始化函数


#endif /* MATLAB */



#endif /* __FILTER_H */
