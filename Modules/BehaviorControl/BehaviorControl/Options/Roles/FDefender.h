option(FDefender)
{
	static bool     flag = false;
	static unsigned int    time = 0;//定义全局变量-比赛时间
	
	common_transition
	{
	  if(theArmContactModel.status[Arms::left].contact == true || theArmContactModel.status[Arms::right].contact == true)
	  {
		  flag = true;
		  time = theFrameInfo.time; //theFrameInfo.time为全局的时间，从机器人开始运行代码从0开始，正常的话半场十分钟总共600000毫秒。
	  }
	  if(flag == true)
	  {
		  theArmMotionRequest.armMotion[Arms::left] = theArmMotionRequest.armMotion[Arms::right] = ArmMotionRequest::keyFrame;
		  theArmMotionRequest.armKeyFrameRequest.arms[Arms::left].motion = theArmMotionRequest.armKeyFrameRequest.arms[Arms::right].motion = ArmKeyFrameRequest::back;
	  }
	  if((theFrameInfo.time - time) > 5000)//从接触障碍手背后持续五秒，可加特殊条件。
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
			OUTPUT_TEXT("FDefender:start");
		}
		transition
		{
			if(state_time > 10000)
				goto readyok;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
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
			OUTPUT_TEXT("FDefender:readyok");
		}
		transition
		{
			if((std::abs(theRobotPose.rotation) < 10_deg && std::abs(theRobotPose.translation.x() + 2500) < 100 && std::abs(theRobotPose.translation.y() + 450) < 100))
				goto fixedRightSpot;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
		}
		action
		{
			HeadControlMode(HeadControl::lookLeftAndRight);
			WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(-theRobotPose.rotation,(-theRobotPose.translation.x() - 2500),(-450 - theRobotPose.translation.y())));
		}
	}
	
	state(fixedRightSpot)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:fixedRightSpot");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
		}
		action
		{
			if(std::abs(theRobotPose.rotation) > 10_deg)
			{
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(-theRobotPose.rotation, 0.f,0.f));
			}
			if(std::abs(theRobotPose.rotation) <= 10_deg)
			{
				Stand();
				if(theLibCodeRelease.timeSinceBallWasSeen < 600 )
					FollowTheTarget(theBallModel.estimate.position.angle());
				else
					HeadControlMode(HeadControl::lookLeftAndRight);
			}
		}
	}
	
	state(fixedLeftSpot)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:fixedLeftSpot");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
		}
		action
		{
			if(std::abs(theRobotPose.rotation) > 10_deg)
			{
				WalkToTarget(Pose2f(80.f,80.f,80.f),Pose2f(-theRobotPose.rotation, 0.f,0.f));
			}
			if(std::abs(theRobotPose.rotation) <= 10_deg)
			{
				Stand();
				if(theLibCodeRelease.timeSinceBallWasSeen < 600 )
					FollowTheTarget(theBallModel.estimate.position.angle());
				else
					HeadControlMode(HeadControl::lookLeftAndRight);
			}
		}
	}
	
	state(walkToBall)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:walkToBall");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && std::abs(theLibCodeRelease.gloBallPos.y()) < 1500 && theLibCodeRelease.gloBallPos.x() < -3700)
				goto walkToBall2;
			if(!theLibCodeRelease.bdefender.isPenalized && theLibCodeRelease.bdefender.isUpright && theFrameInfo.getTimeSince(theLibCodeRelease.bdefender.theBallModel.timeWhenLastSeen) < 500 && (theLibCodeRelease.gloBallPos-theLibCodeRelease.bdefender.theRobotPose.translation).norm() < 600 && (theLibCodeRelease.gloBallPos-theLibCodeRelease.bdefender.theRobotPose.translation).norm() < theBallModel.estimate.position.norm())
				goto adjustAngle;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 165.f, 195.f) && theLibCodeRelease.gloBallPos.x() < -1500 && std::abs(theRobotPose.rotation) < 45_deg)
				goto fastKick;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 165.f, 195.f) && theLibCodeRelease.gloBallPos.x() >= -1500 && std::abs(theLibCodeRelease.angleToGoal) < 20_deg)
				goto greatKick;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > 0)
				goto alignToOwnGoal;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
		}
		action
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < -1500)
				{
					theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.gloBallPos.x() -180,theLibCodeRelease.gloBallPos.y() + 30),Pose2f(50.f,50.f,50.f),true);
					FollowTheTarget(theBallModel.estimate.position.angle());
				}
			else
			{
				theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.gloBallPos.x() -180,theLibCodeRelease.gloBallPos.y() + 30),Pose2f(50.f,50.f,50.f),true);
				FollowTheTarget(theBallModel.estimate.position.angle());
				if(theLibCodeRelease.timeSinceBallWasSeen < 300 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 165.f, 195.f))
				{
					if(std::abs(theLibCodeRelease.angleToGoal) >= 20_deg)
					{
						OUTPUT_TEXT("FDefender:alignToGoal");
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
			FollowTheTarget(theBallModel.estimate.position.angle());
		}
	}
	
	state(walkToBall2)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:walkToBall2");
		}
		transition
		{
			if(!theLibCodeRelease.bdefender.isPenalized  && theFrameInfo.getTimeSince(theLibCodeRelease.bdefender.theBallModel.timeWhenLastSeen) < 500 && (theTeamBallModel.position-theLibCodeRelease.bdefender.theRobotPose.translation).norm() < 600)
				goto adjustAngle;
			if(theBallModel.estimate.position.norm() < 400.f)
				goto alignToGoal;
			if(theBallModel.estimate.position.norm() < 400.f && (theLibCodeRelease.gloBallPos.x() > theRobotPose.translation.x() || std::abs(theLibCodeRelease.gloBallPos.y()) <= 1500))
				goto alignBehindBall;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > 0)
				goto alignToOwnGoal;
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
				if((theLibCodeRelease.bdefender.theRobotPose.translation - theRobotPose.translation).norm() < 600 && ((theLibCodeRelease.bdefender.theRobotPose.translation.x() > theRobotPose.translation.x() && theLibCodeRelease.gloBallPos.x() > theLibCodeRelease.bdefender.theRobotPose.translation.x()) || (theLibCodeRelease.bdefender.theRobotPose.translation.x() < theRobotPose.translation.x() && theLibCodeRelease.gloBallPos.x() < theLibCodeRelease.bdefender.theRobotPose.translation.x())))
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
			OUTPUT_TEXT("FDefender:alignToGoal");
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
	
		state(alignBehindBall)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:alignBehindBall");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && std::abs(theRobotPose.rotation - theLibCodeRelease.angleToGoal) >= 20_deg)
				goto alignToGoal;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 165.f, 195.f) && std::abs(theLibCodeRelease.angleToGoal) <= 20_deg)
				goto greatKick;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 165.f, 195.f) && theLibCodeRelease.gloBallPos.x() > theRobotPose.translation.x())
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
					WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f));
				}
			else
			{
			if(theLibCodeRelease.gloBallPos.x() <= theRobotPose.translation.x() || std::abs(theLibCodeRelease.gloBallPos.y()) > 1500)
			{
				WalkToTarget(Pose2f(50.f,50.f,50.f), Pose2f(theLibCodeRelease.angleToGoal, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() + 30.f));
			}
			if(theLibCodeRelease.gloBallPos.x() > theRobotPose.translation.x() || std::abs(theLibCodeRelease.gloBallPos.y()) <= 1500)
			{
				WalkToTarget(Pose2f(50.f,50.f,50.f), Pose2f(0.f, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() + 30.f));
			}
			FollowTheTarget(theBallModel.estimate.position.angle()); 
			
			}
		}
	}
	
	state(adjustAngle)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:adjustAngle");
		}
		transition
		{
			if(theLibCodeRelease.bdefender.isPenalized || !theLibCodeRelease.bdefender.isUpright || theFrameInfo.getTimeSince(theLibCodeRelease.bdefender.theBallModel.timeWhenLastSeen) >= 500 || (theLibCodeRelease.gloBallPos-theLibCodeRelease.bdefender.theRobotPose.translation).norm() > 600 || (theLibCodeRelease.gloBallPos-theLibCodeRelease.bdefender.theRobotPose.translation).norm() >= theBallModel.estimate.position.norm())
				goto walkToBall;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() >0)
				goto doNotMove;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto alignToOwnGoal;
		}
		action
		{
            if(theLibCodeRelease.gloBallPos.y() > 1000 && theLibCodeRelease.gloBallPos.x() < 0 && theLibCodeRelease.gloBallPos.x() > -2000)
            {
                theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.gloBallPos.x() -1000,theLibCodeRelease.gloBallPos.y() - 1000),Pose2f(50.f,50.f,50.f),true);
                FollowTheTarget(theBallModel.estimate.position.angle());

