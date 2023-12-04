class AT_UI_Reporting : MenuBase
{
	//! Static Variables 
	private static const string MENU_TITLE = "AT_REPORTING";
	private static const string GENERATE_BUTTON = "Button0_GEN";
	
	//! Variables
	private Widget rootWidget;
	private SCR_ButtonBaseComponent genButton;
	private InputManager inputManager;
	
	//! generate report 
	private void genReport()
	{
		Print("GENERATING REPORT");
	}
	
	protected override void OnMenuOpen()
	{
		rootWidget = GetRootWidget();
		genButton = SCR_ButtonBaseComponent.GetButtonBase(GENERATE_BUTTON, rootWidget);
		genButton.m_OnClicked.Insert(genReport);
		
		
		//Set up listeners to close menu
		inputManager = GetGame().GetInputManager();
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
		inputManager = GetGame().GetInputManager();
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