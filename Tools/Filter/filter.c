/**
  ******************************************************************************
  * @file           : filter.c
  * @brief          : �˲������������ļ�. �����ݲ��˲���, ��ͨ�˲���, ��ͨ�˲���, ��ͨ�˲���.
                      �˲�������ϵ��: Num ���� b
                      �˲�����ĸϵ��: Den ���� a
                      IIR �˲����ṹ:
                      a(1)*y(n) = b(1)*x(n) + b(2)*x(n-1) + ... + b(order)*x(n-order)- a(2)*y(n-1) - ... - a(order)*y(n-order)
                      IIR �˲���ʵ��:
                      a[0]*y[0] = b[0]*x[0] + b[1]*x[1] + ... + b[order]*x[order] - a[1]*y[1] - ... - a[order]*y[order]
  * @attention      :
                      ������ʹ��ʾ��1, ʹ��C���Լ����˲���ϵ�������˲� (�����ο�):

                        void main(void) {

                        int32_t xn, yn;// ��������ź�

                            Filter_Coe_Init(2000);// �˲���ϵ�������Ƶ�ʳ�ʼ��

                            while(1) {

                                   yn = (int32_t)(Notch_Filter((float)xn));// �˲�����

                            }

                        }

                        ������ʹ��ʾ��2, ���MATLAB���ɵ�IIR�˲���ϵ�������˲� (�����ο�):

                        void main(void) {

                        int32_t xn, yn;// ��������ź�

                            MATLAB_IIR_Coe_Init();// MATLAB IIR�˲���ϵ����ʼ��

                            while(1) {

                                   yn = (int32_t)MATLAB_Fliter(BS_FILTER, (float)xn);// �˲�����

                            }

                        }

  ******************************************************************************
  */


#include "filter.h"

float sample_freq = 500;       // ����Ƶ�� (Hz)
float notch_freq = 50;          // �ݲ��˲��� �ݲ�Ƶ�� (Hz)
float lowpass_freq = 100;       // ��ͨ�˲��� �ͽ�ֹƵ�� (Hz)
float highpass_freq = 1;       // ��ͨ�˲��� �߽�ֹƵ�� (Hz)


filter_dl nt_filter = {0};		// �ݲ��˲����ṹ��
filter_dl lp_filter = {0};		// ��ͨ�˲����ṹ��
filter_dl hp_filter = {0};		// ��ͨ�˲����ṹ��


float nt_a[3] = {0};			// �ݲ��˲�����ĸϵ��
float nt_b[3] = {0};			// �ݲ��˲�������ϵ��


float lp_a[3] = {0};			// ��ͨ�˲�����ĸϵ��
float lp_b[3] = {0};			// ��ͨ�˲�������ϵ��


float hp_a[3] = {0};			// ��ͨ�˲�����ĸϵ��
float hp_b[3] = {0};			// ��ͨ�˲�������ϵ��



/**
  * @brief  �˲���ϵ����ʼ��
  * @note   None
  * @param  freq: �˲�������Ƶ��
  * @retval None
  */
void Filter_Coe_Init(float freq) {

    sample_freq = freq;                             // �����˲�������Ƶ��

	Notch_Filter_Init(nt_a, nt_b, notch_freq);      // �ݲ��˲���ϵ����ʼ��

	Lowpass_Filter_Init(lp_a, lp_b, lowpass_freq);  // ��ͨ�˲���ϵ����ʼ��

	Highpass_Filter_Init(hp_a, hp_b, highpass_freq);// ��ͨ�˲���ϵ����ʼ��

}



/**
  * @brief  �ݲ��˲���ϵ����ʼ��
  * @note   ��ʼ��ʾ��:
			Notch_Filter_Init(nt_a, nt_b);// �ݲ��˲���ϵ����ʼ��
  * @param  nt_a: �ݲ��˲�����ĸϵ��
			nt_b: �ݲ��˲�������ϵ��
            freq: �˲���Ƶ��
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
  * @brief  ��ͨ�˲���ϵ����ʼ��
  * @note   ��ʼ��ʾ��:
			Lowpass_Filter_Init(lp_a, lp_b);// ��ͨ�˲���ϵ����ʼ��
  * @param  lp_a: ��ͨ�˲�����ĸϵ��
			lp_b: ��ͨ�˲�������ϵ��
            freq: �˲���Ƶ��
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
  * @brief  ��ͨ�˲���ϵ����ʼ��
  * @note   ��ʼ��ʾ��:
			Highpass_Filter_Init(hp_a, hp_b);// ��ͨ�˲���ϵ����ʼ��
  * @param  hp_a: ��ͨ�˲�����ĸϵ��
			hp_b: ��ͨ�˲�������ϵ��
            freq: �˲���Ƶ��
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
  * @brief  �ݲ��˲���������
  * @note   ʹ��ʾ��:
			nt_y = Notch_Filter(nt_a, nt_b, (float)signal);// �ݲ��˲���������
  * @param  signal: �˲��ź�
  * @retval nt_y: �ݲ��˲������
  */
