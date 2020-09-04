# CyberSecurity_42school
Projects for deep diving into reverse engineering, learning to reconstruct a code, and understanding it to detect faults.

# General terms
We are given an image ISO for VMV(64 bits). In order to make the following projects we used VirtualBox as virtulization tool, created there virtual machines Ubuntu (64bit) with standard characteristics (suggested by VirtualBox while creating the machine) and minimal memory because it is not needed in the projects. Mounted ISO in Machine "Settings -> Devices -> Controller: IDE" and set up network in Machine "Settings -> Network" choosing "Virtual Host Adapter" that we created in VirtualBox "File -> Host Network Manager" for all virtual machines before.\

By entering the machine we see:

	% ssh level00@192.168.99.10