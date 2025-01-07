/**
  ******************************************************************************
  * @file           : filter.c
  * @brief          : 滤波器函数功能文件. 包括陷波滤波器, 低通滤波器, 高通滤波器, 带通滤波器.
                      滤波器分子系数: Num 或者 b
                      滤波器分母系数: Den 或者 a
                      IIR 滤波器结构:
                      a(1)*y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(order)*x(n-order)- a(2)*y(n-1) - ... - a(order)*y(n-order)
                      IIR 滤波器实现:
                      a[0]*y[0] = b[0]*x[0] + b[1]*x[1] + ... + b[order]*x[order] - a[1]*y[1] - ... - a[order]*y[order]
  * @attention      :
                      主程序使用示例1, 使用C语言计算滤波器系数进行滤波 (仅做参考):

                        void main(void) {

                        int32_t xn, yn;// 输入输出信号

                            Filter_Coe_Init(2000);// 滤波器系数与采样频率初始化

                            while(1) {

                                   yn = (int32_t)(Notch_Filter((float)xn));// 滤波处理

                            }

                        }

                        主程序使用示例2, 结合MATLAB生成的IIR滤波器系数进行滤波 (仅做参考):

                        void main(void) {

                        int32_t xn, yn;// 输入输出信号

                            MATLAB_IIR_Coe_Init();// MATLAB IIR滤波器系数初始化

                            while(1) {

                                   yn = (int32_t)MATLAB_Fliter(BS_FILTER, (float)xn);// 滤波处理

                            }

                        }

  ******************************************************************************
  */


#include "filter.h"

float sample_freq = 500;       // 采样频率 (Hz)
float notch_freq = 50;          // 陷波滤波器 陷波频率 (Hz)
float lowpass_freq = 100;       // 低通滤波器 低截止频率 (Hz)
float highpass_freq = 1;       // 高通滤波器 高截止频率 (Hz)


filter_dl nt_filter = {0};		// 陷波滤波器结构体
filter_dl lp_filter = {0};		// 低通滤波器结构体
filter_dl hp_filter = {0};		// 高通滤波器结构体


float nt_a[3] = {0};			// 陷波滤波器分母系数
float nt_b[3] = {0};			// 陷波滤波器分子系数


float lp_a[3] = {0};			// 低通滤波器分母系数
float lp_b[3] = {0};			// 低通滤波器分子系数


float hp_a[3] = {0};			// 高通滤波器分母系数
float hp_b[3] = {0};			// 高通滤波器分子系数



/**
  * @brief  滤波器系数初始化
  * @note   None
  * @param  freq: 滤波器采样频率
  * @retval None
  */
void Filter_Coe_Init(float freq) {

    sample_freq = freq;                             // 设置滤波器采样频率

	Notch_Filter_Init(nt_a, nt_b, notch_freq);      // 陷波滤波器系数初始化

	Lowpass_Filter_Init(lp_a, lp_b, lowpass_freq);  // 低通滤波器系数初始化

	Highpass_Filter_Init(hp_a, hp_b, highpass_freq);// 高通滤波器系数初始化

}



/**
  * @brief  陷波滤波器系数初始化
  * @note   初始化示例:
			Notch_Filter_Init(nt_a, nt_b);// 陷波滤波器系数初始化
  * @param  nt_a: 陷波滤波器分母系数
			nt_b: 陷波滤波器分子系数
            freq: 滤波器频率
  * @retval None
  */
void Notch_Filter_Init(float *nt_a, float *nt_b, float freq) {
	float w0 = 2.0 * F_PI * freq / sample_freq;
	float alpha = sin(w0) / (2.0 * FILTER_Q);
	nt_a[0] = 1.0 + alpha;
	nt_a[1] = -2.0 * cos(w0);
	nt_a[2] = 1.0 - alpha;
	nt_b[0] = 1.0;
	nt_b[1] = -2.0 * cos(w0);
	nt_b[2] = 1.0;

}