float Notch_Filter(float signal) {

    float nt_y;

	// �����˲������
	nt_y = (nt_b[0] / nt_a[0]) * signal + (nt_b[1] / nt_a[0]) * nt_filter.x1 + (nt_b[2] / nt_a[0]) * nt_filter.x2 \
										- (nt_a[1] / nt_a[0]) * nt_filter.y1 - (nt_a[2] / nt_a[0]) * nt_filter.y2;

    // �����ӳ���
    nt_filter.x2 = nt_filter.x1;
    nt_filter.x1 = signal;
    nt_filter.y2 = nt_filter.y1;
    nt_filter.y1 = nt_y;

    return nt_y;
}


/**
  * @brief  ��ͨ�˲���������
  * @note   ʹ��ʾ��:
			lp_y = Lowpass_Filter(lp_a, lp_b, (float)signal);// ��ͨ�˲���������
  * @param  signal: �˲��ź�
  * @retval lp_y: ��ͨ�˲������
  */
float Lowpass_Filter(float signal) {

	float lp_y;

	// �����˲������
	lp_y = (lp_b[0] / lp_a[0]) * signal + (lp_b[1] / lp_a[0]) * lp_filter.x1 + (lp_b[2] / lp_a[0]) * lp_filter.x2 \
										- (lp_a[1] / lp_a[0]) * lp_filter.y1 - (lp_a[2] / lp_a[0]) * lp_filter.y2;

    // �����ӳ���
    lp_filter.x2 = lp_filter.x1;
    lp_filter.x1 = signal;
    lp_filter.y2 = lp_filter.y1;
    lp_filter.y1 = lp_y;

    return lp_y;

}


/**
  * @brief  ��ͨ�˲���������
  * @note   ʹ��ʾ��:
			hp_y = Highpass_Filter(hp_a, hp_b, (float)signal);// ��ͨ�˲���������
  * @param  signal: �˲��ź�
  * @retval hp_y: ��ͨ�˲������
  */
float Highpass_Filter(float signal) {

	float hp_y;

	// �����˲������
	hp_y = (hp_b[0] / hp_a[0]) * signal + (hp_b[1] / hp_a[0]) * hp_filter.x1 + (hp_b[2] / hp_a[0]) * hp_filter.x2 \
										- (hp_a[1] / hp_a[0]) * hp_filter.y1 - (hp_a[2] / hp_a[0]) * hp_filter.y2;

    // �����ӳ���
    hp_filter.x2 = hp_filter.x1;
    hp_filter.x1 = signal;
    hp_filter.y2 = hp_filter.y1;
    hp_filter.y1 = hp_y;

    return hp_y;

}


/******************************************************************************************/
#if MATLAB


float lp_den[F_ORDER+1] = {0};// ��ͨ�˲�����ĸϵ��
float lp_num[F_ORDER+1] = {0};// ��ͨ�˲�������ϵ��
float lp_xn[F_ORDER+1]  = {0};// ��ͨ�˲�����������
float lp_yn[F_ORDER+1]  = {0};// ��ͨ�˲����������


float hp_den[F_ORDER+1] = {0};// ��ͨ�˲�����ĸϵ��
float hp_num[F_ORDER+1] = {0};// ��ͨ�˲�������ϵ��
float hp_xn[F_ORDER+1]  = {0};// ��ͨ�˲�����������
float hp_yn[F_ORDER+1]  = {0};// ��ͨ�˲����������

float bp_den[F_ORDER+1] = {0};// ��ͨ�˲�����ĸϵ��
float bp_num[F_ORDER+1] = {0};// ��ͨ�˲�������ϵ��
float bp_xn[F_ORDER+1]  = {0};// ��ͨ�˲�����������
float bp_yn[F_ORDER+1]  = {0};// ��ͨ�˲����������

float bs_den[F_ORDER+1] = {0};// �����˲�����ĸϵ��
float bs_num[F_ORDER+1] = {0};// �����˲�������ϵ��
float bs_xn[F_ORDER+1]  = {0};// �����˲�����������
float bs_yn[F_ORDER+1]  = {0};// �����˲����������


