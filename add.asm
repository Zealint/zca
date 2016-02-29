;;
;;  Здесь реализованы функции сложения векторов.
;;  Данный файл является вспомогательным для add.cpp.
;;

[ BITS 32 ]

global ?add@AddAsm@@YIIPAIPBIII@Z
global ?add@AddAsm@@YIIPAIPBI1I@Z
global ?add@AddAsm@@YIIPAIII@Z

section .code

  ; Сложение вектора и лимба
  ; limb_t __fastcall add ( limb_t * z, const limb_t * u, size_t size_u, limb_t s )
  ;    eax                         ecx               edx        [esp+4]   [esp+8]
?add@AddAsm@@YIIPAIPBIII@Z:
  push esi
  push edi

  clc ; Снять флаг переноса

  mov edi, ecx	           ; edi = z
  mov esi, edx	           ; esi = u  
  mov eax, [ esp + 8 + 8 ] ; eax = s
  mov ecx, [ esp + 4 + 8 ] ; ecx = size_u

  jecxz .end             

  lea esi, [ esi + ecx * 4 ] ; Сдвинуть указатель u к концу вектора
  lea edi, [ edi + ecx * 4 ] ; Сдвинуть указатель z к концу вектора
  neg ecx                    ; i = -size_u - начинаем цикл от -size_u+1 до 0

  add eax, [ esi + ecx * 4 ] ; eax = u[0] + s
  mov [ edi + ecx * 4 ], eax ; z[0] = eax
  mov eax, 0                 ; Это понадобится после .end

  lea ecx, [ ecx + 1 ] ; Если size_u = 1, то пропускаем цикл
  jecxz .end

.loop:	
  mov edx, [ esi + ecx * 4 ]
  adc edx, 0
  mov [ edi + ecx * 4 ], edx  
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	
  adc eax, 0 ; Результат равен переносу

  pop edi
  pop esi

  ret 8

  ; Сложение векторов с одинаковой длиной
  ; limb_t __fastcall add ( limb_t * z, const limb_t * u, const limb_t * v, size_t size )
  ;    eax                         ecx               edx           [esp+4]      [esp+8]
?add@AddAsm@@YIIPAIPBI1I@Z:
  push esi
  push edi

  mov edi, ecx	            ; edi = z
  mov esi, [ esp + 4 + 8 ]	; esi = v
  mov ecx, [ esp + 8 + 8 ]  ; ecx = size

  ; Сдвинуть указатели к концу массивов
  lea edi, [ edi + ecx * 4 ]
  lea esi, [ esi + ecx * 4 ]
  lea edx, [ edx + ecx * 4 ]
  neg ecx ; i = -n. 
  jz .end ; Если длина была нулевой
  clc

.loop:	
  mov eax, [ esi + ecx * 4 ]
  adc eax, [ edx + ecx * 4 ]
  mov [ edi + ecx * 4 ], eax
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	
  sbb eax, eax ; Получить перенос в eax
  neg eax

  pop edi
  pop esi

  ret 8


  ; Сложение вектора и лимба на месте
  ; limb_t __fastcall add ( limb_t * u, size_t size_u, limb_t s )
  ;    eax                         ecx            edx   [esp+4]
?add@AddAsm@@YIIPAIII@Z:

  clc ; Снять флаг переноса

  mov eax, [ esp + 4 ] ; eax = s
  xchg ecx, edx        ; ecx = size_u, edx = u

  jecxz .end             

  lea edx, [ edx + ecx * 4 ] ; Сдвинуть указатель u к концу вектора
  neg ecx

  add [ edx + ecx * 4 ], eax ; u[0] += s
  mov eax, 0
  jnc .end

  lea ecx, [ ecx + 1 ] ; Если size_u = 1, то пропускаем цикл
  jecxz .end

.loop:	
  adc [ edx + ecx * 4 ], eax
  jnc .end
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	
  adc eax, 0 ; Результат равен переносу

  ret 4
