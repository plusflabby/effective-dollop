class AT_UI_PlayerProfile : AT_UI_MENU_BASE
{
	private static const string m_sTextBoxPlayerNames = "RichText0110";
	private TextWidget playerNames;
	
	
	private static const string m_sTextBoxNames = "RichText11001";
	private static const string m_sTextBoxPlaytime = "RichText00001";
	
	private void requestData(string dataName, string textWidgetToPlaceData)
	{
		array<string> ev_data = new array<string>();
		ev_data.Insert(AT_GLOBALS.client.m_sAtUiProfileUID);
		ev_data.Insert(dataName);
		ev_data.Insert(textWidgetToPlaceData);
		AT_Global.client.AT_EVENT_CLASS.add(new AT_Event(ev_data, AT_Events.PlayerProfileGetData, "PROFILE_GET_" + dataName));
	}
	
	private override void OnMenuOpen()
	{
		TEXT_TITLE = "UI_PlayerProfile";
		super.OnMenuOpen();
		
		playerNames = TextWidget.Cast(rootWidget.FindAnyWidget(m_sTextBoxPlayerNames));
		playerNames.SetText(AT_GLOBALS.client.m_sAtUiProfileUID);
		
		requestData("Names", m_sTextBoxNames);
		GetGame().GetCallqueue().CallLater(processData, 3000, false);
		
		requestData("PlayTime", m_sTextBoxPlaytime);
		GetGame().GetCallqueue().CallLater(processPlayTime, 2500, false);
	}
	
	private void processPlayTime()
	{
		foreach (array<string> dataToProcess : AT_GLOBALS.client.profileData)
		{
			if (dataToProcess == null)
				return;
			
			string dataGet = dataToProcess.Get(0);
			if (dataGet != m_sTextBoxPlaytime)
				return;
			
			string dataPlaytime = dataToProcess.Get(1);
			
			TextWidget txt = TextWidget.Cast(rootWidget.FindAnyWidget(dataGet));
			txt.SetText(dataPlaytime);
		}
	}
	
	private void processData()
	{
		Print(AT_GLOBALS.client.profileData);
		foreach (array<string> dataToProcess : AT_GLOBALS.client.profileData)
		{
			if (dataToProcess == null)
				return;
			array<string> data = dataToProcess;
			Print(data);
			string nameOfWidget = data.Get(0);
			
			if (nameOfWidget != m_sTextBoxNames)
				return;
			
			Print(nameOfWidget);
			data.Remove(0);
			Print(data);
			string names = string.Empty;
			Print(names);
			foreach (string name : data)
				names = names + string.Format("%1\n", name);
			
			//Print(names);
			Print(nameOfWidget);
			
			TextWidget txt = TextWidget.Cast(rootWidget.FindAnyWidget(nameOfWidget));
			txt.SetText(names);
			
			AT_GLOBALS.client.profileData.RemoveItem(dataToProcess);
		}
	}
}