option(BDefender)
{
	static bool     flag = false;
	static bool    flag2=false;
	static unsigned int    time = 0;//定义全局变量-比赛时间
	
	common_transition
	{
	  if(theArmContactModel.status[Arms::left].contact == true || theArmContactModel.status[Arms::right].contact == true || theLibCodeRelease.Obstacle.x() < 500)
	  {
		  flag = true;
		  time = theFrameInfo.time; //theFrameInfo.time为全局的时间，从机器人开始运行代码从0开始，正常的话半场十分钟总共600000毫秒。
	  }
	  if(flag == true)
	  {
		  theArmMotionRequest.armMotion[Arms::left] = theArmMotionRequest.armMotion[Arms::right] = ArmMotionRequest::keyFrame;
		  theArmMotionRequest.armKeyFrameRequest.arms[Arms::left].motion = theArmMotionRequest.armKeyFrameRequest.arms[Arms::right].motion = ArmKeyFrameRequest::back;
	  }
	  if((theFrameInfo.time - time) > 2000 && theLibCodeRelease.Obstacle.x() > 500)//从接触障碍手背后持续五秒，可加特殊条件。
		  flag = false;
	  if(flag == false)
	  {
		  theArmMotionRequest.armMotion[Arms::left] = theArmMotionRequest.armMotion[Arms::right] = ArmMotionRequest::none;
	      theArmMotionRequest.armKeyFrameRequest.arms[Arms::left].motion = theArmMotionRequest.armKeyFrameRequest.arms[Arms::right].motion = ArmKeyFrameRequest::useDefault;
	  }
	}
	
	initial_state(start)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:start");
		}
		transition
		{
			if(state_time > 10000)
				goto readyok;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
//                goto KickToPoint;
				goto walkToBall;
			if((theLibCodeRelease.timeSinceBallWasSeen >= 600 || (theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > 0)) && state_time > 5000)
				goto alignToOwnGoal;
		}
		action
		{
			Stand();
			HeadControlMode(HeadControl::lookLeftAndRight);
		}
	}
	
	state(readyok)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:readyok");
		}
		transition
		{
			if((std::abs(theRobotPose.rotation) < 10_deg && std::abs(theRobotPose.translation.x() + 3500) < 100 && std::abs(theRobotPose.translation.y() - 450) < 100))
				goto fixedLeftSpot;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
		}
		action
		{
			HeadControlMode(HeadControl::lookLeftAndRight);
			WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(-theRobotPose.rotation,(-theRobotPose.translation.x() - 3500),(450 - theRobotPose.translation.y())));    //走向全局坐标的（-3500，450），通过机器人的全局坐标将目标点转换为机器人的相对坐标
		}
	}
	
	state(fixedRightSpot)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:fixedRightSpot");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
