# Preparations

Entering bonus0 home directory we see a bonus0 binary with sticky bit in access rights.
By objdumping the binary we see 3 functions main, pp and p:

	bonus0@RainFall:~$ objdump -D -M intel-mnemonic bonus0 > /tmp/dump_bon0

Why do we do objdump first? 