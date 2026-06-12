// ATIVIDADE N2 - Sistema de Gerenciamento de Pneus

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define USUARIO "uniavan"
#define SENHA    123
#define MAX_CAMINHOES 30
#define MAX_PNEUS 18  // Carreta tem o maximo de 18 pneus

// ─────────────────────────────────────────
//  Estruturas
// ─────────────────────────────────────────

typedef struct {
    char usuario[20];
    int  senha;
} Login;

typedef struct {
    int eixoDianteiro;   // número de eixos dianteiros
    int eixoTraseiro;    // número de eixos traseiros
} Eixo;

typedef struct {
    char placa[20];
    char tipo[20];
    Eixo eixo;
    int  qntdTotalPneus;
    int  marcacaoFogo[MAX_PNEUS]; // numeros de fogo de cada pneu
    int  cadastrado;              // 1 = slot em uso, 0 = vazio
} Caminhao;

// ─────────────────────────────────────────
//  Protótipos
// ─────────────────────────────────────────

void  limparBuffer(void);
void  limparTela(void);
void  pausar(void);
int   validarLogin(void);
void  telaLegenda(void);
int   numeroFogoJaExiste(Caminhao caminhoes[], int total, int num, int excluirIdx);
void  telaCadastrarCaminhao(Caminhao caminhoes[], int *total);
void  telaListarCaminhoes(Caminhao caminhoes[], int total);
void  telaVerificarInformacoes(Caminhao caminhoes[], int total);
void  telaFazerRodizio(Caminhao caminhoes[], int total);
void  imprimirDisposicaoPneus(Caminhao *c);

// ─────────────────────────────────────────
//  Utilitários
// ─────────────────────────────────────────

void limparBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void limparTela(void) {
    printf("\033[2J\033[H");   // sequência ANSI — funciona em Linux/Mac
}

void pausar(void) {
    printf("\nPressione ENTER para continuar...");
    limparBuffer();
}

// ─────────────────────────────────────────
//  01 – Login
// ─────────────────────────────────────────

int validarLogin(void) {
    Login login;
    int tentativas = 3;

    while (tentativas > 0) {
        limparTela();
        printf("╔══════════════════════════════╗\n");
        printf("║     SISTEMA DE PNEUS v1.0    ║\n");
        printf("╚══════════════════════════════╝\n\n");
        printf("  Tentativas restantes: %d\n\n", tentativas);

        printf("  Usuário : ");
        fgets(login.usuario, sizeof(login.usuario), stdin);
        login.usuario[strcspn(login.usuario, "\n")] = '\0';

        printf("  Senha   : ");
        if (scanf("%d", &login.senha) != 1) {
            limparBuffer();
            tentativas--;
            continue;
        }
        limparBuffer();

        if (strcmp(login.usuario, USUARIO) == 0 && login.senha == SENHA) {
            printf("\n  Login efetuado com sucesso!\n");
            pausar();
            return 1;
        }

        printf("\n  Usuário ou senha incorretos.\n");
        tentativas--;
        pausar();
    }

    printf("\n  Número máximo de tentativas atingido. Saindo...\n");
    return 0;
}

// ─────────────────────────────────────────
//  05 – Legenda dos eixos
// ─────────────────────────────────────────