/*			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && theFrameInfo.getTimeSince(theLibCodeRelease.striker1.timeWhenLastPacketReceived) < 2000)  //大于7000
            {
                if((float)theLibCodeRelease.striker1.theBallModel.timeWhenDisappeared < theBehaviorParameters.ballNotSeenTimeOut)
                {
                    if((float)theLibCodeRelease.striker1.theBallModel.timeWhenDisappeared < theBehaviorParameters.ballNotSeenTimeOut)
                        goto knowTeammateBallStriker1;
                    else
                        goto searchForBall;
                }
                else
                    goto searchForBall;
            }
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && theFrameInfo.getTimeSince(theLibCodeRelease.striker2.timeWhenLastPacketReceived) < 2000)  //大于7000
            {
                if((float)theLibCodeRelease.striker2.theBallModel.timeWhenDisappeared < theBehaviorParameters.ballNotSeenTimeOut)
                {
                    if((float)theLibCodeRelease.striker2.theBallModel.timeWhenDisappeared < theBehaviorParameters.ballNotSeenTimeOut)
                        goto knowTeammateBallStriker2;
                    else
                        goto searchForBall;
                }
                else
                    goto searchForBall;
            }*/
            if(std::abs(theRobotPose.rotation) <= 10_deg)
                goto doNotMove;
		}
		action
		{
			if(std::abs(theRobotPose.rotation) > 10_deg)
			{
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(-theRobotPose.rotation, 0.f,0.f));
			}
		}
	}
	
	state(fixedLeftSpot)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:fixedLeftSpot");
		}
		transition
		{
			if((theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < -1500) || (theLibCodeRelease.timeSinceBallWasSeen < 600 && theBallModel.estimate.position.norm() < theLibCodeRelease.fdefender.theBallModel.estimate.position.norm() && theLibCodeRelease.between(theLibCodeRelease.gloBallPos.x(),-1500,0)))
//                goto KickToPoint;
				goto walkToBall;
/*			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
            {
                if(theFrameInfo.getTimeSince(theLibCodeRelease.fdefender.timeWhenLastPacketReceived) < 2000)
                {
                    if((float)theLibCodeRelease.fdefender.theBallModel.timeWhenDisappeared > theBehaviorParameters.ballNotSeenTimeOut) 
                        goto searchForBall;
                    else
                        goto knowTeammateBallFdefender;
                }
                else
                    goto searchForBall;
            }*/
            if(std::abs(theRobotPose.rotation) <= 10_deg)
                goto doNotMove;
				
		}
		action
		{
			if(std::abs(theRobotPose.rotation) > 10_deg)
			{
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(-theRobotPose.rotation, 0.f,0.f));
			}
		}
	}
    
    state(knowTeammateBallStriker2)
    {
        if(state_time < 1)
            OUTPUT_TEXT("B:knowTeammateBall2");
        transition
        {
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 || theFrameInfo.getTimeSince(theLibCodeRelease.striker2.timeWhenLastPacketReceived) > 2000 || (float)theFrameInfo.getTimeSince(theLibCodeRelease.striker2.theBallModel.timeWhenLastSeen) > 2000)
                goto searchForBall;
        }
        action
        {
            Stand();
            WalkToTarget(Pose2f(100.f,100.f,100.f),Pose2f(theLibCodeRelease.ballPos.angle(), 0.f,0.f));
            FollowTheTarget(theLibCodeRelease.ballPos.angle()); //考虑是否使用看到球的机器人的球坐标，而不是通信球模型
        }
    }
    
    state(knowTeammateBallStriker1)
    {
        if(state_time < 1)
            OUTPUT_TEXT("B:knowTeammateBall1");
        transition
        {
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 || theFrameInfo.getTimeSince(theLibCodeRelease.striker1.timeWhenLastPacketReceived) > 2000 || (float)theFrameInfo.getTimeSince(theLibCodeRelease.striker1.theBallModel.timeWhenLastSeen) > 2000)
                goto searchForBall;
        }
        action
        {
            Stand();
            WalkToTarget(Pose2f(100.f,100.f,100.f),Pose2f(theLibCodeRelease.ballPos.angle(), 0.f,0.f));
            FollowTheTarget(theLibCodeRelease.ballPos.angle()); //考虑是否使用看到球的机器人的球坐标，而不是通信球模型
        }
    }
    
	
	state(walkToBall)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:walkToBall");
		}
		transition
		{
			if((theLibCodeRelease.timeSinceBallWasSeen < 600 && std::abs(theLibCodeRelease.gloBallPos.y()) < 1500 && theLibCodeRelease.gloBallPos.x() < -3700) || theBallModel.estimate.position.norm() < 1000)
				goto walkToBall2;
			if(!theLibCodeRelease.fdefender.isPenalized && theLibCodeRelease.fdefender.isUpright && theFrameInfo.getTimeSince(theLibCodeRelease.fdefender.theBallModel.timeWhenLastSeen) < 500 && theLibCodeRelease.striker1.theBallModel.estimate.position.norm() < 600 && (theLibCodeRelease.gloBallPos-theLibCodeRelease.fdefender.theRobotPose.translation).norm() < theBallModel.estimate.position.norm() && theFrameInfo.getTimeSince(theLibCodeRelease.fdefender.timeWhenLastPacketReceived) < 2000)
				goto adjustAngle;
			if(!theLibCodeRelease.striker1.isPenalized && theLibCodeRelease.striker1.isUpright && theFrameInfo.getTimeSince(theLibCodeRelease.striker1.theBallModel.timeWhenLastSeen) < 1000 && theLibCodeRelease.striker1.theBallModel.estimate.position.norm() < 600 &&  theFrameInfo.getTimeSince(theLibCodeRelease.striker1.timeWhenLastPacketReceived) < 2000)
				goto adjustAngle;
//			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 165.f, 195.f) && theLibCodeRelease.gloBallPos.x() < -1500 && std::abs(theRobotPose.rotation) < 45_deg)
//				goto fastKick;
//			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 165.f, 195.f) && theLibCodeRelease.gloBallPos.x() >= -1500 && std::abs(theLibCodeRelease.angleToGoal) < 20_deg)
//				goto greatKick;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > 0 && theBallModel.estimate.position.norm() > 1500) //可以修改，站在此地，看着球
                goto doNotMove;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut) //大于7000
                goto searchForBall;
		}
		action
		{
            FollowTheTarget(theBallModel.estimate.position.angle());
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < -1500)
				{
					theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.gloBallPos.x() -180,theLibCodeRelease.gloBallPos.y() + 30),Pose2f(50.f,50.f,50.f),true);
					FollowTheTarget(theBallModel.estimate.position.angle());
				}
			else
			{
				theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.gloBallPos.x() -180,theLibCodeRelease.gloBallPos.y() + 30),Pose2f(50.f,50.f,50.f),true);
				if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 165.f, 195.f))
				{
					if(std::abs(theLibCodeRelease.angleToGoal) >= 20_deg)
					{
						OUTPUT_TEXT("BDefender:alignToGoal");
						if(theLibCodeRelease.angleToGoal >= 0)
							{
								WalkAtAbsoluteSpeed(Pose2f(0.6f, 0.f, -160.f));
								FollowTheTarget(theBallModel.estimate.position.angle());
							}
						if(theLibCodeRelease.angleToGoal < 0)
							{
								WalkAtAbsoluteSpeed(Pose2f(-0.6f, 0.f, 160.f));
								FollowTheTarget(theBallModel.estimate.position.angle());
							}
					}
				}
			}
		}
	}
	
	state(walkToBall2)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:walkToBall2");
		}
		transition
		{
            if(!theLibCodeRelease.striker1.isPenalized && theLibCodeRelease.striker1.isUpright && theFrameInfo.getTimeSince(theLibCodeRelease.striker1.theBallModel.timeWhenLastSeen) < 1000 && theLibCodeRelease.striker1.theBallModel.estimate.position.norm() < 600 &&  theFrameInfo.getTimeSince(theLibCodeRelease.striker1.timeWhenLastPacketReceived) < 2000)
				goto adjustAngle;
			if(!theLibCodeRelease.fdefender.isPenalized  && theFrameInfo.getTimeSince(theLibCodeRelease.fdefender.theBallModel.timeWhenLastSeen) < 500 && (theTeamBallModel.position-theLibCodeRelease.fdefender.theRobotPose.translation).norm() < 600)
				goto adjustAngle;
			if(theBallModel.estimate.position.norm() < 200.f)
                goto alignToPoint;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > 0 && theBallModel.estimate.position.norm() > 1000)
				goto doNotMove;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
		}
		action
		{
			if(!theLibCodeRelease.keeper.isPenalized && theLibCodeRelease.keeper.isUpright && theFrameInfo.getTimeSince(theLibCodeRelease.keeper.theBallModel.timeWhenLastSeen) < 500 && (theTeamBallModel.position-theLibCodeRelease.keeper.theRobotPose.translation).norm() < 600 && (theTeamBallModel.position-theLibCodeRelease.keeper.theRobotPose.translation).norm() < theBallModel.estimate.position.norm())
			{
				Stand();
				FollowTheTarget(theBallModel.estimate.position.angle());
			}
			else
			{
				if((theLibCodeRelease.fdefender.theRobotPose.translation - theRobotPose.translation).norm() < 600 && ((theLibCodeRelease.fdefender.theRobotPose.translation.x() > theRobotPose.translation.x() && theLibCodeRelease.gloBallPos.x() > theLibCodeRelease.fdefender.theRobotPose.translation.x()) || (theLibCodeRelease.fdefender.theRobotPose.translation.x() < theRobotPose.translation.x() && theLibCodeRelease.gloBallPos.x() < theLibCodeRelease.fdefender.theRobotPose.translation.x())))
				{
                    FollowTheTarget(theBallModel.estimate.position.angle());
                    Stand();
				}
				else
				{
					WalkToTarget(Pose2f(100.f,100.f,100.f),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f));
					if(std::abs(theBallModel.estimate.position.angle())<15_deg)
					{
						WalkToTarget(Pose2f(100.f,100.f,100.f), theBallModel.estimate.position);
					}
					FollowTheTarget(theBallModel.estimate.position.angle());
				}
			}
		}
	}
	
	state(alignToGoal)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:alignToGoal");
		}
		transition
		{
			if( std::abs(theRobotPose.rotation - theLibCodeRelease.angleToGoal) < 20_deg)
				goto alignBehindBall;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
		}
		action
		{
			if(theLibCodeRelease.angleToGoal >= 0)
				WalkAtAbsoluteSpeed(Pose2f(0.6f, 0.f, -160.f));
			if(theLibCodeRelease.angleToGoal < 0)
				WalkAtAbsoluteSpeed(Pose2f(-0.6f, 0.f, 160.f));
		}
	}
	
		state(alignBehindBall)   //大脚和快踢的对齐动作
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:alignBehindBall");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && std::abs(theRobotPose.rotation - theLibCodeRelease.angleToGoal) >= 20_deg)
				goto alignToGoal;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 185.f, 215.f) && std::abs(theLibCodeRelease.angleToGoal) <= 20_deg)
				goto greatKick;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 185.f, 215.f) && theLibCodeRelease.gloBallPos.x() > theRobotPose.translation.x())
				goto fastKick;
			if(theLibCodeRelease.timeSinceBallWasSeen< 600 && theBallModel.estimate.position.norm() >= 400.f)
				goto walkToBall;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
		}
		action
		{
			if(std::abs(theBallModel.estimate.position.angle()) > 40_deg)
				{
					WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f)); //对齐球
				}
			else
			{
                if(theLibCodeRelease.gloBallPos.x() <= theRobotPose.translation.x() || std::abs(theLibCodeRelease.gloBallPos.y()) > 1500)//球在身后
                {
                    WalkToTarget(Pose2f(50.f,50.f,50.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y() + 30.f));
                }
                if(theLibCodeRelease.gloBallPos.x() > theRobotPose.translation.x() || std::abs(theLibCodeRelease.gloBallPos.y()) <= 1500)//球在身前
                {
                    WalkToTarget(Pose2f(50.f,50.f,50.f), Pose2f(0.f, theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y() + 30.f));
                }
                FollowTheTarget(theBallModel.estimate.position.angle()); 
			
			}
		}
	}
