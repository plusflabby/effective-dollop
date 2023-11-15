class SCR_PlayerControllerClass : PlayerControllerClass
{
};

void OwnershipChangedDelegate(bool isChanging, bool becameOwner);
typedef func OwnershipChangedDelegate;
typedef ScriptInvokerBase<OwnershipChangedDelegate> OnOwnershipChangedInvoker;

//------------------------------------------------------------------------------------------------
class SCR_PlayerController : PlayerController
{
	static PlayerController s_pLocalPlayerController;
	static const float WALK_SPEED = 0.5;
	static const float FOCUS_THRESHOLD = 0.01;
	static const float FOCUS_TIMEOUT = 0.2;
	static float s_fADSFocus = 0.7;
	static float s_fFocusTimeout;

	CharacterControllerComponent m_CharacterController;
	private bool m_bIsLocalPlayerController;
	private bool m_bIsPaused;
	bool m_bRetain3PV;
	protected bool m_bGadgetFocus;
	protected bool m_bFocusToggle;
	protected float m_fCharacterSpeed;


	[RplProp(onRplName: "OnRplMainEntityFromID")]
	protected RplId m_MainEntityID;
	protected IEntity m_MainEntity;

	[RplProp()]
	protected bool m_bIsPossessing;

	ref ScriptInvoker<IEntity> m_OnDestroyed = new ScriptInvoker();		// main entity is destroyed
	ref ScriptInvoker<IEntity> m_OnBeforePossess = new ScriptInvoker();		// Before an entity becomes possesed.
	ref ScriptInvoker<IEntity> m_OnPossessed = new ScriptInvoker();		// when entity becomes possessed or control returns to the main entity
	ref ScriptInvoker<IEntity, IEntity> m_OnControlledEntityChanged = new ScriptInvoker();
	
	private ref OnOwnershipChangedInvoker m_OnOwnershipChangedInvoker = new OnOwnershipChangedInvoker();
	//------------------------------------------------------------------------------------------------
	/*!
		\see PlayerController.OnOwnershipChanged for more information.
	*/
	OnOwnershipChangedInvoker GetOnOwnershipChangedInvoker() 
	{ 
		return m_OnOwnershipChangedInvoker; 
	}

	//------------------------------------------------------------------------------------------------
	/*!
		\see PlayerController.OnOwnershipChanged for more information.
	*/
	protected override void OnOwnershipChanged(bool changing, bool becameOwner)
	{
		super.OnOwnershipChanged(changing, becameOwner);
		m_OnOwnershipChangedInvoker.Invoke(changing, becameOwner);
	}

	override void OnControlledEntityChanged(IEntity from, IEntity to)
	{
		// todo: react to change, inform fe. VONController
		m_OnControlledEntityChanged.Invoke(from, to);
	}

