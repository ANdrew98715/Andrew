option(TextRobotBDefenderReadyState)
{
	initial_state(start)
	{
		transition
		{
			if(std::abs(theRobotPose.translation.x() + 3500) < 100 && std::abs(theRobotPose.translation.y() - 450) < 100 && std::abs(theRobotPose.rotation) < 10_deg)
				goto wait;
		}
		action
		{
			theMotionRequest = thePathPlanner.plan(Pose2f(0.f,-3500.f,450.f),Pose2f(50.f,50.f,50.f),false);
			HeadControlMode(HeadControl::lookLeftAndRight);
		}
	}
	
	state(wait)
	{
		action
		{
			Stand();
			HeadControlMode(HeadControl::lookLeftAndRight);
		}
	}
}