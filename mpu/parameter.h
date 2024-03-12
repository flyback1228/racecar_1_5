#include <stdint.h>

typedef struct{
	//header for handshake,"acsr"
	char header[4];

	//firmware version
	uint8_t version;
	uint8_t subversion;

	//pid parameters
	float kp;
	float ki;
	float kd;
	uint8_t pid_frequency;

	//ros parameters
	uint8_t publish_frequency;

	//esc parameters
	float esc_rpm_to_speed_ratio;
	float esc_offset;
	float esc_max;
	float esc_min;

	uint8_t esc_set_precision;
	uint8_t allow_reverse; //a bool


	//steering servo parameters
	float steering_esc_pwm_frequency;
	float steering_offset;
	float steering_to_dutycycle_ratio;
	float steering_max;
	float steering_min;

	uint8_t servo_set_precision;

	//force parameters
	float force_ratio[8];

	//brake parameters
	float brake_pwm_frequency;

	//tailer for handshake,"b401"
	char tailer[4];

} ParameterTypeDef;