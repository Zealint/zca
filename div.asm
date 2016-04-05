;;
;;  Здесь реализованы функции деления векторов.
;;  Данный файл является вспомогательным для div.cpp.
;;

[ BITS 32 ]

global ?div_2_by_1@DivAsm@@YIIAAIIII@Z
global ?div_2_by_1_pre@DivAsm@@YIIAAIIIII@Z
global ?inv_2_by_1@DivAsm@@YIII@Z

section .code

  ; Деление 2/1
  ; limb_t div_2_by_1 (limb_t &q, limb_t u1, limb_t u0, limb_t d);
  ;    eax                   ecx        edx    [esp+4]   [esp+8]
?div_2_by_1@DivAsm@@YIIAAIIII@Z:

  mov eax, [ esp + 4 ] ; edx:eax = (u1,u0)
  div dword [ esp + 8 ]
  mov [ ecx ], eax
  mov eax, edx

  ret 8

  ; Деление 2/1 c предподсчитанным обратным элементом
  ; limb_t div_2_by_1_pre (limb_t &q, limb_t u1, limb_t u0, limb_t d, limb_t v);
  ;    eax                       ecx        edx    [esp+4]   [esp+8]  [esp+12]
?div_2_by_1_pre@DivAsm@@YIIAAIIIII@Z:

  push ebx
  push esi
  push edi

  mov edi, edx
  mov eax, edx
  mov esi, [ esp + 4 + 12 ]
  mov ebx, [ esp + 8 + 12 ]
  mul dword [ esp + 12 + 12 ]
  lea eax, [ edi + edx ]  
  movd xmm0, eax  
  mul ebx  
  sub esi, eax   
  movd eax, xmm0
  sbb edi, edx

  test edi, edi  
  jb .end
  ja .loop
  cmp esi, ebx  
  jb .end
  nop
.loop:
  inc eax  
  sub esi, ebx  
  sbb edi, 0  
  test edi, edi  
  ja .loop
  jb .end
  cmp esi, ebx  
  jae .loop
.end:

  mov [ ecx ], eax
  mov eax, esi
  
  pop edi
  pop esi
  pop ebx

  ret 12

  ; Обратный элемент 2/1
  ; limb_t inv_2_by_1 ( limb_t d );
  ;    eax                   ecx
?inv_2_by_1@DivAsm@@YIII@Z:
  
  or eax, -1
  or edx, -1
  sub edx, ecx
  div ecx
  
  ret 0
