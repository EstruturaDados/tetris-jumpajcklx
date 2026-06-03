#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define CAPACIDADE_FILA 5
#define CAPACIDADE_PILHA 3

// Estrutura da Peça
typedef struct {
    char tipo; // Ex: 'I', 'O', 'T', 'S', 'Z', 'J', 'L'
    int id;
} Peca;

// Estrutura da Fila Circular (Peças Futuras)
typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

// Estrutura da Pilha Linear (Reserva)
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo;
} Pilha;

// Variável global para gerar IDs sequenciais únicos
int proximoId = 1;

// --- FUNÇÕES AUXILIARES DE GERAÇÃO ---
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};
    Peca novaPeca;
    novaPeca.tipo = tipos[rand() % 7];
    novaPeca.id = proximoId++;
    return novaPeca;
}

// --- FUNÇÕES DA FILA CIRCULAR ---
void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

bool filaCheia(FilaCircular *f) {
    return f->tamanho == CAPACIDADE_FILA;
}

bool filaVazia(FilaCircular *f) {
    return f->tamanho == 0;
}

bool enqueue(FilaCircular *f, Peca p) {
    if (filaCheia(f)) return false;
    f->tras = (f->tras + 1) % CAPACIDADE_FILA;
    f->itens[f->tras] = p;
    f->tamanho++;
    return true;
}

Peca dequeue(FilaCircular *f) {
    Peca p = {' ', -1};
    if (filaVazia(f)) return p;
    p = f->itens[f->frente];
    f->frente = (f->frente + 1) % CAPACIDADE_FILA;
    f->tamanho--;
    return p;
}

void mostrarFila(FilaCircular *f) {
    printf("FILA DE PEÇAS FUTURAS: ");
    if (filaVazia(f)) {
        printf("[Vazia]\n");
        return;
    }
    int indice = f->frente;
    for (int i = 0; i < f->tamanho; i++) {
        printf("[%c (ID: %d)] ", f->itens[indice].tipo, f->itens[indice].id);
        indice = (indice + 1) % CAPACIDADE_FILA;
    }
    printf("\n");
}

// --- FUNÇÕES DA PILHA ---
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

bool pilhaCheia(Pilha *p) {
    return p->topo == CAPACIDADE_PILHA - 1;
}

bool pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

bool push(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) return false;
    p->topo++;
    p->itens[p->topo] = peca;
    return true;
}

Peca pop(Pilha *p) {
    Peca peca = {' ', -1};
    if (pilhaVazia(p)) return peca;
    peca = p->itens[p->topo];
    p->topo--;
    return peca;
}

void mostrarPilha(Pilha *p) {
    printf("PILHA DE RESERVA (Topo no início): ");
    if (pilhaVazia(p)) {
        printf("[Vazia]\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[%c (ID: %d)] ", p->itens[i].tipo, p->itens[i].id);
    }
    printf("\n");
}

// --- FUNÇÕES ESTRATÉGICAS (NÍVEL MESTRE) ---

// Opção 4: Trocar a peça da frente da fila com o topo da pilha
void trocarFrenteComTopo(FilaCircular *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("❌ Erro: Fila de peças vazia!\n");
        return;
    }
    if (pilhaVazia(p)) {
        printf("❌ Erro: Pilha de reserva vazia!\n");
        return;
    }
    
    // Troca direta nos arrays usando os ponteiros de controle
    Peca temp = f->itens[f->frente];
    f->itens[f->frente] = p->itens[p->topo];
    p->itens[p->topo] = temp;
    
    printf("🔄 Sucesso: Peça da frente da fila trocada com o topo da pilha!\n");
}