/**
  * @brief  MATLAB�˲�����
  * @note   ֱ�ӵ��ô˺���ʱ, ÿ���˲���ֻ�ܶ�һ·�����ź�ʹ��, ����Ϊÿ���˲������������������������Ĵ���, ���ܻ��ö�·�ź�
            �����Ҫ��ĳ���˲����ظ�ʹ�ö��, ����Ҫ���¶��������������Ĵ�����������ͬ����˲���, ���ҵ��� "MATLAB_IIR_Model"�����˲�����
  * @param  type:   �˲�������
                    LP_FILTER; ��ͨ�˲���
                    HP_FILTER: ��ͨ�˲���
                    BP_FILTER; ��ͨ�˲���
                    BS_FILTER; �����˲���
            xn: �����ź�
  * @retval yn: ����ź�
  */
float MATLAB_Fliter(uint8_t type, float xn) {

    float yn = 0;

    switch(type) {

        case LP_FILTER: yn = MATLAB_IIR_Model(lp_num, lp_den, lp_xn, lp_yn, xn); break;// ��ͨ�˲���

        case HP_FILTER: yn = MATLAB_IIR_Model(hp_num, hp_den, hp_xn, hp_yn, xn); break;// ��ͨ�˲���

        case BP_FILTER: yn = MATLAB_IIR_Model(bp_num, bp_den, bp_xn, bp_yn, xn); break;// ��ͨ�˲���

        case BS_FILTER: yn = MATLAB_IIR_Model(bs_num, bs_den, bs_xn, bs_yn, xn); break;// �����˲���

        default: yn = 0; break;

    }

    return yn;

}


/**
  * @brief  IIR�˲�������ģ�麯��
  * @note       // �����˲������ʾ��
                float MATLAB_IIR_Model(float *Num, float *Den, float *xnReg, float *ynReg, float xn) {
                    ynReg[0] = (Num[0] / Den[0]) * xn + (Num[1] / Den[0]) * xnReg[1] + (Num[2] / Den[0]) * xnReg[2] \
                                                      - (Den[1] / Den[0]) * ynReg[1] - (Den[2] / Den[0]) * ynReg[2];
                    // �����ӳ���
                    xnReg[2] = xnReg[1];
                    xnReg[1] = xn;
                    ynReg[2] = ynReg[1];
                    ynReg[1] = ynReg[0];
                    // �����˲������
                    return ynReg[0];
                }
  * @param  Num:   ָ���˲�������ϵ�� (b)
            Den:   ָ���˲�����ĸϵ�� (a)
            xnReg: ָ���˲�����������
            ynReg: ָ���˲����������
  * @retval ynReg[0]: �˲������
  */
float MATLAB_IIR_Model(float *Num, float *Den, float *xnReg, float *ynReg, float xn) {

    uint8_t i;

    float sum_num = 0, sum_den = 0;

    // ���ӷ�ĸ�ֱ����
    sum_num = sum_num + Num[0] * xn;
    for(i=1; i<F_ORDER+1; i++) {
        sum_num = sum_num + Num[i]*xnReg[i];// �������
        sum_den = sum_den + Den[i]*ynReg[i];// �����ĸ
    }

    // �������
    ynReg[0] = (sum_num - sum_den) / Den[0];

    // ���� xnReg �� ynReg
    xnReg[0] = xn;
    for(i=F_ORDER; i>0; i--) {
        xnReg[i] = xnReg[i-1];
        ynReg[i] = ynReg[i-1];
    }

    // �����˲������
    return ynReg[0];

}


#if (MATLAB_FS==250) // ������250Hz
/**
  * @brief  MATLABϵ����ʼ������
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


#elif (MATLAB_FS==500) // ������500Hz
/**
  * @brief  MATLABϵ����ʼ������
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



#elif (MATLAB_FS==1000) // ������1000Hz
/**
  * @brief  MATLABϵ����ʼ������
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


#elif (MATLAB_FS==2000) // ������2000Hz
/**
  * @brief  MATLABϵ����ʼ������
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


/**************************** MATLAB ��ʼ���˲���ϵ���ű� "Init_Filter_Coe.m" **********************************************/

