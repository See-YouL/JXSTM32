#ifndef __FIRE_H
#define __FIRE_H

void Fire_Init(void); // 初始化火焰传感器
uint16_t FireState_Get(void); // 获取火焰传感器状态
void FireState_Clear(void); // 清除火焰传感器状态

#endif
