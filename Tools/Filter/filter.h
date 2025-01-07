/**
  ******************************************************************************
  * @file           : filter.h
  * @brief          : �˲������������ļ�. �����ݲ��˲���, ��ͨ�˲���, ��ͨ�˲���, ��ͨ�˲���.
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
  * @brief  �Ƿ�ʹ��MATLAB
  * @param  1: ʹ��
  * @param  0: ��ʹ��
  */
#define MATLAB					1


#define F_PI					3.141593		// Բ����
#define FILTER_Q				0.707           // �˲�Ʒ������


extern float sample_freq;                       // ����Ƶ�� (Hz)
extern float notch_freq;                        // �ݲ��˲��� �ݲ�Ƶ�� (Hz)
extern float lowpass_freq;                      // ��ͨ�˲��� �ͽ�ֹƵ�� (Hz)
extern float highpass_freq;                     // ��ͨ�˲��� �߽�ֹƵ�� (Hz)


/**
  * @brief �˲�����������ӳ��߽ṹ��
  */
typedef struct {
    float x1, x2; // �����ӳ���
    float y1, y2; // ����ӳ���
}filter_dl;


extern filter_dl nt_filter;		                // �ݲ��˲����ṹ��
extern filter_dl lp_filter;		                // ��ͨ�˲����ṹ��
extern filter_dl hp_filter;		                // ��ͨ�˲����ṹ��


extern float nt_a[3];                           // �ݲ��˲�����ĸϵ��
extern float nt_b[3];                           // �ݲ��˲�������ϵ��

extern float lp_a[3];                           // ��ͨ�˲�����ĸϵ��
extern float lp_b[3];                           // ��ͨ�˲�������ϵ��

extern float hp_a[3];                           // ��ͨ�˲�����ĸϵ��
extern float hp_b[3];                           // ��ͨ�˲�������ϵ��


void Filter_Coe_Init(float freq);// �˲���ϵ����ʼ��
void Notch_Filter_Init(float *nt_a, float *nt_b, float freq);// �ݲ��˲���ϵ����ʼ��
void Lowpass_Filter_Init(float *lp_a, float *lp_b, float freq);// ��ͨ�˲���ϵ����ʼ��
void Highpass_Filter_Init(float *hp_a, float *hp_b, float freq);// ��ͨ�˲���ϵ����ʼ��
float Notch_Filter(float signal);// �ݲ��˲���������
float Lowpass_Filter(float signal);// ��ͨ�˲���������
float Highpass_Filter(float signal);// ��ͨ�˲���������



/******************************************************************************************/
#if MATLAB

/* �˲�����������Ϊż��, ����λ������Ŀǰֻ��ʹ��2��, �������߻ᵼ���������, ��������4�׸�ͨ��4�׵�ͨ����. (�����������̻����0, �����������)*/
#define F_ORDER					2               // �˲�������


/* MATLAB IIR�˲�����Ƶ��, ��ѡ250Hz, 500Hz, 1000Hz, 2000Hz. (�����ʹ����������Ƶ��, ���Լ���������ϵ��) */
#define MATLAB_FS		        500            // �˲�������Ƶ��


extern float lp_den[F_ORDER+1];                 // ��ͨ�˲�����ĸϵ��
extern float lp_num[F_ORDER+1];                 // ��ͨ�˲�������ϵ��
extern float lp_xn[F_ORDER+1];                  // ��ͨ�˲�����������
extern float lp_yn[F_ORDER+1];                  // ��ͨ�˲����������

extern float hp_den[F_ORDER+1];                 // ��ͨ�˲�����ĸϵ��
extern float hp_num[F_ORDER+1];                 // ��ͨ�˲�������ϵ��
extern float hp_xn[F_ORDER+1];                  // ��ͨ�˲�����������
extern float hp_yn[F_ORDER+1];                  // ��ͨ�˲����������

extern float bp_den[F_ORDER+1];                 // ��ͨ�˲�����ĸϵ��
extern float bp_num[F_ORDER+1];                 // ��ͨ�˲�������ϵ��
extern float bp_xn[F_ORDER+1];                  // ��ͨ�˲�����������
extern float bp_yn[F_ORDER+1];                  // ��ͨ�˲����������

extern float bs_den[F_ORDER+1];                 // �����˲�����ĸϵ��
extern float bs_num[F_ORDER+1];                 // �����˲�������ϵ��
extern float bs_xn[F_ORDER+1];                  // �����˲�����������
extern float bs_yn[F_ORDER+1];                  // �����˲����������


#define LP_FILTER               0               //��ͨ�˲���
#define HP_FILTER               1               //��ͨ�˲���
#define BP_FILTER               2               //��ͨ�˲���
#define BS_FILTER               3               //�����˲���


float MATLAB_Fliter(uint8_t type, float xn);// MATLAB�˲�����
float MATLAB_IIR_Model(float *Num, float *Den, float *xnReg, float *ynReg, float xn);// IIR�˲�������ģ�麯��
void MATLAB_IIR_Coe_Init(void);// MATLABϵ����ʼ������


#endif /* MATLAB */



#endif /* __FILTER_H */
