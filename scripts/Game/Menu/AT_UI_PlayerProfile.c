class AT_UI_PlayerProfile : AT_UI_MENU_BASE
{
	private static const string m_sTextBoxNames = "RichText0110";
	private static const string m_sTextPlaytime = "Text3111"; 
	private static const string m_sTextBiGuid = "Text333"; 
	private static const string m_sTextBoxPlayerLogins = "RichText11001";
	// private static const string m_sTextBoxAdminActions = "RichText00001";
	
	private void requestData(string dataName, string textWidgetToPlaceData)
	{
		if (AT_GLOBALS.client.DEBUG)
			Print("requestData()", LogLevel.WARNING);
		
		Print("requestData()dataName"+dataName, LogLevel.WARNING);
		Print("requestData()textWidgetToPlaceData"+textWidgetToPlaceData, LogLevel.WARNING);
		array<string> ev_data = new array<string>();
		ev_data.Insert(AT_GLOBALS.client.m_sAtUiProfileUID);
		ev_data.Insert(dataName);
		ev_data.Insert(textWidgetToPlaceData);
		AT_Global.client.AT_EVENT_CLASS.add(new AT_Event(ev_data, AT_Events.PlayerProfileGetData, "PROFILE_GET_" + dataName));
	}
	
	private override void OnMenuOpen()
	{
		if (AT_Global.client.DEBUG)
			Print("OnMenuOpen()UI_PlayerProfile", LogLevel.WARNING);
		
		TEXT_TITLE = "UI_PlayerProfile";
		super.OnMenuOpen();
		
		TextWidget playerNames = TextWidget.Cast(rootWidget.FindAnyWidget(m_sTextBiGuid));
		playerNames.SetText(AT_GLOBALS.client.m_sAtUiProfileUID);
		
		requestData("Names", m_sTextBoxNames);
		GetGame().GetCallqueue().CallLater(processData, 3000, false);
		
		requestData("PlayTime", m_sTextPlaytime);
		GetGame().GetCallqueue().CallLater(processPlayTime, 2500, false);
		
		requestData("Logins", m_sTextBoxPlayerLogins);
		GetGame().GetCallqueue().CallLater(processPlayerLogins, 2000, false);
	}
	
	private void processPlayerLogins()
	{
		if (AT_Global.client.DEBUG)
			Print("processPlayerLogins()", LogLevel.WARNING);
		
		SCR_PlayerController pc = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		Print("playerController"+pc, LogLevel.WARNING);
		AT_GLOBALS.client.profileData.Insert(AT_MainStatic.stringToArray(pc.profileLogins));
		Print("AT_GLOBALS.client.profileData"+AT_GLOBALS.client.profileData, LogLevel.WARNING);
		
		foreach (array<string> dataToProcess : AT_GLOBALS.client.profileData)
		{
			Print(dataToProcess);
			Print(dataToProcess);
			Print(dataToProcess);
			Print(dataToProcess);
//			Print(dataToProcess);
//			if (dataToProcess == null)
//				return;
//			
//			Print(m_sTextBoxPlayerLogins);
//			string dataGet = dataToProcess.Get(0);
//			if (dataGet != m_sTextBoxPlayerLogins)
//				return;
			
			if (dataToProcess == null)
				return;
			array<string> data = dataToProcess;
			string nameOfWidget = data.Get(0);
			
			if (nameOfWidget != m_sTextBoxNames)
				return;
			
			// get logins without ui-element(first index)
			//array<string> data = dataToProcess;
			// data.Remove(0);
			
			// create string of logins
			string logins = string.Empty;
			foreach (string login : data)
				logins = logins + string.Format("%1\n", login);
			
			// update ui 
			TextWidget txt = TextWidget.Cast(rootWidget.FindAnyWidget(nameOfWidget));
			txt.SetText(logins);
			
			// remove processed logins from profile data  
			AT_GLOBALS.client.profileData.RemoveItem(dataToProcess);
		}
	}
	
	private void processPlayTime()
	{
		if (AT_Global.client.DEBUG)
			Print("processPlayTime()", LogLevel.WARNING);
		
		foreach (array<string> dataToProcess : AT_GLOBALS.client.profileData)
		{
			if (dataToProcess == null)
				return;
			
			string dataGet = dataToProcess.Get(0);
			if (dataGet != m_sTextPlaytime)
				return;
			
			string dataPlaytime = dataToProcess.Get(1);
			
			TextWidget txt = TextWidget.Cast(rootWidget.FindAnyWidget(dataGet));
			txt.SetText(dataPlaytime);
			
			AT_GLOBALS.client.profileData.RemoveItem(dataToProcess);
		}
	}
	
	private void processData()
	{
		if (AT_Global.client.DEBUG)
			Print("processData() start", LogLevel.WARNING);
		
		// Print(AT_GLOBALS.client.profileData);
		foreach (array<string> dataToProcess : AT_GLOBALS.client.profileData)
		{
			if (dataToProcess == null)
				return;
			array<string> data = dataToProcess;
			// Print(data);
			string nameOfWidget = data.Get(0);
			
			if (nameOfWidget != m_sTextBoxNames)
				return;
			
			// Print(nameOfWidget);
			data.Remove(0);
			// Print(data);
			string names = string.Empty;
			Print(names);
			foreach (string name : data)
				names = names + string.Format("%1\n", name);
			
			//Print(names);
			// Print(nameOfWidget);
			
			TextWidget txt = TextWidget.Cast(rootWidget.FindAnyWidget(nameOfWidget));
			txt.SetText(names);
			
			AT_GLOBALS.client.profileData.RemoveItem(dataToProcess);
		}
		
		if (AT_Global.client.DEBUG)
			Print("processData() end", LogLevel.WARNING);
	}
}