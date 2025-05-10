# OLED显示屏调试笔记

## OLED显示屏信息

0.96寸7针SPI接口

![OLED显示屏图片](https://raw.githubusercontent.com/See-YouL/PicGoFhotos/master/20250511040739068.png)

## OLED显示屏资料

[OLED显示屏资料](https://telesky.yuque.com/bdys8w/01/lw9nqcxkk0hffiuz)

接线如下:

- SCK 接 PG12
- SDA 接 PD5
- RES 接 PD4
- DC 接 PD15
- CS 接 PD1

## OLED显示屏程序

程序使用资料中提供的软件接口,并进行整理至`4-1 OLED显示屏调试`,基本功能测试可用,以后程序均以`4-1 OLED显示屏调试`工程文件为模板进行编写

工程模板目录结构如下:

- CORE: 存放启动文件等
- HADRWARE: 存放外设文件
- OBJ: 存放经Keil MDK编译生成的文件
- STM32F10x_FWLib: 存放STM32F10x固件标准库
- SYSTEM: 存放系统文件如延时程序等
- USER: 存放用户文件如main.c

详情进参考`4-1 OLED显示屏调试`
