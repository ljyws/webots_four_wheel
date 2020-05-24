
#include <stdio.h>
#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/supervisor.h>
#include <webots/inertial_unit.h>
#include <webots/receiver.h>
#include <webots/keyboard.h>
#include <time.h>
#include "movebase.h"
#include "navigation.h"
#include "movebase.c"
#include "navigation.c"
#define TIME_STEP 32

int main()
{
  /* 初始化 */
  wb_robot_init();
  /* 加载电机驱动器 */
  WbDeviceTag wheel1 = wb_robot_get_device("wheel1");
  WbDeviceTag wheel2 = wb_robot_get_device("wheel2");
  WbDeviceTag wheel3 = wb_robot_get_device("wheel3");
  WbDeviceTag wheel4 = wb_robot_get_device("wheel4");
  wb_motor_set_position(wheel1, INFINITY);
  wb_motor_set_position(wheel2, INFINITY);
  wb_motor_set_position(wheel3, INFINITY);
  wb_motor_set_position(wheel4, INFINITY);
  wb_motor_set_velocity(wheel1,0);
  wb_motor_set_velocity(wheel2,0);  
  wb_motor_set_velocity(wheel3,0);
  wb_motor_set_velocity(wheel4,0);
  /* 加载接收器 从上帝机器人接收位置坐标信息 */
  WbDeviceTag receiver = wb_robot_get_device("receiver");
  wb_receiver_enable(receiver, 64);
  /* 加载惯性单元 获取 YAW 轴数据 */
  WbDeviceTag inertial_unit = wb_robot_get_device("inertial_unit");
  wb_inertial_unit_enable(inertial_unit, 1); //0,1,2 YAW Y轴

  /* 使能键盘捕获*/
  wb_keyboard_enable(TIME_STEP);
  while (wb_robot_step(TIME_STEP) != -1)
  {
      /* 接收上帝机器人发过来的的位置信息 */
    while (wb_receiver_get_queue_length(receiver) > 0)
    {
      const double *packet = wb_receiver_get_data(receiver);
      global_position.pos_x = packet[1];
      global_position.pos_y = packet[0];
      wb_receiver_next_packet(receiver); //准备接收下一帧
    }
      /* 通过惯性单元获取小车自身方向角 */
    global_position.theta = wb_inertial_unit_get_roll_pitch_yaw(inertial_unit)[2];
    //printf("x=%f  y=%f  theta = %f\r\n",global_position.pos_x,global_position.pos_y,global_position.theta);
    chassis_set_contorl(&chassis_move);


  }
  /* 结束时释放webots内存 */
  wb_robot_cleanup();

  return 0;
}
