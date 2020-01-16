option(KickToPoint, (const Vector2f&) point)
{
    initial_state(alignToPoint)
	{
		transition
		{
			if( std::abs(point.angle()) < 10_deg)
				goto alignBehindBallToPoint;
		}
		action
		{
			if(point.angle() >= 0)
				WalkAtAbsoluteSpeed(Pose2f(0.3f, 0.f, -60.f));
			if(point.angle() < 0)
				WalkAtAbsoluteSpeed(Pose2f(-0.3f, 0.f, 60.f));
		}
	}
        state(alignBehindBallToPoint)
	{
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && point.angle() >= 5_deg)
				goto alignToPoint;
		}
		action
		{
            if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 165.f, 195.f) && std::abs(point.angle()) <= 5_deg)
            {
            Activity(BehaviorStatus::greatKick);
			FollowTheTarget(theBallModel.estimate.position.angle());
			Shoot(KickRequest::greatKickForward);
            }
			if(std::abs(theBallModel.estimate.position.angle()) > 10_deg)
				{
					WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f)); //对齐球
				}
            if(std::abs(point.angle()) > 5_deg)
            {
                if(point.angle() >= 0)
                    WalkAtAbsoluteSpeed(Pose2f(0.3f, 0.f, -60.f));
                if(point.angle() < 0)
                    WalkAtAbsoluteSpeed(Pose2f(-0.3f, 0.f, 60.f));
            }
			else
			{

                    WalkToTarget(Pose2f(50.f,50.f,50.f), Pose2f(0.f, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() + 30.f));
                
                    FollowTheTarget(theBallModel.estimate.position.angle()); 
			}
		}
	}
}
