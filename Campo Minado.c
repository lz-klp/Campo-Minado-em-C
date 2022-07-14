#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <locale.h>

/* Programa elaborado por: Luiz Eduardo Klipe Ribeiro
                           João Pedro Marcondes dos Santos
                           Carlos Eduardo Gasparello
    Ultima Modificação: 28/06/2022
    Professor Edson
    Senai Guarapuava */

// Tabuleiro 8x8

int num_bombas,linha,coluna,jogar = 0,config,acertos = 0,vizinhas = 0,resultado = 2;// variaveis inteiras
int campo[8][8];// matriz que sera o campo
char campo_exibido[8][8]; //matriz que servira como tabuleiro que sera exibido
char vizinhas_char; // variavel para printar o numero de bombas vizinhas

void gotoxy(int x, int y){ //Fução para controlar o cursor
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x,y});
}

void configuracoes(){//função para trocar a cor da fonte
    int cor = 0,cor_invalida = 0; // variaveis de controle

    system("cls"); // limpa a tela

    do{ // exibe o menu de configurações
    gotoxy(46,12);
    printf("-----CONFIGURAÇÕES-----");
    gotoxy(46,13);
    printf("Escolha a cor da fonte:");
    gotoxy(46,14);
    printf("1.Verde(Padrão)");
    gotoxy(46,15);
    printf("2.Amarelo");
    gotoxy(46,16);
    printf("3.Azul");
    gotoxy(46,17);
    printf("4.Vermelho");
    gotoxy(46,18);
    printf("0.Voltar");
    gotoxy(46,19);
    printf("-> ");
    fflush(stdin); // limpa o buffer
    scanf("%i",&cor); // recebe a escolha do usuario

    switch(cor){

    case 1: system("cls");// limpa tela
    system("color 02"); // define como verde
    cor_invalida = 0; // indica que houve uma escolha correta
    break;

    case 2: system("cls");//limpa tela
    system("color 06");// define como amarrelo
    cor_invalida = 0;
    break;

    case 3: system("cls");// limpa tela
    system("color 03");// define como azul
    cor_invalida = 0;// indica que houve uma escolha correta
    break;

    case 4: system("cls"); // limpa tela
    system("color 0c");// define como vermelho
    cor_invalida = 0;// indica que houve uma escolha correta
    break;

    case 0: system("cls"); // escolha para voltar para o menu
    cor_invalida = 0;// indica que houve uma escolha correta
    break;

    default: gotoxy(46,20); // reseta o menu em caso de uma escolha invalida
    printf("Digite um valor valido!");
    Sleep(1000);
    gotoxy(49,19);
    printf(" ");
    cor_invalida = 1;
    }
    }while(cor_invalida ==1);
}

