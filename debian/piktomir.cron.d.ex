#
# Regular cron jobs for the piktomir package
#
0 4	* * *	root	[ -x /usr/bin/piktomir_maintenance ] && /usr/bin/piktomir_maintenance