// Opção 5: Trocar os 3 primeiros da fila com as 3 peças da pilha
void trocarTresPrimeiros(FilaCircular *f, Pilha *p) {
    if (f->tamanho < 3) {
        printf("❌ Erro: A fila precisa de pelo menos 3 peças!\n");
        return;
    }
    if (p->topo != CAPACIDADE_PILHA - 1) { // Precisa ter exatamente 3 elementos (índices 0, 1, 2)
        printf("❌ Erro: A pilha de reserva precisa estar cheia (exatamente 3 peças)!\n");
        return;
    }

    // Mapeamento dos índices circulares da fila
    int idxFila0 = f->frente;
    int idxFila1 = (f->frente + 1) % CAPACIDADE_FILA;
    int idxFila2 = (f->frente + 2) % CAPACIDADE_FILA;

    // Troca mantendo a ordem:
    // 1º da Fila troca com o Topo da Pilha (índice 2)
    // 2º da Fila troca com o Meio da Pilha (índice 1)
    // 3º da Fila troca com a Base da Pilha (índice 0)
    Peca temp;

    temp = f->itens[idxFila0]; f->itens[idxFila0] = p->itens[2]; p->itens[2] = temp;
    temp = f->itens[idxFila1]; f->itens[idxFila1] = p->itens[1]; p->itens[1] = temp;
    temp = f->itens[idxFila2]; f->itens[idxFila2] = p->itens[0]; p->itens[0] = temp;

    printf("🔄 Sucesso: Os 3 primeiros elementos da fila e da pilha foram invertidos estrategicamente!\n");
}

// --- PROGRAMA PRINCIPAL ---
int main() {
    srand(time(NULL)); // Inicializa gerador de números aleatórios

    FilaCircular filaFutura;
    Pilha pilhaReserva;

    inicializarFila(&filaFutura);
    inicializarPilha(&pilhaReserva);

    // Preenche a fila inicial com 5 peças obrigatórias
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enqueue(&filaFutura, gerarPeca());
    }

    int opcao;
    do {
        printf("\n================= TETRIS STACK =================\n");
        mostrarFila(&filaFutura);
        mostrarPilha(&pilhaReserva);
        printf("------------------------------------------------\n");
        printf("1 - Jogar peça (Remover da frente e repor)\n");
        printf("2 - Enviar peça da fila para a reserva (Pilha)\n");
        printf("3 - Usar peça da reserva (Pop do topo)\n");
        printf("4 - Trocar peça da frente com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com os 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        printf("================================================\n\n");

        switch (opcao) {
            case 1: {
                Peca jogada = dequeue(&filaFutura);
                if (jogada.id != -1) {
                    printf("🎮 Peça [%c] (ID: %d) colocada no tabuleiro!\n", jogada.tipo, jogada.id);
                    enqueue(&filaFutura, gerarPeca()); // Repõe imediatamente
                } else {
                    printf("❌ Fila vazia!\n");
                }
                break;
            }
            case 2: {
                if (pilhaCheia(&pilhaReserva)) {
                    printf("❌ Reserva cheia! Não é possível guardar mais peças.\n");
                } else {
                    Peca daFila = dequeue(&filaFutura);
                    push(&pilhaReserva, daFila);
                    printf("📥 Peça [%c] movida para a reserva.\n", daFila.tipo);
                    enqueue(&filaFutura, gerarPeca()); // Mantém a fila com 5 peças
                }
                break;
            }
            case 3: {
                if (pilhaVazia(&pilhaReserva)) {
                    printf("❌ Reserva vazia! Nenhuma peça guardada.\n");
                } else {
                    Peca daReserva = pop(&pilhaReserva);
                    printf("🎮 Peça [%c] (ID: %d) da reserva foi jogada!\n", daReserva.tipo, daReserva.id);
                }
                break;
            }
            case 4:
                trocarFrenteComTopo(&filaFutura, &pilhaReserva);
                break;
            case 5:
                trocarTresPrimeiros(&filaFutura, &pilhaReserva);
                break;
            case 0:
                printf("Saindo do jogo... Obrigado por jogar!\n");
                break;
            default:
                printf("⚠️ Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
