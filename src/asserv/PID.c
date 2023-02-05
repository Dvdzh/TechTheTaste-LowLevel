#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "./header/PID.h"

float Sum_error_right=0, last_error_right=0 ;
float Sum_error_left=0, last_error_left=0;
float Sum_error_trans=0 , last_error_trans=0;
float Sum_error_rot=0, last_error_rot=0;


double PID (float kP,float kI, float kD, float Input, float Setpoint, float *Sum_error, float *last_error) {

	float error = Setpoint - Input;
	(*Sum_error) += (error * timeChange); 							
	//Tim : on peut enlever le time en théorie, car toujours le même, donc on pourrait le prendre en compte dans dans les coefs, but au cas où on change les temps c"est plus simple de le conserver.
	float der_error = (error - (*last_error))/ timeChange; 

	return (kP * error) + (kI * (*Sum_error)) + (kD * der_error);

}


