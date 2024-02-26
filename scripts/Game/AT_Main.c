class AT_MainStatic
{
	protected static string currentMenu;
	
	static void setCurrentMenu(string name)
	{
		currentMenu = name;
	}
	
	static string getUid(int playerId)
	{
		string playerBiUid = GetGame().GetBackendApi().GetPlayerIdentityId(playerId);
		if (!playerBiUid)
		{
			//if (RplSession.Mode() != RplMode.Dedicated)
				playerBiUid = string.Format("bbbbdddd-0000-0000-0000-%1", playerId.ToString(12)); // Peer tool support
			//else
			//	Debug.Error("Dedicated server is not correctly configured to connect to the BI backend.");
		}
		
		return playerBiUid;
	}
	
	static string stringArayToString(array<string> arr)
	{
		string rtn = string.Empty;
		foreach (string item : arr)
		{
			rtn = rtn + string.Format("%1,", item);
		}
		return rtn;
	}
	
	static array<string> stringToArray(string str)
	{
		array<string> strs = new array<string>();
		str.Split(",", strs, true);
		return strs;
	}
	
	// server fnc 
	static bool verifySession(string sessionId)
	{
		#ifdef WORKBENCH
		return true;
		#endif
		
		if (sessionId.IsEmpty())
			return false;
		if (AT_GLOBALS.server.SESSION_CACHE.GetSessionData(sessionId) == AT_GLOBALS.server.Session_Structure_EMPTY)
			return false;
//		if (AT_GLOBALS.server.SESSION_CACHE.GetSessionData(sessionId).sessionId.IsEmpty())
//			return false;
		
		return true;
		
		// session id that's not empty
//		if (sessionId.IsEmpty())
//		{
//			Print("AT -> AT_MainStatic -> verifySession -> Empty sessionId");
//			return false;
//		}
//		Session_Structure sessionGet = AT_GLOBALS.server.SESSION_CACHE.GetSessionData(sessionId);
//		if (!sessionGet)
//			return false;
//		if (sessionGet.sessionId)
//		{
//			if (sessionGet.sessionId.IsEmpty())
//			{
//				Print("AT -> AT_MainStatic -> verifySession -> sessionGet -> Empty sessionId");
//				return false;
//			}
//			else
//				return true;
//		}
//		else
//		{
//			Print("AT -> AT_MainStatic -> verifySession -> sessionGet -> sessionId null");
//			return false;
//		}
	}
}

