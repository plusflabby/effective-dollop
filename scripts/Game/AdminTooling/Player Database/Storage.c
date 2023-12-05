class DB_PlayerProfile : EDF_DbEntity
{
	string m_sBiUID = string.Empty;
	string m_sCreationDateTime = string.Empty;
	string m_sName = string.Empty;
	ref array<string> m_aName = new array<string>();
	string m_sPlayerChatsDbId = string.Empty;
	
	void generate(string biUID, string name)
	{
		SetId(EDF_DbEntityIdGenerator.Generate());
		m_sCreationDateTime = SCR_DateTimeHelper.GetDateTimeLocal();
		m_sBiUID = biUID;
		m_sName = name;
		m_aName = { name };
	}
}