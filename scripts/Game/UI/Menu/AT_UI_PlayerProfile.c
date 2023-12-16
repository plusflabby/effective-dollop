class AT_UI_PlayerProfile : AT_UI_MENU_BASE
{
	private static const string m_sTextBoxPlayerBiUid = "RichText0110";
	private TextWidget playerBiUid;
	
	private override void OnMenuOpen()
	{
		TEXT_TITLE = "UI_PlayerProfile";
		super.OnMenuOpen();
		
		playerBiUid = TextWidget.Cast(rootWidget.FindAnyWidget(m_sTextBoxPlayerBiUid));
		playerBiUid.SetText(m_sAtUiProfileUID);
		
		requestData("Names");
		
		GetGame().GetCallqueue().CallLater(processData, 5000, false);
	}
	
	private void requestData(string dataName)
	{
		array<string> ev_data = new array<string>();
		ev_data.Insert(m_sAtUiProfileUID);
		ev_data.Insert(dataName);
		AT_EVENT_CLASS.add(new AT_Event(ev_data, AT_Events.PlayerProfileGetData, "PROFILE_GET_" + dataName));
	}
	
	private void processData()
	{
		Print(profileData);
		string names = string.Empty;
		foreach (string name : profileData)
		{
			names = names + string.Format("%1\n", name);
		}
		Print(names);
		
		playerBiUid.SetText(names);
	}
}