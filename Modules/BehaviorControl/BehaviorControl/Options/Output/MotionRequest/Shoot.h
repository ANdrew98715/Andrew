option(Shoot, ((KickRequest) KickRequest::KickMotionID) kickMotionType)
    {
      
      initial_state(launch)
      {
        transition
        {
          if(theMotionInfo.motion == MotionRequest::kick && theMotionInfo.kickRequest.kickMotionType == kickMotionType)
            goto execute;
        }
        action
        {
          theMotionRequest.motion =  MotionRequest::kick;
    	  theMotionRequest.kickRequest.armsBackFix = true;
          theMotionRequest.kickRequest.kickMotionType =kickMotionType;
    	  
        }
      }
      state(execute)
      {
        transition
        {
          if(theMotionInfo.kickRequest.kickMotionType == KickRequest::none)
            goto finished;
        }
        action
        {
          theMotionRequest.motion =  MotionRequest::kick;
    	  theMotionRequest.kickRequest.armsBackFix = true;
          theMotionRequest.kickRequest.kickMotionType = KickRequest::none;
        }
      }
      /** The kick has been executed */
      target_state(finished)
      {
        action
        {
    	  theMotionRequest.motion =  MotionRequest::kick;
    	  theMotionRequest.kickRequest.armsBackFix = true;
          theMotionRequest.kickRequest.kickMotionType = KickRequest::none;
        }
      }
    }
