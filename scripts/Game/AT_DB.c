// //////////////////////
// Main Database Class //
// //////////////////////
class AT_DB
{
	protected static string m_sBan = "$profile:AT_BDB.ATbin";
	protected static string m_sPlayer = "$profile:AT_PDB.ATbin";
	protected static string m_sAdmin = "$profile:AT_Admin_List.ATjson";

	//! Save the log to database
	static void saveLog(
		string message,
		LogLevel level,
		string meta1 = string.Empty,
		string meta2 = string.Empty,
		string meta3 = string.Empty,
		string meta4 = string.Empty,
		string meta5 = string.Empty
	)
	{
		ref AT_LoggingLocalStorage logCreation = new AT_LoggingLocalStorage();
		logCreation.m_sMetaOne = meta1;
		logCreation.m_sMetaTwo = meta2;
		logCreation.m_sMetaThree = meta3;
		logCreation.m_sMetaFour = meta4;
		logCreation.m_sMetaFive = meta5;
		logCreation.m_sMessage = message;
		logCreation.m_sLogLevel = SCR_Enum.GetEnumName(LogLevel, level);
		saveNewLog(logCreation);
	}
	private static void saveNewLog(notnull AT_LoggingLocalStorage log)
	{
//		ref AT_LoggingLocalCallback logCallback = new AT_LoggingLocalCallback();
//		AT_GLOBALS.server.atDB.AddOrUpdateAsync(log, logCallback);
		Print(log.m_sMessage, LogLevel.ERROR);
	}

	static void saveNewPlayer(string biUid, string playersName)
	{
		
		DB_PlayerProfile newProfile = DB_PlayerProfile();
		newProfile.generate(biUid, playersName);
		AddOrUpdateProfile(newProfile);
	}
	
	static void AddOrUpdateProfile(DB_PlayerProfile profile)
	{
		AT_GLOBALS.server.atDB.AddOrUpdateAsync(profile, AT_GLOBALS.server.variableOne);
	}
	
	static void AddOrUpdateChat(DB_Chat chat)
	{
		AT_GLOBALS.server.atDB.AddOrUpdateAsync(chat, AT_GLOBALS.server.variableOne);
	}
	
	static void storeAdminAction(string action, string playerBiUid)
	{
		// get profile with bi uid
		PDI_Result getProfile = PlayerDatabaseIntergration.findPlayerProfile(playerBiUid, 1);
		if (getProfile.result_code == PDI_Results.SUCCESS)
		{
			// get admin actions from profile 
			array<string> adminActions = array<string>.Cast(getProfile.player.m_aAdminActions.Clone());
			// add action to end of array 
			adminActions.Insert(action);
			// set value on profile object
			getProfile.player.m_aAdminActions = adminActions;
			// update profile
			AT_DB.AddOrUpdateProfile(getProfile.player);
			// done :D
		}
	}
}

// ////////////////////
// Database Revision //
// ////////////////////

class AT_Database : EDF_JsonFileDbDriver
{
	void AT_Database()
	{
		EDF_JsonFileDbConnectionInfo connectInfo();
		connectInfo.m_sDatabaseName = "AdminTooling";
		connectInfo.m_bPrettify = true;
		connectInfo.m_bUseCache = true;

		Initialize(connectInfo);
	}
}

//! Will return null for client
AT_Database SetUpServerDatabase()
{
	if (Replication.IsServer())
		return AT_Database();

	return null;
}

class callbackAT : EDF_DbOperationStatusOnlyCallback
{
	//override void OnSuccess(Managed context)
	//{
		//Print("SUCCESS", LogLevel.WARNING);
	//}
	override void OnFailure(EDF_EDbOperationStatusCode statusCode, Managed context)
	{
		Print(SCR_Enum.GetEnumName(EDF_EDbOperationStatusCode, statusCode), LogLevel.ERROR);
	}
}