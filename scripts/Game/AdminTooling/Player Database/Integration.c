class PlayerDatabaseIntergration
{
	//! get player from bi uid 
	static PDI_Result getPlayerFromUID(string keyValue)
	{
		if (keyValue.Length() < 12)
			return PDI_Result(PDI_Results.INVALID_SEARCH);
		
		AT_Database_Data_Player findResult = AT_DB.getPlayer(keyValue);
		if (!findResult)
			return PDI_Result(PDI_Results.NOT_FOUND);
		
		PDI_Result result = PDI_Result(PDI_Results.SUCCESS);
		result.player = findResult;
		return result;
	}
	
	//TODO managing player data
}
//ref PlayerDatabaseIntergration playerDatabaseIntergration = new PlayerDatabaseIntergration();

class PDI_Result
{
	ref AT_Database_Data_Player player = AT_Database_Data_Player_EMPTY;
	PDI_Results result_code;
	
	void PDI_Result(PDI_Results result_code_)
	{
		result_code = result_code_;
	}
}

enum PDI_Results
{
	SUCCESS,
	NOT_FOUND,
	INVALID_SEARCH
}