modded class SCR_PlayerController
{
	[RplProp(onRplName: "OnProfileNamesUpdate")]
	string profileNames = string.Empty;
	[RplProp(onRplName: "OnProfilePlaytimeUpdate")]
	string profilePlaytime = string.Empty;
	
	protected void OnProfilePlaytimeUpdate()
	{
		AT_GLOBALS.client.profileData.Insert(AT_MainStatic.stringToArray(profilePlaytime));
	}
	
	protected void OnProfileNamesUpdate()
	{
		Print(profileNames);
		Print(AT_GLOBALS.client.profileData);
		AT_GLOBALS.client.profileData.Insert(AT_MainStatic.stringToArray(profileNames));
		Print(AT_GLOBALS.client.profileData);
	}
	
	[RplRpc(RplChannel.Unreliable, RplRcver.Server)]
	protected void RpcAsk_Authority_Method_PlayerProfile(string request, string sessionId)
	{
		if (AT_MainStatic.verifySession(sessionId) == false)
			return;
		
		array<string> arrayRequest = AT_MainStatic.stringToArray(request);
		Print(arrayRequest);
		string biUid;
		biUid = arrayRequest.Get(0);
		
		string dataToGet;
		dataToGet = arrayRequest.Get(1);
		
		string dataToReturnTo;
		dataToReturnTo = arrayRequest.Get(2);
		
		PDI_Result getProfile = PlayerDatabaseIntergration.findPlayerProfile(biUid, 1);
		Print(getProfile.result_code == PDI_Results.SUCCESS);
		if (getProfile.result_code == PDI_Results.SUCCESS)
		{
			if (dataToGet.Contains("Names"))
			{
				array<string> names = getProfile.player.m_aName;
				names.InsertAt(dataToReturnTo, 0);
				profileNames = AT_MainStatic.stringArayToString(names);
				Print(profileNames);
				Replication.BumpMe();
			}
			else if (dataToGet.Contains("PlayTime"))
			{
				array<string> playtime = new array<string>();
				playtime.Insert(dataToReturnTo);
				playtime.Insert(getProfile.player.m_sPlayTime);
				profilePlaytime = AT_MainStatic.stringArayToString(playtime);
				Print(profilePlaytime);
				Replication.BumpMe();
			}
		}
	}
}