/*    
    state(alignBehindBall2)   //侧踢的对齐动作
    {
        transition
        {
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), 0.f, 30.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 165.f, 195.f) && theLibCodeRelease.gloBallPos.x() > theRobotPose.translation.x())
				goto sideKick_R;
			if(theLibCodeRelease.timeSinceBallWasSeen< 600 && theBallModel.estimate.position.norm() >= 400.f)
				goto walkToBall;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && std::abs(theRobotPose.rotation - theLibCodeRelease.angleToGoal) >= 20_deg)
				goto alignToGoal;
        }
        action
        {
			if(std::abs(theBallModel.estimate.position.angle()) > 40_deg)
				{
					WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f)); //对齐球
				}
			else
			{
                if(theLibCodeRelease.gloBallPos.x() <= theRobotPose.translation.x() || std::abs(theLibCodeRelease.gloBallPos.y()) > 1500)//球在身后
                {
                    WalkToTarget(Pose2f(50.f,50.f,50.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() - 15.f));
                }
                if(theLibCodeRelease.gloBallPos.x() > theRobotPose.translation.x() || std::abs(theLibCodeRelease.gloBallPos.y()) <= 1500)//球在身前
                {
                    WalkToTarget(Pose2f(50.f,50.f,50.f), Pose2f(0.f, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() - 15.f));
                }
                FollowTheTarget(theBallModel.estimate.position.angle()); 
			}
        }
    }
	*/
	state(adjustAngle)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:adjustAngle");
		}
		transition
		{
			if(theLibCodeRelease.fdefender.isPenalized || !theLibCodeRelease.fdefender.isUpright || theFrameInfo.getTimeSince(theLibCodeRelease.fdefender.theBallModel.timeWhenLastSeen) > 600 || theLibCodeRelease.fdefender.theBallModel.estimate.position.norm() >= theBallModel.estimate.position.norm())
				goto walkToBall;
//			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && (theTeamBallModel.position-theLibCodeRelease.fdefender.theRobotPose.translation).norm()< 600)
//				goto doNotMove;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto alignToOwnGoal;
            if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > -1500)
                goto backToFixedRightSpot;
		}
		action
		{
            if(theLibCodeRelease.gloBallPos.y() > 1000 && theLibCodeRelease.gloBallPos.x() < 0 && theLibCodeRelease.gloBallPos.x() > -2000)
            {
                theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.gloBallPos.x() -1000,theLibCodeRelease.gloBallPos.y() - 1000),Pose2f(50.f,50.f,50.f),true);
                FollowTheTarget(theBallModel.estimate.position.angle()); 

