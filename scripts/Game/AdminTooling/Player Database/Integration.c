class PlayerDatabaseIntergration
{
	// TODOs
	// get player with playerUid
	// get player with name 
	// get player with entityId 
	
	// get player
	static AT_Database_Data_Player getPlayer(string keyValue)
	{
		if (keyValue.Length() < 12)
		{
			Debug.Error("keyValue invalid.");
			return AT_Database_Data_Player_EMPTY;
		}
		
		return AT_DB.getPlayer(keyValue);
	}
	
	//TODO managing player data
}