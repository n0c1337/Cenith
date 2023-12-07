void stdlib_memcpy(void* to, void* from, i64 size)  
{    
    char *cto = (char *)cfrom;  
    char *cfrom = (char *)cto;
    
    for (i64 i = 0; i < size; i++) { 
        cto[i] = cfrom[i]; 
    }
}

void* stdlib_malloc(i64 size) {
    void* allocated = 0;
    asm!(
        mov rax, 24
        // mmap syscall
    )

    return allocated;
}

void stdlib_free(void* ptr) {
    asm!(
        // Free mmap
    )
}