void telaLegenda(void) {
    limparTela();
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                  LEGENDA — TIPOS DE EIXOS                   ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n\n");

    printf("┌─────────────────────────────────────────────────────────────┐\n");
    printf("│  TOCO  (4 pneus)                                            │\n");
    printf("│                                                             │\n");
    printf("│   Eixo Dianteiro (1 eixo — 2 pneus):                       │\n");
    printf("│       [pneu] ←—— eixo ——→ [pneu]                           │\n");
    printf("│                                                             │\n");
    printf("│   Eixo Traseiro (1 eixo — 2 pneus):                        │\n");
    printf("│       [pneu] ←—— eixo ——→ [pneu]                           │\n");
    printf("└─────────────────────────────────────────────────────────────┘\n\n");

    printf("┌─────────────────────────────────────────────────────────────┐\n");
    printf("│  TRUCK  (10 pneus)                                          │\n");
    printf("│                                                             │\n");
    printf("│   Eixo Dianteiro (1 eixo — 2 pneus):                       │\n");
    printf("│       [pneu] ←—— eixo ——→ [pneu]                           │\n");
    printf("│                                                             │\n");
    printf("│   Eixo Traseiro (2 eixos — 4 pneus cada, duplos):          │\n");
    printf("│   [pneu][pneu] ←—— eixo ——→ [pneu][pneu]  (eixo 1)        │\n");
    printf("│   [pneu][pneu] ←—— eixo ——→ [pneu][pneu]  (eixo 2)        │\n");
    printf("└─────────────────────────────────────────────────────────────┘\n\n");

    printf("┌─────────────────────────────────────────────────────────────┐\n");
    printf("│  CARRETA 3 EIXOS  (18 pneus)                                │\n");
    printf("│                                                             │\n");
    printf("│   Eixo Dianteiro (1 eixo — 2 pneus):                       │\n");
    printf("│       [pneu] ←—— eixo ——→ [pneu]                           │\n");
    printf("│                                                             │\n");
    printf("│   Eixos Traseiros (4 eixos — 4 pneus cada, duplos):        │\n");
    printf("│   [pneu][pneu] ←—— eixo ——→ [pneu][pneu]  (eixo 1)        │\n");
    printf("│   [pneu][pneu] ←—— eixo ——→ [pneu][pneu]  (eixo 2)        │\n");
    printf("│   [pneu][pneu] ←—— eixo ——→ [pneu][pneu]  (eixo 3)        │\n");
    printf("│   [pneu][pneu] ←—— eixo ——→ [pneu][pneu]  (eixo 4)        │\n");
    printf("└─────────────────────────────────────────────────────────────┘\n\n");

    printf("  Obs.: Pneus duplos nos eixos traseiros = dois pneus lado a  \n");
    printf("  lado em cada extremidade do eixo.                           \n\n");

    pausar();
}

// ─────────────────────────────────────────
//  03 – Validação: número de fogo único
// ─────────────────────────────────────────

// Verifica se 'num' já existe em qualquer caminhão.
// 'excluirIdx' = índice do pneu atual que está sendo editado (-1 para ignorar).
int numeroFogoJaExiste(Caminhao caminhoes[], int total, int num, int excluirIdx) {
    for (int c = 0; c < total; c++) {
        if (!caminhoes[c].cadastrado) continue;
        for (int p = 0; p < caminhoes[c].qntdTotalPneus; p++) {
            if (p == excluirIdx && c == total - 1) continue; // skip pneu em edição
            if (caminhoes[c].marcacaoFogo[p] == num) return 1;
        }
    }
    return 0;
}

// Verifica duplicata dentro do próprio caminhão sendo cadastrado (array local)
int fogoRepetidoLocal(int fogo[], int pos) {
    for (int i = 0; i < pos; i++) {
        if (fogo[i] == fogo[pos]) return 1;
    }
    return 0;
}

// ─────────────────────────────────────────
//  Imprimir disposição dos pneus
// ─────────────────────────────────────────

