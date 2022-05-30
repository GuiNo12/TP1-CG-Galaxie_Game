# TP1-CG-Galaxie-Game 🛸🔫
Trabalho 1 para Computação Gráfica utilizando OpenGL.


## Como instalar a biblioteca irrKlang 1.5.

Na pasta Teste/bin/Debug, selecione os arquivos:
irrKlang.dll, ikpMP3.dll, ikpFlac.dll, libirrKlang.a e libirrKlang.def
e copie-os para /Windows/System32 e para ArquivosdeProgramas/Codeblocks/MinGW/lib

Depois copie o conteúdo de /include disponível em /irrKlang-1.5.0/include que está no git e cole em Teste/include.
Copie os mesmos arquivos de /include e cole em ArquivosdeProgramas/Codeblocks/MinGW//include

Dentro da pasta /irrKlang-1.5.0 no git, copie o arquivo irrKlang.lib para ArquivosdeProgramas/Codeblocks/MinGW/lib

No CodeBlocks, clique com o botão direito no nome do projeto e em Bild Options

![image](https://user-images.githubusercontent.com/93343614/169708904-2c6c1f58-b7ae-4a84-bef9-14fa625ea806.png)

Em Linker settings, adicione os caminhos para os arquivos abaixo que estão em Teste/bin/Debug

![image](https://user-images.githubusercontent.com/93343614/169708954-fe9f9142-3bed-43a1-9abe-8195a7270fc8.png)

Com isso, a biblioteca estará instalada