/**
  * @brief  低通滤波器系数初始化
  * @note   初始化示例:
			Lowpass_Filter_Init(lp_a, lp_b);// 低通滤波器系数初始化
  * @param  lp_a: 低通滤波器分母系数
			lp_b: 低通滤波器分子系数
            freq: 滤波器频率
  * @retval None
  */
void Lowpass_Filter_Init(float *lp_a, float *lp_b, float freq) {

    float w0 = 2.0 * F_PI * freq / sample_freq;
    float alpha = sin(w0) / (2.0 * FILTER_Q);

    lp_a[0] = 1.0 + alpha;
    lp_a[1] = -2.0 * cos(w0);
    lp_a[2] = 1.0 - alpha;

    lp_b[0] = (1.0 - cos(w0)) / 2.0;
    lp_b[1] = 1.0 - cos(w0);
    lp_b[2] = (1.0 - cos(w0)) / 2.0;

}


/**
  * @brief  高通滤波器系数初始化
  * @note   初始化示例:
			Highpass_Filter_Init(hp_a, hp_b);// 高通滤波器系数初始化
  * @param  hp_a: 高通滤波器分母系数
			hp_b: 高通滤波器分子系数
            freq: 滤波器频率
  * @retval None
  */
void Highpass_Filter_Init(float *hp_a, float *hp_b, float freq) {

    float w0 = 2.0 * F_PI * freq / sample_freq;
    float alpha = sin(w0) / (2.0 * FILTER_Q);

    hp_a[0] = 1.0 + alpha;
    hp_a[1] = -2.0 * cos(w0);
    hp_a[2] = 1.0 - alpha;

    hp_b[0] = (1.0 + cos(w0)) / 2.0;
    hp_b[1] = -1.0 - cos(w0);
    hp_b[2] = (1.0 + cos(w0)) / 2.0;

}


/**
  * @brief  陷波滤波器处理函数
  * @note   使用示例:
			nt_y = Notch_Filter(nt_a, nt_b, (float)signal);// 陷波滤波器处理函数
  * @param  signal: 滤波信号
  * @retval nt_y: 陷波滤波器输出
  */
float Notch_Filter(float signal) {

    float nt_y;

	// 计算滤波器输出
	nt_y = (nt_b[0] / nt_a[0]) * signal + (nt_b[1] / nt_a[0]) * nt_filter.x1 + (nt_b[2] / nt_a[0]) * nt_filter.x2 \
										- (nt_a[1] / nt_a[0]) * nt_filter.y1 - (nt_a[2] / nt_a[0]) * nt_filter.y2;

    // 更新延迟线
    nt_filter.x2 = nt_filter.x1;
    nt_filter.x1 = signal;
    nt_filter.y2 = nt_filter.y1;
    nt_filter.y1 = nt_y;

    return nt_y;
}


/**
  * @brief  低通滤波器处理函数
  * @note   使用示例:
			lp_y = Lowpass_Filter(lp_a, lp_b, (float)signal);// 低通滤波器处理函数
  * @param  signal: 滤波信号
  * @retval lp_y: 低通滤波器输出
  */
float Lowpass_Filter(float signal) {

	float lp_y;

	// 计算滤波器输出
	lp_y = (lp_b[0] / lp_a[0]) * signal + (lp_b[1] / lp_a[0]) * lp_filter.x1 + (lp_b[2] / lp_a[0]) * lp_filter.x2 \
										- (lp_a[1] / lp_a[0]) * lp_filter.y1 - (lp_a[2] / lp_a[0]) * lp_filter.y2;

    // 更新延迟线
    lp_filter.x2 = lp_filter.x1;
    lp_filter.x1 = signal;
    lp_filter.y2 = lp_filter.y1;
    lp_filter.y1 = lp_y;

    return lp_y;

}


/**
  * @brief  高通滤波器处理函数
  * @note   使用示例:
			hp_y = Highpass_Filter(hp_a, hp_b, (float)signal);// 高通滤波器处理函数
  * @param  signal: 滤波信号
  * @retval hp_y: 高通滤波器输出
  */
