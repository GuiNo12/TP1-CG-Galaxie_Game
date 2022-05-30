# TP1-CG-Galaxian-Game 🛸🔫
Trabalho 1 para Computação Gráfica utilizando OpenGL inspirado no Galaxian Game.

## Sumário
* [Integrantes](#integrantes)
* [Instruções de Compilação e Execução](#instruções-de-compilação-e-execução)
* [Arquivo Executável](#arquivo-executável)
* [Três Cenas do Jogo](#três-cenas-do-jogo)
* [Vídeo de Demonstração](#vídeo-mostrando-o-jogo)
* [Lista de intens adicionais](#lista-de-intens-adicionais)
* [Bibliotecas utilizadas](#bibliotecas-utilizadas)
* [Como instalar a biblioteca irrKlang 1.5. (Som)](#como-instalar-a-biblioteca-irrKlang-1.5.-som)

## Integrantes
* [Guilherme Nogueira Coelho]
* [Pablo Henrique Santos]
* [Rebeca Larissa Silva Gaia]

## Instruções de Compilação e Execução:
O jogo foi desenvolvido utilizando o CodeBlocks. Para compilar, basta clicar duas vezes no arquivo Teste.cbp disponível em /Teste. O CodeBlocks será inicializado e basta usar as teclas Ctrl + F9.
## Arquivo Executável:
O arquivo executável encontra-se na pasta /Teste/bin/Debug.

## Três cenas do jogo
* ### Jogo Pausado:
  ![image](https://user-images.githubusercontent.com/93343614/171066449-50fd0d2f-77e9-41ea-9116-aab0cdf40449.png)
* ### Jogador Atirando:
  ![image](https://user-images.githubusercontent.com/93343614/171066631-73cdf753-4363-4869-a64e-538c37a56927.png)
* ### Jogador na próxima fase (ganhou uma vida):
  ![image](https://user-images.githubusercontent.com/93343614/171066777-6d9a7d35-5bc3-4311-bd00-6729345f98c7.png)

## Vídeo mostrando o jogo
Link: 

## Lista de intens adicionais
* Texturas animadas (jogador);
* Imagem de fundo;
* Fases infinitas;
* Fases curadas (zumbis andam mais rápidos);
* Pontuação (jogador ganha vida ao passar de fase);
* HUD mostrando vidas do jogador;
* Sons (tiro, morte);

## Bibliotecas utilizadas
* FreeGLUT
* Glew
* SOIL
* irrKlang

## Como instalar a biblioteca irrKlang 1.5. (Som)

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

[Guilherme Nogueira Coelho]: https://github.com/GuiNo12
[Pablo Henrique Santos]: https://github.com/pablohsgc
[Rebeca Larissa Silva Gaia]: https://github.com/reb-gaia
