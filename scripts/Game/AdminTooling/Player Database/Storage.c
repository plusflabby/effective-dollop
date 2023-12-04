class DB_PlayerProfile : EDF_DbEntity
{
	string m_sBiUID;
	string m_sCreationDateTime;
	
	void generate(string biUID)
	{
		SetId(EDF_DbEntityIdGenerator.Generate());
		m_sCreationDateTime = SCR_DateTimeHelper.GetDateTimeLocal();
		m_sBiUID = biUID;
	}
}