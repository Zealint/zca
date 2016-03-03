;;
;;  Здесь реализованы функции умножения векторов.
;;  Данный файл является вспомогательным для mul.cpp.
;;

[ BITS 32 ]

global ?mul_N_by_1@MulAsm@@YIIPAIPBIII@Z
global ?mul_N_by_1@MulAsm@@YIIPAIII@Z

section .code

  ; Умножение вектора на лимба
  ; limb_t __fastcall mul_N_by_1 ( limb_t * z, const limb_t * u, size_t size, limb_t v );
  ;    eax                                ecx               edx      [esp+4]   [esp+8]
?mul_N_by_1@MulAsm@@YIIPAIPBIII@Z:
  push esi
  push edi
  push ebx

  mov edi, ecx              ; edi = z
  mov esi, edx              ; esi = u  
  mov ecx, [ esp + 4 + 12 ] ; ecx = size
  sub esi, edi              ; Теперь [esi+edi] = u[0], [edi] = z[0]
    
  pxor xmm0, xmm0
  movd xmm0, dword [ esp + 8 + 12 ]
  movlhps xmm0, xmm0        ; xmm0 = (0,v,0,v)

  shr ecx, 3                ; Развернём цикл 8 раз
  mov [ esp + 8 + 12 ], ecx ; Временно сохраним здесь число итераций
  xor eax, eax              ; Нужен ноль, а заодно сброс флага переноса
  test ecx, ecx             ; Если ноль итераций
  jz .end8
    
.loop8:  
  
  movdqu xmm4, [ esi + edi ]  ; Считываем 8 значений u (возможно, данные не выровнены)
  movdqu xmm6, [ esi + edi + 16 ]
  movdqa xmm5, xmm4           ; Копируем для будущего умножения
  movdqa xmm7, xmm6
  unpcklps xmm4, xmm4         ; xmm4 = (?, u1, ?, u0)
  unpckhps xmm5, xmm5         ; xmm5 = (?, u3, ?, u2)
  unpcklps xmm6, xmm6         ; xmm6 = (?, u5, ?, u4)
  unpckhps xmm7, xmm7         ; xmm7 = (?, u7, ?, u6)
  pmuludq xmm4, xmm0          ; Перемножаем все 8 значений
  pmuludq xmm5, xmm0
  pmuludq xmm6, xmm0
  pmuludq xmm7, xmm0

  pextrd ebx, xmm4, 0         ; Дальше идёт каскадное сложение
  pextrd ecx, xmm4, 1
  pextrd edx, xmm4, 2
  adc eax, ebx  
  adc ecx, edx                ; Здесь ecx взят из предыдущей итерации
  mov [ edi ], eax
  mov [ edi + 4 ], ecx
  
  pextrd eax, xmm4, 3
  pextrd ebx, xmm5, 0  
  pextrd ecx, xmm5, 1
  pextrd edx, xmm5, 2
  adc eax, ebx  
  adc ecx, edx
  mov [ edi + 8 ], eax
  mov [ edi + 12 ], ecx
  
  pextrd eax, xmm5, 3      
  pextrd ebx, xmm6, 0  
  pextrd ecx, xmm6, 1
  pextrd edx, xmm6, 2
  adc eax, ebx  
  adc ecx, edx
  mov [ edi + 16 ], eax
  mov [ edi + 20 ], ecx
  
  pextrd eax, xmm6, 3
  pextrd ebx, xmm7, 0  
  pextrd ecx, xmm7, 1
  pextrd edx, xmm7, 2
  adc eax, ebx
  adc ecx, edx
  mov [ edi + 24 ], eax
  mov [ edi + 28 ], ecx
  
  pextrd eax, xmm7, 3         ; Перенос в следующую итерацию
  
  lea edi, [ edi + 32 ]
  
  dec dword [ esp + 8 + 12 ]  ; Здесь временно находится счётчик
  jnz .loop8

.end8:
  mov ebx, 0
  adc ebx, eax
  movd dword [ esp + 8 + 12 ], xmm0 ; Вернуть v на место
  mov ecx, [ esp + 4 + 12 ]         ; ecx = size
  and ecx, 7                        ; Оставшиеся итерации
  jecxz .end 

.loop:  
  mov eax, [ esi + edi ]     ; u[i]
  mul dword [ esp + 8 + 12 ] ; u[i]*v
  add eax, ebx
  adc edx, 0
  mov [ edi ], eax
  mov ebx, edx
  lea edi, [ edi + 4 ]
  loop .loop           

.end: 

  mov eax, ebx

  pop ebx
  pop edi
  pop esi

  ret 8

  ; Умножение вектора на лимб на месте
  ; limb_t __fastcall mul_N_by_1 ( limb_t * u, size_t size, limb_t v );
  ;    eax                                ecx          edx   [esp+4]
?mul_N_by_1@MulAsm@@YIIPAIII@Z:
  push esi
  push edi
  push ebx

  clc ; Снять флаг переноса

  mov esi, ecx              ; edi = z
  mov ecx, edx              ; ecx = size
  mov edi, [ esp + 4 + 12 ] ; edi = v

  xor ebx, ebx

  jecxz .end

  lea esi, [ esi + ecx * 4 ] ; Сдвинуть указатель u к концу вектора
  neg ecx                    ; i = -size - начинаем цикл от -size+1 до 0

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
