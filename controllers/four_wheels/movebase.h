#ifndef __MOVEBASE__H
#define __MOVEBASE__H

#include "struct_typedef.h" 

//轮子到中心距离
#define WHEEL_DISTANCE_TO_CENTER 0.32f

fp32 V = 10.00; 
int pc = 0;
float K = 2.0;




//控制模式
typedef enum
{
  CHASSIS_MODE_RC,      //ÊÖ¶¯¿ØÖÆÄ£Ê½£¬´¿ÊÖ¶¯
  CHASSIS_MODE_AUTO,    //×Ô¶¯¿ØÖÆÄ£Ê½£¬×Ô¶¯Ñ²Ïß
  CHASSIS_MODE_RC_AUTO, //¸¨ÖúÄ£Ê½£¬·½ÏòËø¶¨£¬×ËÌ¬Ëø¶¨
} chassis_mode_e;

typedef struct 

{
chassis_mode_e chassis_mode;  
chassis_mode_e last_chassis_mode;   

fp32 vx;
fp32 vy;
fp32 wz;
fp32 vx_set;
fp32 vy_set;
fp32 wz_set;
fp32 chassis_relative_angle;     
fp32 chassis_relative_angle_set; 
fp32 chassis_yaw_set;
fp32 vx_max_speed;  
fp32 vx_min_speed;  
fp32 vy_max_speed;  
fp32 vy_min_speed;  
fp32 chassis_yaw;  
fp32 chassis_pitch; 
fp32 chassis_roll;  
}chassis_move_t;

 // 底盘运动数据结构体
chassis_move_t chassis_move;
static void output_wheel(const float vx_set, const float vy_set, const float wz_set);
void chassis_set_contorl(chassis_move_t *ptr);
void chassis_mode_change(chassis_move_t *ptr);
static void rc_contorl(int rc);
#endif