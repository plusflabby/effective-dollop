/*
Store session information securely, avoiding client-side storage of sensitive data. Use server-side storage mechanisms (e.g., databases, in-memory caches) to store session data.
*/

ref Session_Cache SESSION_CACHE = new ref Session_Cache();

// in-memory cache
class Session_Cache
{
	// Define a structure to hold session data
	private ref map<string, Session_Structure> sessions = new map<string,Session_Structure>();
	
	// Function to add a session to the cache
	void AddSession(string sessionId, Session_Structure data)
	{
		sessions.Insert(sessionId, data);
		//Print("Session added successfully");
	}
	
	// Function to retrieve session data based on session ID
	Session_Structure GetSessionData(string sessionId, bool updateSessionAccessTime = true)
	{
		if (updateSessionAccessTime)
			updateSessionAccessTime(sessionId);
		if (sessions.Contains(sessionId))
		{
	        Print("Retrieved session data: " + sessions.Get(sessionId));
	        return sessions.Get(sessionId);
	    }
		else
		{
	        Print("Session not found");
	        return Session_Structure_EMPTY;
	    }
	}
	
	// Function to revoke or delete a session based on session ID
	void revokeSession(string sessionId)
	{
		if (sessions.Contains(sessionId))
		{
			sessions.Remove(sessionId);
			Print("Session revoked successfully");
		}
		else
			Print("Session not found");
	}
	
	// Function to check session expiry based on inactivity period
	void checkSessionExpiry()
	{
		string currentTime = SCR_DateTimeHelper.GetDateTimeLocal();
		int inactivityPeriod = 1860; //seconds Define the inactivity period
		
		if (sessions.Count() > 0)
		{
			int sessions_count = sessions.Count();
			for (int i = 0; i < sessions_count; i++)
			{
				string sessionId = sessions.GetKey(i);
				Session_Structure session = GetSessionData(sessionId, false);
				
				array<string> one;
				currentTime.Split(" ", one, false);
				string currentTime_time = one.Get(1);
				array<string> currentTime_time_array;
				currentTime_time.Split(":", currentTime_time_array, false);
				
				string session_lastAccessed = session.lastAccessed;
				session_lastAccessed.Split(" ", one, false);
				string session_lastAccessed_time = one.Get(1);
				array<string> session_lastAccessed_time_array;
				session_lastAccessed_time.Split(":", session_lastAccessed_time_array, false);
				
				int timeSinceLastAccess = SCR_DateTimeHelper.GetTimeDifference(currentTime_time.Get(0).ToInt(), currentTime_time.Get(1).ToInt(), currentTime_time.Get(2).ToInt(), session_lastAccessed_time.Get(0).ToInt(), session_lastAccessed_time.Get(1).ToInt(), session_lastAccessed_time.Get(2).ToInt());
				if (timeSinceLastAccess > inactivityPeriod)
					revokeSession(sessionId);
			}
		}
	}
	
	// Function to update last accessed time for a session
	void updateSessionAccessTime(string sessionId)
	{
		if (sessions.Contains(sessionId))
		{
			Session_Structure session_value = GetSessionData(sessionId);
			session_value.lastAccessed = SCR_DateTimeHelper.GetDateTimeLocal();
			sessions.Set(sessionId, session_value);
		}
		else
			Print("Session not found");
	}
	
}

// Define a structure to hold session data including last accessed time
class Session_Structure
{
	string sessionId;
	string data;
	string lastAccessed;
}
ref Session_Structure Session_Structure_EMPTY = new ref Session_Structure();