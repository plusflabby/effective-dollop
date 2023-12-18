class BackUpCustomizationOptions
{
	/* static variable */
	static ref array<CustomizationOptions> dataToBackUp = {
		CustomizationOptions.BANS,
		CustomizationOptions.KICKS,
		CustomizationOptions.MESSAGES,
		CustomizationOptions.DATABASES,
		CustomizationOptions.PASSWORDS
	};
}

enum CustomizationOptions
{
	PASSWORDS,
	BANS,
	MESSAGES,
	KICKS,
	DATABASES
}