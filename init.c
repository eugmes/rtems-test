#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_MAXIMUM_DRIVERS 3

//#define CONFIGURE_USE_DEVFS_AS_BASE_FILESYSTEM
#define CONFIGURE_STACK_CHECKER_ENABLED
#define CONFIGURE_MAXIMUM_SEMAPHORES 3

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 2

#define CONFIGURE_INIT_TASK_STACK_SIZE 5*1024

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMAND_CPUUSE
#define CONFIGURE_SHELL_COMMAND_STACKUSE
//#define CONFIGURE_SHELL_COMMAND_PERIODUSE
//#define CONFIGURE_SHELL_COMMAND_WKSPACE_INFO
//#define CONFIGURE_SHELL_COMMAND_MALLOC_INFO
#define CONFIGURE_SHELL_COMMAND_HALT

#include <rtems/shell.h>
extern rtems_shell_cmd_t Shell_LED_Command;
extern rtems_shell_cmd_t Shell_PWM_Command;
#define CONFIGURE_SHELL_USER_COMMANDS &Shell_LED_Command, &Shell_PWM_Command

#include <rtems/shellconfig.h>

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