//               WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),theTeamBallModel.position.x()-theRobotPose.translation.x()-1000,theTeamBallModel.position.y()-theRobotPose.translation.y()+1000));
//                OUTPUT_TEXT("BallInLeft");
            }
            
            if(theLibCodeRelease.gloBallPos.y() < -1000 && theLibCodeRelease.gloBallPos.x() < 0 && theLibCodeRelease.gloBallPos.x() > -2000)
            {
                theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.gloBallPos.x() -1000,theLibCodeRelease.gloBallPos.y() + 1000),Pose2f(50.f,50.f,50.f),true);
                FollowTheTarget(theBallModel.estimate.position.angle()); 
//                WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),theTeamBallModel.position.x()-theRobotPose.translation.x()-100,theTeamBallModel.position.y()-theRobotPose.translation.y()-100));
//                OUTPUT_TEXT("BallInRight");
            }
            
            
//			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theBallModel.estimate.position.norm() < 600 && (float)std::sqrt(std::pow(theLibCodeRelease.gloBallPos.x()-theLibCodeRelease.fdefender.theRobotPose.translation.x(),2) + std::pow(theLibCodeRelease.gloBallPos.y()-theLibCodeRelease.fdefender.theRobotPose.translation.y(),2)) < 600)
//				{
//					WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(0.f,-600,0.f));
//				}
//			else if(std::abs(theBallModel.estimate.position.angle()) > 15_deg)
//				WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f));
			else if(theLibCodeRelease.between(theLibCodeRelease.gloBallPos.x(),-4500.f,-2000.f) || theLibCodeRelease.between(theLibCodeRelease.gloBallPos.y(),-1000.f,1000.f))
			{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theBallModel.estimate.position.norm() < 600 && (theLibCodeRelease.gloBallPos-theLibCodeRelease.fdefender.theRobotPose.translation).norm() < 600 && theLibCodeRelease.fdefender.theBallModel.estimate.position.norm()<theBallModel.estimate.position.norm())
				{
					WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(0.f,-600,0.f));
				}
             else
                {
                    theMotionRequest = thePathPlanner.plan(Pose2f(0.f,-3000,0),Pose2f(50.f,50.f,50.f),true);//有待调试，确定如何协防
//				Stand();
                    FollowTheTarget(theBallModel.estimate.position.angle());
                }
             }
			}
    }
	
	
	state(greatKick)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:greatKick");
		}
		transition
		{
//			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > 0)
//				goto doNotMove;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
//				goto alignToOwnGoal;
			if(state_time > 2500 && theLibCodeRelease.timeSinceBallWasSeen < 600 && theBallModel.estimate.position.norm() < 1000)
				goto walkToBall;
		}
		action
		{
			Activity(BehaviorStatus::greatKick);
			FollowTheTarget(theBallModel.estimate.position.angle());
			Shoot(KickRequest::greatKickForward);
//            theMotionRequest.walkRequest.walkKickRequest = WalkRequest::WalkKickRequest();
		}
	}
	
	state(fastKick)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:fastKick");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
                goto searchForBall;
			if(state_time > 2500 && theLibCodeRelease.timeSinceBallWasSeen < 600 && theBallModel.estimate.position.norm() < 1000)
				goto walkToBall;
		}
		action
		{
			Activity(BehaviorStatus::greatKick);
			FollowTheTarget(theBallModel.estimate.position.angle());
			Shoot(KickRequest::fastKick);
		}
	}
