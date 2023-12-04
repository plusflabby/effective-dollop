//! Results
class PDI_Result
{
	ref DB_PlayerProfile player;
	PDI_Results result_code;
	
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
	
	//! return player profile 
	static PDI_Result getPlayerProfile(string biUid)
	{
		if (biUid.Length() < 12)
			return PDI_Result(PDI_Results.INVALID_SEARCH);
		if (!findProfile(biUid))
			return PDI_Result(PDI_Results.NOT_FOUND);
		
		EDF_DbFindResultMultiple<EDF_DbEntity> search = atDB.FindAll(type, EDF_DbFind.Field("m_sBiUID").Contains(biUid), limit : 1);
		array<ref EDF_DbEntity> result = search.GetEntities();
		DB_PlayerProfile profile = DB_PlayerProfile.Cast(result.Get(0));
		
		PDI_Result returnValue = PDI_Result(PDI_Results.SUCCESS);
		returnValue.player = profile;
		return returnValue;
	}
	
	//! generate profile 
	static void generateNewProfile(string playerBiUid)
	{
		AT_DB.saveNewPlayer(playerBiUid);
	}
}
