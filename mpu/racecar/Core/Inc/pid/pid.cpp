﻿
#include "pid.h"


template <typename T>
PID<T>::PID() { }

template <typename T>
PID<T>::PID(T *input, T *output, T *setpoint, T kp, T ki, T kd,uint8_t add_input)
{
	output_   = output;
	input_    = input;
	setpoint_ = setpoint;
//	mode_     = PID_MODE_MANUAL;
	
	PID<T>::set_tunings(kp, ki, kd, add_input);
	
	last_time_ = HAL_GetTick();
}


template <typename T>
void PID<T>::compute(void)
{
//	if(mode_ == PID_MODE_MANUAL)return;
	uint32_t now = HAL_GetTick();
	uint32_t dt = now - last_time_;
	
	T input = *input_;
	T error = *setpoint_-input;
	T dInput = input-last_input_;
	//T output;
	

	
	output_sum_ += ki_ * error * dt/1000.0;
	output_sum_ -= kp_ * dInput*add_input_error_to_proportional_;
	clip(&output_sum_, out_min_, out_max_);
	
	T output = output_sum_ - kp_ * error - kd_ * dInput * 1000.0 / dt;
	*output_ = output;
	clip(output_, out_min_, out_max_);
		
	last_input_ = input;
	last_time_  = now;
}

/* ~~~~~~~~~~~~~~~~~ PID Mode ~~~~~~~~~~~~~~~~~~ */
//template <typename T>
//void PID<T>::Set_Mode(PIDMode_TypeDef Mode)
//{
//	mode_ = Mode;
//}


/* ~~~~~~~~~~~~~~~~ PID Limits ~~~~~~~~~~~~~~~~~ */
template <typename T>
void PID<T>::set_output_limits(T min, T max)
{

	if (min >= max)return;
	
	out_min_ = min;
	out_max_ = max;
	
	clip(output_,out_min_,out_max_);
	clip(&output_sum_,out_min_,out_max_);
	
}

template <typename T>
void PID<T>::reset(){
	last_time_ = HAL_GetTick();
}



template <typename T>
void PID<T>::set_tunings(T kp, T ki, T kd, uint8_t add_input)
{
	kp_=kp;
	ki_=ki;
	kd_=kd;
	add_input_error_to_proportional_=add_input;
}

template class PID<float>;
