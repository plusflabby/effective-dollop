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

	protected override void OnMenuOpen()
	{
		TEXT_TITLE = "UI_PlayerManagement";
		super.OnMenuOpen();
		
		//! setup spectate button
		spectateButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_SPECTATE, rootWidget);
		spectateButton.m_OnClicked.Insert(SpectateButton);
		
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
		AT_playerData player = AT_playerData.Cast(listbox.GetItemData(listbox.GetSelectedRow()));
		if (AT_Global.client.AT_EVENT_CLASS)
			AT_Global.client.AT_EVENT_CLASS.add(new AT_Event(player, event_, SCR_Enum.GetEnumName(AT_Events, event_) + "_" + player.id));
	}
	
	private void KickButton() {
		events(AT_Events.Kick);
	}
	
	private void TeleportThereButton() {
		events(AT_Events.TeleportThere);
	}
	
	private void TeleportHereButton() {
		events(AT_Events.TeleportHere);
	}
	
	private void SpectateButton() {
		events(AT_Events.Spectate);
	}
	
	private void SearchButton() {
		//search = PlayerDatabaseIntergration.findPlayerProfile(value); // server should call this and return value to player 
		string value = uidEditBox.GetValue();
		array<string> rpc_data = new array<string>();
		rpc_data.Insert(value);
		AT_GLOBALS.client.AT_EVENT_CLASS.add(new AT_Event(rpc_data, AT_Events.PlayerDatabaseSearch, "PDB_SEARCH"));
		GetGame().GetCallqueue().CallLater(processSearch, 2000, false);
	}
	
	private void OpenProfile()
	{
		AT_textlistbox_string rowData = AT_textlistbox_string.Cast(listboxWidget.GetItemData(listboxWidget.GetSelectedRow()));
		AT_GLOBALS.client.m_sAtUiProfileUID = rowData.str;
		GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_PlayerProfile);
		Close();
	}
	
	protected void processSearch()
	{
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
	}
}