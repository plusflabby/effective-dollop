//! Results
class PDI_Result
{
	ref DB_PlayerProfile player;
	PDI_Results result_code;
	ref array<ref DB_PlayerProfile> results = {};
	
	
	void PDI_Result(PDI_Results result_code_)
	{
		result_code = result_code_;
	}
}

//! Response values
enum PDI_Results
{
	SUCCESS,
	NOT_FOUND,
	INVALID_SEARCH
}

class PlayerDatabaseIntergration
{
	private static typename type = String("DB_PlayerProfile").ToType();
	
//	//! get player from bi uid  TODO UPDATE
//	static PDI_Result getPlayerFromUID(string keyValue)
//	{
//		if (keyValue.Length() < 12)
//			return PDI_Result(PDI_Results.INVALID_SEARCH);
//		
//		AT_Database_Data_Player findResult = AT_DB.getPlayer(keyValue);
//		if (!findResult)
//			return PDI_Result(PDI_Results.NOT_FOUND);
//		
//		PDI_Result result = PDI_Result(PDI_Results.SUCCESS);
//		result.player = findResult;
//		return result;
//	}
	
	//! player has profile 
	static bool findProfile(string biUid)
	{
		EDF_DbFindResultMultiple<EDF_DbEntity> search = atDB.FindAll(type, EDF_DbFind.Field("m_sBiUID").Contains(biUid), limit : 1);
		if (!search.IsSuccess())
			return false;
		
		return search.GetEntities().Count() > 0;
	}
	
	//! get profiles 
	private static array<ref DB_PlayerProfile> getProfiles(string biUid, int limitToReturn = 1)
	{
		EDF_DbFindResultMultiple<EDF_DbEntity> search = atDB.FindAll(type, EDF_DbFind.Field("m_sBiUID").Contains(biUid), limit : limitToReturn);
		array<ref EDF_DbEntity> result = search.GetEntities();
		
		array<ref DB_PlayerProfile> profiles = new array<ref DB_PlayerProfile>();
		foreach (EDF_DbEntity profile : result)
		{
			profiles.Insert(DB_PlayerProfile.Cast(profile));
		}
		
		return profiles;
	}
	
//	//! return player profile 
//	static PDI_Result getPlayerProfile(string biUid)
//	{
//		if (biUid.Length() < 12)
//			return PDI_Result(PDI_Results.INVALID_SEARCH);
//		if (!findProfile(biUid))
//			return PDI_Result(PDI_Results.NOT_FOUND);
//		
//		array<ref DB_PlayerProfile> foundProfile = getProfiles(biUid);
//		PDI_Result returnValue = PDI_Result(PDI_Results.SUCCESS);
//		returnValue.player = foundProfile.Get(0);
//		returnValue.results = foundProfile;
//		return returnValue;
//	}
	
	//! find player profile(s) 
	static PDI_Result findPlayerProfile(string biUid, int limitToReturn = 99)
	{
//		PDI_Result firstSearch = getPlayerProfile(biUid);
//		if (firstSearch.result_code == PDI_Results.SUCCESS || firstSearch.result_code == PDI_Results.INVALID_SEARCH)
//			return firstSearch;
		
		array<ref DB_PlayerProfile> secondSearch = getProfiles(biUid, limitToReturn);
		if (secondSearch.Count() < 1)
			return PDI_Result(PDI_Results.NOT_FOUND); 
		
		PDI_Result returnValue = PDI_Result(PDI_Results.SUCCESS);
		
		if (secondSearch.Count() == 1)
			returnValue.player = secondSearch.Get(0);
		else
			returnValue.results = secondSearch;
		
		return returnValue;
	}
	
	//! generate profile 
	static void generateNewProfile(string playerBiUid, string playerName)
	{
		AT_DB.saveNewPlayer(playerBiUid, playerName);
	}
	
	//! update profile if name change 
	static void updateProfileNames(string nameChanged, string biUid)
	{
		array<ref DB_PlayerProfile> profiles = getProfiles(biUid, 1);
		DB_PlayerProfile profile = profiles.Get(0);
		profile.m_aName.Insert(nameChanged);
		
		//! If > 99 names saved release first aka oldest
		if (profile.m_aName.Count() > 99)
			profile.m_aName.Remove(0);
		
		AT_DB.AddOrUpdateProfile(profile);
	}
	
	//! get profile name of player 
	static string getProfileName(string biUid)
	{
		PDI_Result profileRDI = findPlayerProfile(biUid);
		
		if (profileRDI.result_code == PDI_Results.SUCCESS)
			return profileRDI.player.m_sName;
		else
			return string.Empty;
	}
	
//	static void updateEntireProfile(DB_PlayerProfile profile)
//	{
//		
//	}
}
