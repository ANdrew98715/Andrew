option(PlayingState)
{
  common_transition
  {
      if(theGameInfo.setPlay == 1 && theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
      {
          goto goalFreeKick;
      }
  }
  state(goalFreeKick)
  {
      transition
      {
          if(state_time > 30000)
              goto demo;
      }
      action
      {
          GoalFreeKick();
      }
  }
  initial_state(demo)
  {
    action
    {
		
		if(theRobotInfo.number == 1)
			{
				Keeper();
			}
		if(theRobotInfo.number == 2)
			{
				Striker();
			}
		if(theRobotInfo.number == 3)
			{
				FDefender();
			}
		if(theRobotInfo.number == 4)
			{
				BDefender();
			}
			
		if(theRobotInfo.number == 5)
			{
				Striker();
			}
        
        if(theRobotInfo.number == 6)
            {
                Striker();
            }
	}
  }
}
