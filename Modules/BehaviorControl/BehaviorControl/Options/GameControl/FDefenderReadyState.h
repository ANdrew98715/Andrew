option(FDefenderReadyState)
{
	initial_state(start)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefenderReady:start");
		}
		transition
		{
			if(state_time > 5000 && theRobotPose.translation.y() <= 0)
				goto backToFixedY;
			if(state_time > 5000 && theRobotPose.translation.y() > 0)
				goto backToFixedRightSpot2;
		}
		action
		{
			Stand();
			HeadControlMode(HeadControl::lookLeftAndRight);
		}
	}

  state(wait)
  {
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefenderReady:wait");
		}
	  action
	  {
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
  
  	state(backToFixedY)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefenderReady:backToFixedY");
		}
		transition
		{
			if( std::abs(theRobotPose.translation.y()+450) < 100)
				goto backToFixedX;
		}
		action
		{
			theHeadControlMode = HeadControl::lookLeftAndRight;
			if(theRobotPose.rotation <= 0_deg && theRobotPose.rotation >= -170_deg)
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(-(theRobotPose.rotation + 180_deg), 0.f,0.f));
			if(theRobotPose.rotation > 0_deg && theRobotPose.rotation <= 170_deg)
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f((180_deg - theRobotPose.rotation), 0.f,0.f));
			if(std::abs(pi+theRobotPose.rotation)<=10_deg)
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(0.f,0.f,(theRobotPose.translation.y()+450)));
			if(std::abs(pi-theRobotPose.rotation)<=10_deg)
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(0.f,0.f,(theRobotPose.translation.y()+450))); 
		}
	} 
	
	state(backToFixedRightSpot2)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefenderReady:backToFixedRightSpot2");
		}
		transition
		{
			if(theRobotPose.translation.y() < 0)
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
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(0.f,0.f,-1000.f));
			}
		}
	}	
	
	state(backToFixedX)
	{
		
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefenderReady:backToFixedX");
		}
		transition
		{
			if(std::abs(theRobotPose.translation.x()+2500) < 100)
				goto wait;
		}
		action
		{
			theHeadControlMode = HeadControl::lookLeftAndRight;
			if(std::abs(theRobotPose.rotation) > 15_deg)
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(-theRobotPose.rotation,0.f,0.f)); 
			else
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(0.f,-2500-theRobotPose.translation.x(),0.f)); 
		}
	}
}