void imprimirDisposicaoPneus(Caminhao *c) {
    printf("\n  Tipo  : %s\n", c->tipo);
    printf("  Placa : %s\n", c->placa);
    printf("  Total de pneus: %d\n\n", c->qntdTotalPneus);

    int idx = 0;

    // Eixo Dianteiro — sempre 2 pneus (posições 0 e 1)
    printf("  ┌─── EIXO DIANTEIRO ────────────────────────┐\n");
    printf("  │  Esquerdo: pneu #%-4d   Direito: pneu #%-4d│\n",
           c->marcacaoFogo[0], c->marcacaoFogo[1]);
    printf("  └───────────────────────────────────────────┘\n");
    idx = 2;

    // Eixos Traseiros
    int pneusPorEixo = (strcmp(c->tipo, "TOCO") == 0) ? 2 : 4;
    int nEixosTraseiros = c->eixo.eixoTraseiro;

    for (int e = 0; e < nEixosTraseiros; e++) {
        printf("  ┌─── EIXO TRASEIRO %d ────────────────────────┐\n", e + 1);
        if (pneusPorEixo == 2) {
            printf("  │  Esquerdo: pneu #%-4d   Direito: pneu #%-4d │\n",
                   c->marcacaoFogo[idx], c->marcacaoFogo[idx + 1]);
            idx += 2;
        } else {
            // 4 pneus: duplos
            printf("  │  Esq-externo: #%-4d  Esq-interno: #%-4d   │\n",
                   c->marcacaoFogo[idx], c->marcacaoFogo[idx + 1]);
            printf("  │  Dir-interno: #%-4d  Dir-externo: #%-4d   │\n",
                   c->marcacaoFogo[idx + 2], c->marcacaoFogo[idx + 3]);
            idx += 4;
        }
        printf("  └────────────────────────────────────────────┘\n");
    }
    printf("\n");
}

// ─────────────────────────────────────────
//  02 / 03 – Cadastrar Caminhão
// ─────────────────────────────────────────

void telaCadastrarCaminhao(Caminhao caminhoes[], int *total) {
    if (*total >= MAX_CAMINHOES) {
        printf("\n  Limite de %d caminhões atingido.\n", MAX_CAMINHOES);
        pausar();
        return;
    }

    limparTela();
    printf("╔══════════════════════════════════════════╗\n");
    printf("║         CADASTRAR CAMINHÃO               ║\n");
    printf("╚══════════════════════════════════════════╝\n\n");

    Caminhao c;
    memset(&c, 0, sizeof(Caminhao));

    // ── 02: Escolha do tipo de eixo ──
    int opcao = 0;
    printf("  Selecione o tipo de caminhão:\n");
    printf("  1 - TOCO        (4 pneus  | 1 eixo dianteiro + 1 traseiro)\n");
    printf("  2 - TRUCK       (10 pneus | 1 eixo dianteiro + 2 traseiros duplos)\n");
    printf("  3 - CARRETA 3 EIXOS (18 pneus | 1 eixo dianteiro + 4 traseiros duplos)\n");
    printf("\n  Opção: ");

    while (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > 3) {
        limparBuffer();
        printf("  Opção inválida. Digite 1, 2 ou 3: ");
    }
    limparBuffer();

    switch (opcao) {
        case 1:
            strcpy(c.tipo, "TOCO");
            c.eixo.eixoDianteiro = 1;
            c.eixo.eixoTraseiro  = 1;
            c.qntdTotalPneus     = 2 + (1 * 2); // 4
            break;
        case 2:
            strcpy(c.tipo, "TRUCK");
            c.eixo.eixoDianteiro = 1;
            c.eixo.eixoTraseiro  = 2;
            c.qntdTotalPneus     = 2 + (2 * 4); // 10
            break;
        case 3:
            strcpy(c.tipo, "CARRETA 3 EIXOS");
            c.eixo.eixoDianteiro = 1;
            c.eixo.eixoTraseiro  = 4;
            c.qntdTotalPneus     = 2 + (4 * 4); // 18
            break;
    }

    // ── Placa ──
    printf("\n  Informe a placa do caminhão (%s): ", c.tipo);
    fgets(c.placa, sizeof(c.placa), stdin);
    c.placa[strcspn(c.placa, "\n")] = '\0';

    // ── Verificar placa duplicada ──
    for (int i = 0; i < *total; i++) {
        if (caminhoes[i].cadastrado && strcmp(caminhoes[i].placa, c.placa) == 0) {
            printf("\n  Já existe um caminhão com essa placa!\n");
            pausar();
            return;
        }
    }

    // ── 03: Marcações de fogo (únicas) ──
    printf("\n  Informe o número de fogo para cada pneu.\n");
    printf("  (O número deve ser único em todo o sistema)\n\n");

    for (int i = 0; i < c.qntdTotalPneus; i++) {
        int num, valido;
        do {
            valido = 1;
            printf("  Pneu %2d de %2d (marcação de fogo): ", i + 1, c.qntdTotalPneus);
            if (scanf("%d", &num) != 1) {
                limparBuffer();
                printf("  Digite apenas números inteiros.\n");
                valido = 0;
                continue;
            }
            limparBuffer();

            if (num <= 0) {
                printf("  O número de fogo deve ser positivo.\n");
                valido = 0;
                continue;
            }

            // Checar duplicata nos outros caminhões já cadastrados
            if (numeroFogoJaExiste(caminhoes, *total, num, -1)) {
                printf("  Número %d já está em uso em outro caminhão. Escolha outro.\n", num);
                valido = 0;
                continue;
            }

            // Checar duplicata dentro deste próprio caminhão
            c.marcacaoFogo[i] = num;
            if (fogoRepetidoLocal(c.marcacaoFogo, i)) {
                printf("  Número %d já foi atribuído a outro pneu deste caminhão.\n", num);
                valido = 0;
            }
        } while (!valido);
    }

    c.cadastrado = 1;
    caminhoes[*total] = c;
    (*total)++;

    printf("\n  ✔ Caminhão cadastrado com sucesso!\n\n");
    imprimirDisposicaoPneus(&caminhoes[*total - 1]);
    pausar();
}

