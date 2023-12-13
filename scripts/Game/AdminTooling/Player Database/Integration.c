//! Results
class PDI_Result
{
	static ref DB_PlayerProfile player;
	static PDI_Results result_code;
	static ref array<ref DB_PlayerProfile> results = {};
	
	static private int sizeTotal;
	static private int size_player;
	static private int size_result_code;
	static private int size_results;
	
	void PDI_Result()
	{
		if (player)
			size_player = player.GetSizeOf();
		if (results)
			size_results = results.GetSizeOf();
		if (result_code)
			size_result_code = 4;
		sizeTotal = size_player + size_results + size_result_code;
	}
	
	//! Takes snapshot and encodes it into packet using as few bits as possible.
	static void Encode(SSnapSerializerBase snapshot, ScriptCtx ctx, ScriptBitSerializer packet)
	{
		Print("Encode" + sizeTotal);
        snapshot.Serialize(packet, sizeTotal);
    }

	//! Takes packet and decodes it into snapshot. Returns true on success or false when an error occurs.
	static bool Decode(ScriptBitSerializer packet, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		Print("Decode" + sizeTotal);
		return snapshot.Serialize(packet, sizeTotal);
	}

	//! Compares two snapshots. Returns true when they match or false otherwise.
	static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx ctx)
	{
		Print("SnapCompare" + sizeTotal);
		return lhs.CompareSnapshots(rhs, sizeTotal);
	}

	//! Compares instance against snapshot. Returns true when they match or false otherwise.
	static bool PropCompare(PDI_Result instance, SSnapSerializerBase snapshot, ScriptCtx ctx)
	{
		Print("PropCompare" + sizeTotal);
		int sizePlayer = 0;
		if (instance.player)
			sizePlayer = instance.player.GetSizeOf();
		
		int sizeResultCode = 0;
		if (instance.result_code)
			sizeResultCode = size_result_code;
		
		int sizeResults = 0;
		if (instance.results)
			sizeResults = instance.results.GetSizeOf();
		
		return snapshot.Compare(instance.player, sizePlayer) 
			&& snapshot.Compare(instance.result_code, size_result_code) 
			&& snapshot.Compare(instance.results, sizeResults);
	}

	//! Writes data from an instance into snapshot. Opposite of Inject().
	static bool Extract(PDI_Result instance, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		Print("Extract" + sizeTotal);
		int sizePlayer = 0;
		if (instance.player)
			sizePlayer = instance.player.GetSizeOf();
		snapshot.SerializeBytes(instance.player, sizePlayer);
		
		int sizeResultCode = 0;
		if (instance.result_code)
			sizeResultCode = size_result_code;
		snapshot.SerializeBytes(instance.result_code, sizeResultCode);
		
		int sizeResults = 0;
		if (instance.results)
			sizeResults = instance.results.GetSizeOf();
		snapshot.SerializeBytes(instance.results, sizeResults);

		player = instance.player;
		result_code = instance.result_code;
		results = instance.results;
		
		return true;
	}

	//! Writes data from snapshot into instance. Opposite of Extract().
	static bool Inject(SSnapSerializerBase snapshot, ScriptCtx ctx, PDI_Result instance)
	{
		Print("Inject" + sizeTotal);
		instance.player = player;
		instance.result_code = result_code;
		instance.results = results;
		
		int sizePlayer = 0;
		if (instance.player)
			sizePlayer = instance.player.GetSizeOf();
		snapshot.SerializeBytes(instance.player, sizePlayer);
		
		int sizeResultCode = 0;
		if (instance.result_code)
			sizeResultCode = size_result_code;
		snapshot.SerializeBytes(instance.result_code, sizeResultCode);
		
		int sizeResults = 0;
		if (instance.results)
			sizeResults = instance.results.GetSizeOf();
		snapshot.SerializeBytes(instance.results, sizeResults);
		
		return true;
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
	
	//! find player profile(s) 
	static PDI_Result findPlayerProfile(string biUid, int limitToReturn = 99)
	{
		PDI_Result returnValue = PDI_Result();
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
