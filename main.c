#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct Pergunta {
    char enunciado[256];
    char alternativas[3][256]; 
    char resposta;       
    int tempo;
    int respondida; // Novo campo para marcar se a pergunta foi respondida
    struct Pergunta *prox;
} Pergunta;

typedef struct Jogador {
    char nome[50];
    float tempoTotal;
} Jogador;

typedef struct Vida {
    struct Vida *prox;
} Vida;

// As 5 funções de Pergunta (adicionar, liberar, pergunta aleatoria )

void adicionarPergunta(Pergunta **head, const char *enunciado, const char *alt1, const char *alt2, const char *alt3, char resposta, int tempo) {
    Pergunta *novaPergunta = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(novaPergunta->enunciado, enunciado);
    strcpy(novaPergunta->alternativas[0], alt1);
    strcpy(novaPergunta->alternativas[1], alt2);
    strcpy(novaPergunta->alternativas[2], alt3);
    novaPergunta->resposta = resposta;
    novaPergunta->tempo = tempo;
    novaPergunta->respondida = 0;
    novaPergunta->prox = NULL;

    if (*head == NULL) {
        *head = novaPergunta;
    } else {
        Pergunta *temp = *head;
        while (temp->prox != NULL) {
            temp = temp->prox;
        }
        temp->prox = novaPergunta;
    }
}

void liberarPerguntas(Pergunta *head) {
    Pergunta *temp;
    while (head != NULL) {
        temp = head;
        head = head->prox;
        free(temp);
    }
}

Pergunta* perguntaAleatoria(Pergunta *head) {
    Pergunta *current = head;
    int totalPerguntas = 0;

    // Conta o número total de perguntas
    while (current != NULL) {
        totalPerguntas++;
        current = current->prox;
    }

    // Se todas as perguntas foram respondidas, retorna NULL
    if (totalPerguntas == 0) {
        return NULL;
    }

    Pergunta *perguntaSelecionada = NULL;
    do {
        int indiceAleatorio = rand() % totalPerguntas; // Gera um número entre 0 e totalPerguntas-1
        current = head;
        for (int i = 0; i < indiceAleatorio && current->prox != NULL; i++) {
            current = current->prox;
        }
        if (!current->respondida) {
            perguntaSelecionada = current;
        }
    } while (perguntaSelecionada == NULL);

    return perguntaSelecionada;
}

// Funções de Vida
void adicionarVida(Vida **head) {
    Vida *novaVida = (Vida*)malloc(sizeof(Vida));
    novaVida->prox = *head;
    *head = novaVida;
}

void removerVida(Vida **head) {
    if (*head != NULL) {
        Vida *temp = *head;
        *head = (*head)->prox;
        free(temp);
    }
}

int contarVidas(Vida *head) {
    int count = 0;
    while (head != NULL) {
        count++;
        head = head->prox;
    }
    return count;
}

void liberarVidas(Vida *head) {
    Vida *temp;
    while (head != NULL) {
        temp = head;
        head = head->prox;
        free(temp);
    }
}

// funções de ranking
void bubbleSort(Jogador ranking[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (ranking[j].tempoTotal > ranking[j + 1].tempoTotal) {
                Jogador temp = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = temp;
            }
        }
    }
}

void carregarRanking(Jogador ranking[], int *n) {
    FILE *file = fopen("ranking.txt", "r");
    if (file != NULL) {
        *n = 0;
        while (fscanf(file, "%49s - %f\n", ranking[*n].nome, &ranking[*n].tempoTotal) == 2) {
            (*n)++;
        }
        fclose(file);
    }
}

void salvarRanking(Jogador ranking[], int n) {
    FILE *file = fopen("ranking.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < n; i++) {
            fprintf(file, "%s - %.2f\n", ranking[i].nome, ranking[i].tempoTotal);
        }
        fclose(file);
    }
}

void exibirRanking(Jogador ranking[], int numJogadores) {
    ClearBackground(RAYWHITE);
    DrawText("Ranking dos Jogadores:", 100, 50, 30, DARKBLUE);

    for (int i = 0; i < numJogadores && i < 10; i++) {
        DrawText(TextFormat("%d. %s - %.2f segundos", i + 1, ranking[i].nome, ranking[i].tempoTotal), 
                 100, 100 + i * 40, 20, BLACK);
    }

    DrawText("Pressione ENTER para voltar ao menu", 100, 500, 20, DARKGRAY);
}

void exibirInstrucoes() {
    ClearBackground(RAYWHITE);
    DrawText("Instruções:", 100, 50, 30, DARKBLUE);
    DrawText("1. Escolha uma alternativa para cada pergunta.", 100, 100, 20, BLACK);
    DrawText("2. Responda todas as perguntas certas o mais rápido possível, se você não responder nenhuma, game over.", 100, 140, 20, BLACK);
    DrawText("3. O tempo total será usado para o ranking.", 100, 180, 20, BLACK);
    DrawText("Pressione ENTER para voltar ao menu", 100, 500, 20, DARKGRAY);
}