	//------------------------------------------------------------------------------------------------
	//! TODO: Unhack please, perhaps run the setters directly in SCR_GameplaySettings
	//! All these settings could be set with static methods in CharacterControllerComponent
	void SetGameUserSettings()
	{
		IEntity controlledEntity = GetControlledEntity();
		if (!controlledEntity)
		{
			return;
		}
		m_CharacterController = CharacterControllerComponent.Cast(controlledEntity.FindComponent(CharacterControllerComponent));
		if (!m_CharacterController)
		{
			return;
		}

		BaseContainer aimSensitivitySettings = GetGame().GetGameUserSettings().GetModule("SCR_AimSensitivitySettings");

		if (aimSensitivitySettings)
		{
			float aimSensitivityMouse;
			float aimSensitivityGamepad;
			float aimMultipADS;

			if (aimSensitivitySettings.Get("m_fMouseSensitivity", aimSensitivityMouse) &&
				aimSensitivitySettings.Get("m_fStickSensitivity", aimSensitivityGamepad) &&
				aimSensitivitySettings.Get("m_fAimADS", aimMultipADS))
			{
				m_CharacterController.SetAimingSensitivity(aimSensitivityMouse, aimSensitivityGamepad, aimMultipADS);
			}


		}

		BaseContainer gameplaySettings = GetGame().GetGameUserSettings().GetModule("SCR_GameplaySettings");

		if (gameplaySettings)
		{
			bool stickyADS = true;
			if (gameplaySettings.Get("m_bStickyADS", stickyADS))
				m_CharacterController.SetStickyADS(stickyADS);

			bool stickyGadgets = true;
			if (gameplaySettings.Get("m_bStickyGadgets", stickyGadgets))
				m_CharacterController.SetStickyGadget(stickyGadgets);

			EVehicleDrivingAssistanceMode drivingAssistance;
			if(GetGame().GetIsClientAuthority())
			{
				if (gameplaySettings.Get("m_eDrivingAssistance", drivingAssistance))
					VehicleControllerComponent.SetDrivingAssistanceMode(drivingAssistance);
			}
			else
			{
				if (gameplaySettings.Get("m_eDrivingAssistance", drivingAssistance))
					VehicleControllerComponent_SA.SetDrivingAssistanceMode(drivingAssistance);
			}
		}

		//TODO: we might want to set default focusInADS to 100 on XBOX and PSN ( default for mouse control should be 70 - see SCR_GameplaySettings )
		BaseContainer fovSettings = GetGame().GetGameUserSettings().GetModule("SCR_FieldOfViewSettings");
		if (fovSettings)
		{
			float focusInADS = 0.5;
			if (fovSettings.Get("m_fFocusInADS", focusInADS))
				s_fADSFocus = focusInADS;
		}
	}

	//------------------------------------------------------------------------------------------------
	/*!
	Set entity which will be possessed by player.
	Possed entity is controlled by player, but it's not *the* player.
	\param entity Entity to be possessed, or null to return control back to the original player
	*/
	void SetPossessedEntity(IEntity entity)
	{
		if (!m_bIsPossessing)
		{
			if (entity)
			{
				m_OnBeforePossess.Invoke(entity);
				//--- Start posessing
				m_bIsPossessing = true;

				//--- Remember previously controlled entity
				IEntity controlledEntity = GetControlledEntity();
				m_MainEntityID = RplId.Invalid();
				if (controlledEntity)
				{
					RplComponent rpl = RplComponent.Cast(controlledEntity.FindComponent(RplComponent));
					if (rpl)
						m_MainEntityID = rpl.Id();
				}

				OnRplMainEntityFromID(); //--- ToDo: Remove? BumpMe should call it automatically.
				Replication.BumpMe();

				//-- Tell manager we're possessing an entity
				SCR_PossessingManagerComponent possessingManager = SCR_PossessingManagerComponent.GetInstance();
				if (possessingManager)
					possessingManager.SetMainEntity(GetPlayerId(), entity, controlledEntity, m_bIsPossessing);

				//--- Switch control
				RplComponent rpl = RplComponent.Cast(entity.FindComponent(RplComponent));
				if (rpl)
					rpl.GiveExt(GetRplIdentity(), false);
				SetAIActivation(entity, false);
				SetControlledEntity(entity);
				m_OnPossessed.Invoke(entity);
			}
		}
		else
		{
			if (!entity)
			{
				//--- Stop possessing
				m_bIsPossessing = false;

				//--- Forget main entity
				m_MainEntityID = RplId.Invalid();
				OnRplMainEntityFromID(); //--- ToDo: Remove?
				Replication.BumpMe();

				SCR_PossessingManagerComponent possessingManager = SCR_PossessingManagerComponent.GetInstance();
				if (possessingManager)
					possessingManager.SetMainEntity(GetPlayerId(), GetControlledEntity(), m_MainEntity, m_bIsPossessing);

				//--- Switch control
				IEntity controlledEntity = GetControlledEntity();
				if (controlledEntity)
				{
					RplComponent rpl = RplComponent.Cast(controlledEntity.FindComponent(RplComponent));
					if (rpl)
						rpl.GiveExt(RplIdentity.Local(), false);

					SetAIActivation(controlledEntity, true);
				}
				SetControlledEntity(m_MainEntity);
				m_OnPossessed.Invoke(m_MainEntity);

				//--- SetControlledEntity(null) doesn't work yet. ToDo: Remove this check once it's implemented
				if (GetControlledEntity() != m_MainEntity)
					Print(string.Format("Error when switching control back to m_MainEntity = %1!", m_MainEntity), LogLevel.WARNING);
			}
			else
			{
				//--- Switch possessing
				SetPossessedEntity(null);
				SetPossessedEntity(entity);
				m_OnPossessed.Invoke(entity);
			}
		}
	}

