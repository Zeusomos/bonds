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
	le