void menu(){ // função que cria o menu
    int escolha = 0 ,esc_invalida = 0; // variaveis inteiras para salvar a escolha do usuario e analisar se ela é valida
    system("cls"); // limpa a tela
   // for's que criam o contorno do menu
    for(int i = 1; i <= 118; i++){ // contorno superior
        gotoxy(i,0);
        printf("-");
        Sleep(1); // intervalo de um milisegundo
    }
    for(int j = 0;j <= 29;j++){ //  lado direito
        gotoxy(118,j);
        printf("|");
        Sleep(1);// intervalo de um milisegundo
    }
    for(int k = 117; k >= 0; k--){ // contorno de baixo
        gotoxy(k,29);
        printf("-");
        Sleep(1);// intervalo de um milisegundo
    }
    for(int l = 29; l >= 0; l--){ // lado esquerdo
        gotoxy(0,l);
        printf("|");
        Sleep(1);// intervalo de um milisegundo
    }

    do{
    gotoxy(46,12); //criação do menu com auxilio da função gotoxy para centralizar ele
    printf("-----CAMPO MINADO-----");
    Sleep(1000); // espera um segundo
    gotoxy(46,13);
    printf("Escolha a dificuldade: ");
    gotoxy(46,14);
    printf("1.Fácil (Tabuleiro 8x8 8 bombas)");
    gotoxy(46,15);
    printf("2.Médio (Tabuleiro 8x8 20 bombas)");
    gotoxy(46,16);
    printf("3.Difícil (Tabuleiro 8x8 40 bombas)");
    gotoxy(46,17);
    printf("4.(Quase) Impossível (Tabuleiro 8x8 63 bombas)");
    gotoxy(46,18);
    printf("0.Configrações");
    gotoxy(1,28);
    printf("Criado por: Luiz Klipe, João Pedro Marcondes, Carlos Gasparello");
    gotoxy(46,19);
    printf("-> ");
    fflush(stdin);
    scanf("%d",&escolha); // salva a escolha do usuario

    switch(escolha){  // muda o numero de minas de acordo com a dificuldade escolhida
    case 1: num_bombas = 8; // nivel facil
    esc_invalida = 0; // varaivel de controle de repetição
    jogar = 1; // variavel para iniciar o jogo
    break;

    case 2: num_bombas = 20;// nivel medio
    esc_invalida = 0;// varaivel de controle de repetição
    jogar = 1; // variavel para iniciar o jogo
    break;

    case 3: num_bombas = 40;// nivel dificil
    esc_invalida = 0;// varaivel de controle de repetição
    jogar = 1; // variavel para iniciar o jogo
    break;

    case 4: num_bombas = 63;// nivel impossivel
    esc_invalida = 0;// varaivel de controle de repetição
    jogar = 1; // variavel para iniciar o jogo
    break;

    case 0: esc_invalida = 0;// varaivel de controle de repetição
    config = 1; // variavel para chamar o menu de configurações
    break;

    default: gotoxy(46,20); // reseta o menu em caso de uma escolha invalida
    printf("Digite um valor valido!");
    Sleep(1000);
    gotoxy(49,19);
    printf(" ");
    esc_invalida = 1;
    }
    }while(esc_invalida == 1);
}

void limparCampo(){ // limpa os tabuleiros, para que as posições das minas possam ser sorteadas corretamente em seguida
    for(int i = 0; i < 8; i++){ // orientação da linha das matrizes
        for(int j = 0; j < 8; j++){// orientação da coluna das matrizes
            campo[i][j] = 0; // campo principal
            campo_exibido[i][j] = ' '; // campo exibido na tela
        }
    }
}

void plantarMinas(int quant_minas){ // sortea as posições das bombas
    int lin,col; // variaveis inteiras para a linha e coluna sorteada para a bomba
    srand(time(NULL)); // função para mudar o valor da seed

    for(int i = 1; i <= quant_minas; i++){ // for que controla o numero de bombas sorteadas
        lin = rand() % 8; // sortea a linha
        col = rand() % 8; // sortea a coluna

        if(campo[lin][col] == 0){ // caso nao tiver bombas na casa sorteada planta a bomba
            campo[lin][col]  = 1;
        }
        else{ // se nao regride a variavel do for para repetir o processo
             i--;
        }
    }
}