	//------------------------------------------------------------------------------------------------
	/*!
	Set intial main entity of a player, for a case where an existing entity should be assigned instead of spawning a new one
	\param entity is the subject entity
	*/
	void SetInitialMainEntity(notnull IEntity entity)
	{
		RplComponent rpl = RplComponent.Cast(entity.FindComponent(RplComponent));
		if (!rpl)
			return;

		m_MainEntityID = rpl.Id();
		OnRplMainEntityFromID();
		Replication.BumpMe();

		SCR_PossessingManagerComponent possessingManager = SCR_PossessingManagerComponent.GetInstance();
		if (possessingManager)
			possessingManager.SetMainEntity(GetPlayerId(), GetControlledEntity(), entity, m_bIsPossessing);

		rpl.GiveExt(GetRplIdentity(), false); // transfer ownership
		SetAIActivation(entity, false);
		SetControlledEntity(entity);

		m_OnPossessed.Invoke(entity);
	}

	//------------------------------------------------------------------------------------------------
	/*!
	Check if player is currently possessing an entity.
	\return True when possessing
	*/
	bool IsPossessing()
	{
		return m_bIsPossessing;
	}
	//------------------------------------------------------------------------------------------------
	/*!
	Get player's main entity.
	When not possessing, this will be the same as GetControlledEntity()
	When possessing, this will be player's main entity which was controlled before possessing started
	\return Main player entity
	*/
	IEntity GetMainEntity()
	{
		if (m_bIsPossessing)
			return m_MainEntity;
		else
			return GetControlledEntity();
	}
	//------------------------------------------------------------------------------------------------
	protected void OnRplMainEntityFromID()
	{
		//m_MainEntity = IEntity.Cast(Replication.FindItem(m_MainEntityID));
		RplComponent rpl = RplComponent.Cast(Replication.FindItem(m_MainEntityID));
		if (rpl)
			m_MainEntity = rpl.GetEntity();
	}
	//------------------------------------------------------------------------------------------------
	protected void SetAIActivation(IEntity entity, bool activate)
	{
		if (!entity)
			return;

		AIControlComponent aiControl = AIControlComponent.Cast(entity.FindComponent(AIControlComponent));
		if (!aiControl)
			return;

		if (activate)
			aiControl.ActivateAI();
		else
			aiControl.DeactivateAI();
	}

	//------------------------------------------------------------------------------------------------
	//! Returns either a valid ID of local player or 0
	static int GetLocalPlayerId()
	{
		PlayerController pPlayerController = GetGame().GetPlayerController();
		if (!pPlayerController)
			return 0;

		return pPlayerController.GetPlayerId();
	}