/*    
    state(sideKick_R)
    {
        if(state_time < 1)
            OUTPUT_TEXT("Bdefender:sideKick_R");
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > 0)
				goto doNotMove;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
			if(state_time > 2000 && theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
        }
        action
        {
            Activity(BehaviorStatus::greatKick);
            FollowTheTarget(theBallModel.estimate.position.angle());
            Shoot(KickRequest::sideKick_R);
        }
    }
    state(sideKick_L)
    {
        if(state_time < 1)
            OUTPUT_TEXT("Bdefender:sideKick_L");
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > 0)
				goto doNotMove;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
			if(state_time > 2000 && theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
        }
        action
        {
            Activity(BehaviorStatus::greatKick);
            FollowTheTarget(theBallModel.estimate.position.angle());
            Shoot(KickRequest::sideKick_L);
        }
    }
	*/
	state(backToFixedRightSpot)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:backToFixedRightSpot");
		}
		transition
		{
			if((theRobotPose.translation-theLibCodeRelease.BRightPoint).norm() < 100 && std::abs(theRobotPose.rotation) < 10_deg)
				goto fixedRightSpot;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < -1500)
				goto walkToBall;
		}
		action
		{
			theMotionRequest = thePathPlanner.plan(Pose2f(0.f,-3500.f,-450.f),Pose2f(50.f,50.f,50.f),false);
			HeadControlMode(HeadControl::lookLeftAndRight);
		}
	}
	
	state(backToFixedLeftSpot)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:backToFixedLeftSpot");
		}
		transition
		{
			if((theRobotPose.translation-theLibCodeRelease.BLeftPoint).norm() < 100 && std::abs(theRobotPose.rotation) < 10_deg)
				goto fixedLeftSpot;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < -1500)
				goto walkToBall;
		}
		action
		{
			theMotionRequest = thePathPlanner.plan(Pose2f(0.f,-3500.f,450.f),Pose2f(50.f,50.f,50.f),false);
			HeadControlMode(HeadControl::lookLeftAndRight);
		}
	}
	
	state(alignToOwnGoal)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:alignToOwnGoal");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
			if(std::abs(theLibCodeRelease.angleToOwnGoal) < 15_deg)
			{
				if(theRobotPose.translation.y()>0 || theLibCodeRelease.gloBallPos.x() > 0)
					goto backToFixedLeftSpot;
				else
					goto backToFixedRightSpot;
			}
		}
		action
		{
            WalkToTarget(Pose2f(100.f,100.f,100.f),Pose2f(theLibCodeRelease.angleToOwnGoal, 0.f,0.f));
		}
	}
    


            
    state(doNotMove)
    {
        transition
        {
            if((theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0 )|| theBallModel.estimate.position.norm() < 1000)
                goto walkToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen > 600)
                goto searchForBall;
        }
        action
        {
            OUTPUT_TEXT("doNotMove");
            Stand();
            FollowTheTarget(theBallModel.estimate.position.angle());            
        }
    }