float Highpass_Filter(float signal) {

	float hp_y;

	// 计算滤波器输出
	hp_y = (hp_b[0] / hp_a[0]) * signal + (hp_b[1] / hp_a[0]) * hp_filter.x1 + (hp_b[2] / hp_a[0]) * hp_filter.x2 \
										- (hp_a[1] / hp_a[0]) * hp_filter.y1 - (hp_a[2] / hp_a[0]) * hp_filter.y2;

    // 更新延迟线
    hp_filter.x2 = hp_filter.x1;
    hp_filter.x1 = signal;
    hp_filter.y2 = hp_filter.y1;
    hp_filter.y1 = hp_y;

    return hp_y;

}


/******************************************************************************************/
#if MATLAB


float lp_den[F_ORDER+1] = {0};// 低通滤波器分母系数
float lp_num[F_ORDER+1] = {0};// 低通滤波器分子系数
float lp_xn[F_ORDER+1]  = {0};// 低通滤波器输入数组
float lp_yn[F_ORDER+1]  = {0};// 低通滤波器输出数组


float hp_den[F_ORDER+1] = {0};// 高通滤波器分母系数
float hp_num[F_ORDER+1] = {0};// 高通滤波器分子系数
float hp_xn[F_ORDER+1]  = {0};// 高通滤波器输入数组
float hp_yn[F_ORDER+1]  = {0};// 高通滤波器输出数组

float bp_den[F_ORDER+1] = {0};// 带通滤波器分母系数
float bp_num[F_ORDER+1] = {0};// 带通滤波器分子系数
float bp_xn[F_ORDER+1]  = {0};// 带通滤波器输入数组
float bp_yn[F_ORDER+1]  = {0};// 带通滤波器输出数组

float bs_den[F_ORDER+1] = {0};// 带阻滤波器分母系数
float bs_num[F_ORDER+1] = {0};// 带阻滤波器分子系数
float bs_xn[F_ORDER+1]  = {0};// 带阻滤波器输入数组
float bs_yn[F_ORDER+1]  = {0};// 带阻滤波器输出数组


/**
  * @brief  MATLAB滤波函数
  * @note   直接调用此函数时, 每个滤波器只能对一路输入信号使用, 是因为每个滤波器单独分配了输入输出数组寄存器, 不能混用多路信号
            如果需要将某个滤波器重复使用多次, 则需要重新定义输入输出数组寄存器分配给多个同类的滤波器, 并且调用 "MATLAB_IIR_Model"进行滤波处理
  * @param  type:   滤波器类型
                    LP_FILTER; 低通滤波器
                    HP_FILTER: 高通滤波器
                    BP_FILTER; 带通滤波器
                    BS_FILTER; 带阻滤波器
            xn: 输入信号
  * @retval yn: 输出信号
  */
float MATLAB_Fliter(uint8_t type, float xn) {

    float yn = 0;

    switch(type) {

        case LP_FILTER: yn = MATLAB_IIR_Model(lp_num, lp_den, lp_xn, lp_yn, xn); break;// 低通滤波器

        case HP_FILTER: yn = MATLAB_IIR_Model(hp_num, hp_den, hp_xn, hp_yn, xn); break;// 高通滤波器

        case BP_FILTER: yn = MATLAB_IIR_Model(bp_num, bp_den, bp_xn, bp_yn, xn); break;// 带通滤波器

        case BS_FILTER: yn = MATLAB_IIR_Model(bs_num, bs_den, bs_xn, bs_yn, xn); break;// 带阻滤波器

        default: yn = 0; break;

    }

    return yn;

}


/**
  * @brief  IIR滤波器功能模块函数
  * @note       // 二阶滤波器输出示例
                float MATLAB_IIR_Model(float *Num, float *Den, float *xnReg, float *ynReg, float xn) {
                    ynReg[0] = (Num[0] / Den[0]) * xn + (Num[1] / Den[0]) * xnReg[1] + (Num[2] / Den[0]) * xnReg[2] \
                                                      - (Den[1] / Den[0]) * ynReg[1] - (Den[2] / Den[0]) * ynReg[2];
                    // 更新延迟线
                    xnReg[2] = xnReg[1];
                    xnReg[1] = xn;
                    ynReg[2] = ynReg[1];
                    ynReg[1] = ynReg[0];
                    // 返回滤波器输出
                    return ynReg[0];
                }
  * @param  Num:   指向滤波器分子系数 (b)
            Den:   指向滤波器分母系数 (a)
            xnReg: 指向滤波器输入数组
            ynReg: 指向滤波器输出数组
  * @retval ynReg[0]: 滤波器输出
  */
