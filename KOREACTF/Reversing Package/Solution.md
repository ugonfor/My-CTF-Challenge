# Reversing_Package

작성자 및 출제자 : **CyKor 유효곤**

### **목차**

1. Anti-Decompiling
2. Anti-Debug (ptrace)
3. Code Patching
4. Anti-Debug (breakpoint)

## Solution

### Anti-Decompiling

Main함수를 살펴보면 IDA가 graph나 decompile을 못하고 있는 것을 확인할 수 있다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled.png)

이는 아래 사진을 확인해보면, xor_data함수를 기점으로 코드가 망가져있는 것을 확인할 수 있는 데 이것이 원인이다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%201.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%201.png)

그래서 망가진 부분들을 다 nop로 바꿔주고, 마지막에 leave ret으로 calling convention을 맞춰줘서 디컴파일 결과를 확인해보자.

그러면 다음과 같다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%202.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%202.png)

xor_data함수가 실행된 이후에는 nop로 패치를 했기에 실제로는 loc_400cc4부터 계속해서 실행될 것이다.

이때, xor_data함수는 두번째 인자값을 key로 하여 첫번째 인자값으로 받은 주소부터 세번째 인자값만큼 xor을 하게 된다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%203.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%203.png)

즉, code영역이 입력받은 값을 키로 하여 xor이 이뤄지는 것을 확인할 수 있다. 그렇기에 Decompile이 잘 안되었고, 이 값을 잘 맞춰주면, Decompile이 이뤄질 것이다.

### Anti-Debug (ptrace)

분석을 위해 gdb로 파일을 실행해보면, 바로 종료되는 것을 확인할 수 있다.

안티디버깅이 되어있는 것이라 예측 가능하다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%204.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%204.png)

_init_array를 확인해보면 함수가 정의되어 있는 것을 확인할 수 있다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%205.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%205.png)

정의되어 있는 함수는 ptrace를 이용해서 디버거가 붙어있으면 바로 종료되게 되어있는 코드가 작성된 함수임을 확인할 수 있다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%206.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%206.png)

bp를 걸고 rax값을 0으로 설정해주면 간단히 우회가 가능한 것을 확인 가능하다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%207.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%207.png)

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%208.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%208.png)

### Code Patching

함수를 실행하면 `input:`텍스트와 함께 인풋을 입력받는 것을 확인할 수 있다.

`main`함수를 확인하면 인풋값의 길이가 2인것을 확인할 수 있다.

그런데, main함수도 아래에 보면 xor_data함수처럼 function Epilogue를 가져야 하기에 마지막 2바이트는 `leave; ret`(`\xC9\xC3`) 임을 짐작할 수 있다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%209.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%209.png)

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2010.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2010.png)

그래서 `\xA6\x84` 과 Xor을 해주면 `Go`와 `oG` 가 나온다. 둘 중 하나가 키가 될 것이다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2011.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2011.png)

확인해보면 `Go` 가 맞다는 걸 알 수 있다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2012.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2012.png)

그래서 `Go`를 키로 패치를 진행한다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2013.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2013.png)

이후 patch1을 IDA로 열어 main함수를 확인하면 다음과 같다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2014.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2014.png)

또다시 "input: "으로 입력값을 입력받고, 길이가 5인지 확인한다.

이제 입력값을 키로 qword_400948을 xor을 하게되는 데, qword_400948는 아래 사진과 같다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2015.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2015.png)

이후 함수로 호출을 하는걸 보면 함수임을 예상할 수 있다. 그러면 함수 프롤로그를 이뤄야 하기에 

`push rbp; mov rbp,rsp` 로 시작할 것이다. 그러면 데이터는 `55 48 89 E5`로 시작할 것이다. 하지만 5글자라기엔 한자 부족하지 않는가?

이는 함수가 local data가 있다면, `push rbp; mov rbp,rsp; sub rsp,size` 로 시작하기 때문이다. 이를 고려하면 함수 프롤로그는 다음과 같다. `55 48 89 E5 48 83 EC xx` xx는 사이즈이다.

