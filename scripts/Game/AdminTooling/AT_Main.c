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
			if (RplSession.Mode() != RplMode.Dedicated)
				playerBiUid = string.Format("bbbbdddd-0000-0000-0000-%1", playerId.ToString(12)); // Peer tool support
			else
				Debug.Error("Dedicated server is not correctly configured to connect to the BI backend.");
		}
		
		return playerBiUid;
	}
}

