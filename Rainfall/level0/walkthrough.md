# Preparations

Entering level0 home directory we see a level0 binary with sticky bit in access rights.

# Code writing

We start to work with gdb in Intel syntax (set disassembly-flavor intel):

	(gdb) disass main
	Dump of assembler c