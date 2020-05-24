
#include <stdio.h>
#include <string.h>
#include <webots/emitter.h>
#include <webots/robot.h>
#include <webots/supervisor.h>

#define TIME_STEP 64

int main()
{
  WbNodeRef wheel_node;
  WbFieldRef robot_translation_field;
  WbDeviceTag emitter;
  int j;
  double robot_initial_translation[3] = {0,0,0};
  double packet[3];
  const double *robot_translation;

  wb_robot_init();

  emitter = wb_robot_get_device("emitter");

  wheel_node = wb_supervisor_node_get_from_def("four_omni_robot");
  robot_translation_field = wb_supervisor_node_get_field(wheel_node, "translation");
  robot_translation = wb_supervisor_field_get_sf_vec3f(robot_translation_field);
  for (j = 0; j < 3; j++)
    robot_initial_translation[j] = robot_translation[j];

  // printf("ball initial translation = %g %g %g\n",ball_translation[0],ball_translation[1],ball_translation[2]);

  while (wb_robot_step(TIME_STEP) != -1)
  {
    robot_translation = wb_supervisor_field_get_sf_vec3f(robot_translation_field);
    packet[0] = robot_translation[0]; // ball X
    packet[1] = robot_translation[2]; // ball Z
    wb_emitter_send(emitter, packet, sizeof(packet));
  }
  wb_robot_cleanup();

  return 0;
}
