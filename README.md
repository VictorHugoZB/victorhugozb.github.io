# Computação Gráfica - Atividade 1
### Victor Hugo Zaninette Bernardino - RA: 11201811443

## Descrição do projeto

O projeto visa recriar o clássico jogo chamado "breakout", que consiste em destruir todos os blocos que se encontram na parte superior da tela, sem deixar com que a bolinha em movimento caia para fora da tela. No canto superior direito existe a opção de aumentar ou diminuir a velocidade da barra controlada pela jogador, bem como selecionar os níveis de 1 até 3. Os níveis 2 e 3 acrescentam no jogo blocos cinzas, que não são destrutíveis (bem como não são exigidos para conclusão do nível). A barra pode ser controlada com as teclas "a" e "d", ou seta esquerda / seta direita.

## Código

Assim como o jogo asteroids apresentado em aula, o projeto atual consiste em uma divisão de arquivos por objetos dentro do jogo, e cada um com funções como create, paint, update, destroy, funções essas que são chamadas dentro do main.cpp. Para este jogo, foram criadas as seguintes classes: border, que renderiza a borda do jogo; ball, renderiza e atualiza a bolinha dentro do jogo; bar, renderiza e atualiza a barra controlada pelo jogador; block, atualiza e renderiza os blocos que são atingidos pela bolinha na parte superior da tela.

Funções repetitivas, como a de renderizar os pontos ou objetos na tela foram utilizadas em vários dos arquivos, e são muito similares as apresentadas no asteroids. Abaixo serão apresentados alguns dos principais pontos do projeto:

### A classe da "bolinha" -> ball.cpp

Para essa classe, vale destacar que a bolinha é renderizada como um ponto do OpenGL, utilizando a flag GL_POINTS com count = 1. O tamanho do ponto foi aumentado utilizando o vertex shader ball.vert, localizado em assets. Além disso, o fragment shader ball.frag foi utilizado para deixar seu formato circular.

Outro ponto de destaque para essa classe é a sua função de update. Nela, é checado a colisão da bolinha com as bordas do cenário e com a barra controlada pelo jogador. Ainda, no caso de ser uma colisão com a barra controlada pelo jogador, uma função é calculada baseada em seno e cosseno para redirecionar a bola de forma consistente.

### A classe da barra -> bar.cpp

Para renderizar a barra foi utilizado geometria indexada (EBOs). Em sua função update, é controlado o deslocamento da barra conforme input do jogador.

### A classe dos blocos -> block.cpp

Essa classe controla uma lista de blocos renderizados por geometria indexada (EBOs). Em sua função de update é checada a colisão da bolinha com algum dos blocos em tela. Caso ocorra uma colisão, os blocos envolvidos são removidos de tela e o direcionamento da bolinha é recalculada. Além disso, existem também blocos que não são destrutíveis, e esses são sinalizados com a flag m_destructable.

### A classe da borda -> border.cpp

Essa classe foi utilizada unicamente para desenhar as bordas do jogo. Para sua renderização foi utilizada a flag GL_LINE_STRIP do openGL para desenhar uma linha.

### O arquivo gamedata.hpp

Esse arquivo controla não somente o estado do jogo (como playing, game over e win), como também o nível do jogo, que pode ser 1, 2 ou 3, conforme descrito acima.

### window.cpp

No arquivo principal, além de ser chamado as funções de create, paint, update e destroy, algumas outras funções são realizadas. Primeiramente, é controlado o tamanho da tela de jogo na função onResize, cujos parâmetros são utilizados em onPaint. Além disso nela também se encontra a função checkEndGameConditions, que checa se o estado está em gameOver, além de checar se todos os blocos quebráveis já foram removidos, definindo assim se o jogador venceu ou não. Por fim, caso o jogador complete o nível 1, automaticamente carregará o nível 2, e o mesmo para o nível 3, entretanto, o nível também pode ser selecionado no menu.




