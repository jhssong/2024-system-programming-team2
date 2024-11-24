#include "common.h"

userinfo user_info;
teaminfo team_info;
teamlist team_list;
short user_table[TABLE_MAX_TIME][TABLE_MAX_DAY];

void init_global_variable() {
	memset(&user_info, 0, sizeof(userinfo));
	memset(&team_info, 0, sizeof(teaminfo));
	memset(&team_list, 0, sizeof(teamlist));

	strncpy(team_info.team_pw, "0", MAX_PW_SIZE);
}