int main(void) {
    // Inicializa a janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "QuizShark");

    // Define o FPS alvo para uma contagem de tempo mais precisa
    SetTargetFPS(60);

    // Criação das perguntas
    Pergunta *head = NULL;
    adicionarPergunta(&head, "1. Qual é a localização da Igrejinha de Piedade?", " a) Recife", " b) Jaboatão dos Guararapes", " c) Olinda", 'B', 40);
    adicionarPergunta(&head, "2. Em que século foi fundada a Igrejinha de Piedade?", " a) Século XVI", " b) Século XVII", " c) Século XVIII", 'B', 40);
    adicionarPergunta(&head, "3. Qual é o estilo arquitetônico da Igrejinha de Piedade?", " a) Barroco", " b) Neoclássico", " c) Maneirista", 'C', 30);
    adicionarPergunta(&head, "4. Qual é a importância da Igrejinha de Piedade?", " a) É um ponto turístico", " b) É um local histórico", " c) Não tem importância", 'B', 30);
    adicionarPergunta(&head, "5. Por que a Igrejinha de Piedade é considerada um ponto crítico para incidentes com tubarões?", " a) Pela poluição da água", " b) Devido à abertura nos arrecifes", " c) Por causa da pesca excessiva", 'B', 30);
    adicionarPergunta(&head, "6. Que medidas as autoridades têm tomado para lidar com os ataques de tubarão nas praias perto da Igrejinha de Piedade?", " a) Aumentar a fiscalização", " b) Proibir o banho de mar", " c) Construir barreiras", 'A', 30);
    adicionarPergunta(&head, "7. Qual é a relevância da Igrejinha de Piedade no contexto histórico e social de Pernambuco?", " a) É um ponto de referência para turistas apenas", " b) Está ligada ao ciclo da cana-de-açúcar e à história colonial", " c) Não possui relevância histórica", 'B', 30);
    adicionarPergunta(&head, "8. Qual foi a ação recomendada por especialistas em resposta ao aumento de ataques de tubarão em Piedade?", " a) Construir barreiras no mar", " b) Implantar restrições de banho em áreas de risco", " c) Organizar campanhas de conscientização", 'B', 30);

    Pergunta *perguntaAtual = head;

    // Variáveis de tela e tempo
    int screen = 0;
    float timer = 0.0f;
    float totalTime = 0.0f;
    int perguntasRespondidas = 0;
    bool jogoAcabou = false;

    // Variável para armazenar o nome do jogador
    char nomeJogador[50] = "";
    int nomeIndex = 0;

    // Ranking de jogadores
    Jogador ranking[100]; // Aumentado para suportar mais jogadores
    int numJogadores = 0;

    // Carrega o ranking do arquivo
    carregarRanking(ranking, &numJogadores);

    // Lista encadeada de vidas
    Vida *vidas = NULL;
    for (int i = 0; i < 3; i++) {
        adicionarVida(&vidas);
    }

    // Loop principal do jogo
    while (!WindowShouldClose()) {
        // Atualiza o timer
        if (!jogoAcabou) {
            timer += GetFrameTime();
            totalTime += GetFrameTime();
        }

        // Inicia o desenho
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenha conteúdo baseado na tela atual
        if (screen == 0) {
            DrawText("QuizShark!", 250, 100, 40, DARKBLUE);
            
            Rectangle buttonPlay = { 300, 250, 200, 50 };
            Rectangle buttonRanking = { 300, 320, 200, 50 };
            Rectangle buttonInstructions = { 300, 390, 200, 50 };
            Rectangle buttonExit = { 300, 460, 200, 50 };
 
            DrawRectangleRec(buttonPlay, SKYBLUE);
            DrawRectangleLinesEx(buttonPlay, 2, DARKGRAY);
            DrawText("Jogar", buttonPlay.x + 75, buttonPlay.y + 15, 20, BLACK);

            DrawRectangleRec(buttonRanking, SKYBLUE);
            DrawRectangleLinesEx(buttonRanking, 2, DARKGRAY);
            DrawText("Ranking", buttonRanking.x + 50, buttonRanking.y + 15, 20, BLACK);

            DrawRectangleRec(buttonInstructions, SKYBLUE);
            DrawRectangleLinesEx(buttonInstructions, 2, DARKGRAY);
            DrawText("Instruções", buttonInstructions.x + 40, buttonInstructions.y + 15, 20, BLACK);

            DrawRectangleRec(buttonExit, SKYBLUE);
            DrawRectangleLinesEx(buttonExit, 2, DARKGRAY);
            DrawText("Sair", buttonExit.x + 85, buttonExit.y + 15, 20, BLACK);


            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, buttonPlay)) {
                    screen = 1; 
                } else if (CheckCollisionPointRec(mousePoint, buttonRanking)) {
                    screen = 4; 
                } else if (CheckCollisionPointRec(mousePoint, buttonInstructions)) {
                    screen = 5; 
                } else if (CheckCollisionPointRec(mousePoint, buttonExit)) {
                    CloseWindow();
                }
            }
        }

        if (screen == 1) {
            DrawText("Digite seu nome:", 250, 100, 40, DARKBLUE);
            DrawText(nomeJogador, 250, 200, 40, DARKBLUE);
            DrawText("Pressione Enter para começar!", 210, 450, 20, DARKGRAY);

            // Captura a entrada do teclado para o nome do jogador
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (nomeIndex < 49)) {
                    nomeJogador[nomeIndex] = (char)key;
                    nomeIndex++;
                    nomeJogador[nomeIndex] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && nomeIndex > 0) {
                nomeIndex--;
                nomeJogador[nomeIndex] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER) && nomeIndex > 0) {
                screen = 2;
                perguntaAtual = perguntaAleatoria(head);
                timer = 0.0f;
            }
            
        } else if (screen == 2) {
            // Caixa de fundo para a pergunta
            DrawRectangle(100, 100, 600, 100, LIGHTGRAY);
            DrawRectangleLines(100, 100, 600, 100, DARKGRAY);
            DrawText(perguntaAtual->enunciado, 120, 130, 20, DARKBLUE);

            // Botões com alternativas
            Rectangle button1 = { 100, 250, 600, 50 };
            Rectangle button2 = { 100, 320, 600, 50 };
            Rectangle button3 = { 100, 390, 600, 50 };

            DrawRectangleRec(button1, SKYBLUE);
            DrawRectangleLinesEx(button1, 2, DARKGRAY);
            DrawText(perguntaAtual->alternativas[0], button1.x + 20, button1.y + 15, 20, BLACK);

            DrawRectangleRec(button2, SKYBLUE);
            DrawRectangleLinesEx(button2, 2, DARKGRAY);
            DrawText(perguntaAtual->alternativas[1], button2.x + 20, button2.y + 15, 20, BLACK);

            DrawRectangleRec(button3, SKYBLUE);
            DrawRectangleLinesEx(button3, 2, DARKGRAY);
            DrawText(perguntaAtual->alternativas[2], button3.x + 20, button3.y + 15, 20, BLACK);

            // Desenha as vidas restantes
            int vidasRestantes = contarVidas(vidas);
            for (int i = 0; i < vidasRestantes; i++) {
                DrawText("❤️", 700 + i * 30, 20, 20, RED);
            }

            // Lógica para marcar a pergunta como respondida e passar para a próxima
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, button1) || CheckCollisionPointRec(mousePoint, button2) || CheckCollisionPointRec(mousePoint, button3)) {
                    char respostaSelecionada;
                    if (CheckCollisionPointRec(mousePoint, button1)) respostaSelecionada = 'A';
                    else if (CheckCollisionPointRec(mousePoint, button2)) respostaSelecionada = 'B';
                    else respostaSelecionada = 'C';

                    if (respostaSelecionada != perguntaAtual->resposta) {
                        removerVida(&vidas);
                        if (contarVidas(vidas) == 0) {
                            jogoAcabou = true; // Para o temporizador
                            screen = 4; // Vai para a tela de game over
                        }
                    }

                    perguntaAtual->respondida = 1; // Marca a pergunta como respondida
                    perguntasRespondidas++;
                    if (perguntasRespondidas == 8) {
                        screen = 3; // Vai para a tela de resultados
                        jogoAcabou = true; // Para o temporizador

                        // Adiciona o jogador atual ao ranking e ordena
                        strcpy(ranking[numJogadores].nome, nomeJogador);
                        ranking[numJogadores].tempoTotal = totalTime;
                        numJogadores++;
                        bubbleSort(ranking, numJogadores);

                        // Salva o ranking atualizado no arquivo
                        salvarRanking(ranking, numJogadores);
                    } else {
                        perguntaAtual = perguntaAleatoria(head);
                    }
                }
            }
        } else if (screen == 3) {
            // Tela de resultados
            DrawText("Parabéns! Você respondeu todas as perguntas.", 100, 100, 20, DARKBLUE);
            DrawText(TextFormat("Tempo total: %.2f segundos", totalTime), 100, 150, 20, DARKBLUE);

            // Desenha o ranking
            exibirRanking(ranking, numJogadores);
            if (IsKeyPressed(KEY_ENTER)) {
                screen = 0; // Volta ao menu principal
            }

        } else if (screen == 4) {
            // Tela de game over
            DrawText("Game Over! Você perdeu todas as suas vidas.", 100, 100, 20, DARKBLUE);
            DrawText("Pressione ENTER para voltar ao menu", 100, 500, 20, DARKGRAY);

            if (IsKeyPressed(KEY_ENTER)) {
                screen = 0; // Volta ao menu principal
            }
           
        } else if (screen == 5) {
            exibirInstrucoes();
            if (IsKeyPressed(KEY_ENTER)) {
                screen = 0; // Volta ao menu principal
            }
        }
        EndDrawing();
    }

    // Libera a memória alocada para as perguntas
    liberarPerguntas(head);

    // Libera a memória das vidas
    liberarVidas(vidas);

    // Fecha a janela e limpa os recursos
    CloseWindow();

    return 0;
}
