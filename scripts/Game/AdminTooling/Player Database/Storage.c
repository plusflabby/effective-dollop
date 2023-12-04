class DB_PlayerProfile : EDF_DbEntity
{
	string m_sBiUID;
	string m_sCreationDateTime;
	string m_sName;
	array<string> m_aName;
	
	void generate(string biUID, string name)
	{
		SetId(EDF_DbEntityIdGenerator.Generate());
		m_sCreationDateTime = SCR_DateTimeHelper.GetDateTimeLocal();
		m_sBiUID = biUID;
		m_sName = name;
	}
}