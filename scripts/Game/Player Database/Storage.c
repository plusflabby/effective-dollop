class DB_PlayerProfile : EDF_DbEntity
{
	string m_sBiUID = string.Empty;
	string m_sCreationDateTime = string.Empty;
	string m_sName = string.Empty;
	ref array<string> m_aName = new array<string>();
	string m_sPlayerChatsDbId = string.Empty;
	//! Formatted playtime example = "0y:0M:0D:0h:0m:0s"
	string m_sPlayTime = string.Empty;
	//! array of SCR_DateTimeHelper.GetDateTimeLocal()
	ref array<string> m_aPlayerLogins = new array<string>();
	//! example of action string = "ADMIN_PROFILE_ID:ACTION:TIME"
	ref array<string> m_aAdminActions = new array<string>();
	
	void generate(string biUID, string name)
	{
		SetId(EDF_DbEntityIdGenerator.Generate());
		m_sCreationDateTime = SCR_DateTimeHelper.GetDateTimeLocal();
		m_sBiUID = biUID;
		m_sName = name;
		m_aName = { name };
	}
}