void mostrarCampo(){//printa o segundo tabuleiro que sera exibido na tela
    gotoxy(41,5); // novamente com auxilio do gotoxy para centralizar ele na tela
    printf(" | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |"); // indica o numero das colunas
    printf("\tAcertos: %i",acertos);
    gotoxy(41,6);
    printf("-|---+---+---+---+---+---+---+---|");
    gotoxy(41,7);
    printf("0| %c | %c | %c | %c | %c | %c | %c | %c |",campo_exibido[0][0],campo_exibido[0][1],campo_exibido[0][2],campo_exibido[0][3],campo_exibido[0][4],campo_exibido[0][5],campo_exibido[0][6],campo_exibido[0][7]);
    gotoxy(41,8);
    printf(" |---+---+---+---+---+---+---+---|");
    gotoxy(41,9);
    printf("1| %c | %c | %c | %c | %c | %c | %c | %c |",campo_exibido[1][0],campo_exibido[1][1],campo_exibido[1][2],campo_exibido[1][3],campo_exibido[1][4],campo_exibido[1][5],campo_exibido[1][6],campo_exibido[1][7]);
    gotoxy(41,10);
    printf(" |---+---+---+---+---+---+---+---|");
    gotoxy(41,11);
    printf("2| %c | %c | %c | %c | %c | %c | %c | %c |",campo_exibido[2][0],campo_exibido[2][1],campo_exibido[2][2],campo_exibido[2][3],campo_exibido[2][4],campo_exibido[2][5],campo_exibido[2][6],campo_exibido[2][7]);
    gotoxy(41,12);
    printf(" |---+---+---+---+---+---+---+---|");
    gotoxy(41,13);
    printf("3| %c | %c | %c | %c | %c | %c | %c | %c |",campo_exibido[3][0],campo_exibido[3][1],campo_exibido[3][2],campo_exibido[3][3],campo_exibido[3][4],campo_exibido[3][5],campo_exibido[3][6],campo_exibido[3][7]);
    gotoxy(41,14);
    printf(" |---+---+---+---+---+---+---+---|");
    gotoxy(41,15);
    printf("4| %c | %c | %c | %c | %c | %c | %c | %c |",campo_exibido[4][0],campo_exibido[4][1],campo_exibido[4][2],campo_exibido[4][3],campo_exibido[4][4],campo_exibido[4][5],campo_exibido[4][6],campo_exibido[4][7]);
    gotoxy(41,16);
    printf(" |---+---+---+---+---+---+---+---|");
    gotoxy(41,17);
    printf("5| %c | %c | %c | %c | %c | %c | %c | %c |",campo_exibido[5][0],campo_exibido[5][1],campo_exibido[5][2],campo_exibido[5][3],campo_exibido[5][4],campo_exibido[5][5],campo_exibido[5][6],campo_exibido[5][7]);
    gotoxy(41,18);
    printf(" |---+---+---+---+---+---+---+---|");
    gotoxy(41,19);
    printf("6| %c | %c | %c | %c | %c | %c | %c | %c |",campo_exibido[6][0],campo_exibido[6][1],campo_exibido[6][2],campo_exibido[6][3],campo_exibido[6][4],campo_exibido[6][5],campo_exibido[6][6],campo_exibido[6][7]);
    gotoxy(41,20);
    printf(" |---+---+---+---+---+---+---+---|");
    gotoxy(41,21);
    printf("7| %c | %c | %c | %c | %c | %c | %c | %c |",campo_exibido[7][0],campo_exibido[7][1],campo_exibido[7][2],campo_exibido[7][3],campo_exibido[7][4],campo_exibido[7][5],campo_exibido[7][6],campo_exibido[7][7]);
    gotoxy(41,22);
    printf(" |---+---+---+---+---+---+---+---|");
}

void verificaResultado(){ // verifica todos os cenarios possiveis
    acertos = 0;  // variavel para contar os acertos e verificar vitorias

    if(campo[linha][coluna] == 1){ // verifica derrota
        resultado = 0;
    }
    else { //acertou um lugar vazio
        campo_exibido[linha][coluna] = 'X'; // atualiza o valor da matriz para controle
    }

    for(int i =0; i<8;i++){ // for que ira contar os acertos
        for(int j = 0;j<8;j++){
            if(campo_exibido[i][j] != ' '){ // se o quadrado nao estiver vazio, ou seja o jogador acertou
                acertos++; // o valor da variavel é incrementado
            }
        }
     }

    if((64 - acertos) == num_bombas){ // verifica vitoria
        resultado = 1;
    }
}

int coordValida(int l,int c){ // função que verifica se as coordenadas geradas na proxima função sao validas
    if(l >= 0 && l < 8 && c >= 0 && c < 8){ // verifica se a coordena esta fora do scope da matriz
        return 1;
    }
    else{
        return 0;
    }
}

