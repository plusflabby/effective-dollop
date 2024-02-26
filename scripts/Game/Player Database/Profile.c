modded class SCR_PlayerController
{
	[RplProp(onRplName: "OnProfileNamesUpdate")]
	string profileNames = string.Empty;
	[RplProp(onRplName: "OnProfilePlaytimeUpdate")]
	string profilePlaytime = string.Empty;
	[RplProp(onRplName: "OnProfileLoginsUpdate")]
	string profileLogins = string.Empty;

	protected void OnProfilePlaytimeUpdate()
	{
		if (AT_GLOBALS.client.DEBUG)
			Print("OnProfilePlaytimeUpdate()", LogLevel.WARNING);
		
		AT_GLOBALS.client.profileData.Insert(AT_MainStatic.stringToArray(profilePlaytime));
	}
	protected void OnProfileNamesUpdate()
	{
		if (AT_Global.client.DEBUG)
			Print("OnProfileNamesUpdate()", LogLevel.WARNING);
		
		AT_GLOBALS.client.profileData.Insert(AT_MainStatic.stringToArray(profileNames));
	}
	protected void OnProfileLoginsUpdate()
	{
		if (AT_Global.client.DEBUG)
			Print("OnProfileLoginsUpdate()", LogLevel.WARNING);
		
		AT_GLOBALS.client.profileData.Insert(AT_MainStatic.stringToArray(profileLogins));
	}

	[RplRpc(RplChannel.Unreliable, RplRcver.Server)]
	protected void RpcAsk_Authority_Method_PlayerProfile(string request, string sessionId)
	{
		if (AT_GLOBALS.server.DEBUG)
			Print("RpcAsk_Authority_Method_PlayerProfile() start", LogLevel.ERROR);
		
		if (AT_MainStatic.verifySession(sessionId) == false)
			return;

		array<string> arrayRequest = AT_MainStatic.stringToArray(request);
		string biUid;
		biUid = arrayRequest.Get(0);

		string dataToGet;
		dataToGet = arrayRequest.Get(1);

		string dataToReturnTo;
		dataToReturnTo = arrayRequest.Get(2);

		PDI_Result getProfile = PlayerDatabaseIntergration.findPlayerProfile(biUid, 1);
		//Print(getProfile.result_code == PDI_Results.SUCCESS);
		
		if (AT_GLOBALS.server.DEBUG)
			Print(
				"RpcAsk_Authority_Method_PlayerProfile() getProfile.result_code="+getProfile.result_code,
				LogLevel.ERROR
			);
		
		if (getProfile.result_code == PDI_Results.SUCCESS)
		{
			if (AT_GLOBALS.server.DEBUG)
				Print(
					"RpcAsk_Authority_Method_PlayerProfile() dataToGet="+dataToGet,
					LogLevel.ERROR
				);
			
			if (dataToGet == "Names")
			{
				array<string> names = getProfile.player.m_aName;
				names.InsertAt(dataToReturnTo, 0);
				profileNames = AT_MainStatic.stringArayToString(names);
				Print(profileNames);

				Replication.BumpMe();

			}
			else if (dataToGet == "PlayTime")
			{
				array<string> playtime = new array<string>();
				playtime.Insert(dataToReturnTo);
				playtime.Insert(getProfile.player.m_sPlayTime);
				profilePlaytime = AT_MainStatic.stringArayToString(playtime);
				Print(profilePlaytime);

				Replication.BumpMe();
			}
			else if (dataToGet == "Logins")
			{
				array<string> logins = getProfile.player.m_aPlayerLogins;
				logins.InsertAt(dataToReturnTo, 0);
				profileLogins = AT_MainStatic.stringArayToString(logins);
				Print(profileLogins);

				Replication.BumpMe();
			}
		}
		
		if (AT_GLOBALS.server.DEBUG)
			Print("RpcAsk_Authority_Method_PlayerProfile() end", LogLevel.ERROR);
		
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_Authority_Method_PlayerProfile_PlayTime(int playerId)
	{
		// get bi uid
		string biUid = AT_MainStatic.getUid(playerId);
		// get profile with bi uid
		PDI_Result getProfile = PlayerDatabaseIntergration.findPlayerProfile(biUid, 1);
		if (getProfile.result_code == PDI_Results.SUCCESS)
		{
			// get playtime from profile
			string playTime = getProfile.player.m_sPlayTime;
			if (playTime.IsEmpty())
				playTime = "0";
			// update value
			int newPlayTime = playTime.ToInt() + 5;
			// set value on profile object
			getProfile.player.m_sPlayTime = newPlayTime.ToString();
			// update profile
			AT_DB.AddOrUpdateProfile(getProfile.player);
			// done :D
		}
	}
	
	[RplRpc(RplChannel.Reliable, RplRcver.Server)]
	protected void RpcAsk_Authority_Method_PlayerProfile_Join(int playerId)
	{
		// get bi uid
		string biUid = AT_MainStatic.getUid(playerId);
		// get profile with bi uid
		PDI_Result getProfile = PlayerDatabaseIntergration.findPlayerProfile(biUid, 1);
		if (getProfile.result_code == PDI_Results.SUCCESS)
		{
			// get m_aPlayerLogins from profile 
			array<string> playerLogins = array<string>.Cast(getProfile.player.m_aPlayerLogins.Clone());
			// add datetime to beginning of array 
			playerLogins.InsertAt(SCR_DateTimeHelper.GetDateTimeUTC(), 0);
			// set value on profile object
			getProfile.player.m_aPlayerLogins = playerLogins;
			// update profile
			AT_DB.AddOrUpdateProfile(getProfile.player);
			// done :D
		}
	}
}
