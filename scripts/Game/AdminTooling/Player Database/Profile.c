modded class SCR_PlayerController
{
	[RplProp(onRplName: "OnProfileInformationUpdate")]
	string profileInformation = string.Empty;
	
	protected void OnProfileInformationUpdate()
	{
		Print(profileInformation);
		Print(profileData);
		array<string> info = AT_MainStatic.stringToArray(profileInformation);
		Print(info);
		foreach (string data : info)
		{
			profileData.Insert(data);
		}
		Print(info);
		
		Print(profileData);
	}
	
	[RplRpc(RplChannel.Unreliable, RplRcver.Server)]
	protected void RpcAsk_Authority_Method_PlayerProfile(string request)
	{
		array<string> arrayRequest = AT_MainStatic.stringToArray(request);
		Print(arrayRequest);
		string biUid;
		biUid = arrayRequest.Get(0);
		
		string dataToGet;
		dataToGet = arrayRequest.Get(1);
		
		PDI_Result getProfile = PlayerDatabaseIntergration.findPlayerProfile(biUid, 1);
		Print(getProfile.result_code == PDI_Results.SUCCESS);
		if (getProfile.result_code == PDI_Results.SUCCESS)
		{
			Print(dataToGet.Contains("Names"));
			if (dataToGet.Contains("Names"))
			{
				profileInformation = AT_MainStatic.stringArayToString(getProfile.player.m_aName);
				Print(profileInformation);
				Replication.BumpMe();
			}
		}
	}
}

string m_sAtUiProfileUID = string.Empty;
ref array<string> profileData = new array<string>();