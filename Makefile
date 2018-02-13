all: myshell

myshell:shell.c
	gcc shell.c -o myshell
    
clear:
	rm myshell
	
