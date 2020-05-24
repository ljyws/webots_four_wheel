#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/inertial_unit.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <webots/keyboard.h>
#include "movebase.h"
#include "navigation.h"

#define MAX_WHEEL_SPEED 4.0f
#define NORMAL_MAX_CHASSIS_SPEED_X 10.0f
#define NORMAL_MAX_CHASSIS_SPEED_Y 10.0f
#define CHASSIS_WZ_SET_SCALE 1.0f

float wheel_speed[4] = {0.00f};
int rc;

static void output_wheel( float vx_set,  float vy_set,  float wz_set)
{
  //将三角函数计算定义为静态变量写入内存中减少计算量
  static float cos_45 = 0.707106781, sin_45 = 0.707106781;

  //根据运动关系进行解算
  wheel_speed[0] = +vx_set * cos_45 + vy_set * sin_45 + wz_set * WHEEL_DISTANCE_TO_CENTER;
  wheel_speed[1] = -vx_set * sin_45 + vy_set * cos_45 + wz_set * WHEEL_DISTANCE_TO_CENTER;
  wheel_speed[2] = -vx_set * sin_45 - vy_set * cos_45 + wz_set * WHEEL_DISTANCE_TO_CENTER;
  wheel_speed[3] = +vx_set * cos_45 - vy_set * sin_45 + wz_set * WHEEL_DISTANCE_TO_CENTER;

  WbDeviceTag wheel1 = wb_robot_get_device("wheel1");
  WbDeviceTag wheel2 = wb_robot_get_device("wheel2");
  WbDeviceTag wheel3 = wb_robot_get_device("wheel3");
  WbDeviceTag wheel4 = wb_robot_get_device("wheel4");
  wb_motor_set_velocity(wheel1,wheel_speed[0]);
  wb_motor_set_velocity(wheel2,wheel_speed[1]);  
  wb_motor_set_velocity(wheel3,wheel_speed[2]);
  wb_motor_set_velocity(wheel4,wheel_speed[3]);
}

void chassis_set_contorl(chassis_move_t *ptr)
{
  if (ptr == NULL)
  {
    return;
  }
  //捕获键盘输入
  rc = wb_keyboard_get_key();
  
  if (rc == 'Q')
  ptr->chassis_mode = CHASSIS_MODE_RC ;
  
  if (rc == 'W')
  ptr->chassis_mode = CHASSIS_MODE_AUTO ;
    
  chassis_mode_change(ptr);

  if (ptr->chassis_mode == CHASSIS_MODE_RC)//手动控制
  {
   /*手动键盘控制*/
   rc_contorl(rc);
  }
  else if(ptr->chassis_mode == CHASSIS_MODE_AUTO)
  {
   /* 导航任务 */
    navigation_task();
  }
  printf("mode : %d \n",ptr->chassis_mode);
}

void chassis_mode_change(chassis_move_t *ptr)
{
  if (ptr == NULL)
  {
    return;
  }
  if (ptr->last_chassis_mode == ptr->chassis_mode)
  {
    return;
  }
  if ((ptr->last_chassis_mode != CHASSIS_MODE_RC) && ptr->chassis_mode == CHASSIS_MODE_RC)
  {
    ptr->chassis_relative_angle_set = 0.0f;
  }
  else if ((ptr->last_chassis_mode != CHASSIS_MODE_AUTO) && ptr->chassis_mode == CHASSIS_MODE_AUTO)
  {
    ptr->chassis_yaw_set = ptr->chassis_yaw;
  }
  ptr->last_chassis_mode = ptr->chassis_mode;
}

static void rc_contorl(int rc)
{
    if ((rc >= 0) && rc != pc) 
    {
    switch (rc) 
          {  
      case WB_KEYBOARD_UP:
            output_wheel(0.0 , 10.0 , 0.0 );
        break;
      case WB_KEYBOARD_DOWN:
            output_wheel(0.0 , -10.0, 0.0);
        break;
      case WB_KEYBOARD_LEFT:
            output_wheel(-10.0, 0.0 , 0.0);
        break;
      case WB_KEYBOARD_RIGHT:
            output_wheel(10.0 , 0.0 , 0.0);
        break;
      case 'A':
            output_wheel(0.0 , 0.0 , 5.0);
        break;  
      case 'D':
            output_wheel(0.0 , 0.0 , -5.0);
        break;
      case 'S':
            output_wheel(0.0 , 0.0 , 0.0);
        break;

        } pc = rc;
        }
}