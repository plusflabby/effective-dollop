class AT_UI_MENU_BASE : SCR_SuperMenuBase
{
	protected string TEXT_TITLE;
	protected Widget rootWidget;
	protected SCR_PlayerController playerController;
	
	protected override void OnMenuOpen()
	{
		if (AT_GLOBALS.client.DEBUG)
			Print("OnMenuOpen()", LogLevel.WARNING);
		
		AT_MainStatic.setCurrentMenu(TEXT_TITLE);
		playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		
		#ifndef WORKBENCH
		//! Check for session, if not login menu and session is set open menu, else close menu and open login 
		if (TEXT_TITLE != "Auth")
		{
			//if (playerController.m_aSessionUid.Count() < 1)
			if (AT_GLOBALS.client.sessionId.IsEmpty())
			{
				Close();
				GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.AT_AUTH);
			}
		}
		#endif
		
		rootWidget = GetRootWidget();
		
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
	}
	
	protected override void OnMenuClose()
	{
		if (AT_Global.client.DEBUG)
			Print("OnMenuClose()", LogLevel.WARNING);
		
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