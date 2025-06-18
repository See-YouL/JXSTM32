#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"
#include "stm32f10x_conf.h"

void Servo_Init(void);
void Servo_SetAngle(float angle);

#endif
