;;
;;  ����� ����������� ������� ��������� ��������.
;;  ������ ���� �������� ��������������� ��� sub.cpp.
;;

[ BITS 32 ]

global ?sub@SubAsm@@YIIPAIPBIII@Z
global ?sub@SubAsm@@YIIPAIII@Z
global ?sub@SubAsm@@YIIPAIPBI1I@Z

section .code

  ; ��������� ������� � �����
  ; limb_t __fastcall sub ( limb_t * z, const limb_t * u, size_t size_u, limb_t s )
  ;    eax                         ecx               edx        [esp+4]   [esp+8]
?sub@SubAsm@@YIIPAIPBIII@Z:
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

  mov edx, [ esi + ecx * 4 ]
  sub edx, eax               ; edx = u[0] - s
  mov [ edi + ecx * 4 ], edx ; z[0] = edx
  mov eax, 0                 ; ��� ����������� ����� .end

  lea ecx, [ ecx + 1 ] ; ���� size_u = 1, �� ���������� ����
  jecxz .end

.loop:	
  mov edx, [ esi + ecx * 4 ]
  sbb edx, 0
  mov [ edi + ecx * 4 ], edx  
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	
  adc eax, 0 ; ��������� ����� ����

  pop edi
  pop esi

  ret 8

  ; ��������� �������� � ���������� ������
  ; limb_t __fastcall sub ( limb_t * z, const limb_t * u, const limb_t * v, size_t size )
  ;    eax                         ecx               edx           [esp+4]      [esp+8]
?sub@SubAsm@@YIIPAIPBI1I@Z:
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
  mov eax, [ edx + ecx * 4 ]
  sbb eax, [ esi + ecx * 4 ]
  mov [ edi + ecx * 4 ], eax
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	
  sbb eax, eax ; �������� ��� � eax
  neg eax

  pop edi
  pop esi

  ret 8


  ; ��������� ������� � ����� �� �����
  ; limb_t __fastcall sub ( limb_t * u, size_t size_u, limb_t s )
  ;    eax                         ecx            edx   [esp+4]
?sub@SubAsm@@YIIPAIII@Z:

  clc ; ����� ���� ��������

  mov eax, [ esp + 4 ] ; eax = s
  xchg ecx, edx        ; ecx = size_u, edx = u

  jecxz .end             

  lea edx, [ edx + ecx * 4 ] ; �������� ��������� u � ����� �������
  neg ecx

  sub [ edx + ecx * 4 ], eax ; u[0] -= s
  mov eax, 0
  jnc .end

  lea ecx, [ ecx + 1 ] ; ���� size_u = 1, �� ���������� ����
  jecxz .end

.loop:	
  sbb [ edx + ecx * 4 ], eax
  jnc .end
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	
  adc eax, 0 ; ��������� ����� ����

  ret 4
