push rbp
mov rbp, rsp
push rbx
push rsi
push rdi
inc rdi
dec rdi
inc byte [rdi]
dec byte [rdi]
cmp byte [rdi], 0