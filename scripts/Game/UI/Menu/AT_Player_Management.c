class AT_Player_ManagementUI : MenuBase
{
	// Static variables
	protected static const string TEXT_TITLE = "PlayerManagement";
	protected static const string BUTTON_KICK = "KickButton";
	protected static const string BUTTON_BAN = "Button1";
	protected static const string BUTTON_TELEPORT_THERE = "Button2";
	protected static const string BUTTON_TELEPORT_HERE = "Button3";
	protected static const string BUTTON_SPECTATE = "Button4";
	
	// Variables
	protected TextListboxWidget listbox;

	protected override void OnMenuOpen()
	{
		Print("AT_Player_ManagementUI->OnMenuOpen: menu opened", LogLevel.NORMAL);
		
		Widget rootWidget = GetRootWidget();
		if (!rootWidget)
		{
			Print("AT_Player_ManagementUI->OnMenuOpen: rootWidget layout error", LogLevel.ERROR);
			return;
		}
		
		//setup spectate button
		SCR_ButtonBaseComponent spectateButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_SPECTATE, rootWidget);
		if (!spectateButton)
		{
			Debug.Error("AT_Player_ManagementUI->OnMenuOpen: spectateButton error");
			return;
		}
		spectateButton.m_OnClicked.Insert(SpectateButton);
		
		//setup kick button
		SCR_ButtonBaseComponent kickButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_KICK, rootWidget);
		if (!kickButton)
		{
			Debug.Error("AT_Player_ManagementUI->OnMenuOpen: kickButton error");
			return;
		}
		kickButton.m_OnClicked.Insert(KickButton);
		
		//setup teleport there button
		SCR_ButtonBaseComponent tpThereButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_TELEPORT_THERE, rootWidget);
		if (!tpThereButton)
		{
			Debug.Error("AT_Player_ManagementUI->OnMenuOpen: tpThereButton error");
			return;
		}
		tpThereButton.m_OnClicked.Insert(TeleportThereButton);
		
		//setup teleport here button
		SCR_ButtonBaseComponent tpHereButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_TELEPORT_HERE, rootWidget);
		if (!tpThereButton)
		{
			Debug.Error("AT_Player_ManagementUI->OnMenuOpen: tpHereButton error");
			return;
		}
		tpHereButton.m_OnClicked.Insert(TeleportHereButton);
		
		// Here we will populate the players list
		listbox = TextListboxWidget.Cast(rootWidget.FindAnyWidget("playerslistbox"));
		if (!listbox)
		{
			Print("AT_Player_ManagementUI->OnMenuOpen: listbox error", LogLevel.ERROR);
			return;
		}
		
		//Set up listeners to close menu
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
		{
			// this is for the menu/dialog to close when pressing ESC
			// an alternative is to have a button with the SCR_NavigationButtonComponent component
			// and its Action Name field set to MenuBack - this would activate the button on ESC press
			inputManager.AddActionListener("MenuOpen", EActionTrigger.DOWN, Close);
			inputManager.AddActionListener("MenuBack", EActionTrigger.DOWN, Close);
#ifdef WORKBENCH // in Workbench, F10 is used because ESC closes the preview
			inputManager.AddActionListener("MenuOpenWB", EActionTrigger.DOWN, Close);
			inputManager.AddActionListener("MenuBackWB", EActionTrigger.DOWN, Close);
#endif
		}
		
		//Tell AT_Main this menu is IsOpen
		AT_MainStatic.setCurrentMenu(TEXT_TITLE);
		
		loadPlayerListBox();
	}

	protected override void OnMenuClose()
	{
		InputManager inputManager = GetGame().GetInputManager();
		if (inputManager)
		{
			inputManager.RemoveActionListener("MenuOpen", EActionTrigger.DOWN, Close);
			inputManager.RemoveActionListener("MenuBack", EActionTrigger.DOWN, Close);
#ifdef WORKBENCH
			inputManager.RemoveActionListener("MenuOpenWB", EActionTrigger.DOWN, Close);
			inputManager.RemoveActionListener("MenuBackWB", EActionTrigger.DOWN, Close);
#endif
		}
	}
	
	protected void loadPlayerListBox()
	{
		array<int> playerIDs = new array<int>;
		GetGame().GetPlayerManager().GetPlayers(playerIDs);
		
		// add players to listbox
		for (int i; i < playerIDs.Count(); i++;)
		{
			AT_playerData player = new AT_playerData();
			player.id = playerIDs.Get(i);
			listbox.AddItem(GetGame().GetPlayerManager().GetPlayerName(playerIDs.Get(i)), player, 0);
		}
		
		// Select first row
		listbox.SelectRow(0);
	}
	
	protected void KickButton() {
		AT_playerData player = AT_playerData.Cast(listbox.GetItemData(listbox.GetSelectedRow()));
		if (AT_EVENT_CLASS)
			AT_EVENT_CLASS.add(new AT_Event(player, AT_Events.Kick, "Kick_Id_" + player.id));
	}
	
	protected void TeleportThereButton() {
		AT_playerData player = AT_playerData.Cast(listbox.GetItemData(listbox.GetSelectedRow()));
		if (AT_EVENT_CLASS)
			AT_EVENT_CLASS.add(new AT_Event(player, AT_Events.TeleportThere, "TP_There_Id_" + player.id));
	}
	
	protected void TeleportHereButton() {
		AT_playerData player = AT_playerData.Cast(listbox.GetItemData(listbox.GetSelectedRow()));
		if (AT_EVENT_CLASS)
			AT_EVENT_CLASS.add(new AT_Event(player, AT_Events.TeleportHere, "TP_Here_Id_" + player.id));
	}
	
	protected void SpectateButton() {
		AT_playerData player = AT_playerData.Cast(listbox.GetItemData(listbox.GetSelectedRow()));
		if (AT_EVENT_CLASS)
			AT_EVENT_CLASS.add(new AT_Event(player, AT_Events.Spectate, "Spectate_Id_" + player.id));
	}
}

class AT_playerData
{
	int id;
}
