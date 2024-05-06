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
			if (AT_GLOBALS.client.DEBUG)
				playerBiUid = "000-000-000-000";
			else if (RplSession.Mode() != RplMode.Dedicated)
				playerBiUid = string.Format("bbbbdddd-0000-0000-0000-%1", playerId.ToString(12)); // Peer tool support
			else
				Debug.Error("Dedicated server is not correctly configured to connect to the BI backend.");
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
		
		return true;
	}
}

