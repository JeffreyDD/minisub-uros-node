#ifndef NODE_H_
#define NODE_H_

#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rclc_parameter/rclc_parameter.h>

#include "util.h"

rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;
rclc_executor_t executor;
rclc_parameter_server_t param_server;

void node_setup(char* node_name){
    allocator = rcl_get_default_allocator();

    //create init_options
    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

    // create node
    RCCHECK(rclc_node_init_default(&node, node_name, "", &support));

    // Create parameter service
    RCCHECK(rclc_parameter_server_init_default(&param_server, &node));
    printf("here.\n");

    // create executor
    RCCHECK(rclc_executor_init(&executor, &support.context, 6, &allocator));
}

#endif