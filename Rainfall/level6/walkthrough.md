# Preparations

Entering level6 home directory we see a level6 binary with sticky bit in access rights.
By objdumping the binary we see 3 functions: main, m and n:

	level6@RainFall:~$ objdump -D -M intel-mnemo