//    state(doNotMove)
//    {
//        if(state_time < 1)
//            OUTPUT_TEXT("B:doNotMove");
//        transition
//        {    
//            if(theFrameInfo.getTimeSince(theLibCodeRelease.fdefender.timeWhenLastPacketReceived) < 2000)
//            {
//                if((theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < -1500 && theLibCodeRelease.fdefender.theBallModel.estimate.position.norm() > 600) || (theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theLibCodeRelease.gloBallPos.x(),-1500.f,0.f) && theLibCodeRelease.fdefender.theBallModel.estimate.position.norm() > theBallModel.estimate.position.norm()) || (theLibCodeRelease.timeSinceBallWasSeen < 600 && theBallModel.estimate.position.norm() < 1000))
//                {
//                    if(theFrameInfo.getTimeSince(theLibCodeRelease.striker1.timeWhenLastPacketReceived) < 2000)
//                    {
//                        if(theLibCodeRelease.striker1.theBallModel.estimate.position.norm() > 600 && !theLibCodeRelease.striker1.isPenalized)
//                            goto walkToBall;
//                    }
//                    if(theFrameInfo.getTimeSince(theLibCodeRelease.striker2.timeWhenLastPacketReceived) < 2000)
//                    {
//                        if(theLibCodeRelease.striker2.theBallModel.estimate.position.norm() > 600 && theLibCodeRelease.striker2.status == Teammate::PLAYING)
//                            goto walkToBall;
//                    }                    
//                }
//            }
//
//            if(theFrameInfo.getTimeSince(theLibCodeRelease.striker2.timeWhenLastPacketReceived) < 2000 && theFrameInfo.getTimeSince(theLibCodeRelease.fdefender.timeWhenLastPacketReceived) < 2000 && theFrameInfo.getTimeSince(theLibCodeRelease.striker1.timeWhenLastPacketReceived) < 2000)
//            if(theLibCodeRelease.timeSinceBallWasSeen > 600)
//            {
//                if(theFrameInfo.getTimeSince(theLibCodeRelease.fdefender.timeWhenLastPacketReceived) < 2000 && (float)theLibCodeRelease.fdefender.theBallModel.timeWhenDisappeared < theBehaviorParameters.ballNotSeenTimeOut)
//                    goto knowTeammateBall;
//                else
//                    goto searchForBall;
//            }
//            
//        }
//        action
//        {
//            Stand();
//            FollowTheTarget(theBallModel.estimate.position.angle());
//        }
//    }
    
    
	state(searchForBall)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:searchForBall");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > -1500 && theBallModel.estimate.position.norm() > theLibCodeRelease.fdefender.theBallModel.estimate.position.norm() && theFrameInfo.getTimeSince(theLibCodeRelease.fdefender.timeWhenLastPacketReceived))
			{
                goto doNotMove;
			}
            if(theLibCodeRelease.timeSinceBallWasSeen > 600 && theFrameInfo.getTimeSince(theLibCodeRelease.striker1.theBallModel.timeWhenLastSeen) < 600 && theFrameInfo.getTimeSince(theLibCodeRelease.striker1.timeWhenLastPacketReceived) < 2000)
                goto knowTeammateBallStriker1;
            if(theLibCodeRelease.timeSinceBallWasSeen > 600 && (float)theFrameInfo.getTimeSince(theLibCodeRelease.striker2.theBallModel.timeWhenLastSeen) < 600 && theFrameInfo.getTimeSince(theLibCodeRelease.striker2.timeWhenLastPacketReceived) < 2000)
                goto knowTeammateBallStriker2;
			if((theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < -1500) || (theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theLibCodeRelease.gloBallPos.x(),-1500.f,0.f) && theLibCodeRelease.fdefender.theBallModel.estimate.position.norm() > theBallModel.estimate.position.norm()))
