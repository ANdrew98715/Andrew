option(FollowTheTarget, (float) pan)
{
  /** Simply sets the necessary angles */
  initial_state(lookForward)
  {
    action
    { 
      SetHeadPanTilt(pan, 0.38f, 150_deg);
    }
  }
}