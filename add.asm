;;
;;  ����� ����������� ������� �������� ��������.
;;  ������ ���� �������� ��������������� ��� add.cpp.
;;

[ BITS 32 ]

global ?add@AddAsm@@YIIPAIPBIII@Z
global ?add@AddAsm@@YIIPAIPBI1I@Z
global ?add@AddAsm@@YIIPAIII@Z

section .code

  ; �������� ������� � �����
  ; limb_t __fastcall add ( limb_t * z, const limb_t * u, size_t size_u, limb_t s )
  ;    eax                         ecx               edx        [esp+4]   [esp+8]
?add@AddAsm@@YIIPAIPBIII@Z:
  push esi
  push edi

  clc ; ����� ���� ��������

  mov edi, ecx	           ; edi = z
  mov esi, edx	           ; esi = u  
  mov eax, [ esp + 8 + 8 ] ; eax = s
  mov ecx, [ esp + 4 + 8 ] ; ecx = size_u

  jecxz .end             

  lea esi, [ esi + ecx * 4 ] ; �������� ��������� u � ����� �������
  lea edi, [ edi + ecx * 4 ] ; �������� ��������� z � ����� �������
  neg ecx                    ; i = -size_u - �������� ���� �� -size_u+1 �� 0

  add eax, [ esi + ecx * 4 ] ; eax = u[0] + s
  mov [ edi + ecx * 4 ], eax ; z[0] = eax
  mov eax, 0                 ; ��� ����������� ����� .end

  lea ecx, [ ecx + 1 ] ; ���� size_u = 1, �� ���������� ����
  jecxz .end

.loop:	
  mov edx, [ esi + ecx * 4 ]
  adc edx, 0
  mov [ edi + ecx * 4 ], edx  
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	
  adc eax, 0 ; ��������� ����� ��������

  pop edi
  pop esi

  ret 8

  ; �������� �������� � ���������� ������
  ; limb_t __fastcall add ( limb_t * z, const limb_t * u, const limb_t * v, size_t size )
  ;    eax                         ecx               edx           [esp+4]      [esp+8]
?add@AddAsm@@YIIPAIPBI1I@Z:
  push esi
  push edi

  mov edi, ecx	            ; edi = z
  mov esi, [ esp + 4 + 8 ]	; esi = v
  mov ecx, [ esp + 8 + 8 ]  ; ecx = size

  ; �������� ��������� � ����� ��������
  lea edi, [ edi + ecx * 4 ]
  lea esi, [ esi + ecx * 4 ]
  lea edx, [ edx + ecx * 4 ]
  neg ecx ; i = -n. 
  jz .end ; ���� ����� ���� �������
  clc

.loop:	
  mov eax, [ esi + ecx * 4 ]
  adc eax, [ edx + ecx * 4 ]
  mov [ edi + ecx * 4 ], eax
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	
  sbb eax, eax ; �������� ������� � eax
  neg eax

  pop edi
  pop esi

  ret 8


  ; �������� ������� � ����� �� �����
  ; limb_t __fastcall add ( limb_t * u, size_t size_u, limb_t s )
  ;    eax                         ecx            edx   [esp+4]
?add@AddAsm@@YIIPAIII@Z:

  clc ; ����� ���� ��������

  mov eax, [ esp + 4 ] ; eax = s
  xchg ecx, edx        ; ecx = size_u, edx = u

  jecxz .end             

  lea edx, [ edx + ecx * 4 ] ; �������� ��������� u � ����� �������
  neg ecx

  add [ edx + ecx * 4 ], eax ; u[0] += s
  mov eax, 0
  jnc .end

  lea ecx, [ ecx + 1 ] ; ���� size_u = 1, �� ���������� ����
  jecxz .end

.loop:	
  adc [ edx + ecx * 4 ], eax
  jnc .end
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	
  adc eax, 0 ; ��������� ����� ��������

  ret 4
