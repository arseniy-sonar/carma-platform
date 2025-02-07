/*
 * Copyright (C) <SUB><year> LEIDOS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#pragma once

#include <rclcpp/rclcpp.hpp>
#include <functional>
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/empty.hpp>

#include <carma_ros2_utils/carma_lifecycle_node.hpp>
#include "<SUB><package_name>/<SUB><package_name>_config.hpp"

namespace <SUB><package_name>
{

  /**
   * \brief TODO for USER: Add class description
   * 
   */
  class Node : public carma_ros2_utils::CarmaLifecycleNode
  {

  private:
    // Subscribers
    carma_ros2_utils::SubPtr<std_msgs::msg::String> example_sub_;

    // Publishers
    carma_ros2_utils::PubPtr<std_msgs::msg::String> example_pub_;

    // Service Clients
    carma_ros2_utils::ClientPtr<std_srvs::srv::Empty> example_client_;

    // Service Servers
    carma_ros2_utils::ServicePtr<std_srvs::srv::Empty> example_service_;

    // Timers
    rclcpp::TimerBase::SharedPtr example_timer_;

    // Node configuration
    Config config_;

  public:
    /**
     * \brief Node constructor 
     */
    explicit Node(const rclcpp::NodeOptions &);

    /**
     * \brief Example callback for dynamic parameter updates
     */
    rcl_interfaces::msg::SetParametersResult 
    parameter_update_callback(const std::vector<rclcpp::Parameter> &parameters);

    /**
     * \brief Example timer callback
     */
    void example_timer_callback();

    /**
     * \brief Example subscription callback
     */
    void example_callback(std_msgs::msg::String::UniquePtr msg);

    /**
      * \brief Example service callback
      */
    void example_service_callback(const std::shared_ptr<rmw_request_id_t> header,
                                  const std::shared_ptr<std_srvs::srv::Empty::Request> request,
                                  std::shared_ptr<std_srvs::srv::Empty::Response> response);

    ////
    // Overrides
    ////
    carma_ros2_utils::CallbackReturn handle_on_configure(const rclcpp_lifecycle::State &);

    /**
     * TODO for USER: The following lifecycle overrides are also available if needed
     * handle_on_activate
     * handle_on_deactivate
     * handle_on_cleanup
     * handle_on_shutdown
     * handle_on_error
     */
  };

} // <SUB><package_name>
