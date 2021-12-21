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

#include <iostream>

/**
 * \brief Stuct containing the algorithm configuration values for the YieldPluginConfig
 */
struct PlatoonPluginConfig
{
  // following parameters are for general platooning plugin
  double vehicleLength         = 5.0;     // m
  int    maxPlatoonSize        = 10;      // 1
  int    algorithmType         = 0;       // N/A
  int    statusMessageInterval = 100;     // ms
  int    infoMessageInterval   = 200;     // ms
  // following parameters are for IHP gap regulation algorithm
  double time_step             = 15;      // s
  double epislon               = 0.001;   // m/s
  
  // following parameters are for platoon forming and operation
  double timeHeadway           = 2.0;     // s
  double standStillHeadway     = 12.0;    // m
  double maxAllowedJoinTimeGap = 15.0;    // s
  double maxAllowedJoinGap     = 90.0;    // m
  double desiredJoinTimeGap    = 4.0;     // s
  double desiredJoinGap        = 30.0;    // m
  double waitingStateTimeout   = 25.0;    // s
  double cmdSpeedMaxAdjustment = 10.0;    // m/s
    
  // following parameters are mainly for APF leader selection
  // UCLA: Rename the next four variables for better explainability.
  double minAllowableHeadaway    = 1.6;   // s
  double headawayStableLowerBond = 1.7;   // s
  double maxAllowableHeadaway    = 4.0;   // s
  double headawayStableUpperBond = 3.9;   // s
  
  double minGap                  = 22.0;  // m
  double maxGap                  = 32.0;  // m
  double maxCrosstrackError      = 2.0;   // m

  // width of lane, temporary parameter that used to check end of lane change.
  double laneWidth              = 3.0;    // m

  std::string vehicleID       = "default_id";

  // ---------------------- UCLA: parameters for IHP platoon trajectory regulation ----------------
  /**
  * \brief Parameter sets for IHP platoon trajectory regulation algorithm. 
  * Please refer to the updated design doc for detailed parameter description.
  */
  double ss_theta = 4.0; // Stanstill determining threshold, in m/s.
  double standstill = 2.0; // Stanstill reaction time adjuster, in s.
  double inter_tau = 1.5; // Inter-platoon time gap, refer to bumper to bumper gap time, in s.
  double intra_tau = 0.6; // Intra-platoon time gao, refer to bumper to bumper gap time, in s.
  double gap_weight = 0.9; // Weighted ratio for time-gap based calculation, unitless.
  //------------------------------------------------------------------------------------------------


  friend std::ostream& operator<<(std::ostream& output, const PlatoonPluginConfig& c)
  {
    output << "PlatoonPluginConfig { " << std::endl
          << "maxPlatoonSize: " << c.maxPlatoonSize << std::endl
          << "algorithmType: " << c.algorithmType << std::endl
          << "statusMessageInterval: " << c.statusMessageInterval << std::endl
          << "infoMessageInterval: " << c.infoMessageInterval << std::endl
          << "timeHeadway: " << c.timeHeadway << std::endl
          << "standStillHeadway: " << c.standStillHeadway << std::endl
          << "maxAllowedJoinTimeGap: " << c.maxAllowedJoinTimeGap << std::endl
          << "desiredJoinTimeGap: " << c.desiredJoinTimeGap << std::endl
          << "desiredJoinGap: " << c.desiredJoinGap << std::endl
          << "waitingStateTimeout: " << c.waitingStateTimeout << std::endl
          << "cmdSpeedMaxAdjustment: " << c.cmdSpeedMaxAdjustment << std::endl
          << "minAllowableHeadaway: " << c.minAllowableHeadaway << std::endl
          << "maxAllowableHeadaway: " << c.maxAllowableHeadaway << std::endl
          << "headawayStableLowerBond: " << c.headawayStableLowerBond << std::endl
          << "headawayStableUpperBond: " << c.headawayStableUpperBond << std::endl
          << "kpminGapPID: " << c.minGap << std::endl
          << "maxGap: " << c.maxGap << std::endl
          << "maxCrosstrackError: " << c.maxCrosstrackError << std::endl
          << "vehicleID: " << c.vehicleID << std::endl
          << "}" << std::endl;
    return output;
  }
};
