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
		ref AT_LoggingLocalCallback logCallback = new AT_LoggingLocalCallback();
		atDB.AddOrUpdateAsync(log, logCallback);
		Print(log.m_sMessage, LogLevel.NORMAL);
	}

	static void saveNewPlayer(string biUid)
	{
		ref callbackAT variableOne = new callbackAT();
		AT_PlayerProfile newProfile = new AT_PlayerProfile();
		newProfile.setBiUid(biUid);
		atDB.AddOrUpdateAsync(newProfile, variableOne);
	}

	static AT_Database_Data_Player getPlayer(string biUid)
	{
		//AT_Database_Data_Player playerData;
		//SCR_BinLoadContext loadContext = new SCR_BinLoadContext();
		//loadContext.LoadFromFile(m_sPlayer);
		//loadContext.ReadValue(biUid, playerData);

		EDF_DbFindResultMultiple<EDF_DbEntity> results = atDB.FindAll(
			EDF_DbEntity/*,
			EDF_DbFind.Field("m_sBiUid").Equals(biUid)*/
		);

		Print(results.IsSuccess().ToString(), LogLevel.WARNING);
		Print(results.IsSuccess().ToString(), LogLevel.WARNING);
		Print(SCR_Enum.GetEnumName(EDF_EDbOperationStatusCode, results.GetStatusCode()), LogLevel.WARNING);
		Print(SCR_Enum.GetEnumName(EDF_EDbOperationStatusCode, results.GetStatusCode()), LogLevel.WARNING);
		Print(SCR_Enum.GetEnumName(EDF_EDbOperationStatusCode, results.GetStatusCode()), LogLevel.WARNING);
		Print(SCR_Enum.GetEnumName(EDF_EDbOperationStatusCode, results.GetStatusCode()), LogLevel.WARNING);
		Print(SCR_Enum.GetEnumName(EDF_EDbOperationStatusCode, results.GetStatusCode()), LogLevel.WARNING);
		Print(SCR_Enum.GetEnumName(EDF_EDbOperationStatusCode, results.GetStatusCode()), LogLevel.WARNING);
		Print(results.GetSizeOf().ToString(), LogLevel.WARNING);
		Print(results.GetSizeOf().ToString(), LogLevel.WARNING);
		Print(results.GetSizeOf().ToString(), LogLevel.WARNING);
		Print(results.GetSizeOf().ToString(), LogLevel.WARNING);
		Print(results.GetSizeOf().ToString(), LogLevel.WARNING);
		Print(results.GetSizeOf().ToString(), LogLevel.WARNING);
		Print(results.GetSizeOf().ToString(), LogLevel.WARNING);
		Print(results.GetSizeOf().ToString(), LogLevel.WARNING);
		if (!results.IsSuccess())
			return AT_Database_Data_Player_EMPTY;

		//results.GetEntities
		array<ref EDF_DbEntity> entities = results.GetEntities();
		Print("ERRRRRRRRRRRRRRR", LogLevel.WARNING);
		entities.Debug();

		return AT_Database_Data_Player_EMPTY;
	}

	//static void updatePlayer(AT_Database_Data_Player player, string playerUid)
	//{
	//	saveNewPlayer(playerUid, player);
	//}

	/*
	static bool doesAdminConfigExist()
	{
		return FileIO.FileExists(m_sAdmin);
	}

	static void createAdminConfig()
	{
		AT_Database_Data_Admins changeMeAdmin = new AT_Database_Data_Admins();
		changeMeAdmin.name = "CHANGE_TO_YOUR_NAME";

		SCR_JsonSaveContext saveContext = new SCR_JsonSaveContext();
		saveContext.WriteValue("CHANGE_THIS_TO_UID", changeMeAdmin);
		saveContext.SaveToFile(m_sAdmin);
	}

	static bool isAdminFromUid(string uid)
	{
		SCR_JsonLoadContext loadContext = new SCR_JsonLoadContext();
		loadContext.LoadFromFile(m_sAdmin);

		AT_Database_Data_Admins admin;
		loadContext.ReadValue(uid, admin);

		if (!admin)
			return false;
		else
			return true;
	}*/
}

// /////////////////
// Stored classed //
// /////////////////
class AT_Database_Data_Ban : Managed
{
	// ////////////
	// Variables //
	// ////////////
	protected string m_sUid = "EMPTY_UID";
	protected string m_sReason = "EMPTY_REASON";

	// //////////////////////
	// Getters and setters //
	// //////////////////////
	string getUid()
	{
		return m_sUid;
	}
	string getReason()
	{
		return m_sReason;
	}
	void setUid(string uid)
	{
		m_sUid = uid;
	}
	void setReason(string reason)
	{
		m_sReason = reason;
	}
}

ref AT_Database_Data_Player AT_Database_Data_Player_EMPTY = new ref AT_Database_Data_Player();
class AT_Database_Data_Player : Managed
{
	// ////////////
	// Variables //
	// ////////////
	protected string m_sUid = "EMPTY_UID";
	protected string m_sJoined = "EMPTY_JOINED_DATETIME";
	private Managed profileElements;
	private Managed interactions;
	private Managed preferences;

	// //////////////////////
	// Getters and setters //
	// //////////////////////
	// Uid
	string getUid()
	{
		return m_sUid;
	}
	void setUid(string uid)
	{
		m_sUid = uid;
	}
	// Joined
	string getJoined()
	{
		return m_sJoined;
	}
	void setJoined(string joined)
	{
		m_sJoined = joined;
	}
	// update profileElements
	void updateProfileElements(Managed newProfileElements)
	{
		profileElements = newProfileElements;
	}
}
class AT_Database_Data_Admins : JsonApiStruct
{
	string name;

	void AT_Database_Data_Admins()
	{
		RegV("name");
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
//! Will be server's connect
ref AT_Database atDB = SetUpServerDatabase();

class AT_PlayerProfile : EDF_DbEntity
{
	//! This will be the bohemia unique identifier
	string m_sBiUid;
	//! This will be set at creating by SCR_DateTimeHelper.GetDateTimeLocal()
	string m_sJoinDateTime;

	void AT_PlayerProfile()
	{
		SetId(EDF_DbEntityIdGenerator.Generate());
		m_sJoinDateTime = SCR_DateTimeHelper.GetDateTimeLocal();
	}

	void setBiUid(string biUid)
	{
		m_sBiUid = biUid;
	}


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