// ─────────────────────────────────────────
//  Listar todos os caminhões
// ─────────────────────────────────────────

void telaListarCaminhoes(Caminhao caminhoes[], int total) {
    limparTela();
    printf("╔══════════════════════════════════════════╗\n");
    printf("║         CAMINHÕES CADASTRADOS            ║\n");
    printf("╚══════════════════════════════════════════╝\n\n");

    if (total == 0) {
        printf("  Nenhum caminhão cadastrado.\n\n");
        pausar();
        return;
    }

    for (int i = 0; i < total; i++) {
        if (!caminhoes[i].cadastrado) continue;
        printf("  [%d] Placa: %-10s  Tipo: %-16s  Pneus: %d\n",
               i + 1,
               caminhoes[i].placa,
               caminhoes[i].tipo,
               caminhoes[i].qntdTotalPneus);
    }
    printf("\n");
    pausar();
}

// ─────────────────────────────────────────
//  Verificar informações de um caminhão
// ─────────────────────────────────────────

void telaVerificarInformacoes(Caminhao caminhoes[], int total) {
    limparTela();
    printf("╔══════════════════════════════════════════╗\n");
    printf("║      VERIFICAR INFORMAÇÕES               ║\n");
    printf("╚══════════════════════════════════════════╝\n\n");

    if (total == 0) {
        printf("  Nenhum caminhão cadastrado.\n\n");
        pausar();
        return;
    }

    char pesquisa[20];
    printf("  Digite a placa para pesquisar: ");
    fgets(pesquisa, sizeof(pesquisa), stdin);
    pesquisa[strcspn(pesquisa, "\n")] = '\0';

    for (int i = 0; i < total; i++) {
        if (!caminhoes[i].cadastrado) continue;
        if (strcmp(caminhoes[i].placa, pesquisa) == 0) {
            printf("\n  ✔ Caminhão encontrado!\n");
            imprimirDisposicaoPneus(&caminhoes[i]);
            pausar();
            return;
        }
    }

    printf("\n  Placa '%s' não encontrada.\n\n", pesquisa);
    pausar();
}

// ─────────────────────────────────────────
//  04 – Rodízio de pneus
// ─────────────────────────────────────────