/*

% �˲���ϵ����ʼ���ļ�, ��ϵ��д��"Filter_coe.txt"
% ��ʼ����ͨ, ��ͨ, ��ͨ, �����˲���ϵ��

clear;clc;

ORDER = 2;% �˲�������
fs = 2000;% ����Ƶ��

% �����˲�������
f_lp = 400;% ��ͨ�˲���Ƶ�ʲ���
f_hp = 1;% ��ͨ�˲���Ƶ�ʲ���
f_bp_w1 = f_hp; f_bp_w2 = f_lp;% ��ͨ�˲���Ƶ�ʲ���
f_bs_w1  = 49.5;  f_bs_w2  = 50.5; %  50Hz�����˲���Ƶ�ʲ���


% ��ȡ IIR �����˲���ϵ��
[Num_LP, Den_LP] = butter(ORDER, f_lp/(fs/2), 'low');% ��ͨ�˲���ϵ��
[Num_HP, Den_HP] = butter(ORDER, f_hp/(fs/2), 'high');% ��ͨ�˲���ϵ��
[Num_BP, Den_BP] = butter(ORDER/2, [f_bp_w1/(fs/2), f_bp_w2/(fs/2)]);% ��ͨ�˲���ϵ��
[Num_BS, Den_BS] = butter(ORDER/2, [f_bs_w1/(fs/2), f_bs_w2/(fs/2)], 'stop');% �����˲���ϵ��


% ��ӡ����˲���ϵ�����ļ�"Filter_coe.txt"
fid = fopen('Filter_coe.txt','w+');%Filter_coe.txt�ļ�

    fprintf(fid, 'void MATLAB_IIR_Coe_Init(void) {\n');% ���������

    % ��ӡ��ͨ�˲���ϵ��
    % ��ĸϵ�� Den ���
    fprintf(fid, '\n    // Low pass filter, order = %d, fs = %d, f_lp = %d\n', ORDER, fs, f_lp);% ����˲�����Ϣ
    for i = 1 : (ORDER+1)
        fprintf(fid, '    lp_den[%d] = %.5f;\n',i-1, roundn(Den_LP(i), -5));% ��5λС������������ȡ��
    end
    % ����ϵ�� Num ���
    for i = 1 : (ORDER+1)
        fprintf(fid, '    lp_num[%d] = %.5f;\n',i-1, roundn(Num_LP(i), -5));% ��5λС������������ȡ��
    end

    % ��ӡ��ͨ�˲���ϵ��
    % ��ĸϵ�� Den ���
    fprintf(fid, '\n    // High pass filter, order = %d, fs = %d, f_hp = %d\n', ORDER, fs, f_hp);% ����˲�����Ϣ
    for i = 1 : (ORDER+1)
        fprintf(fid, '    hp_den[%d] = %.5f;\n',i-1, roundn(Den_HP(i), -5));% ��5λС������������ȡ��
    end
    % ����ϵ�� Num ���
    for i = 1 : (ORDER+1)
        fprintf(fid, '    hp_num[%d] = %.5f;\n',i-1, roundn(Num_HP(i), -5));% ��5λС������������ȡ��
    end

    % ��ӡ��ͨ�˲���ϵ��
    % ��ĸϵ�� Den ���
    fprintf(fid, '\n    // Band pass filter, order = %d, fs = %d, f_bp_w1 = %d, f_bp_w2 = %d\n', ORDER, fs, f_bp_w1, f_bp_w2);% ����˲�����Ϣ
    for i = 1 : (ORDER+1)
        fprintf(fid, '    bp_den[%d] = %.5f;\n',i-1, roundn(Den_BP(i), -5));% ��5λС������������ȡ��
    end
    % ����ϵ�� Num ���
    for i = 1 : (ORDER+1)
        fprintf(fid, '    bp_num[%d] = %.5f;\n',i-1, roundn(Num_BP(i), -5));% ��5λС������������ȡ��
    end

    % ��ӡ�����˲���ϵ��
    % ��ĸϵ�� Den ���
    fprintf(fid, '\n    // Band stop filter, order = %d, fs = %d, f_bs_w1 = %.1f, f_bs_w2 = %.1f\n', ORDER, fs, f_bs_w1, f_bs_w2);% ����˲�����Ϣ
    for i = 1 : (ORDER+1)
        fprintf(fid, '    bs_den[%d] = %.5f;\n',i-1, roundn(Den_BS(i), -5));% ��5λС������������ȡ��
    end
    % ����ϵ�� Num ���
    for i = 1 : (ORDER+1)
        fprintf(fid, '    bs_num[%d] = %.5f;\n',i-1, roundn(Num_BS(i), -5));% ��5λС������������ȡ��
    end

    fprintf(fid, '\n}\n');% ���������

fclose(fid);

*/


#endif /* MATLAB */

