# follow-bot

This is based on [this instructable](http://www.instructables.com/id/Its-Alive-A-little-robot-that-follows-you-with-its/).  The build is the same, but the code is different.

Instead of constantly moving, trying to find the closest object, the bot will stay put if the distance detected doesn't exceed some tolerance level.  The level is currently set to 25% of the last reading.  If the distance exceeds the tolerance, it will go ahead and look left and right.  If one of those results in a closer object, it will either go back (in the case of "left") or stay right.  If neither is closer than the "current" reading, then it will move back to that position.  Mostly I just tried to eliminate extra movement here.

Play around with it and see what works for you.

