#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>


#define SCREEN_WIDTH 913
#define SCREEN_HEIGHT 913

typedef struct Pergunta {
    char enunciado[256];
    char alternativas[3][256]; 
    char resposta;       
    int tempo;
    int respondida; 
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

void reiniciarVidas(Vida **head, int numVidas) {
    liberarVidas(*head);
    *head = NULL;
    for (int i = 0; i < numVidas; i++) {
        adicionarVida(head);
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

int centerX(int width) {
    return (SCREEN_WIDTH - width) / 2;
}

int centerY(int height) {
    return (SCREEN_HEIGHT - height) / 2;
}

void exibirRanking(Jogador ranking[], int numJogadores) {
    ClearBackground(RAYWHITE);
    DrawText("Ranking dos Jogadores", centerX(550), centerY(100)-200, 50, BLACK);
    DrawRectangle(centerX(400), 275, 400, 450, BLACK);

    if (numJogadores == 0) {
        DrawText("Nenhum jogador no ranking ainda", centerX(400), 300, 20, DARKGRAY);
    } else {
        for (int i = 0; i < numJogadores && i < 10; i++) {
            DrawText(TextFormat("%d. %s - %.2f segundos", i + 1, ranking[i].nome, ranking[i].tempoTotal), centerX(290), 300 + i * 40, 20, WHITE);
        }
    }
}

void exibirInstrucoes(Texture2D igreja) {
    ClearBackground(RAYWHITE);
    DrawTexture(igreja, 0, 0, WHITE);
    DrawRectangle(centerX(480), 300, 550, 200, BLACK);

    DrawText("Instruções:", centerX(330), centerY(100) - 180, 50, WHITE);
    DrawText("1. Escolha uma alternativa para cada pergunta.", centerX(400), centerY(20) - 100, 20, WHITE);
    DrawText("2. Responda todas as perguntas certas o mais ", centerX(400), centerY(20) - 60, 20, WHITE);
    DrawText("   rápido possível, se você não responder ", centerX(400), centerY(20) - 40, 20, WHITE);
    DrawText("   nenhuma, game over.", centerX(400), centerY(20) - 20, 20, WHITE);
    DrawText("3. O tempo total será usado para o ranking.", centerX(400), centerY(0), 20, WHITE);
    DrawText("Pressione ENTER para voltar ao menu", centerX(400), centerY(20) + 300, 20, DARKGRAY);
}

void exibirNovaTela(Texture2D igreja, Texture2D girl3) {
    ClearBackground(RAYWHITE);
    DrawTexture(igreja, 0, 0, WHITE);

    static int colorIndex = 0;
    static Color rainbowColors[] = { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE };
    colorIndex = (colorIndex + 1) % (sizeof(rainbowColors) / sizeof(rainbowColors[0]));

    int fontSize = 60;
    DrawText("Parabéns!!!", centerX(600), centerY(120), fontSize, rainbowColors[colorIndex]);

    // Draw girl3 texture in the bottom left corner
    DrawTexture(girl3, 0, SCREEN_HEIGHT - girl3.height, WHITE);
}

int main(void) {
    // Inicializa a janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "QuizShark");

    // Carrega a imagem de coração
    Texture2D heart = LoadTexture("imgs/heart.png");
    if (heart.id == 0) {
        printf("Erro ao carregar a imagem de coração\n");
        return 1;
    }

    // Carrega a imagem da logo
    Texture2D logo = LoadTexture("imgs/logo.png");
    if (logo.id == 0) {
        printf("Erro ao carregar a logo\n");
        return 1;
    }

    // Carrega a imagem de fundo
    Texture2D mar1 = LoadTexture("imgs/mar1.png");
    if (mar1.id == 0) {
        printf("Erro ao carregar a imagem do mar1\n");
        return 1;
    }

    Texture2D mar2 = LoadTexture("imgs/mar2.png");
    if (mar2.id == 0) {
        printf("Erro ao carregar a imagem do mar2\n");
        return 1;
    }

    Texture2D mar3 = LoadTexture("imgs/mar3.png");
    if (mar3.id == 0) {
        printf("Erro ao carregar a imagem do mar3\n");
        return 1;
    }

    Texture2D igreja = LoadTexture("imgs/igreja.png");
    if (igreja.id == 0) {
        printf("Erro ao carregar a imagem da igreja\n");
        return 1;
    }

    Texture2D shark = LoadTexture("imgs/shark.png");
    if (shark.id == 0) {
        printf("Erro ao carregar shark\n");
        return 1;
    }

    Texture2D headShark = LoadTexture("imgs/headShark.png");
    if (headShark.id == 0) {
        printf("Erro ao carregar headShark\n");
        return 1;
    }
 
    Texture2D girl1 = LoadTexture("imgs/girl1.png");
    if (girl1.id == 0) {
        printf("Erro ao carregar a imagem extra\n");
        return 1;
    }

    Texture2D girl2 = LoadTexture("imgs/girl2.png");
    if (girl2.id == 0) {
        printf("Erro ao carregar a imagem extra\n");
        return 1;
    }

    Texture2D girl3 = LoadTexture("imgs/girl3.png");
    if(girl3.id == 0) {
        printf("Erro ao carregar a imagem extra\n");
        return 1;
    }

    // Define o FPS alvo para uma contagem de tempo mais precisa
    SetTargetFPS(60);

    // Criação das perguntas
    Pergunta *head = NULL;
    adicionarPergunta(&head, ". Qual é a localização da Igrejinha de Piedade?", " a) Recife", " b) Jaboatão dos Guararapes", " c) Olinda", 'B', 40);
    adicionarPergunta(&head, ". Em que século foi fundada a Igrejinha de Piedade?", " a) Século XVI", " b) Século XVII", " c) Século XVIII", 'B', 40);
    adicionarPergunta(&head, ". Qual é o estilo arquitetônico da Igrejinha de Piedade?", " a) Barroco", " b) Neoclássico", " c) Maneirista", 'C', 30);
    adicionarPergunta(&head, ". Qual é a importância da Igrejinha de Piedade?", " a) É um ponto turístico", " b) É um local histórico", " c) Não tem importância", 'B', 30);
    adicionarPergunta(&head, ". Por que a Igrejinha de Piedade é considerada \n\n ponto crítico para incidentes com tubarões?", " a) Pela poluição da água", " b) Devido à abertura nos arrecifes", " c) Por causa da pesca excessiva", 'B', 30);
    adicionarPergunta(&head, ". Que medidas as autoridades têm tomado para lidar com \n\n os ataques de tubarão nas praias perto da Igrejinha de Piedade?", " a) Aumentar a fiscalização", " b) Proibir o banho de mar", " c) Construir barreiras", 'A', 30);
    adicionarPergunta(&head, ". Qual é a relevância da Igrejinha de Piedade \n\n no contexto histórico e social de Pernambuco?", " a) É um ponto de referência para turistas apenas", " b) Está ligada ao ciclo da cana-de-açúcar", " c) Não possui relevância histórica", 'B', 30);
    adicionarPergunta(&head, ". Qual foi a ação recomendada por especialistas em \n\n resposta ao aumento de ataques de tubarão em Piedade?", " a) Construir barreiras no mar", " b) Implantar restrições de banho em áreas de risco", " c) Organizar campanhas de conscientização", 'B', 30);

    Pergunta *perguntaAtual = head;

    // Variáveis de tela e tempo
    int screen = 0;
    float timer = 0.0f;
    float totalTime = 0.0f;
    float newScreenTimer = 0.0f; // Timer for the new screen
    int perguntasRespondidas = 0;
    bool jogoAcabou = false;
    int indicePergunta = 1; 

    char nomeJogador[50] = "";
    int nomeIndex = 0;

    Jogador ranking[100]; 
    int numJogadores = 0;

    carregarRanking(ranking, &numJogadores);

    Vida *vidas = NULL;
    for (int i = 0; i < 3; i++) {
        adicionarVida(&vidas);
    }

    float sharkPosX = 0;
    bool showHeadShark = false;
    float headSharkTimer = 0.0f;
    bool showGirl1 = false;
    float girl1Timer = 0.0f;
    bool showGirl2 = false;
    float girl2Timer = 0.0f;

    while (!WindowShouldClose()) {
        // Atualiza o timer
        if (!jogoAcabou) {
            timer += GetFrameTime();
            totalTime += GetFrameTime();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (screen == 0) {
            DrawTexture(igreja, 0, 0, WHITE);
            DrawTexture(logo, centerX(logo.width), centerY(logo.height) - 200, WHITE);
            
            Rectangle buttonPlay = { centerX(200), centerY(50) - 100, 200, 50 };
            Rectangle buttonRanking = { centerX(200), centerY(50), 200, 50 };
            Rectangle buttonInstructions = { centerX(200), centerY(50) + 100, 200, 50 };
            Rectangle buttonExit = { centerX(200), centerY(50) + 200, 200, 50 };

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
                    // Reinicializar as variáveis para uma nova partida
                    jogoAcabou = false;
                    timer = 0.0f;
                    totalTime = 0.0f;
                    perguntasRespondidas = 0;
                    indicePergunta = 1;
                    
                    // Reinicializar as perguntas para não respondidas
                    Pergunta *temp = head;
                    while (temp != NULL) {
                        temp->respondida = 0;
                        temp = temp->prox;
                    }

                    // Reinicializar as vidas
                    reiniciarVidas(&vidas, 3);

                    // Configurar o nome e outras configurações da nova partida
                    nomeIndex = 0;
                    nomeJogador[0] = '\0';
                    
                    screen = 1; 
                } else if (CheckCollisionPointRec(mousePoint, buttonRanking)) {
                    screen = 3; 
                } else if (CheckCollisionPointRec(mousePoint, buttonInstructions)) {
                    screen = 5; 
                } else if (CheckCollisionPointRec(mousePoint, buttonExit)) {
                    CloseWindow();
                }
            }
        }

        if (screen == 1) {
            DrawTexture(igreja, 0, 0, WHITE);
            DrawText("Digite seu nome:", centerX(400), centerY(40) - 200, 40, BLACK);
            DrawText(nomeJogador, centerX(400), centerY(40) - 100, 40, BLACK);
            DrawText("Pressione Enter para começar!", centerX(400), centerY(20) + 300, 20, BLACK);

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
                indicePergunta = 1;
                sharkPosX = 0; // Reinicia a posição do tubarão
            }
            
        } else if (screen == 2) {
            // Desenha a imagem de fundo
            DrawTexture(mar1, 0, 0, WHITE);

            // Converte o indice da pergunta para string, para poder imprimir na tela
            char indice[12];
            sprintf(indice, "%d", indicePergunta);
            DrawText(indice, centerX(625), 130, 20, BLACK);
            DrawText(perguntaAtual->enunciado, centerX(600), 130, 20, BLACK);

            // Botões com alternativas
            Rectangle button1 = { centerX(600), 250, 600, 50 };
            Rectangle button2 = { centerX(600), 320, 600, 50 };
            Rectangle button3 = { centerX(600), 390, 600, 50 };

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
                DrawTexture(heart, centerX(heart.width) + i * 30, 20, WHITE);
            }

            // Desenha o tubarão ou a cabeça do tubarão
            if (showHeadShark) {
                DrawTexture(headShark, sharkPosX, SCREEN_HEIGHT - headShark.height - 50, WHITE);
                headSharkTimer += GetFrameTime();
                if (headSharkTimer >= 4.0f) {
                    showHeadShark = false;
                    headSharkTimer = 0.0f;
                }
            } else {
                DrawTexture(shark, sharkPosX, SCREEN_HEIGHT - shark.height - 50, WHITE);
            }

            // Desenha a imagem extra a +10% de distância do tubarão
            float girl1PosX = sharkPosX + SCREEN_WIDTH * 0.10f;
            if (showGirl2) {
                DrawTexture(girl2, girl1PosX, SCREEN_HEIGHT - girl2.height - 50, WHITE);
                girl2Timer += GetFrameTime();
                if (girl2Timer >= 4.0f) {
                    showGirl2 = false;
                    girl2Timer = 0.0f;
                }
            } else if (showGirl1) {
                DrawTexture(girl1, girl1PosX, SCREEN_HEIGHT - girl1.height - 50, WHITE);
                girl1Timer += GetFrameTime();
                if (girl1Timer >= 4.0f) {
                    showGirl1 = false;
                    girl1Timer = 0.0f;
                }
            } else {
                DrawTexture(girl1, girl1PosX, SCREEN_HEIGHT - girl1.height - 50, WHITE);
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
                        int vidasRestantes = contarVidas(vidas);
                        if (vidasRestantes == 0) {
                            jogoAcabou = true; // Para o temporizador
                            screen = 4; 
                        } else {
                            showHeadShark = true;
                            showGirl1 = true;
                            showGirl2 = true;
                            if (vidasRestantes == 2) {
                                sharkPosX += SCREEN_WIDTH * 0.25f; // Movimenta o tubarão 25% da largura da tela
                            } else if (vidasRestantes == 1) {
                                sharkPosX += SCREEN_WIDTH * 0.25f; // Movimenta o tubarão 50% da largura da tela
                            }
                        }
                    }

                    perguntaAtual->respondida = 1; // Marca a pergunta como respondida
                    perguntasRespondidas++;
                    indicePergunta++; 
                    if (perguntasRespondidas == 8) {
                        screen = 6; 
                        jogoAcabou = true; // Para o temporizador

                        // Adiciona o jogador atual ao ranking e ordena
                        strcpy(ranking[numJogadores].nome, nomeJogador);
                        ranking[numJogadores].tempoTotal = totalTime;
                        numJogadores++;
                        bubbleSort(ranking, numJogadores);

                        salvarRanking(ranking, numJogadores);
                    } else {
                        perguntaAtual = perguntaAleatoria(head);
                    }
                }
            }
        } else if (screen == 6) {
            exibirNovaTela(igreja, girl3);
            newScreenTimer += GetFrameTime();
            if (newScreenTimer >= 5.0f) {
                screen = 3; // Go to ranking screen after 5 seconds
                newScreenTimer = 0.0f;
            }
        } else if (screen == 3) {
            DrawTexture(mar1, 0, 0, WHITE);
            exibirRanking(ranking, numJogadores);
            DrawText("Pressione ENTER para continuar", centerX(400), centerY(20) + 300, 20, LIGHTGRAY);
            if (IsKeyPressed(KEY_ENTER)) {
                screen = 0; 
            }
        } else if (screen == 4) {
            DrawTexture(mar1, 0, 0, WHITE);
            sharkPosX = SCREEN_WIDTH * 0.75f; // Posiciona o tubarão em 75% da largura da tela
            DrawTexture(shark, sharkPosX, SCREEN_HEIGHT - shark.height - 50, WHITE);
            DrawText("Game Over! Você perdeu todas as suas vidas.", 100, 100, 20, BLACK);
            DrawText("Pressione ENTER para voltar ao igreja", centerX(400), centerY(20) + 300, 20, BLACK);

            if (IsKeyPressed(KEY_ENTER)) {
                screen = 0; 
                sharkPosX = 0; // Reinicia a posição do tubarão
            }
           
        } else if (screen == 5) {
            exibirInstrucoes(igreja);
            if (IsKeyPressed(KEY_ENTER)) {
                screen = 0; 
            }
        } else if (screen == 6) {
            exibirNovaTela(igreja, girl3);
            if (IsKeyPressed(KEY_ENTER)) {
                screen = 0; 
            }
        }
        EndDrawing();
    }

    // Libera a textura da imagem de coração
    UnloadTexture(heart);

    // Libera a textura da imagem de fundo
    UnloadTexture(mar1);

    UnloadTexture(igreja);

    UnloadTexture(girl1);

    UnloadTexture(girl3);

    // Libera a memória alocada para as perguntas
    liberarPerguntas(head);

    // Libera a memória das vidas
    liberarVidas(vidas);

    // Fecha a janela e limpa os recursos
    CloseWindow();

    return 0;
}
