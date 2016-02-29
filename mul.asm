;;
;;  ����� ����������� ������� ��������� ��������.
;;  ������ ���� �������� ��������������� ��� mul.cpp.
;;

[ BITS 32 ]

global ?mul_N_by_1@MulAsm@@YIIPAIPBIII@Z
global ?mul_N_by_1@MulAsm@@YIIPAIII@Z

section .code

  ; ��������� ������� �� �����
  ; limb_t __fastcall mul_N_by_1 ( limb_t * z, const limb_t * u, size_t size, limb_t v );
  ;    eax                                ecx               edx      [esp+4]   [esp+8]
?mul_N_by_1@MulAsm@@YIIPAIPBIII@Z:
  push esi
  push edi
  push ebx

  clc ; ����� ���� ��������

  mov edi, ecx	           ; edi = z
  mov esi, edx	           ; esi = u  
  mov ecx, [ esp + 4 + 12 ] ; ecx = size
  
  jecxz .end

  lea esi, [ esi + ecx * 4 ] ; �������� ��������� u � ����� �������
  lea edi, [ edi + ecx * 4 ] ; �������� ��������� z � ����� �������
  neg ecx                    ; i = -size - �������� ���� �� -size+1 �� 0

  xor ebx, ebx

.loop:	
  mov eax, [ esi + ecx * 4 ]  
  mul dword [ esp + 8 + 12 ]
  add eax, ebx
  adc edx, 0
  mov [ edi + ecx * 4 ], eax
  mov ebx, edx
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	

  mov eax, ebx

  pop ebx
  pop edi
  pop esi

  ret 8

  ; ��������� ������� �� ���� �� �����
  ; limb_t __fastcall mul_N_by_1 ( limb_t * u, size_t size, limb_t v );
  ;    eax                                ecx          edx   [esp+4]
?mul_N_by_1@MulAsm@@YIIPAIII@Z:
  push esi
  push edi
  push ebx

  clc ; ����� ���� ��������

  mov esi, ecx	           ; edi = z
  mov ecx, edx ; ecx = size
  mov edi, [ esp + 4 + 12 ] ; edi = v

  jecxz .end

  lea esi, [ esi + ecx * 4 ] ; �������� ��������� u � ����� �������
  neg ecx                    ; i = -size - �������� ���� �� -size+1 �� 0

  xor ebx, ebx

.loop:	
  mov eax, [ esi + ecx * 4 ]  
  mul edi
  add eax, ebx
  adc edx, 0
  mov [ esi + ecx * 4 ], eax
  mov ebx, edx
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	

  mov eax, ebx

  pop ebx
  pop edi
  pop esi

  ret 4
