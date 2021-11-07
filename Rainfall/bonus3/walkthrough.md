# Preparations

Entering bonus3 home directory we see a bonus3 binary with sticky bit in access rights.
By objdumping the binary we see a main function:

	bonus3@RainFall:~$ objdump -D -M intel-mnemonic bonus3 > /tmp/dump_bon3

Why do we do objdump first? Maybe in this exercise a function exists but is not used so we need to see all the functions.

# Exploitation

Here we have work with an argument and opportunity to use shell from the binary if a comparison is right. If we look on the binary and how buffer is filled in, we see that to some part of the buffer that should be result of `atoi` of the first argument we need to put '\0'. And after we will compare the beginning of the buffer and argv[1]. That means that with the help of `atoi` we need to put '\0' to the beginning of the buffer that argv[1] should also be '\0'. So we give to the binary empty argument.

# Result

    bonus3@RainFall:~$ ./bonus3 ""
    $ whoami
    end
    $ cat /home/user/end/.pass
    3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