float MATLAB_IIR_Model(float *Num, float *Den, float *xnReg, float *ynReg, float xn) {

    uint8_t i;

    float sum_num = 0, sum_den = 0;

    // 分子分母分别计算
    sum_num = sum_num + Num[0] * xn;
    for(i=1; i<F_ORDER+1; i++) {
        sum_num = sum_num + Num[i]*xnReg[i];// 计算分子
        sum_den = sum_den + Den[i]*ynReg[i];// 计算分母
    }

    // 计算输出
    ynReg[0] = (sum_num - sum_den) / Den[0];

    // 更新 xnReg 与 ynReg
    xnReg[0] = xn;
    for(i=F_ORDER; i>0; i--) {
        xnReg[i] = xnReg[i-1];
        ynReg[i] = ynReg[i-1];
    }

    // 返回滤波器输出
    return ynReg[0];

}


#if (MATLAB_FS==250) // 采样率250Hz
/**
  * @brief  MATLAB系数初始化函数
  * @note   None
  * @param  None
  * @retval None
  */
void MATLAB_IIR_Coe_Init(void) {

    // Low pass filter, order = 2, fs = 250, f_lp = 100
    lp_den[0] = 1.00000;
    lp_den[1] = 1.14298;
    lp_den[2] = 0.41280;
    lp_num[0] = 0.63895;
    lp_num[1] = 1.27789;
    lp_num[2] = 0.63895;

    // High pass filter, order = 2, fs = 250, f_hp = 5.000000e-01
    hp_den[0] = 1.00000;
    hp_den[1] = -1.98223;
    hp_den[2] = 0.98239;
    hp_num[0] = 0.99115;
    hp_num[1] = -1.98231;
    hp_num[2] = 0.99115;

    // Band pass filter, order = 2, fs = 250, f_bp_w1 = 5.000000e-01, f_bp_w2 = 100
    bp_den[0] = 1.00000;
    bp_den[1] = -0.47945;
    bp_den[2] = -0.50164;
    bp_num[0] = 0.75082;
    bp_num[1] = 0.00000;
    bp_num[2] = -0.75082;

    // Band stop filter, order = 2, fs = 250, f_bs_w1 = 49.5, f_bs_w2 = 50.5
    bs_den[0] = 1.00000;
    bs_den[1] = -0.61041;
    bs_den[2] = 0.97518;
    bs_num[0] = 0.98759;
    bs_num[1] = -0.61041;
    bs_num[2] = 0.98759;

}


#elif (MATLAB_FS==500) // 采样率500Hz
/**
  * @brief  MATLAB系数初始化函数
  * @note   None
  * @param  None
  * @retval None
  */
void MATLAB_IIR_Coe_Init(void) {

    // Low pass filter, order = 2, fs = 500, f_lp = 200
    lp_den[0] = 1.00000;
    lp_den[1] = 1.14298;
    lp_den[2] = 0.41280;
    lp_num[0] = 0.63895;
    lp_num[1] = 1.27789;
    lp_num[2] = 0.63895;

    // High pass filter, order = 2, fs = 500, f_hp = 1
    hp_den[0] = 1.00000;
    hp_den[1] = -1.98223;
    hp_den[2] = 0.98239;
    hp_num[0] = 0.99115;
    hp_num[1] = -1.98231;
    hp_num[2] = 0.99115;

    // Band pass filter, order = 2, fs = 500, f_bp_w1 = 1, f_bp_w2 = 200
    bp_den[0] = 1.00000;
    bp_den[1] = -0.47945;
    bp_den[2] = -0.50164;
    bp_num[0] = 0.75082;
    bp_num[1] = 0.00000;
    bp_num[2] = -0.75082;

    // Band stop filter, order = 2, fs = 500, f_bs_w1 = 49.5, f_bs_w2 = 50.5
    bs_den[0] = 1.00000;
    bs_den[1] = -1.60796;
    bs_den[2] = 0.98751;
    bs_num[0] = 0.99376;
    bs_num[1] = -1.60796;
    bs_num[2] = 0.99376;

}



