ref array<string> Roles = {
	"administrator",
	"moderator",
	"user",
	"Guest/Visitor"
};

ref array<array<string>> Permissions = {
	{
		"administrator",
		
		"Manage Users",
		"Manage Roles",
		"Server Configuration",
		"Access Audit Logs",
		"Full Access"
	},
	{
		"moderator",
		
		"Manage Players",
		"Content Moderation",
		"Access Reports"
	},
	{
		"user",
		
		"Edit Profile",
		"Access Forums or Community Features",
		"View Server Information"
	},
	{
		"Guest/Visitor",
		
		"View Public Content",
		"Limited Access"
	}
};