void bombasVizinhas(){ //  contara o numero de bombas vizinhas nas oitos casas circundantes
    if(coordValida(linha - 1, coluna - 1) == 1 && campo[linha - 1][coluna - 1] == 1){
        vizinhas++; // se tiver uma bomba na casa e se ela existir o numero de bombas vizinhas é incrementado
    }
    if (coordValida(linha -1, coluna) == 1 && campo[linha-1][coluna] == 1){
        vizinhas++;// se tiver uma bomba na casa e se ela existir o numero de bombas vizinhas é incrementado
    }
    if (coordValida(linha -1, coluna +1) == 1 && campo[linha-1][coluna + 1] == 1){
        vizinhas++;// se tiver uma bomba na casa e se ela existir o numero de bombas vizinhas é incrementado
    }
    if (coordValida(linha, coluna -1) ==  1 && campo[linha][coluna - 1] == 1){
        vizinhas++;// se tiver uma bomba na casa e se ela existir o numero de bombas vizinhas é incrementado
    }
    if (coordValida(linha, coluna +1) == 1 && campo[linha][coluna + 1] == 1){
        vizinhas++;// se tiver uma bomba na casa e se ela existir o numero de bombas vizinhas é incrementado
    }
    if (coordValida(linha +1, coluna -1) == 1 && campo[linha+1][coluna - 1] == 1){
        vizinhas++;// se tiver uma bomba na casa e se ela existir o numero de bombas vizinhas é incrementado
    }
    if (coordValida(linha +1, coluna) == 1 && campo[linha+1][coluna] == 1){
        vizinhas++;// se tiver uma bomba na casa e se ela existir o numero de bombas vizinhas é incrementado
    }
    if (coordValida(linha +1, coluna +1) == 1 && campo[linha+1][coluna + 1] == 1){
        vizinhas++;// se tiver uma bomba na casa e se ela existir o numero de bombas vizinhas é incrementado
    }

    vizinhas_char = vizinhas + '0'; // transforma o dado int para char, atualiza o campo exibido e reseta a variavel
    campo_exibido[linha][coluna] = vizinhas_char;
    vizinhas = 0;
}

void mensagem(){ // exibe as mensagens de vitoria e derrota
    if(resultado == 0){// derrota
        system("cls"); // limpa o console e exibe a mensagem centralizada com auxilio do gotoxy
        gotoxy(53,14);
        printf("GAME OVER!");
        gotoxy(46,15);
        printf("Você explodiu uma Bomba!");
    }
    else if(resultado == 1){ // vitoria
        system("cls");// limpa o console e exibe a mensagem centralizada com auxilio do gotoxy
        gotoxy(53,14);
        printf("Parabéns você ganhou!");
    }

}

int main(){
    system("color 02"); // muda a cor do console e o idioma
    setlocale(LC_ALL,"Portuguese");

    // chama as funções para "preparar" o jogo
    do{
    menu(); // menu
    if(config == 1){ // controle para chamar ou nao a função da configuração de cor
        configuracoes();
        config = 0; // limpa a variavel
    }
    }while(jogar == 0); // roda o menu enquanto nao for iniciado o jogo

    limparCampo(); // prepara o campo para jogar
    plantarMinas(num_bombas);

    do{
    system("cls"); // limpa a tela
    mostrarCampo();// exibe o campo

    gotoxy(38,24); // exibe a mensagem pedindo a coordenada da tentativa na linha correta
    printf("Digite a Coordenada do seu chute (Formato Linha Coluna): ");
    fflush(stdin);
    scanf("%d%d",&linha,&coluna);//Recebe a tenativa do jogador

    verificaResultado(); // verifica o resultado para repetir ou nao o processo
    bombasVizinhas(); // atualiza o valor das bombas vizinhas

    }while(resultado == 2); // mantem o jogo rodando enquanto não for identifcada vitoria ou derota

    mensagem(); // exibe a mensagem de vitoria ou derrota

    gotoxy(0,27); // encerra o programa sem interferir na mensagem exibida
    return(0);
}
