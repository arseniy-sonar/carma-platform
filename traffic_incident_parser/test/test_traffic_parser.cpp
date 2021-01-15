/*
 * Copyright (C) 2020 LEIDOS.
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

#include "traffic_incident_parser_worker.h"
#include <gtest/gtest.h>
#include <carma_wm/CARMAWorldModel.h>
#include <carma_wm/WMTestLibForGuidance.h>

namespace traffic
{

TEST(TrafficIncidentParserWorkerTest, testMobilityMessageParser1)
{

  std::shared_ptr<carma_wm::CARMAWorldModel> cmw = std::make_shared<carma_wm::CARMAWorldModel>();
  TrafficIncidentParserWorker traffic_worker(std::static_pointer_cast<const carma_wm::WorldModel>(cmw),[](auto msg){});
    
  std::string mobility_strategy_params="lat:0.435,lon:0.555,closed_lanes:1,downtrack:5,uptrack:5";
  traffic_worker.mobilityMessageParser(mobility_strategy_params);
  
  EXPECT_EQ(traffic_worker.latitude,0.435);
  EXPECT_EQ(traffic_worker.longitude,0.555);
  EXPECT_EQ(traffic_worker.closed_lane,1);
  EXPECT_EQ(traffic_worker.down_track,5);
  EXPECT_EQ(traffic_worker.up_track,5);  
  
  }


TEST(TrafficIncidentParserWorkerTest, testMobilityMessageParser2)
{

  std::shared_ptr<carma_wm::CARMAWorldModel> cmw = std::make_shared<carma_wm::CARMAWorldModel>();
  TrafficIncidentParserWorker traffic_worker(std::static_pointer_cast<const carma_wm::WorldModel>(cmw),[](auto msg){});
      
  std::string mobility_strategy_params="lat:0.75,lon:0.555,closed_lanes:2,downtrack:75,uptrack:55";
  traffic_worker.mobilityMessageParser(mobility_strategy_params);
  
  EXPECT_EQ(traffic_worker.latitude,0.75);
  EXPECT_EQ(traffic_worker.longitude,0.555);
  EXPECT_EQ(traffic_worker.closed_lane,2);
  EXPECT_EQ(traffic_worker.down_track,75);
  EXPECT_EQ(traffic_worker.up_track,55);  
  
  }


TEST(TrafficIncidentParserWorkerTest, testMobilityMessageParser3)
{

  std::shared_ptr<carma_wm::CARMAWorldModel> cmw = std::make_shared<carma_wm::CARMAWorldModel>();
  TrafficIncidentParserWorker traffic_worker(std::static_pointer_cast<const carma_wm::WorldModel>(cmw),[](auto msg){});
   
  std::string mobility_strategy_params="lat:0.3,lon:0.95,closed_lanes:3,downtrack:57,uptrack:59";
  traffic_worker.mobilityMessageParser(mobility_strategy_params);
  
  EXPECT_EQ(traffic_worker.latitude,0.3);
  EXPECT_EQ(traffic_worker.longitude,0.95);
  EXPECT_EQ(traffic_worker.closed_lane,3);
  EXPECT_EQ(traffic_worker.down_track,57);
  EXPECT_EQ(traffic_worker.up_track,59);  
  
  }

  TEST(TrafficIncidentParserWorkerTest, earthToMapFrame)
{

  std::shared_ptr<carma_wm::CARMAWorldModel> cmw = std::make_shared<carma_wm::CARMAWorldModel>();
  TrafficIncidentParserWorker traffic_worker(std::static_pointer_cast<const carma_wm::WorldModel>(cmw),[](auto msg){});
  std_msgs::String projection_msg;
  projection_msg.data="+proj=tmerc +lat_0=39.46636844371259 +lon_0=-76.16919523566943 +k=1 +x_0=0 +y_0=0 +datum=WGS84 +units=m +vunits=m +no_defs";

  traffic_worker.projectionCallback(projection_msg);

  std::string mobility_strategy_params="lat:39.46636844371259,lon:-76.16919523566943,closed_lanes:3,downtrack:57,uptrack:59";
  traffic_worker.mobilityMessageParser(mobility_strategy_params);

  lanelet::BasicPoint2d local_point=traffic_worker.earthToMapFrame();
    
  EXPECT_NEAR(local_point.x(),0,0.001);
  EXPECT_NEAR(local_point.y(),0,0.001);
}

  TEST(TrafficIncidentParserWorkerTest, earthToMapFrame)
{

  auto cmw= getGuidanceTestMap();
  carma_wm::test::setRouteByLanelets({1200, 1201,1202,1203}, cmw);
  
  TrafficIncidentParserWorker traffic_worker(std::static_pointer_cast<const carma_wm::WorldModel>(cmw),[](auto msg){});
  std_msgs::String projection_msg;
  projection_msg.data="+proj=tmerc +lat_0=39.46636844371259 +lon_0=-76.16919523566943 +k=1 +x_0=0 +y_0=0 +datum=WGS84 +units=m +vunits=m +no_defs";

  traffic_worker.projectionCallback(projection_msg);

  std::string mobility_strategy_params="lat:39.46636844371259,lon:-76.16919523566943,closed_lanes:3,downtrack:57,uptrack:59";
  traffic_worker.mobilityMessageParser(mobility_strategy_params);
  
  traffic_worker.composeTrafficControlMesssage();


  

  //lanelet::BasicPoint2d local_point=traffic_worker.earthToMapFrame();
    
  EXPECT_NEAR(local_point.x(),0,0.001);
  EXPECT_NEAR(local_point.y(),0,0.001);
}

}//traffic
