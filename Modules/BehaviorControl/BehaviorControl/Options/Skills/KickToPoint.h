option(KickToPoint)
{
    state
    {
                if(std::abs(theBallModel.estimate.position.angle()) > 40_deg)
            WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theLibCodeRelease.BRightPointTest.angle(),0.f,0.f)); //对齐球
        else
            WalkToTarget(Pose2f(50.f,50.f,50.f),Pose2f(theLibCodeRelease.BRightPointTest.angle(),pointX,pointY));
    }
    action
    {
            
        goto greatKick;
        
    }
}