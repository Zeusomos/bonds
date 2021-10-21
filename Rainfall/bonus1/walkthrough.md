
# Preparations

Entering bonus1 home directory we see a bonus1 binary with sticky bit in access rights.
By objdumping the binary we see a main function:

	bonus1@RainFall:~$ objdump -D -M intel-mnemonic bonus1 > /tmp/dump_bon1

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Exploitation

There is a variable `numb` that takes a value after `atoi` of argument sent to the binary. And if this value equals address 0x574f4c46(\x46\x4c\x4f\x57 in little-endian), we get the terminal under `bonus2` user. `Atoi` can give back a negative value if there is minus before the numeric value in the string. We have a term that `numb` should be less than 9 so we give a negative number that in `memcpy` become `size_t` and `INT_MIN(-2147483648) == 0u`. So `INT_MIN` plus 11 (-2147483637) will be 11u. Finally we give -2147483637, 40 bytes of rubbish for alignment and address \x46\x4c\x4f\x57 that we need.

# Result