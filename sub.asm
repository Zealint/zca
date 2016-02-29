;;
;;  Здесь реализованы функции вычитания векторов.
;;  Данный файл является вспомогательным для sub.cpp.
;;

[ BITS 32 ]

global ?sub@SubAsm@@YIIPAIPBIII@Z
global ?sub@SubAsm@@YIIPAIII@Z
global ?sub@SubAsm@@YIIPAIPBI1I@Z

section .code

  ; Вычитание вектора и лимба
  ; limb_t __fastcall sub ( limb_t * z, const limb_t * u, size_t size_u, limb_t s )
  ;    eax                         ecx               edx        [esp+4]   [esp+8]
?sub@SubAsm@@YIIPAIPBIII@Z:
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

  mov edx, [ esi + ecx * 4 ]
  sub edx, eax               ; edx = u[0] - s
  mov [ edi + ecx * 4 ], edx ; z[0] = edx
  mov eax, 0                 ; Это понадобится после .end

  lea ecx, [ ecx + 1 ] ; Если size_u = 1, то пропускаем цикл
  jecxz .end

.loop:	
  mov edx, [ esi + ecx * 4 ]
  sbb edx, 0
  mov [ edi + ecx * 4 ], edx  
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	
  adc eax, 0 ; Результат равен заёму

  pop edi
  pop esi

  ret 8

  ; Вычитание векторов с одинаковой длиной
  ; limb_t __fastcall sub ( limb_t * z, const limb_t * u, const limb_t * v, size_t size )
  ;    eax                         ecx               edx           [esp+4]      [esp+8]
?sub@SubAsm@@YIIPAIPBI1I@Z:
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
  mov eax, [ edx + ecx * 4 ]
  sbb eax, [ esi + ecx * 4 ]
  mov [ edi + ecx * 4 ], eax
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	
  sbb eax, eax ; Получить заём в eax
  neg eax

  pop edi
  pop esi

  ret 8


  ; Вычитание вектора и лимба на месте
  ; limb_t __fastcall sub ( limb_t * u, size_t size_u, limb_t s )
  ;    eax                         ecx            edx   [esp+4]
?sub@SubAsm@@YIIPAIII@Z:

  clc ; Снять флаг переноса

  mov eax, [ esp + 4 ] ; eax = s
  xchg ecx, edx        ; ecx = size_u, edx = u

  jecxz .end             

  lea edx, [ edx + ecx * 4 ] ; Сдвинуть указатель u к концу вектора
  neg ecx

  sub [ edx + ecx * 4 ], eax ; u[0] -= s
  mov eax, 0
  jnc .end

  lea ecx, [ ecx + 1 ] ; Если size_u = 1, то пропускаем цикл
  jecxz .end

.loop:	
  sbb [ edx + ecx * 4 ], eax
  jnc .end
  lea ecx, [ ecx + 1 ]
  jecxz .end          
  jmp .loop           

.end:	
  adc eax, 0 ; Результат равен заёму

  ret 4