#elif (MATLAB_FS==1000) // 采样率1000Hz
/**
  * @brief  MATLAB系数初始化函数
  * @note   None
  * @param  None
  * @retval None
  */
void MATLAB_IIR_Coe_Init(void) {

    // Low pass filter, order = 2, fs = 1000, f_lp = 300
    lp_den[0] = 1.00000;
    lp_den[1] = 0.36953;
    lp_den[2] = 0.19582;
    lp_num[0] = 0.39134;
    lp_num[1] = 0.78267;
    lp_num[2] = 0.39134;

    // High pass filter, order = 2, fs = 1000, f_hp = 1
    hp_den[0] = 1.00000;
    hp_den[1] = -1.99111;
    hp_den[2] = 0.99115;
    hp_num[0] = 0.99557;
    hp_num[1] = -1.99113;
    hp_num[2] = 0.99557;

    // Band pass filter, order = 2, fs = 1000, f_bp_w1 = 1, f_bp_w2 = 300
    bp_den[0] = 1.00000;
    bp_den[1] = -0.83756;
    bp_den[2] = -0.15517;
    bp_num[0] = 0.57758;
    bp_num[1] = 0.00000;
    bp_num[2] = -0.57758;

    // Band stop filter, order = 2, fs = 1000, f_bs_w1 = 49.5, f_bs_w2 = 50.5
    bs_den[0] = 1.00000;
    bs_den[1] = -1.89617;
    bs_den[2] = 0.99374;
    bs_num[0] = 0.99687;
    bs_num[1] = -1.89617;
    bs_num[2] = 0.99687;

}


#elif (MATLAB_FS==2000) // 采样率2000Hz
/**
  * @brief  MATLAB系数初始化函数
  * @note   None
  * @param  None
  * @retval None
  */
void MATLAB_IIR_Coe_Init(void) {

    // Low pass filter, order = 2, fs = 2000, f_lp = 300
    lp_den[0] = 1.00000;
    lp_den[1] = -0.74779;
    lp_den[2] = 0.27221;
    lp_num[0] = 0.13111;
    lp_num[1] = 0.26221;
    lp_num[2] = 0.13111;

    // High pass filter, order = 2, fs = 2000, f_hp = 1
    hp_den[0] = 1.00000;
    hp_den[1] = -1.99556;
    hp_den[2] = 0.99557;
    hp_num[0] = 0.99778;
    hp_num[1] = -1.99556;
    hp_num[2] = 0.99778;

    // Band pass filter, order = 2, fs = 2000, f_bp_w1 = 1, f_bp_w2 = 300
    bp_den[0] = 1.00000;
    bp_den[1] = -1.32454;
    bp_den[2] = 0.32666;
    bp_num[0] = 0.33667;
    bp_num[1] = 0.00000;
    bp_num[2] = -0.33667;

    // Band stop filter, order = 2, fs = 2000, f_bs_w1 = 49.5, f_bs_w2 = 50.5
    bs_den[0] = 1.00000;
    bs_den[1] = -1.97228;
    bs_den[2] = 0.99686;
    bs_num[0] = 0.99843;
    bs_num[1] = -1.97228;
    bs_num[2] = 0.99843;

}


#endif /* MATLAB_FS */


/**************************** MATLAB 初始化滤波器系数脚本 "Init_Filter_Coe.m" **********************************************/

