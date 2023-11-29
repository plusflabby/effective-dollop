// //////////////////////
// Main Database Class //
// //////////////////////
class AT_DB
{
	protected static string m_sBan = "$profile:AT_BDB.ATbin";
	protected static string m_sPlayer = "$profile:AT_PDB.ATbin";
	protected static string m_sAdmin = "$profile:AT_Admin_List.ATjson";

	static void saveNewPlayer(string key, AT_Database_Data_Player data)
	{
		SCR_BinSaveContext saveContext = new SCR_BinSaveContext();
		saveContext.WriteValue(key, data);
		saveContext.SaveToFile(m_sPlayer);
	}

	static AT_Database_Data_Player getPlayer(string key)
	{
		AT_Database_Data_Player playerData;
		SCR_BinLoadContext loadContext = new SCR_BinLoadContext();
		loadContext.LoadFromFile(m_sPlayer);
		loadContext.ReadValue(key, playerData);
		return playerData;
	}
	
	static void updatePlayer(AT_Database_Data_Player player, string playerUid)
	{
		saveNewPlayer(playerUid, player);
	}
	
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
	}
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
