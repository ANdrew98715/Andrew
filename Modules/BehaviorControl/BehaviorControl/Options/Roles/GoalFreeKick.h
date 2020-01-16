/** A test striker option without common decision */
option(GoalFreeKick)
{
  initial_state(start)
  {
    transition
    {
      if(theBallModel.estimate.position.norm() < 500.f)
          goto alignBehindBall;
      if(state_time > 1500 && theLibCodeRelease.timeSinceBallWasSeen < 600)
          goto walkToBall;
      if((theLibCodeRelease.BLeftPoint - theRobotPose.translation).norm() < 600)
          goto walkToRight;
      if((theLibCodeRelease.BRightPoint - theRobotPose.translation).norm() < 600)
          goto walkToLeft;
    }
    action
    {
        HeadControlMode(HeadControl::lookForward);
        if((theLibCodeRelease.BLeftPoint - theRobotPose.translation).norm() < (theLibCodeRelease.BRightPoint - theRobotPose.translation).norm())
            theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.BLeftPoint.x()-180,theLibCodeRelease.BLeftPoint.y()+30),Pose2f(50.f,50.f,50.f),true);
        else
            theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.BRightPoint.x(),theLibCodeRelease.BRightPoint.y()),Pose2f(50.f,50.f,50.f),true);
        OUTPUT_TEXT("FreeKickStart");
    }
  }
  
  state(walkToRight)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen < 600)
          goto walkToBall;
      if(theBallModel.estimate.position.norm() < 500.f)
          goto alignBehindBall;
      if((theLibCodeRelease.BRightPoint - theRobotPose.translation).norm() < 500)
          goto searchForBall;
    }
    action
    {
      HeadControlMode(HeadControl::lookForward);
      theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.BRightPoint.x(),theLibCodeRelease.BRightPoint.y()),Pose2f(50.f,50.f,50.f),true);
      OUTPUT_TEXT("walkToRight");
    }
  }
  
  state(walkToLeft)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen < 600)
          goto walkToBall;
      if(theBallModel.estimate.position.norm() < 500.f)
          goto alignBehindBall;
      if((theLibCodeRelease.BLeftPoint - theRobotPose.translation).norm() < 500)
          goto searchForBall;
    }
    action
    {
      HeadControlMode(HeadControl::lookForward);
      theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.BLeftPoint.x(),theLibCodeRelease.BLeftPoint.y()),Pose2f(50.f,50.f,50.f),true);
      OUTPUT_TEXT("walkToLeft");
    }
  }
  
  state(walkToBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto searchForBall;
      if(theBallModel.estimate.position.norm() < 200.f)
        goto alignBehindBall;
    }
    action
    {
        OUTPUT_TEXT("Free:walkToBall");
      HeadControlMode(HeadControl::lookForward);
      WalkToTarget(Pose2f(50.f, 50.f, 50.f), theBallModel.estimate.position);
    }
  }
  
  state(turnToBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto searchForBall;
      if(std::abs(theBallModel.estimate.position.angle()) < 5_deg)
        goto walkToBall;
    }
    action
    {
        OUTPUT_TEXT("Free:turnToBall");
      HeadControlMode(HeadControl::lookForward);
      WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
    }
  }



/*  state(alignToGoal)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto searchForBall;
      if(std::abs(theLibCodeRelease.angleToGoal) < 10_deg && std::abs(theBallModel.estimate.position.y()) < 100.f)
        goto alignBehindBall;
    }
    action
    {
        OUTPUT_TEXT("Free:alignToGoal");
      HeadControlMode(HeadControl::lookForward);
      WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 400.f, theBallModel.estimate.position.y()));
    }
  }*/

  state(alignBehindBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto searchForBall;
      if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f)
         && theLibCodeRelease.between(theBallModel.estimate.position.x(), 160.f, 200.f)
         && std::abs(theLibCodeRelease.pointOneToRobot.angle()) < 5_deg)
        goto kick;
    }
		action
		{
            OUTPUT_TEXT("Free:alignBehindBall");
            if(std::abs(theBallModel.estimate.position.angle()) > 10_deg)
				{
					WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f)); //对齐球
				}
            if(std::abs(theLibCodeRelease.pointOneToRobot.angle()) > 5_deg)
            {
                if(theLibCodeRelease.pointOneToRobot.angle() >= 0)
                    WalkAtAbsoluteSpeed(Pose2f(0.3f, 0.f, -60.f));
                if(theLibCodeRelease.pointOneToRobot.angle() < 0)
                    WalkAtAbsoluteSpeed(Pose2f(-0.3f, 0.f, 60.f));
            }
			else
			{

                    WalkToTarget(Pose2f(50.f,50.f,50.f), Pose2f(0.f, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() + 30.f));
                
                    FollowTheTarget(theBallModel.estimate.position.angle()); 
			
			}
//      theHeadControlMode = HeadControl::lookForward;
//      WalkToTarget(Pose2f(80.f, 80.f, 80.f), Pose2f(theLibCodeRelease.angleToPoint, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() + 30.f));
		}
  }

  state(kick)
  {
    transition
    {
      if(state_time > 3000 || (state_time > 10 && action_done))
        goto start;
    }
    action
    {
        OUTPUT_TEXT("Free:kick");
      HeadControlMode(HeadControl::lookForward);
      Shoot(KickRequest::greatKickForward);
  }
  }

  state(searchForBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen < 300)
        goto turnToBall;
    }
    action
    {
      HeadControlMode(HeadControl::lookForward);
      WalkAtRelativeSpeed(Pose2f(1.f, 0.f, 0.f));
      OUTPUT_TEXT("Free:searchForBall");
    }
  }
}