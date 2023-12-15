//! Results
class PDI_Result
{
	static ref DB_PlayerProfile player = null;
	static PDI_Results result_code = PDI_Results.INVALID_SEARCH;
	static ref array<ref DB_PlayerProfile> results = {};
}


class PDI_Result_Class
{
	int m_iCode;
	string m_sBiUids;
	
	//! get bi uids as array 
	array<string> getUids()
	{
		Print(m_sBiUids);
		array<string> strs = {};
		m_sBiUids.Split(",", strs, true);
		Print(strs);
		return strs;
	}
	
	static PDI_Result_Class createClass(PDI_Results code, array<string> uids)
	{
		PDI_Result_Class returnClass = new PDI_Result_Class();
		
		returnClass.m_iCode = code;
		
		foreach (string uid : uids)
		{
//			if (m_sBiUids)
//				returnClass.m_sBiUids = string.Format("%1,%2", returnClass.m_sBiUids, uid);
//			else
				returnClass.m_sBiUids = string.Format("%1,", uid);
		}
		
		return returnClass;
	}
	
	static bool Extract(PDI_Result_Class instance, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		snapshot.SerializeInt(instance.m_iCode);
		snapshot.SerializeString(instance.m_sBiUids);
		
		return true;
	}
	static bool Inject(SSnapSerializerBase snapshot, ScriptCtx ctx, PDI_Result_Class instance)
	{
		snapshot.SerializeInt(instance.m_iCode);
		snapshot.SerializeString(instance.m_sBiUids);
		
		return true;
	}
	static void Encode(SSnapSerializerBase snapshot, ScriptCtx ctx, ScriptBitSerializer packet)
	{
		snapshot.EncodeInt(packet);		// m_iCode
		snapshot.EncodeString(packet);	// m_sBiUids
	}
	static bool Decode(ScriptBitSerializer packet, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		snapshot.DecodeInt(packet);		// m_iCode
		snapshot.DecodeString(packet);	// m_sBiUids
		return true;
	}
	static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx ctx)
	{
		return lhs.CompareSnapshots(rhs, 4) && lhs.CompareStringSnapshots(rhs);
	}
	static bool PropCompare(PDI_Result_Class instance, SSnapSerializerBase snapshot, ScriptCtx ctx)
	{
		return snapshot.CompareInt(instance.m_iCode)
			&& snapshot.CompareString(instance.m_sBiUids);
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
	
	//! player has profile 
	static bool findProfile(string biUid)
	{
		EDF_DbFindResultMultiple<EDF_DbEntity> search = atDB.FindAll(type, EDF_DbFind.Field("m_sBiUID").Contains(biUid), limit : 1);
		if (!search.IsSuccess())
			return false;
		
		return search.GetEntities().Count() > 0;
	}
	
	//! get profiles 
	private static array<ref DB_PlayerProfile> getProfiles(string searchValue, int limitToReturn = 1)
	{
		EDF_DbFindResultMultiple<EDF_DbEntity> search;
		search = atDB.FindAll(type, EDF_DbFind.Field("m_sBiUID").Contains(searchValue), limit : limitToReturn);
		
		// 0 in first search so try names 
		if (search.GetEntities().Count() == 0)
		{
			search = atDB.FindAll(type, EDF_DbFind.Field("m_aName").Contains(searchValue), limit : limitToReturn);
		}
		
		array<ref EDF_DbEntity> result = search.GetEntities();
		array<ref DB_PlayerProfile> profiles = new array<ref DB_PlayerProfile>();
		foreach (EDF_DbEntity profile : result)
		{
			profiles.Insert(DB_PlayerProfile.Cast(profile));
		}
		
		return profiles;
	}
	
	//! find player profile(s) 
	static PDI_Result findPlayerProfile(string biUid, int limitToReturn = 99)
	{
		PDI_Result returnValue = new PDI_Result();
		array<ref DB_PlayerProfile> secondSearch = getProfiles(biUid, limitToReturn);
		if (secondSearch.Count() < 1)
		{
			returnValue.result_code = PDI_Results.NOT_FOUND;
			return returnValue;
		}
		
		returnValue.result_code = PDI_Results.SUCCESS;
		
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
	
	static bool profileNameIsStored(string biUid, string name)
	{
		PDI_Result profileRDI = findPlayerProfile(biUid);
		
		if (profileRDI.result_code == PDI_Results.SUCCESS)
			if (profileRDI.player.m_sName != name)
				return profileRDI.player.m_aName.Contains(name);
		
		return false;
	}
}
