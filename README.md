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