#ifndef NODE_H_
#define NODE_H_

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <builtin_interfaces/msg/time.h>

#include "util.h"
#include "config.h"

rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;
rclc_executor_t executor;

struct timespec cur_ts;

void node_setup(char* node_name){
    allocator = rcl_get_default_allocator();

    //create init_options
    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

    // create node
    RCCHECK(rclc_node_init_default(&node, node_name, "", &support));

    // create executor
    RCCHECK(rclc_executor_init(&executor, &support.context, 6, &allocator));

    // Synchronize time with the agent
    rmw_uros_sync_session(TIMESYNC_TIMEOUT);
}

void update_time_header(builtin_interfaces__msg__Time* time_header){
		time_header->sec = cur_ts.tv_sec;
		time_header->nanosec = cur_ts.tv_nsec;
}

#endif