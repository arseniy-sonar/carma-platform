#pragma once

/*
 * Copyright (C) 2021 LEIDOS.
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

#include <ros/ros.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <lanelet2_core/LaneletMap.h>
#include <lanelet2_core/primitives/Point.h>
#include <lanelet2_extension/regulatory_elements/StopRule.h>
#include <lanelet2_extension/regulatory_elements/CarmaTrafficSignal.h>
#include <lanelet2_core/primitives/LaneletOrArea.h>
#include <lanelet2_extension/projection/local_frame_projector.h>
#include <carma_wm/WorldModelUtils.h>
#include <cav_msgs/MapData.h>
#include <lanelet2_core/Forward.h>
#include <lanelet2_extension/regulatory_elements/SignalizedIntersection.h>
#include <lanelet2_core/geometry/LaneletMap.h>
namespace carma_wm
{

struct LANE_DIRECTION
{
  static const uint8_t INGRESS = 1;
  static const uint8_t EGRESS = 2;
};

using namespace lanelet::units::literals;

/*! \brief This class manages and keeps track of all signalized intersections in the map.
           All of the SPAT and MAP standard's lane ids to lanelet id mapping is recorded here.
           NOTE: This class functions do not update the map given.
 */
class SignalizedIntersectionManager
{
public:
  SignalizedIntersectionManager(){}

  /*! 
  *  \brief Copy operator that copies everything except the traffic signal states. 
            This is to keep the states although the map is updated or a similar event happened
            NOTE: The function does not update the map with new elements
  *  \param[out] other manager
  */
  SignalizedIntersectionManager& operator=(SignalizedIntersectionManager other);

  /*! 
  *  \brief Create relevant signalized intersection and carma traffic signals based on the MAP.msg and the lanelet_map
            NOTE: The function does not update the map with new elements
  *  \param[out] intersections to return
  *  \param[out] traffic_signals to return
  *  \param map_msg MAP.msg that consists all static data portion of the intersection
  *  \param map lanelet_map to check 
  *  \param routing_graph of the lanelet map to accurately detect lanes
  */
  void createIntersectionFromMapMsg(std::vector<lanelet::SignalizedIntersectionPtr>& intersections, std::vector<lanelet::CarmaTrafficSignalPtr>& traffic_signals, const cav_msgs::MapData& map_msg, 
                                    const std::shared_ptr<lanelet::LaneletMap>& map, std::shared_ptr<const lanelet::routing::RoutingGraph> routing_graph);

  /*! 
  *  \brief Returns mapping of MAP lane id to lanelet id for the given map and intersection.msg in the MAP.msg.
            This function also records signal_group_id_to its own lanelet id, and also signal group to entry and exit lanelets id mappings
  *  \param[out] entry lane id to lanelet id mappings to return
  *  \param[out] exit  lane id to lanelet id mappings to return
  *  \param intersection MAP.msg that consists all static data portion of the intersection
  *  \param map lanelet_map to check 
  *  \param routing_graph of the lanelet map to accurately detect lanes
  *  \throw invalid_argument if given coordinates in the msg doesn't exist in the map
  *         TODO: Need to think about error buffer in the future. Map msgs are made from google maps or open streets maps normally so this function might run into some errors from that.
  */
  void convertLaneToLaneletId(std::unordered_map<uint8_t, lanelet::Id>& entry, std::unordered_map<uint8_t, lanelet::Id>& exit, const cav_msgs::IntersectionGeometry& intersection, 
                              const std::shared_ptr<lanelet::LaneletMap>& map, std::shared_ptr<const lanelet::routing::RoutingGraph> current_routing_graph);

  /*!
   * \brief Sets the max lane width in meters. Map msg points are associated to a lanelet if they are 
   *        within this distance to a lanelet as map msg points are guaranteed to apply to a single lane
   */
  void setMaxLaneWidth(double max_lane_width);

  /*! 
  *  \brief Returns existing signalized intersection with same entry and exit llts if exists.
  *  \param entry_llts of the intersection
  *  \param exit_llts of the intersection
  *  \param map lanelet_map to check 
  *  \return id of the matching intersection in the map, or lanelet::InvalId if none exists
  */
  lanelet::Id matchSignalizedIntersection(const lanelet::Lanelets& entry_llts, const lanelet::Lanelets& exit_llts, const std::shared_ptr<lanelet::LaneletMap>& map);
  
  /*! 
  *  \brief Saves the georeference string to be used for converting MAP.msg coordinates
  *  \param target_frame PROJ string of the map
  */
  void setTargetFrame(const std::string& target_frame);

  /*! 
  *  \brief Returns carma traffic signal and saves all id relevant id mappings (signal group to lanelet id) internally
  *  \param signal_group_id of the traffic signal
  *  \param exit_llts of the signal_group
  *  \param entry_llts of the signal_group
  *  \return traffic signal corresponding to the signal group
  */
  std::shared_ptr<lanelet::CarmaTrafficSignal> createTrafficSignalUsingSGID(uint8_t signal_group_id, const lanelet::Lanelets& entry_lanelets, const lanelet::Lanelets& exit_lanelets);

  /*! 
  *  \brief Returns carma traffic signal and saves all id relevant id mappings (signal group to lanelet id) internally
  *  \param signal_group_id of the traffic signal
  *  \param exit_llts of the signal_group
  *  \param entry_llts of the signal_group
  *  \return traffic signal corresponding to the signal group
  */
  lanelet::Lanelets identifyInteriorLanelets(const lanelet::Lanelets& entry_llts, const std::shared_ptr<lanelet::LaneletMap>& map);

  // SignalizedIntersection quick id lookup
  std::unordered_map<uint16_t, lanelet::Id> intersection_id_to_regem_id_;

  // CarmaTrafficSignal quick id lookup
  std::unordered_map<uint8_t, lanelet::Id> signal_group_to_traffic_light_id_;

  // CarmaTrafficSignal exit lanelets ids quick lookup
  std::unordered_map<uint8_t, std::unordered_set<lanelet::Id>> signal_group_to_exit_lanelet_ids_;

  // CarmaTrafficSignal entry lanelets ids quick lookup
  std::unordered_map<uint8_t, std::unordered_set<lanelet::Id>> signal_group_to_entry_lanelet_ids_;

  // Traffic signal states and their end_time mappings.
  std::unordered_map<uint16_t, std::unordered_map<uint8_t,std::vector<std::pair<boost::posix_time::ptime, lanelet::CarmaTrafficSignalState>>>> traffic_signal_states_; //[intersection_id][signal_group_id]

  // Last received signal state from SPAT
  std::unordered_map<uint16_t, std::unordered_map<uint8_t,std::pair<boost::posix_time::ptime, lanelet::CarmaTrafficSignalState>>> last_seen_state_; //[intersection_id][signal_group_id]

  // traffic signal state counter
  std::unordered_map<uint16_t, std::unordered_map<uint8_t,int>> signal_state_counter_; //[intersection_id][signal_group_id]

private:
  // PROJ string of current map
  std::string target_frame_ = "";

  // Max width of lane in meters
  double max_lane_width_ = 4;
};

}  // namespace carma_wm