	//------------------------------------------------------------------------------------------------
	/*!
	Get entity controlled by player on this machine.
	\return Controlled entity
	*/
	static IEntity GetLocalControlledEntity()
	{
		PlayerController pPlayerController = GetGame().GetPlayerController();
		if (pPlayerController)
			return pPlayerController.GetControlledEntity();

		return null;
	}
	/*!
	Get player's main entity on this machine.
	When not possessing, this will be the same as GetControlledEntity()
	When possessing, this will be player's main entity which was controlled before possessing started
	\return Main player entity
	*/
	static IEntity GetLocalMainEntity()
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (playerController)
			return playerController.GetMainEntity();
		else
			return null;
	}

	//------------------------------------------------------------------------------------------------
	/*!
	Get faction of currently controlled local player entity.
	\return Faction
	*/
	static Faction GetLocalControlledEntityFaction()
	{
		PlayerController playerController = GetGame().GetPlayerController();
		if (!playerController)
			return null;

		IEntity controlledEntity = playerController.GetControlledEntity();
		if (!controlledEntity)
			return null;

		FactionAffiliationComponent factionAffiliation = FactionAffiliationComponent.Cast(controlledEntity.FindComponent(FactionAffiliationComponent));
		if (factionAffiliation)
			return factionAffiliation.GetAffiliatedFaction();
		else
			return null;
	}
	/*!
	Get faction of local player's main entity.
	When not possessing, the entity will be the same as GetControlledEntity()
	When possessing, the entity will be player's main entity which was controlled before possessing started
	\return Faction
	*/
	static Faction GetLocalMainEntityFaction()
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
		if (!playerController)
			return null;

		IEntity controlledEntity = playerController.GetMainEntity();
		if (!controlledEntity)
			return null;

		FactionAffiliationComponent factionAffiliation = FactionAffiliationComponent.Cast(controlledEntity.FindComponent(FactionAffiliationComponent));
		if (factionAffiliation)
			return factionAffiliation.GetAffiliatedFaction();
		else
			return null;
	}

	//------------------------------------------------------------------------------------------------
	override void OnDestroyed(IEntity killer)
	{
		m_OnDestroyed.Invoke(killer);
	}

	override void EOnFrame(IEntity owner, float timeSlice)
	{
		if (!s_pLocalPlayerController)
			UpdateLocalPlayerController();

		if (m_bIsLocalPlayerController)
		{
			UpdateControls();
			//UpdateUI();
		}
	}

	//! Find if this is local player controller. We assume that this never changes during scenario.
	protected void UpdateLocalPlayerController()
	{
		m_bIsLocalPlayerController = this == GetGame().GetPlayerController();
		if (!m_bIsLocalPlayerController)
			return;

		s_pLocalPlayerController = this;
		InputManager inputManager = GetGame().GetInputManager();
		if (!inputManager)
			return;

		inputManager.AddActionListener("WeaponChangeMagnification", EActionTrigger.VALUE, ChangeMagnification);
		inputManager.AddActionListener("CharacterWalk", EActionTrigger.DOWN, OnWalk);
		inputManager.AddActionListener("CharacterWalk", EActionTrigger.UP, OnEndWalk);
		inputManager.AddActionListener("FocusToggle", EActionTrigger.DOWN, ActionFocusToggle);
		inputManager.AddActionListener("FocusToggleUnarmed", EActionTrigger.DOWN, ActionFocusToggleUnarmed);
		inputManager.AddActionListener("Inventory", EActionTrigger.DOWN, ActionOpenInventory );
		inputManager.AddActionListener("TacticalPing", EActionTrigger.DOWN, ActionGesturePing );
		inputManager.AddActionListener("TacticalPingHold", EActionTrigger.DOWN, ActionGesturePingHold );
		inputManager.AddActionListener("TacticalPingHold", EActionTrigger.UP, ActionGesturePingHold );
		inputManager.AddActionListener("WeaponSwitchOptics", EActionTrigger.UP, ChangeWeaponOptics);
	}

	//! Update disabling of character controls in menus
	protected void UpdateControls()
	{
		bool disableControls = GetGame().GetMenuManager().IsAnyMenuOpen();
		if (m_bIsPaused != disableControls)
		{
			m_bIsPaused = disableControls;
			SetDisableControls(disableControls);
		}
	}

	protected void UpdateUI()
	{
		ChimeraCharacter char = ChimeraCharacter.Cast(GetControlledEntity());
		if (!char)
			return;
		CharacterAnimationComponent animComp = char.GetAnimationComponent();
		if (!animComp)
			return;
		// Command ladder is present only when character is using ladder
		CharacterCommandLadder ladderCMD = animComp.GetCommandHandler().GetCommandLadder();
		if (!ladderCMD)
			return;
		int lrExitState = ladderCMD.CanExitLR();
		if (lrExitState & 0x1)
		{
			Print("Can exit right");
		}
		if (lrExitState & 0x2)
		{
			Print("Can exit left");
		}
	}

	protected void ChangeMagnification(float value)
	{
		SCR_CharacterControllerComponent characterController = GetCharacterController();
		if (characterController)
			characterController.SetNextSightsFOVInfo(value);
	}

	protected void ChangeWeaponOptics()
	{
		SCR_CharacterControllerComponent characterController = GetCharacterController();
		if (characterController)
			characterController.SetNextSights();
	}

	protected SCR_CharacterControllerComponent GetCharacterController()
	{
		ChimeraCharacter char = ChimeraCharacter.Cast(GetControlledEntity());
		if (!char)
			return null;

		return SCR_CharacterControllerComponent.Cast(char.GetCharacterController());
	}

	// Parameter value:
	// TRUE:  Disables the controls
	// FALSE: Enables the controls
	private void SetDisableControls(bool value)
	{
		SCR_CharacterControllerComponent characterController = GetCharacterController();
		if (!characterController)
			return;

		characterController.SetDisableViewControls(value);
		characterController.SetDisableWeaponControls(value);
		characterController.SetDisableMovementControls(value)
	}

	override void OnPrepareTestCase(ActionManager am, float dt, IEntity characterEntity)
	{
		// condition from character controller
		if (characterEntity == null)
			return;
	}

	//------------------------------------------------------------------------------------------------
	/*! Focus input degree for analogue input
	\param adsProgress ADS focus percentage
	\param dt Delta time
	\return focus Amount of focus between 0 and 1
	*/
	float GetFocusValue(float adsProgress = 0, float dt = -1)
	{
		if (!m_CharacterController)
			return 0;

		float focus;

		// Autofocus
		if (adsProgress > 0)
			focus = s_fADSFocus * Math.Min(adsProgress, 1);

		InputManager inputManager = GetGame().GetInputManager();

		// Cancel toggled focus when focus is held
		bool inputDigital = inputManager.GetActionTriggered("Focus");
		if (inputDigital && m_bFocusToggle)
			m_bFocusToggle = false;

		// Conditions must be consistent with ActionFocusToggle and ActionFocusToggleUnarmed
		ChimeraCharacter character = m_CharacterController.GetCharacter();
		if (m_bFocusToggle && character)
		{
			if (character.IsInVehicle())
			{
				// Cancel toggle focus when in vehicle and aiming through gadget (binocular, compass)
				if (m_bGadgetFocus)
					m_bFocusToggle = false;

				// Cancel toggle focus when in vehicle and not in forced freelook
				if (!m_CharacterController.IsFreeLookEnforced())
					m_bFocusToggle = false;
			}
			else
			{
				// Cancel toggle focus when not in vehicle and holding item in hands
				if (m_CharacterController.GetCurrentItemInHands())
					m_bFocusToggle = false;

				// Cancel toggle focus when not in vehicle and holding gadget
				if (m_CharacterController.IsGadgetInHands())
					m_bFocusToggle = false;
			}
		}

		// Ground vehicles have different focus action to prevent conflict with brakes
		float inputAnalogue;
		if (!inputManager.IsContextActive("CarContext"))
			inputAnalogue = inputManager.GetActionValue("FocusAnalog");

		// Prevent focus warping back while toggling ADS on controller
		// analogue: track timeout as we have no input filter that has thresholds or delays and returns axis value yet
		if (inputAnalogue < FOCUS_THRESHOLD)
			s_fFocusTimeout = FOCUS_TIMEOUT;
		else if (dt > 0 && !m_bFocusToggle)
			s_fFocusTimeout -= dt;
		else
			s_fFocusTimeout = -1;

		// Cancel toggle focus with analogue input
		if (m_bFocusToggle && s_fFocusTimeout < 0)
			m_bFocusToggle = false;

		// Combine all valid input sources
		float input;
		if (m_bFocusToggle || inputDigital)
			input = 1;
		else if (s_fFocusTimeout < 0)
			input = inputAnalogue;

		if (input > 0)
			focus = Math.Max(focus, input);

		// Ensure return value always within 0-1
		focus = Math.Clamp(focus, 0, 1);
		return focus;
	}

	//------------------------------------------------------------------------------------------------
	//! Automatic focusing while gadget is aimed down sights
	void SetGadgetFocus(bool gadgetFocus)
	{
		m_bGadgetFocus = gadgetFocus;
	}

	//------------------------------------------------------------------------------------------------
	//! Automatic focusing while gadget is aimed down sights
	bool GetGadgetFocus()
	{
		return m_bGadgetFocus;
	}

	//------------------------------------------------------------------------------------------------
	void ActionFocusToggle(float value = 0.0, EActionTrigger reason = 0)
	{
		if (!m_CharacterController)
			return;

		// Conditions must be consistent with GetFocusValue logic
		ChimeraCharacter character = m_CharacterController.GetCharacter();
		if (character && !character.IsInVehicle())
			return;

		// Cancel toggle focus when in vehicle and aiming through gadget (binocular, compass)
		if (m_bGadgetFocus)
			return;

		// Cancel toggle focus when in vehicle and not in forced freelook
		if (!m_CharacterController.IsFreeLookEnforced())
			return;

		m_bFocusToggle = !m_bFocusToggle;
	}

	//------------------------------------------------------------------------------------------------
	void ActionFocusToggleUnarmed(float value = 0.0, EActionTrigger reason = 0)
	{
		if (!m_CharacterController)
			return;

		// Conditions must be consistent with GetFocusValue logic
		ChimeraCharacter character = m_CharacterController.GetCharacter();
		if (character && character.IsInVehicle())
			return;

		// Cancel toggle focus when not in vehicle and holding item in hands
		if (m_CharacterController.GetCurrentItemInHands())
			return;

		// Cancel toggle focus when not in vehicle and holding gadget
		if (m_CharacterController.IsGadgetInHands())
			return;

		// Allow cancelling unarmed focus while picking up items
		// Disallow enabling unarmed focus while picking up items, as it may become irreleant quickly
		// Reason is player may want to enter ADS before ready, while intent is unclear
		if (!m_bFocusToggle && m_CharacterController.IsPlayingItemGesture())
			return;

		m_bFocusToggle = !m_bFocusToggle;
	}

	//------------------------------------------------------------------------------------------------
	void OnWalk()
	{
		if (!m_CharacterController || m_CharacterController.GetDynamicSpeed() == WALK_SPEED)
			return;

		m_fCharacterSpeed = m_CharacterController.GetDynamicSpeed();
		m_CharacterController.SetDynamicSpeed(WALK_SPEED);
	}

	//------------------------------------------------------------------------------------------------
	void OnEndWalk()
	{
		if (!m_CharacterController || m_CharacterController.GetDynamicSpeed() == m_fCharacterSpeed)
			return;

		m_CharacterController.SetDynamicSpeed(m_fCharacterSpeed);
	}

	//------------------------------------------------------------------------------------------------
	void ActionOpenInventory()
	{
		IEntity entity = s_pLocalPlayerController.GetControlledEntity();
		if (!entity)
			return;

		SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(entity.FindComponent(SCR_InventoryStorageManagerComponent));
		if (inventory)
			inventory.Action_OpenInventory();
	}

	//------------------------------------------------------------------------------------------------
	void ActionGesturePing(float value = 0.0, EActionTrigger reason = 0)
	{
		if (!m_CharacterController)
			return;

		// Press and forget variant... eg press comma once - character will point with it's finger for 1 second (including blending time from animation graph ~300ms)
		m_CharacterController.TryStartCharacterGesture(ECharacterGestures.POINT_WITH_FINGER, 1500);
	}

	//------------------------------------------------------------------------------------------------
	void ActionGesturePingHold(float value = 0.0, EActionTrigger reason = 0)
	{
		if (!m_CharacterController)
			return;

		// Hold key variant... hold period - character will point with it's finger until period key is released
		if (reason == EActionTrigger.DOWN)
		{
			m_CharacterController.TryStartCharacterGesture(ECharacterGestures.POINT_WITH_FINGER);
		} else if ( reason == EActionTrigger.UP)
		{
			m_CharacterController.StopCharacterGesture();
		}
	}
};