//                goto KickToPoint;
                goto walkToBall;
			if((state_time > 10000 && (theRobotPose.translation-theLibCodeRelease.BRightPoint).norm()> 300 && (theRobotPose.translation-theLibCodeRelease.BLeftPoint).norm() > 300) ||std::abs(theRobotPose.rotation)>20_deg || (theLibCodeRelease.timeSinceBallWasSeen < 600 && theBallModel.estimate.position.norm() > 1000 && theRobotPose.translation.x() > 0))
			{
				if((theRobotPose.translation-theLibCodeRelease.BRightPoint).norm() < (theRobotPose.translation-theLibCodeRelease.BLeftPoint).norm())
					goto backToFixedRightSpot;
				else
					goto backToFixedLeftSpot;
			}
//            if(std::abs(theRobotPose.rotation)>5_deg)
//            {
//                if((theRobotPose.translation-theLibCodeRelease.BRightPoint).norm() < 300)
//                    goto backToFixedRightSpot;
//                else((theRobotPose.translation-theLibCodeRelease.BLeftPoint).norm() < 300)
//                    goto backToFixedLeftSpot;
//            }
//			if(turn > 1)       //左右换位
//			{
//                turn = 0;
//				if(theRobotPose.translation.y()>0)
//					goto backToFixedLeftSpot;
//				else
//					goto backToFixedRightSpot;
//			}
            if((theLibCodeRelease.timeSinceBallWasSeen-30000)%30000 < 500 &&(theLibCodeRelease.timeSinceBallWasSeen-30000)%30000 > 0)
            {
                goto turnAround;
            }
		}
		action
		{
            OUTPUT_TEXT(theFrameInfo.getTimeSince(theLibCodeRelease.striker1.theBallModel.timeWhenLastSeen));
//            OUTPUT_TEXT(theFrameInfo.getTimeSince(theLibCodeRelease.striker1.timeWhenLastPacketReceived));
			if(theLibCodeRelease.timeSinceBallWasSeen >= 600)
			{
				HeadControlMode(HeadControl::lookLeftAndRight);
                Stand();
			
//                if(theLibCodeRelease.timeSinceBallWasSeen%20000 < 6500)
//                {
//                    HeadControlMode(HeadControl::lookLeftAndRight);
//                    WalkAtRelativeSpeed(Pose2f(1.f,0.f,0.f));
//                }
            }
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() >= 0)
			{
				Stand();
				FollowTheTarget(theBallModel.estimate.position.angle());
			}
		}
	}
    
    state(turnAround)
    {
        if(state_time < 1)
            OUTPUT_TEXT("turnAround");
        transition
        {
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > 0)
			{
                Stand();
                FollowTheTarget(theBallModel.estimate.position.angle());
			}
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
					goto walkToBall;
            if(state_time > 5000)
            {
              
                if(std::abs(theRobotPose.rotation)<10_deg)
                    goto searchForBall;
            }
        }
        action
        {
            HeadControlMode(HeadControl::lookLeftAndRight);
            WalkAtRelativeSpeed(Pose2f(0.8f,0.f,0.f));            
        }
    }

    
