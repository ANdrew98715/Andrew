/**
 * @file LibCodeReleaseProvider.cpp
 */

#include "LibCodeReleaseProvider.h"

MAKE_MODULE(LibCodeReleaseProvider, behaviorControl);

void LibCodeReleaseProvider::update(LibCodeRelease& libCodeRelease)
{
  libCodeRelease.timeSinceBallWasSeen = theFrameInfo.getTimeSince(theBallModel.timeWhenLastSeen);
  libCodeRelease.angleToOwnGoal = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOwnGroundline, 0.f)).angle();
  libCodeRelease.angleToGoal = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline, 0.f)).angle();
  libCodeRelease.between = [&](float value, float min, float max) -> bool
  {
    return value >= min && value <= max;
  };
  
  libCodeRelease.FLeftPoint = Vector2f(-2500.f,450.f);
  libCodeRelease.FRightPoint = Vector2f(-2500.f,-450.f);
  libCodeRelease.BRightPoint = Vector2f(-3500.f,-450.f);
  libCodeRelease.BLeftPoint = Vector2f(-3500.f,450.f);
  
  libCodeRelease.pointOne = libCodeRelease.striker1.theRobotPose.translation;
  libCodeRelease.pointTwo = libCodeRelease.fdefender.theRobotPose.translation;
  
  libCodeRelease.pointOneToRobot = theRobotPose.inversePose * libCodeRelease.pointOne;  //点的全局坐标转为机器人的相对坐标
  
/*  libCodeRelease.pointX = libCodeRelease.gloBallPos.x() - 180*std::cos(libCodeRelease.BRightPointTest.angle()) + 30*std::sin(libCodeRelease.BRightPointTest.angle());
  libCodeRelease.pointY = libCodeRelease.gloBallPos.y() + 180*std::sin(libCodeRelease.BRightPointTest.angle()) + 30*std::cos(libCodeRelease.BRightPointTest.angle());*/
  
/*  libCodeRelease.fdefenderAndBall = (float) std::sqrt(std::pow(libCodeRelease.fdefender.theBallModel.estimate.position.x(),2) + std::pow(libCodeRelease.fdefender.theBallModel.estimate.position.y(),2));
  
  libCodeRelease.bdefenderAndBall = (float) std::sqrt(std::pow(libCodeRelease.bdefender.theBallModel.estimate.position.x(),2) + std::pow(libCodeRelease.bdefender.theBallModel.estimate.position.y(),2));*/
  
    libCodeRelease.gloBallPos = theRobotPose * theBallModel.estimate.position;  //球的全局坐标
    libCodeRelease.ballPos = theRobotPose.inversePose * theTeamBallModel.position; //球的相对坐标
  
	if(theBallModel.estimate.position.angle() > 75_deg)   //转身角度
		libCodeRelease.turnbody = pi/2;
	if(theBallModel.estimate.position.angle() < -75_deg)
		libCodeRelease.turnbody = -pi/2;
	if(std::abs(theBallModel.estimate.position.angle()) < 75_deg)
		libCodeRelease.turnbody = 0;
        
  libCodeRelease.Ballvelocity = std::sqrt(std::pow(theBallModel.estimate.velocity.x() ,2) + std::pow(theBallModel.estimate.velocity.y(),2)); //球的全局速度
  
  for (auto const& teammate : theTeamData.teammates)//通信
  {
      switch(teammate.number)
      {
		  case 1:
		  libCodeRelease.keeper = teammate; break;
		  case 2:
		  libCodeRelease.striker1 = teammate; break;
          case 3:
          libCodeRelease.fdefender = teammate; break;
          case 4:
          libCodeRelease.bdefender = teammate; break;
		  case 5:
          libCodeRelease.striker2 = teammate; break;
          default : break;
      }
  }
  
  for(const auto& obstacle : theObstacleModel.obstacles)
  {
      libCodeRelease.Obstacle = obstacle.center;

  }

}