void telaFazerRodizio(Caminhao caminhoes[], int total) {
    limparTela();
    printf("╔══════════════════════════════════════════╗\n");
    printf("║         RODÍZIO DE PNEUS                 ║\n");
    printf("╚══════════════════════════════════════════╝\n\n");

    if (total == 0) {
        printf("  Nenhum caminhão cadastrado.\n\n");
        pausar();
        return;
    }

    char pesquisa[20];
    printf("  Digite a placa do caminhão: ");
    fgets(pesquisa, sizeof(pesquisa), stdin);
    pesquisa[strcspn(pesquisa, "\n")] = '\0';

    int idx = -1;
    for (int i = 0; i < total; i++) {
        if (caminhoes[i].cadastrado && strcmp(caminhoes[i].placa, pesquisa) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("\n  Placa '%s' não encontrada.\n\n", pesquisa);
        pausar();
        return;
    }

    Caminhao *c = &caminhoes[idx];

    // Mostrar disposição atual
    printf("\n  Disposição ATUAL dos pneus:\n");
    imprimirDisposicaoPneus(c);

    // Solicitar os dois pneus a trocar
    int fogo1, fogo2;
    int pos1 = -1, pos2 = -1;

    printf("  Informe o número de fogo do 1º pneu a trocar: ");
    scanf("%d", &fogo1);
    limparBuffer();

    printf("  Informe o número de fogo do 2º pneu a trocar: ");
    scanf("%d", &fogo2);
    limparBuffer();

    if (fogo1 == fogo2) {
        printf("\n  Os dois pneus são iguais. Nenhuma troca realizada.\n\n");
        pausar();
        return;
    }

    // Buscar posições dos pneus NESTE caminhão
    for (int p = 0; p < c->qntdTotalPneus; p++) {
        if (c->marcacaoFogo[p] == fogo1) pos1 = p;
        if (c->marcacaoFogo[p] == fogo2) pos2 = p;
    }

    if (pos1 == -1) {
        printf("\n  Pneu #%d não encontrado neste caminhão.\n\n", fogo1);
        pausar();
        return;
    }
    if (pos2 == -1) {
        printf("\n  Pneu #%d não encontrado neste caminhão.\n\n", fogo2);
        pausar();
        return;
    }

    // Realizar a troca
    int tmp = c->marcacaoFogo[pos1];
    c->marcacaoFogo[pos1] = c->marcacaoFogo[pos2];
    c->marcacaoFogo[pos2] = tmp;

    printf("\n  ✔ Rodízio realizado: pneu #%d e pneu #%d foram trocados!\n", fogo1, fogo2);

    // Mostrar nova disposição
    printf("\n  Nova disposição dos pneus:\n");
    imprimirDisposicaoPneus(c);
    pausar();
}

// ─────────────────────────────────────────
//  Menu Principal
// ─────────────────────────────────────────

int main(void) {
    SetConsoleOutputCP(CP_UTF8);
    // Login
    if (!validarLogin()) return 0;

    Caminhao caminhoes[MAX_CAMINHOES];
    memset(caminhoes, 0, sizeof(caminhoes));
    int total = 0;

    int opcao = 0;

    do {
        limparTela();
        printf("╔══════════════════════════════════════════╗\n");
        printf("║      SISTEMA DE GERENCIAMENTO DE PNEUS   ║\n");
        printf("╚══════════════════════════════════════════╝\n\n");
        printf("  1 - Cadastrar Caminhão\n");
        printf("  2 - Listar Caminhões\n");
        printf("  3 - Verificar Informações de um Caminhão\n");
        printf("  4 - Fazer Rodízio de Pneus\n");
        printf("  5 - Legenda dos Eixos\n");
        printf("  6 - Sair\n");
        printf("\n  Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = 0;
        } else {
            limparBuffer();
        }

        switch (opcao) {
            case 1: telaCadastrarCaminhao(caminhoes, &total); break;
            case 2: telaListarCaminhoes(caminhoes, total);    break;
            case 3: telaVerificarInformacoes(caminhoes, total); break;
            case 4: telaFazerRodizio(caminhoes, total);       break;
            case 5: telaLegenda();                            break;
            case 6: printf("\n  Encerrando o sistema. Até logo!\n\n"); break;
            default:
                printf("\n  Opção inválida. Tente novamente.\n\n");
                pausar();
        }
    } while (opcao != 6);

    return 0;
}