이제, 5글자를 xor해보게 되면 다음과 같이 나온다. 즉, Key는 CyKor이다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2016.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2016.png)

똑같이 xor해주고(size2인 0xB4만큼) patch2파일을 확인해보면

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2017.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2017.png)

xor한 함수가 아래처럼 제대로 나와있는 것을 알 수 있다.

함수내용은 방금 2번의 과정과 똑같고, 길이가 13이 되었다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2018.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2018.png)

그런데, 지금까지 데이터는 `55 48 89 E5 48 83 EC xx` 프롤로그가 이렇게 된다는 것밖에 없기에 잠시 함수를 더 확인해보자

아래사진은 방금 우리가 복호화한 함수이다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2019.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2019.png)

`mov rax, fs:28h` 부분은 canary check을 하는 부분인 데, 이 부분은 모든 함수에 존재하게 된다. 

그리고, 현재 비슷한 형식이 반복되었기에 또 반복되는 함수도 이와 비슷하지 않을까? 라는 생각을 해보게 된다.

그렇게 되면 우리가 알 수 있는 함수의 시작부분은 다음과 같다.

`55 48 89 E5 48 83 EC xx 64 48 8B 04 25 28 00 00 00` 총 17글자까지 알수 있게 되었다.

현재 요구하는 것은 13글자이기에 xor해보도록 하자

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2020.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2020.png)

Key는 `K0R34UNIV_CTF` 이다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2021.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2021.png)

그러면 복구되는 함수는 다음과 같다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2022.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2022.png)

이제는 19글자에 대해서 xor을 해야하는 데, 현재 복호화된 두 함수가 매우 유사하다는 걸 보면, 이후 함수도 유사할 것이라 예상할 수 있다.

아래 사진을 보면 xor eax, eax까지 동일한 것을 볼 수 있다. 

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2023.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2023.png)

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2024.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2024.png)

그래서 `55 48 89 E5 48 83 EC xx 64 48 8B 04 25 28 00 00 00 48 89 45 F8 31 C0`

까지 이어질 것이라 예상가능하다. 이중 19글자만 추리면 `55 48 89 E5 48 83 EC xx 64 48 8B 04 25 28 00 00 00 48 89` 이다.

이 값들이랑 xor을 해보게 되면 

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2025.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2025.png)

아래처럼 Key가 나타나는 것을 알 수 있다.

그런데 이중에 xx으로 나타나는 부분이 있지 않는가?

그래서 중간에 한바이트가 오류가 날 수 있다.

Congratulation_Guys의 스펠링은 왼쪽과 같은데 사진을 보면 중간에 u대신 E가 있다. 이부분 수정이 필요하다.

- 이부분 Flag Congratulation_Guys로 바꿈 (보기도 어려운데 오타잡는건 에바라서..)

다시 반복

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2026.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2026.png)

다음과 같다.  이부분은 따로 input을 넣는 부분이 없는 것 같다. 그러니 실행을 해보자.

**function 400877은 crc32이다. github에 상수 검색해보면 나옴

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2027.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2027.png)

### Anti-Debug (breakpoint)

gdb에 붙여서 anti-debug를 우회하고 실행해보면 다음과 같다.

하지만, 정상적으로 flag가 출력되지 않는 데, 

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2028.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2028.png)

이부분은 위에서 crc32 체크를 코드영역에 대해서 하게 되는 데, bp를 걸게되면 1바이트가 0xcc로 변하기 때분에 crc32 값이 변해서 제대로 출력되지 않은 것이다.

이제 bp를 지우고서 다시 실행하면 다음처럼 제대로 flag가 출력된다.

![Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2029.png](Reversing_Package%201a629bbc990d4927827af0ce715d5454/Untitled%2029.png)

flag : `You_could_feel_the_various_flavors_of_reversing,right?`