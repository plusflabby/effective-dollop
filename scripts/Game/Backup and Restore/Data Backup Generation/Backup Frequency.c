class BackUpFrequency
{
	/* static variable */
	static Frequency frequencyForBackupInMinutes = Frequency.DAILY;
}

enum Frequency
{
	DAILY,
	WEEKLY,
	MONTHLY,
	YEARLY
}