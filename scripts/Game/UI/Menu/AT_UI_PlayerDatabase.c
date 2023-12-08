class AT_UI_PlayerDatabase : MenuBase
{
	// Static variables
	protected static const string TEXT_TITLE = "PlayerDatabaseSearch";
//	protected static const string VLW_RESULTS = "VerticalLayout01101";
//	protected static const string VLW_RESULT = "Button122";

	Widget rootWidget;
//	VerticalLayoutWidget vlWidget;

	protected override void OnMenuOpen()
	{
		Print("AT_UI_PlayerDatabase->OnMenuOpen: menu opened", LogLevel.SPAM);

		rootWidget = GetRootWidget();
		
		//setup search button
		

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

	
}