/*

% 滤波器系数初始化文件, 将系数写入"Filter_coe.txt"
% 初始化低通, 高通, 带通, 带阻滤波器系数

clear;clc;

ORDER = 2;% 滤波器阶数
fs = 2000;% 采样频率

% 各类滤波器参数
f_lp = 400;% 低通滤波器频率参数
f_hp = 1;% 高通滤波器频率参数
f_bp_w1 = f_hp; f_bp_w2 = f_lp;% 带通滤波器频率参数
f_bs_w1  = 49.5;  f_bs_w2  = 50.5; %  50Hz带阻滤波器频率参数


% 获取 IIR 各类滤波器系数
[Num_LP, Den_LP] = butter(ORDER, f_lp/(fs/2), 'low');% 低通滤波器系数
[Num_HP, Den_HP] = butter(ORDER, f_hp/(fs/2), 'high');% 高通滤波器系数
[Num_BP, Den_BP] = butter(ORDER/2, [f_bp_w1/(fs/2), f_bp_w2/(fs/2)]);% 带通滤波器系数
[Num_BS, Den_BS] = butter(ORDER/2, [f_bs_w1/(fs/2), f_bs_w2/(fs/2)], 'stop');% 带阻滤波器系数


% 打印输出滤波器系数到文件"Filter_coe.txt"
fid = fopen('Filter_coe.txt','w+');%Filter_coe.txt文件

    fprintf(fid, 'void MATLAB_IIR_Coe_Init(void) {\n');% 输出函数名

    % 打印低通滤波器系数
    % 分母系数 Den 输出
    fprintf(fid, '\n    // Low pass filter, order = %d, fs = %d, f_lp = %d\n', ORDER, fs, f_lp);% 输出滤波器信息
    for i = 1 : (ORDER+1)
        fprintf(fid, '    lp_den[%d] = %.5f;\n',i-1, roundn(Den_LP(i), -5));% 第5位小数后四舍五入取整
    end
    % 分子系数 Num 输出
    for i = 1 : (ORDER+1)
        fprintf(fid, '    lp_num[%d] = %.5f;\n',i-1, roundn(Num_LP(i), -5));% 第5位小数后四舍五入取整
    end

    % 打印高通滤波器系数
    % 分母系数 Den 输出
    fprintf(fid, '\n    // High pass filter, order = %d, fs = %d, f_hp = %d\n', ORDER, fs, f_hp);% 输出滤波器信息
    for i = 1 : (ORDER+1)
        fprintf(fid, '    hp_den[%d] = %.5f;\n',i-1, roundn(Den_HP(i), -5));% 第5位小数后四舍五入取整
    end
    % 分子系数 Num 输出
    for i = 1 : (ORDER+1)
        fprintf(fid, '    hp_num[%d] = %.5f;\n',i-1, roundn(Num_HP(i), -5));% 第5位小数后四舍五入取整
    end

    % 打印带通滤波器系数
    % 分母系数 Den 输出
    fprintf(fid, '\n    // Band pass filter, order = %d, fs = %d, f_bp_w1 = %d, f_bp_w2 = %d\n', ORDER, fs, f_bp_w1, f_bp_w2);% 输出滤波器信息
    for i = 1 : (ORDER+1)
        fprintf(fid, '    bp_den[%d] = %.5f;\n',i-1, roundn(Den_BP(i), -5));% 第5位小数后四舍五入取整
    end
    % 分子系数 Num 输出
    for i = 1 : (ORDER+1)
        fprintf(fid, '    bp_num[%d] = %.5f;\n',i-1, roundn(Num_BP(i), -5));% 第5位小数后四舍五入取整
    end

    % 打印带阻滤波器系数
    % 分母系数 Den 输出
    fprintf(fid, '\n    // Band stop filter, order = %d, fs = %d, f_bs_w1 = %.1f, f_bs_w2 = %.1f\n', ORDER, fs, f_bs_w1, f_bs_w2);% 输出滤波器信息
    for i = 1 : (ORDER+1)
        fprintf(fid, '    bs_den[%d] = %.5f;\n',i-1, roundn(Den_BS(i), -5));% 第5位小数后四舍五入取整
    end
    % 分子系数 Num 输出
    for i = 1 : (ORDER+1)
        fprintf(fid, '    bs_num[%d] = %.5f;\n',i-1, roundn(Num_BS(i), -5));% 第5位小数后四舍五入取整
    end

    fprintf(fid, '\n}\n');% 输出函数名

fclose(fid);

*/


#endif /* MATLAB */

