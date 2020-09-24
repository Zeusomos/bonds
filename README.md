# CyberSecurity_42school
Projects for deep diving into reverse engineering, learning to reconstruct a code, and understanding it to detect faults.

# General terms
We are given an image ISO for VMV(64 bits). In order to make the following projects we used VirtualBox as virtulization tool, created there virtual machines Ubuntu (64bit) with standard characteristics (suggested by VirtualBox while creating the machine) and minimal memory because it is not needed in the projects. Mounted ISO in Machine "Settings -> Devices -> Controller: IDE" and set up network in Machine "Settings -> Network" choosing "Virtual Host Adapter" that we created in VirtualBox "File -> Host Network Manager" for all virtual machines before.\

By entering the machine we see:

	% ssh level00@192.168.99.106 -p 4242
		   _____                      _____               _
	  / ____|                    / ____|             | |
	 | (___  _ __   _____      _| |     _ __ __ _ ___| |__
	  \___ \| '_ \ / _ \ \ /\ / / |    | '__/ _` / __| '_ \
	  ____) | | | | (_) \ V  V /| |____| | | (_| \__ \ | | |
	 |_____/|_| |_|\___/ \_/\_/  \_____|_|  \__,_|___/_| |_|

  	Good luck & Have fun

          192.168.99.106
	level00@192.168.99.106's password: level00

And:


	% ssh level0@192.168.99.103 -p 4242
	  _____       _       ______    _ _
	 |  __ \     (_)     |  ____|  | | |
	 | |__) |__ _ _ _ __ | |__ __ _| | |
	 |  _  /  _` | | '_ \|  __/ _` | | |
	 | | \ \ (_| | | | | | | | (_| | | |
	 |_|  \_\__,_|_|_| |_|_|  \__,_|_|_|

                 Good luck & Have fun

  	To start, ssh with level0/level0 on 192.168.99.103:4242
	level0@192.168.99.103's password: level0

It is more convenient to use local console and connect via SSH. In the projects we use different utils, create files a bit and so on.\
Our main goal is to use everything given in the ISO to find exploitations for the current user and reach next user password file or user untill we come to the last one. So each level is a user:

	level00:x:2000:2000::/home/user/level00:/bin/bash
	level01:x:2001:2001::/home/user/level01:/bin/bash
	level02:x:2002:2002::/home/user/level02:/bin/bash
	level03:x:2003:2003::/home/user/level03:/bin/bash
	level04:x:2004:2004::/home/user/level04:/bin/bash
	level05:x:2005:2005::/home/user/level05:/bin/bash
	level06:x:2006:2006::/home/user/level06:/bin/bash
	level07:x:2007:2007::/home/user/level07:/bin/bash
	level08:x:2008:2008::/home/user/level08:/bin/bash
	level09:x:2009:2009::/home/user/level09:/bin/bash
	level10:x:2010:2010::/home/user/level10:/bin/bash
	level11:x:2011:2011::/home/user/level11:/bin/bash
	level12:x:2012:2012::/home/user/level12:/bin/bash
	level13:x:2013:2013::/home/user/level13:/bin/bash
	level14:x:2014:2014::/home/user/level14:/bin/bash
	flag00:x:3000:3000::/home/flag/flag00:/bin/bash
	flag01:42hDRfypTqqnw:3001:3001::/home/flag/flag01:/bin/bash
	flag02:x:3002:3002::/home/flag/flag02:/bin/bash
	flag03:x:3003:3003::/home/flag/flag03:/bin/bash
	flag04:x:3004:3004::/home/flag/flag04:/bin/bash
	flag05:x:3005:3005::/home/flag/flag05:/bin/bash
	flag06:x:3006:3006::/home/flag/flag06:/bin/bash
	flag07:x:3007:3007::/home/flag/flag07:/bin/bash
	flag08:x:3008:3008::/home/flag/flag08:/bin/bash
	flag09:x:3009:3009::/home/flag/flag09:/bin/bash
	flag10:x:3010:3010::/home/flag/flag10:/bin/bash
	flag11:x:3011:3011::/home/flag/flag11:/bin/bash
	flag12:x:3012:3012::/home/flag/flag12:/bin/bash
	flag13:x:3013:3013::/home/flag/flag13:/bin/bash
	flag14:x:3014:3014::/home/flag/flag14:/bin/bash

And:

	level0:x:2020:2020::/home/user/level0:/bin/bash
	level1:x:2030:2030::/home/user/level1:/bin/bash
	level2:x:2021:2021::/home/user/level2:/bin/bash
	level3:x:2022:2022::/home/user/level3:/bin/bash
	level4:x:2025:2025::/home/user/level4:/bin/bash
	level5:x:2045:2045::/home/use