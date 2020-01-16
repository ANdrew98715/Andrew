option(BDefenderReadyState)
{
  	initial_state(start)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefenderReady:start");
            OUTPUT_TEXT(theRobotPose.translation.y());
		}
		transition
		{
			if(state_time > 5000 && theRobotPose.translation.y() >= 0)
				goto backToFixedY;
			if(state_time > 5000 && theRobotPose.translation.y() < 0)
				goto backToFixedLeftSpot2;
		}
		action
		{
			Stand();
			HeadControlMode(HeadControl::lookLeftAndRight);
		}
	}

  state(wait)
  {
	  action
	  {
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefenderReady:wait");
		}
		  if(std::abs(theRobotPose.rotation) > 10_deg)
		  {
			  WalkToTarget(Pose2f(50.f, 30.f, 30.f),Pose2f(-theRobotPose.rotation,0.f,0.f));
		  }
		  else
		   {
			   Stand();
		   }
		   theHeadControlMode = HeadControl::lookLeftAndRight;
	  }
  }
 
//	state(backToFixedY)
//	{
//		if(state_time < 1)
//		{
//			OUTPUT_TEXT("BDefenderReady:backToFixedY");
//		}
//		transition
//		{
//			if(std::abs(theRobotPose.translation.y() - 450) < 100)
//				goto backToFixedX;
//		}
//		action
//		{
//			if(theRobotPose.rotation <= 0_deg && theRobotPose.rotation >= -170_deg)
//				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(-(theRobotPose.rotation + 180_deg), 0.f,0.f));
//			if(theRobotPose.rotation > 0_deg && theRobotPose.rotation <= 170_deg)
//				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f((180_deg - theRobotPose.rotation), 0.f,0.f));
//			if(std::abs(pi+theRobotPose.rotation)<=10_deg)
//				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(0.f,0.f,(theRobotPose.translation.y()-450)));
//			if(std::abs(pi-theRobotPose.rotation)<=10_deg)
//				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(0.f,0.f,(theRobotPose.translation.y()-450))); 
//			theHeadControlMode = HeadControl::lookLeftAndRight;
//		}
//	}
  
	state(backToFixedLeftSpot2)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefenderReady:backToFixedLeftSpot2");
		}
		transition
		{
			if(theRobotPose.translation.y() > 0)
				goto backToFixedY;
		}
		action
		{
			HeadControlMode(HeadControl::lookLeftAndRight);
			if(std::abs(theRobotPose.rotation) > 10_deg)
			{
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(-theRobotPose.rotation,0.f,0.f));
			}
			if(std::abs(theRobotPose.rotation) <= 10_deg)
			{
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(0.f,0.f,1000.f));
			}
		}
	}
	
	state(backToFixedY)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefenderReady:backToFixedX");
		}
		transition
		{
			if((theRobotPose.translation - theLibCodeRelease.BLeftPoint).norm() < 100)
				goto wait;
			
		}
		action
		{
			theHeadControlMode = HeadControl::lookLeftAndRight;
            theMotionRequest = thePathPlanner.plan(Pose2f(0.f,-3500,450),Pose2f(50.f,50.f,50.f),true);
//			WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(0.f,3500+theRobotPose.translation.x(),0.f));
//			if(std::abs(theRobotPose.translation.x()+3500) < 100)
//				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(-theRobotPose.rotation,0.f,0.f)); 
		}
	}
}