/*    state(KickToPoint)
    {
        if(state_time < 1)
            OUTPUT_TEXT("KickToPoint");
        transition
        {
            if( theBallModel.estimate.position.norm() < 300)
                goto alignToPoint;
        }
        action
        {
            FollowTheTarget(theBallModel.estimate.position.angle());
            theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.gloBallPos.x() -180,theLibCodeRelease.gloBallPos.y() + 30),Pose2f(50.f,50.f,50.f),true);

//            if(std::abs(theBallModel.estimate.position.angle()) > 10_deg)
//                WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f)); //对齐球
//            else
            if(theBallModel.estimate.position.norm() < 1000)
                WalkToTarget(Pose2f(50.f,50.f,50.f), Pose2f(0.f, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() + 30.f));
        }
    }*/


    state(alignToPoint)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:alignToPoint");
		}
		transition
		{
			if( std::abs(theLibCodeRelease.pointOneToRobot.angle()) < 10_deg)
				goto alignBehindBallToPoint;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
		}
		action
		{
			if(theLibCodeRelease.pointOneToRobot.angle() >= 0)
				WalkAtAbsoluteSpeed(Pose2f(0.6f, 0.f, -140.f));
			if(theLibCodeRelease.pointOneToRobot.angle() < 0)
				WalkAtAbsoluteSpeed(Pose2f(-0.6f, 0.f, 140.f));
		}
	}
        state(alignBehindBallToPoint)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("BDefender:alignBehindBallToPoint");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.pointOneToRobot.angle() >= 5_deg)
				goto alignToPoint;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 165.f, 195.f) && std::abs(theLibCodeRelease.pointOneToRobot.angle()) <= 5_deg)
				goto greatKick;
			if(theLibCodeRelease.timeSinceBallWasSeen< 600 && theBallModel.estimate.position.norm() >= 400.f)
                goto walkToBall2;
//				goto KickToPoint;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
		}
		action
		{
//            OUTPUT_TEXT(theLibCodeRelease.pointOneToRobot.angle());
			if(std::abs(theBallModel.estimate.position.angle()) > 10_deg)
				{
					WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f)); //对齐球
				}
            if(std::abs(theLibCodeRelease.pointOneToRobot.angle()) > 5_deg)
            {
                if(theLibCodeRelease.pointOneToRobot.angle() >= 0)
                    WalkAtAbsoluteSpeed(Pose2f(0.6f, 0.f, -140.f));
                if(theLibCodeRelease.pointOneToRobot.angle() < 0)
                    WalkAtAbsoluteSpeed(Pose2f(-0.6f, 0.f, 140.f));
            }
			else
			{

                    WalkToTarget(Pose2f(50.f,50.f,50.f), Pose2f(0.f, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() + 30.f));
                
                    FollowTheTarget(theBallModel.estimate.position.angle()); 
			
			}
		}
	}
}