//                OUTPUT_TEXT("BallInLeft-f");
            }
            
            if(theLibCodeRelease.gloBallPos.y() < -1000 && theLibCodeRelease.gloBallPos.x() < 0 && theLibCodeRelease.gloBallPos.x() > -2000)
            {
                theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.gloBallPos.x() -1000,theLibCodeRelease.gloBallPos.y() + 1000),Pose2f(50.f,50.f,50.f),true);
                FollowTheTarget(theBallModel.estimate.position.angle()); 
//                OUTPUT_TEXT("BallInRight-f");
            }
			else if(theLibCodeRelease.between(theLibCodeRelease.gloBallPos.x(),-4500.f,-2000.f) || theLibCodeRelease.between(theLibCodeRelease.gloBallPos.y(),-1000.f,1000.f))
			{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theBallModel.estimate.position.norm() < 600 && (theLibCodeRelease.gloBallPos-theLibCodeRelease.bdefender.theRobotPose.translation).norm() < 600 &&theLibCodeRelease.bdefender.theBallModel.estimate.position.norm()<theBallModel.estimate.position.norm())
				{
					WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(0.f,-600,0.f));
				}
             else
             {
				Stand();
				FollowTheTarget(theBallModel.estimate.position.angle());
             }
			}            
//			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theBallModel.estimate.position.norm() < 600 && (float)std::sqrt(std::pow(theLibCodeRelease.gloBallPos.x()-theLibCodeRelease.bdefender.theRobotPose.translation.x(),2) + std::pow(theLibCodeRelease.gloBallPos.y()-theLibCodeRelease.bdefender.theRobotPose.translation.y(),2)) < 600)
//				{
//					WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(0.f,-600,0.f));//speed and 机器人坐标的位置（角度，X坐标，Y坐标）
//				}
//			else if(std::abs(theBallModel.estimate.position.angle()) > 15_deg)
//				WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f));
//			else
//			{
//				Stand();
//				FollowTheTarget(theBallModel.estimate.position.angle());
//			}
		}
	}
			
	state(greatKick)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:greatKick");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > 0)
				goto alignToOwnGoal;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto alignToOwnGoal;
			if(state_time > 2500 && theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
		}
		action
		{
			FollowTheTarget(theBallModel.estimate.position.angle());
			Shoot(KickRequest::greatKickForward);
		}
	}
	
	state(fastKick)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:fastKick");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > 0)
				goto alignToOwnGoal;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
			if(state_time > 2000 && theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
		}
		action
		{
			FollowTheTarget(theBallModel.estimate.position.angle());
			Shoot(KickRequest::fastKick);
		}	
	}
	
	state(backToFixedRightSpot)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:backToFixedRightSpot");
		}
		transition
		{
			if(std::abs(theRobotPose.translation.x() + 2500) < 100 && std::abs(theRobotPose.translation.y() + 450) < 100 && std::abs(theRobotPose.rotation) < 10_deg)
				goto fixedRightSpot;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
		}
		action
		{
			theMotionRequest = thePathPlanner.plan(Pose2f(0.f,-2500.f,-450.f),Pose2f(50.f,50.f,50.f),false);
			HeadControlMode(HeadControl::lookLeftAndRight);
		}
	}
	
	state(backToFixedLeftSpot)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:backToFixedLeftSpot");
		}
		transition
		{
			if(std::abs(theRobotPose.translation.x() + 2500) < 100 && std::abs(theRobotPose.translation.y() - 450) < 100 && std::abs(theRobotPose.rotation) < 10_deg)
				goto fixedLeftSpot;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
		}
		action
		{
			theMotionRequest = thePathPlanner.plan(Pose2f(0.f,-2500.f,450.f),Pose2f(50.f,50.f,50.f),false);
			HeadControlMode(HeadControl::lookLeftAndRight);
		}
	}
	
	state(alignToOwnGoal)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:alignToOwnGoal");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
				goto walkToBall;
			if(std::abs(theLibCodeRelease.angleToOwnGoal) < 15_deg)
			{
				if(theRobotPose.translation.y() < 0 || theLibCodeRelease.gloBallPos.x() > 0)
					goto backToFixedRightSpot;
				else
					goto backToFixedLeftSpot;
			}
		}
		action
		{
			WalkToTarget(Pose2f(100.f,100.f,100.f),Pose2f(theLibCodeRelease.angleToOwnGoal, 0.f,0.f));
		}
	}


    state(doNotMove)
    {
        if(state_time < 1)
            OUTPUT_TEXT("F:doNotMove");
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < 0)
                goto walkToBall;
            if(theLibCodeRelease.timeSinceBallWasSeen > 600)
                goto searchForBall;
        }
        action
        {
            Stand();
            FollowTheTarget(theBallModel.estimate.position.angle());            
        }
    }

    state(knowTeammateBall)
    {
        if(state_time < 1)
            OUTPUT_TEXT("F:knowTeammateBall");
        transition
        {
            if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theBallModel.estimate.position.norm() < theLibCodeRelease.bdefender.theBallModel.estimate.position.norm())
                goto walkToBall;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600)
			{
                goto searchForBall;
			}
        }
        action
        {
            Stand();
            FollowTheTarget(theLibCodeRelease.ballPos.angle()); //考虑是否使用看到球的机器人的球坐标，而不是通信球模型
        }
    }

	state(searchForBall)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:searchForBall");
		}
		transition
		{
			if((theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() > -1500 && theBallModel.estimate.position.norm() > theLibCodeRelease.bdefender.theBallModel.estimate.position.norm()))
			{
                goto doNotMove;
			}
            if(theLibCodeRelease.timeSinceBallWasSeen > 600 && (float)theLibCodeRelease.bdefender.theBallModel.timeWhenDisappeared < theBehaviorParameters.ballNotSeenTimeOut && theFrameInfo.getTimeSince(theLibCodeRelease.bdefender.timeWhenLastPacketReceived) < 2000)
                goto knowTeammateBall;
			if((theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.gloBallPos.x() < -1500) || (theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theLibCodeRelease.gloBallPos.x(),-1500.f,0.f) && theLibCodeRelease.bdefender.theBallModel.estimate.position.norm() > theBallModel.estimate.position.norm()))
                goto walkToBall;
			if((state_time > 10000 && (theRobotPose.translation-theLibCodeRelease.FRightPoint).norm()> 300 && (theRobotPose.translation-theLibCodeRelease.FLeftPoint).norm() > 300) ||std::abs(theRobotPose.rotation)>20_deg)
			{
				if((theRobotPose.translation-theLibCodeRelease.FRightPoint).norm() < (theRobotPose.translation-theLibCodeRelease.BLeftPoint).norm())
					goto backToFixedRightSpot;
				else
					goto backToFixedLeftSpot;
			}
            if((theLibCodeRelease.timeSinceBallWasSeen-30000)%30000 < 500 &&(theLibCodeRelease.timeSinceBallWasSeen-30000)%30000 > 0)
            {
                goto turnAround;
            }
		}
		action
		{
			if(theLibCodeRelease.timeSinceBallWasSeen >= 600)
			{
				HeadControlMode(HeadControl::lookLeftAndRight);
                Stand();
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
            OUTPUT_TEXT("F:turnAround");
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
            WalkAtRelativeSpeed(Pose2f(0.7f,0.f,0.f));            
        }
    }

    
    state(KickToPoint)
    {
        transition
        {
            if( theBallModel.estimate.position.norm() < 400)
                goto alignToPoint;
        }
        action
        {
            FollowTheTarget(theBallModel.estimate.position.angle());

            if(std::abs(theBallModel.estimate.position.angle()) > 20_deg)
                WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f)); //对齐球
            else
                WalkToTarget(Pose2f(50.f,50.f,50.f), Pose2f(0.f, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() + 30.f));

            OUTPUT_TEXT("FKickToPoint");                
        }
    }
    	state(alignToPoint)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:alignToPoint");
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
				WalkAtAbsoluteSpeed(Pose2f(0.3f, 0.f, -160.f));
			if(theLibCodeRelease.pointOneToRobot.angle() < 0)
				WalkAtAbsoluteSpeed(Pose2f(-0.3f, 0.f, 160.f));
		}
	}
        state(alignBehindBallToPoint)
	{
		if(state_time < 1)
		{
			OUTPUT_TEXT("FDefender:alignBehindBallToPoint");
		}
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.pointOneToRobot.angle() >= 5_deg)
				goto alignToPoint;
			if(theLibCodeRelease.timeSinceBallWasSeen < 600 && theLibCodeRelease.between(theBallModel.estimate.position.y(), -45.f, -15.f) && theLibCodeRelease.between(theBallModel.estimate.position.x(), 165.f, 195.f) && std::abs(theLibCodeRelease.pointOneToRobot.angle()) <= 5_deg)
				goto greatKick;
			if(theLibCodeRelease.timeSinceBallWasSeen< 600 && theBallModel.estimate.position.norm() >= 400.f)
				goto KickToPoint;
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto searchForBall;
		}
		action
		{
            OUTPUT_TEXT(theLibCodeRelease.pointOneToRobot.angle());
			if(std::abs(theBallModel.estimate.position.angle()) > 10_deg)
				{
					WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theBallModel.estimate.position.angle(),0.f,0.f)); //对齐球
				}
            if(std::abs(theLibCodeRelease.pointOneToRobot.angle()) > 5_deg)
            {
                if(theLibCodeRelease.pointOneToRobot.angle() >= 0)
                    WalkAtAbsoluteSpeed(Pose2f(0.3f, 0.f, -160.f));
                if(theLibCodeRelease.pointOneToRobot.angle() < 0)
                    WalkAtAbsoluteSpeed(Pose2f(-0.3f, 0.f, 160.f));
            }
			else
			{

                    WalkToTarget(Pose2f(50.f,50.f,50.f), Pose2f(0.f, theBallModel.estimate.position.x() - 180.f, theBallModel.estimate.position.y() + 30.f));
                
                    FollowTheTarget(theBallModel.estimate.position.angle()); 
			
			}
		}
	}
	
}