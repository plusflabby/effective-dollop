class AT_UI_PlayerDatabase : MenuBase
{
	// Static variables
	protected static const string TEXT_TITLE = "PlayerDatabaseSearch";
	protected static const string BUTTON_SEARCH = "Button0";
	protected static const string EDIT_BOX_PLAYERUID = "EditBoxRoot0";
	protected static const string WINDOW_RESULTS = "Window0";
//	protected static const string VLW_RESULTS = "VerticalLayout01101";
//	protected static const string VLW_RESULT = "Button122";

	Widget rootWidget;
	SCR_EditBoxComponent uidEditBox;
	TextWidget resultTextWidget;
	ref PDI_Result search;
	WindowWidget windowWidget;
//	VerticalLayoutWidget vlWidget;

	protected override void OnMenuOpen()
	{
		Print("AT_UI_PlayerDatabase->OnMenuOpen: menu opened", LogLevel.SPAM);

		rootWidget = GetRootWidget();
		
		//setup search button
		SCR_ButtonBaseComponent searchButton = SCR_ButtonBaseComponent.GetButtonBase(BUTTON_SEARCH, rootWidget);
		if (!searchButton)
		{
			Debug.Error("AT_UI_PlayerDatabase->OnMenuOpen: searchButton error");
			return;
		}
		searchButton.m_OnClicked.Insert(SearchButton);

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

	private void SearchButton() {
		resultTextWidget = TextWidget.Cast(rootWidget.FindAnyWidget("resulttext"));
		uidEditBox = SCR_EditBoxComponent.Cast(SCR_EditBoxComponent.GetEditBoxComponent(EDIT_BOX_PLAYERUID, rootWidget));
		string value = uidEditBox.GetValue();
		//Print(value);
		
		search = PlayerDatabaseIntergration.findPlayerProfile(value);
		//Print(search.ToString());
		
		switch (search.result_code)
		{
			case PDI_Results.SUCCESS:
			{
				if (search.results.Count() > 1)
				{
					string header = "SUCCESSFULLY FOUND | Found "+search.results.Count().ToString();
					
					foreach (DB_PlayerProfile profile : search.results)
					{
						header = header + "\n" + profile.m_sBiUID + " : " + profile.m_sName;
					}
					
					resultTextWidget.SetText(header);
					resultTextWidget.SetVisible(true);
					
					//Text00001273563
					
				}
				else
				{
//					resultTextWidget.SetText("SUCCESSFULLY FOUND | Found UID="+search.player.m_sBiUID);
//					resultTextWidget.SetVisible(true);
					//! Open player's profile
					m_sAtUiProfileUID = search.player.m_sBiUID;
					GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_PlayerProfile);
					Close();
				}
				break;
			}
			case PDI_Results.NOT_FOUND:
			{
				resultTextWidget.SetText("NOT FOUND");
				resultTextWidget.SetVisible(true);
				break;
			}
			case PDI_Results.INVALID_SEARCH:
			{
				resultTextWidget.SetText("INVALID SEARCH VALUE, Enter in player's BI UID");
				resultTextWidget.SetVisible(true);
				break;
			}
		}
		
	}
}
