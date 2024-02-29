class AT_Player_ManagementUI : AT_UI_MENU_BASE
{
	// Static variables
	protected static const string BUTTON_KICK = "KickButton";
	protected static const string BUTTON_BAN = "Button1";
	protected static const string BUTTON_TELEPORT_THERE = "Button2";
	protected static const string BUTTON_TELEPORT_HERE = "Button3";
	protected static const string BUTTON_SPECTATE = "Button4";
	protected static const string LISTBOX_PLAYERS = "playerslistbox";
	protected static const string BUTTON_SEARCH = "Button0";
	protected static const string EDIT_BOX_PLAYERUID = "EditBoxRoot0";
	protected static const string WINDOW_RESULTS = "Window0";
	protected static const string TEXT_LIST_BOX_RESULTS = "Text00001273563";
	protected static const string BUTTON_OPEN_PROFILE = "Button011113";
	
	// Variables
	protected TextListboxWidget listbox;
	protected SCR_ButtonBaseComponent spectateButton;
	protected SCR_ButtonBaseComponent kickButton;
	protected SCR_ButtonBaseComponent tpThereButton;
	protected SCR_ButtonBaseComponent tpHereButton;
	protected SCR_EditBoxComponent uidEditBox;
	protected TextWidget resultTextWidget;
	//protected ref PDI_Result search;
	protected SCR_ButtonBaseComponent searchButton;
	protected WindowWidget windowWidget;
	protected TextListboxWidget listboxWidget;
	protected SCR_ButtonBaseComponent openProfileButton;
	
	// Game Features Update (PArt 1)
	protected SCR_ButtonBaseComponent BUTTON_PLAYER_GODMODE;
	protected static const string BUTTON_PLAYER_GODMODE_STR = "Button5";
	protected SCR_ButtonBaseComponent BUTTON_VEHICLE_GODMODE;
	protected static const string BUTTON_VEHICLE_GODMODE_STR = "Button6";
	protected SCR_ButtonBaseComponent BUTTON_MAP_DISPLAY;
	protected static const string BUTTON_MAP_DISPLAY_STR = "Button7";
	
	private void PLAYER_GODMODE() {
		if (AT_Global.client.DEBUG)
			Print("PLAYER_GODMODE()", LogLevel.WARNING);
		
		events(AT_Events.PlayerGodMode);
	}
	private void VEHICLE_GODMODE() {
		if (AT_Global.client.DEBUG)
			Print("VEHICLE_GODMODE()", LogLevel.WARNING);
		
		events(AT_Events.VehicleGodMode);
	}
	private void MAP_DISPLAY() {
		if (AT_Global.client.DEBUG)
			Print("MAP_DISPLAY()", LogLevel.WARNING);
		
		events(AT_Events.MapDisplay);
	}
	
	protected override void OnMenuOpen()
	{
		if (AT_GLOBALS.client.DEBUG)
			Print("OnMenuOpen()UI_PlayerManagement", LogLevel.WARNING);
		
		TEXT_TITLE = "UI_PlayerManagement";
		super.OnMenuOpen();
		
		//! setup player godmode button
		BUTTON_PLAYER_GODMODE = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_PLAYER_GODMODE_STR, rootWidget);
		BUTTON_PLAYER_GODMODE.m_OnClicked.Insert(PLAYER_GODMODE);
		
		//! setup vehicle godmode button
		BUTTON_VEHICLE_GODMODE = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_VEHICLE_GODMODE_STR, rootWidget);
		BUTTON_VEHICLE_GODMODE.m_OnClicked.Insert(VEHICLE_GODMODE);
		
		//! setup map display button
		BUTTON_MAP_DISPLAY = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_MAP_DISPLAY_STR, rootWidget);
		BUTTON_MAP_DISPLAY.m_OnClicked.Insert(SpectateButton);
		
		//! setup spectate button
		spectateButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_SPECTATE, rootWidget);
		spectateButton.m_OnClicked.Insert(MAP_DISPLAY);
		
		//! setup kick button
		kickButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_KICK, rootWidget);
		kickButton.m_OnClicked.Insert(KickButton);
		
		//! setup teleport there button
		tpThereButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_TELEPORT_THERE, rootWidget);
		tpThereButton.m_OnClicked.Insert(TeleportThereButton);
		
		//! setup teleport here button
		tpHereButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_TELEPORT_HERE, rootWidget);
		tpHereButton.m_OnClicked.Insert(TeleportHereButton);
		
		//! setup player database 
		searchButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_SEARCH, rootWidget);
		searchButton.m_OnClicked.Insert(SearchButton);
		
		resultTextWidget = TextWidget.Cast(rootWidget.FindAnyWidget("resulttext"));
		uidEditBox = SCR_EditBoxComponent.Cast(SCR_EditBoxComponent.GetEditBoxComponent(EDIT_BOX_PLAYERUID, rootWidget));
		
		windowWidget = WindowWidget.Cast(rootWidget.FindAnyWidget(WINDOW_RESULTS));
		listboxWidget = TextListboxWidget.Cast(rootWidget.FindAnyWidget(TEXT_LIST_BOX_RESULTS));
		openProfileButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_OPEN_PROFILE, rootWidget);
		openProfileButton.m_OnClicked.Insert(OpenProfile);
		
		resultTextWidget.SetVisible(false);
		windowWidget.SetVisible(false);
		
		//! Here we will setup and populate the players list
		listbox = TextListboxWidget.Cast(rootWidget.FindAnyWidget(LISTBOX_PLAYERS));
		loadPlayerListBox();
	}
	
	private void loadPlayerListBox()
	{
		if (AT_Global.client.DEBUG)
			Print("loadPlayerListBox()", LogLevel.WARNING);
		
		array<int> playerIDs = new array<int>;
		PlayerManager pm = GetGame().GetPlayerManager();
		pm.GetPlayers(playerIDs);
		
		// add players to listbox
		for (int i; i < playerIDs.Count(); i++;)
		{
			AT_playerData player = new AT_playerData();
			player.id = playerIDs.Get(i);
			listbox.AddItem(pm.GetPlayerName(playerIDs.Get(i)), player, 0);
		}
		
		// Select first row
		listbox.SelectRow(0);
	}
	
	private void events(AT_Events event_)
	{
		if (AT_Global.client.DEBUG)
			Print("events()", LogLevel.WARNING);
		
		AT_playerData player = AT_playerData.Cast(listbox.GetItemData(listbox.GetSelectedRow()));
		if (AT_Global.client.AT_EVENT_CLASS)
			AT_Global.client.AT_EVENT_CLASS.add(new AT_Event(player, event_, SCR_Enum.GetEnumName(AT_Events, event_) + "_" + player.id));
	}
	
	private void KickButton() {
		if (AT_Global.client.DEBUG)
			Print("KickButton()", LogLevel.WARNING);
		
		events(AT_Events.Kick);
	}
	
	private void TeleportThereButton() {
		if (AT_Global.client.DEBUG)
			Print("TeleportThereButton()", LogLevel.WARNING);
		
		events(AT_Events.TeleportThere);
	}
	
	private void TeleportHereButton() {
		if (AT_Global.client.DEBUG)
			Print("TeleportHereButton()", LogLevel.WARNING);
		
		events(AT_Events.TeleportHere);
	}
	
	private void SpectateButton() {
		if (AT_Global.client.DEBUG)
			Print("SpectateButton()", LogLevel.WARNING);
		
		events(AT_Events.Spectate);
	}
	
	private void SearchButton() {
		if (AT_Global.client.DEBUG)
			Print("SearchButton()", LogLevel.WARNING);
		
		//search = PlayerDatabaseIntergration.findPlayerProfile(value); // server should call this and return value to player 
		string value = uidEditBox.GetValue();
		array<string> rpc_data = new array<string>();
		rpc_data.Insert(value);
		AT_GLOBALS.client.AT_EVENT_CLASS.add(new AT_Event(rpc_data, AT_Events.PlayerDatabaseSearch, "PDB_SEARCH"));
		GetGame().GetCallqueue().CallLater(processSearch, 2000, false);
	}
	
	private void OpenProfile()
	{
		if (AT_Global.client.DEBUG)
			Print("OpenProfile()", LogLevel.WARNING);
		
		AT_textlistbox_string rowData = AT_textlistbox_string.Cast(listboxWidget.GetItemData(listboxWidget.GetSelectedRow()));
		AT_GLOBALS.client.m_sAtUiProfileUID = rowData.str;
		GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_PlayerProfile);
		Close();
	}
	
	protected void processSearch()
	{
		if (AT_Global.client.DEBUG)
			Print("processSearch()", LogLevel.WARNING);
		
//		Print(playerController.playerDatabaseResult);
//		Print(playerController.playerDatabaseResult);
//		Print(playerController.playerDatabaseResult);
//		Print(playerController.playerDatabaseResult);
		if (!playerController.playerDatabaseResult)
		{
			resultTextWidget.SetText("INVALID SEARCH, Enter in player's BI UID");
			resultTextWidget.SetVisible(true);
			return;
		}
		PDI_Result_Class search = playerController.playerDatabaseResult;
		switch (search.m_iCode)
		{
			case PDI_Results.SUCCESS:
			{
				array<string> arr = search.getUids();
				string header = "SUCCESSFULLY FOUND RESULT(S) | #"+arr.Count().ToString();
				resultTextWidget.SetText(header);
				
				if (arr.Count() > 0)
				{
					listboxWidget.ClearItems();
					foreach (string biUid : arr)
					{
						listboxWidget.AddItem(biUid, AT_textlistbox_string(biUid), 0);
					}
					listboxWidget.SelectRow(0);
					windowWidget.SetVisible(true);
				}
				
				break;
			}
			case PDI_Results.NOT_FOUND:
			{
				resultTextWidget.SetText("NOT FOUND");
				windowWidget.SetVisible(false);
				break;
			}
			case PDI_Results.INVALID_SEARCH:
			{
				resultTextWidget.SetText("INVALID SEARCH VALUE, Enter in player's BI UID");
				windowWidget.SetVisible(false);
				
				break;
			}
		}
		resultTextWidget.SetVisible(true);
	}
}

class AT_playerData
{
	int id;
}

class AT_textlistbox_string
{
	string str;
	
	void AT_textlistbox_string(string str_)
	{
		str = str_;
		if (AT_Global.client.DEBUG)
			Print("AT_textlistbox_string constructor()", LogLevel.WARNING);
	}
}