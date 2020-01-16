/**
 * @file LibCodeRelease.h
 */

#pragma once
#include "Tools/Function.h"
#include "Representations/Communication/TeamData.h"
#include "Representations/Perception/ObstaclesPercepts/ObstaclesFieldPercept.h"
#include "Tools/Modeling/Obstacle.h"
#include "Representations/Modeling/TeamBallModel.h"

STREAMABLE(LibCodeRelease,
{
  FUNCTION(bool(float value, float min, float max)) between,
  (int) (0) numOfPlayers,
  (float) obstacleAndRobotDistance,
  (float) angleToGoal,
  (float) angleToOwnGoal,
  (int) timeSinceBallWasSeen,//看到球的时刻
  (Vector2f) gloBallPos, //球的全局坐标
  (Vector2f) ballPos, //球的相对坐标
  (Vector2f) BRightPoint,
  (Vector2f) BLeftPoint,
  (Vector2f) FRightPoint,
  (Vector2f) FLeftPoint,
  
  (Vector2f) pointOneToRobot,
  (Vector2f) pointOne,
  (Vector2f) pointTwo,
  (int) (0) holdBall,
  
  (Vector2f) Obstacle,
  (float) pointX,
  (float) pointY,
  (float) turnbody,
  (double) Ballvelocity,//球的速度
  (bool) (false) closestObstacleDistance,
  (Teammate) bdefender,//通信队友后卫1
  (Teammate) striker1,
  (Teammate) striker2,
  (Teammate) fdefender,//通信队友后卫2
  (Teammate) keeper,//通信队友守门员
//  (Obstacle) obstacle,//障碍
  (double) angleToBall,//与球的角度
  (double) ballDistance,//通信得到球的距离
  (TeamBallModel) contributors,
  (Vector2f) ballPosition,
  (ObstaclesFieldPercept::Obstacle) closestObstacle,
  //(ObstaclesFieldPercept::Obstacle) obstacles[10],
});
