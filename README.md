# Documentação - Trabalho Prático Programação Concorrente

## Nome dos integrantes:
- Lucas Tourinho - 2221133
- Tiago de Jesus Chehab – 2211194

## Introdução
Para o trabalho final da disciplina, foi requerido a criação de um sistema que simule o jogo “Keep Solving and Nobody Explodes”, nesta simulação é necessário apenas um jogador que precisará desarmar uma bomba. O documento detalha a  implementação da estrutura do jogo usando a concorrência e foi feita na linguagem de programação C. Na execução utiliza-se de threads, seções críticas e sincronização para garantir o funcionamento correto do jogo.

## Explicação e mecânica do jogo
O jogo simula uma situação de emergência onde se deve desarmar uma bomba antes do término do tempo, ela contém diferentes tipos de módulos explosivos e um número limitado de bancadas. É preciso um grande nível de coordenação para que o jogador consiga cumprir com o objetivo do jogo,  desarmar o explosivo.

O número de Tedax e bancadas é configurável no começo do jogo, tendo um máximo de 10 tedax e 5 bancadas. Múltiplos Tedax podem se alinhar para usar as bancadas, cada Tedax só pode interagir com um módulo por vez, o jogo continua indefinidamente até que todos os módulos sejam desarmados, ou uma grande fila de módulos se acumule (causando um estado de emergência).

## Fluxo do jogo
1. Geração de módulos:
- Os módulos são criados periodicamente pela thread “module_board_func”.
- Cada módulo tem um identificador único, um tipo (x, c, ou t) e um número de interações necessárias para ser desarmado:
- x (Botões): Exige pressionar uma quantidade de botões.
- c (Sequência): Exige inserir uma sequência de caracteres.
- t (Temporizador): Requer aguardar um tempo específico.
2. Coordenação:
- A thread “coordinator_func” permite ao jogador designar manualmente um módulo específico para um Tedax, uma bancada específica onde o módulo será desarmado.
- Se nenhuma ação for tomada, os Tedax continuam a buscar módulos automaticamente.
3. Desarmamento de módulos:
- Cada Tedax, controlado pela thread “tedax_func”, monitora os módulos disponíveis.
- Quando um módulo é atribuído:
- O Tedax ocupa uma bancada (usando um semáforo para sincronização).
- Realiza as interações necessárias para desarmar o módulo.
- Após concluir o desarmamento:
- O módulo é marcado como desarmado.
- A bancada é liberada para outro Tedax.
4. Interface: 
- A thread “display_func” atualiza continuamente a interface, exibindo os módulos pendentes, ocupados e em progresso, bancadas ocupadas e disponíveis, e o status do tedax.
5. Fila de espera (Bônus):
- Se todos os Tedax estiverem ocupados ou as bancadas estiverem indisponíveis:
- Os Tedax aguardam em uma fila para obter acesso assim que os recursos forem liberados.

## Threads
- module_board_func: Gera novos módulos explosivos em intervalos fixos e os adiciona à fila de módulos.
- display_func: Atualiza a interface do jogo em tempo real, exibindo informações sobre os módulos, Tedax e bancadas.
- coordinator_func: Coordena manualmente a designação de módulos aos Tedax e bancadas.
- tedax_func: Representa cada Tedax, que espera por um módulo disponível e interage com ele até desarmá-lo.
- Principal (main): Responsável por iniciar e finalizar as threads e gerenciar a configuração inicial.

## Seções críticas
- Fila de Módulos: Protegida por um mutex (“module_queue_lock”) para evitar condições de corrida durante a leitura e escrita.
- Bancadas: Controladas por um semáforo (“benches”), que regula o acesso às bancadas disponíveis e gerencia a fila de Tedax esperando para utilizá-las.

## Sincronização
- Mutexes: Garantem consistência na fila de módulos e no estado de cada módulo.
- Semáforos: Gerenciam o uso das bancadas, permitindo que múltiplos Tedax aguardem e acessem bancadas de forma organizada.

## Compilação, execução e requisitos



Comandos Disponíveis
Compilar o Projeto:

bash
make
O binário será gerado em bin/game.
Executar o Jogo:

bash
./bin/game
Limpar Arquivos de Compilação:

bash
make clean
Remove todos os arquivos objetos (.o) e o binário gerado.
Limpeza Completa:

bash
make distclean
Remove todos os diretórios gerados (obj e bin).
Ajuda:

bash
make help
Exibe os comandos disponíveis no Makefile.
3. Requisitos
Compilador GCC: Certifique-se de que o GCC está instalado:

bash
gcc --version
Bibliotecas ncurses: Instale as dependências necessárias:

bash
sudo apt update
sudo apt install libncurses5-dev libncursesw5-dev
4. Testando o Projeto
Compile:

bash
make
Execute:

bash
./bin/game
Limpe e Compile Novamente:

bash
make clean
make
Remova Todos os Diretórios Gerados:

bash
make distclean
