/**
 * @file LibCodeReleaseProvider.h
 */

#include "Tools/Module/Module.h"
#include "Representations/BehaviorControl/BehaviorStatus.h"
#include "Representations/BehaviorControl/Libraries/LibCodeRelease.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Modeling/BallModel.h"
#include "Representations/Modeling/RobotPose.h"
#include "Tools/Module/Module.h"
#include "Representations/BehaviorControl/Libraries/LibCodeRelease.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Modeling/BallModel.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Communication/TeamData.h"
#include "Representations/Modeling/ObstacleModel.h"
#include "Representations/Modeling/TeamBallModel.h"
#include "Representations/Perception/ObstaclesPercepts/ObstaclesFieldPercept.h"
#include "Representations/Perception/ObstaclesPercepts/ObstaclesImagePercept.h"
#include "Representations/MotionControl/ArmMotionRequest.h"

MODULE(LibCodeReleaseProvider,
{,
  REQUIRES(BallModel),
  REQUIRES(FieldDimensions),
  REQUIRES(FrameInfo),
  REQUIRES(RobotPose),
  USES(BehaviorStatus),
  REQUIRES(TeamData),
  REQUIRES(TeamBallModel),
  REQUIRES(ObstaclesImagePercept),
  REQUIRES(ObstaclesFieldPercept),
  REQUIRES(ObstacleModel),
  PROVIDES(LibCodeRelease),
  
});

class LibCodeReleaseProvider : public LibCodeReleaseProviderBase
{
  void update(LibCodeRelease& libCodeRelease);
};