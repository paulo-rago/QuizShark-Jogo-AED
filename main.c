#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>

typedef struct Pergunta {
    char enunciado[256];
    char alternativas[3][256];  // 3 alternativas para a pergunta
    char resposta;       // Letra da resposta correta ('A', 'B', 'C')
    int tempo;
    int respondida; // Novo campo para marcar se a pergunta foi respondida
    struct Pergunta *prox;
} Pergunta;

typedef struct Jogador {
    char nome[50];
    float tempoTotal;
} Jogador;

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

int main(void)
{
    // Inicializa a janela
    InitWindow(800, 600, "QuizShark");

    // Define o FPS alvo para uma contagem de tempo mais precisa
    SetTargetFPS(60);

    // Criação das perguntas
    Pergunta *q1 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q1->enunciado, "1. Qual é a localização da Igrejinha de Piedade?");
    strcpy(q1->alternativas[0], " a) Recife");
    strcpy(q1->alternativas[1], " b) Jaboatão dos Guararapes");
    strcpy(q1->alternativas[2], " c) Olinda");
    q1->resposta = 'B';
    q1->tempo = 40;
    q1->respondida = 0; // Inicializa como não respondida
    
    Pergunta *head = q1;
    Pergunta *q2 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q2->enunciado, "2. Em que século foi fundada a Igrejinha de Piedade?");
    strcpy(q2->alternativas[0], " a) Século XVI");
    strcpy(q2->alternativas[1], " b) Século XVII");
    strcpy(q2->alternativas[2], " c) Século XVIII");
    q2->resposta = 'B';
    q2->tempo = 40;
    q2->respondida = 0; // Inicializa como não respondida
    q1->prox = q2;

    Pergunta *q3 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q3->enunciado, "3. Qual é o estilo arquitetônico da Igrejinha de Piedade?");
    strcpy(q3->alternativas[0], " a) Barroco");
    strcpy(q3->alternativas[1], " b) Neoclássico");
    strcpy(q3->alternativas[2], " c) Maneirista");
    q3->resposta = 'C';
    q3->tempo = 30;
    q3->respondida = 0; // Inicializa como não respondida
    q2->prox = q3;

    Pergunta *q4 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q4->enunciado, "4. Qual é a importância da Igrejinha de Piedade?");
    strcpy(q4->alternativas[0], " a) É um ponto turístico");
    strcpy(q4->alternativas[1], " b) É um local histórico");
    strcpy(q4->alternativas[2], " c) Não tem importância");
    q4->resposta = 'B';
    q4->tempo = 30;
    q4->respondida = 0; // Inicializa como não respondida
    q3->prox = q4;

    Pergunta *q5 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q5->enunciado, "5. Por que a Igrejinha de Piedade é considerada um ponto crítico para incidentes com tubarões?");
    strcpy(q5->alternativas[0], " a) Pela poluição da água");
    strcpy(q5->alternativas[1], " b) Devido à abertura nos arrecifes");
    strcpy(q5->alternativas[2], " c) Por causa da pesca excessiva");
    q5->resposta = 'B';
    q5->tempo = 30;
    q5->respondida = 0; // Inicializa como não respondida
    q4->prox = q5;

    Pergunta *q6 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q6->enunciado, "6. Que medidas as autoridades têm tomado para lidar com os ataques de tubarão nas praias perto da Igrejinha de Piedade?");
    strcpy(q6->alternativas[0], " a) Aumentar a fiscalização");
    strcpy(q6->alternativas[1], " b) Proibir o banho de mar");
    strcpy(q6->alternativas[2], " c) Construir barreiras");
    q6->resposta = 'A';
    q6->tempo = 30;
    q6->respondida = 0; // Inicializa como não respondida
    q5->prox = q6;

    Pergunta *q7 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q7->enunciado, "7. Qual é a relevância da Igrejinha de Piedade no contexto histórico e social de Pernambuco?");
    strcpy(q7->alternativas[0], " a) É um ponto de referência para turistas apenas");
    strcpy(q7->alternativas[1], " b) Está ligada ao ciclo da cana-de-açúcar e à história colonial");
    strcpy(q7->alternativas[2], " c) Não possui relevância histórica");
    q7->resposta = 'B';
    q7->tempo = 30;
    q7->respondida = 0; // Inicializa como não respondida
    q6->prox = q7;

    Pergunta *q8 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q8->enunciado, "8. Qual foi a ação recomendada por especialistas em resposta ao aumento de ataques de tubarão em Piedade?");
    strcpy(q8->alternativas[0], " a) Construir barreiras no mar");
    strcpy(q8->alternativas[1], " b) Implantar restrições de banho em áreas de risco");
    strcpy(q8->alternativas[2], " c) Organizar campanhas de conscientização");
    q8->resposta = 'B';
    q8->tempo = 30;
    q8->respondida = 0; // Inicializa como não respondida
    q7->prox = q8;
    q8->prox = NULL;

    Pergunta *perguntaAtual = q1;

    // Variáveis de tela e tempo
    int screen = 0;
    float timer = 0.0f;
    float totalTime = 0.0f;
    int perguntasRespondidas = 0;
    bool todasPerguntasRespondidas = false;

    // Variável para armazenar o nome do jogador
    char nomeJogador[50] = "";
    int nomeIndex = 0;

    // Ranking de jogadores
    Jogador ranking[5] = {
        {"Jogador1", 120.0f},
        {"Jogador2", 150.0f},
        {"Jogador3", 180.0f},
        {"Jogador4", 200.0f},
        {"Jogador5", 220.0f}
    };

    // Loop principal do jogo
    while (!WindowShouldClose())
    {
        // Atualiza o timer
        if (!todasPerguntasRespondidas) {
            timer += GetFrameTime();
            totalTime += GetFrameTime();
        }

        // Inicia o desenho
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Desenha conteúdo baseado na tela atual
        if (screen == 0)
        {
            DrawText("Digite seu nome:", 250, 100, 40, DARKBLUE);
            DrawText(nomeJogador, 250, 200, 40, DARKBLUE);
            DrawText("Pressione Enter para começar!", 210, 450, 20, DARKGRAY);

            // Captura a entrada do teclado para o nome do jogador
            int key = GetCharPressed();
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (nomeIndex < 49))
                {
                    nomeJogador[nomeIndex] = (char)key;
                    nomeIndex++;
                    nomeJogador[nomeIndex] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && nomeIndex > 0)
            {
                nomeIndex--;
                nomeJogador[nomeIndex] = '\0';
            }

            if (IsKeyPressed(KEY_ENTER) && nomeIndex > 0) {
                screen = 1;
                perguntaAtual = perguntaAleatoria(head);
                timer = 0.0f;
            }
        }
        else if (screen == 1)
        {
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

            // Lógica para marcar a pergunta como respondida e passar para a próxima
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePoint = GetMousePosition();
                if (CheckCollisionPointRec(mousePoint, button1) || CheckCollisionPointRec(mousePoint, button2) || CheckCollisionPointRec(mousePoint, button3))
                {
                    perguntaAtual->respondida = 1; // Marca a pergunta como respondida
                    perguntasRespondidas++;
                    if (perguntasRespondidas == 8) {
                        screen = 2; // Vai para a tela de resultados
                        todasPerguntasRespondidas = true; // Para o temporizador

                        // Salva o nome do jogador e o tempo total em um arquivo de texto
                        FILE *file = fopen("ranking.txt", "a");
                        if (file != NULL) {
                            fprintf(file, "%s - %.2f segundos\n", nomeJogador, totalTime);
                            fclose(file);
                        }
                    } else {
                        perguntaAtual = perguntaAleatoria(head);
                    }
                }
            }
        }
        else if (screen == 2)
        {
            // Tela de resultados
            DrawText("Parabéns! Você respondeu todas as perguntas.", 100, 100, 20, DARKBLUE);
            DrawText(TextFormat("Tempo total: %.2f segundos", totalTime), 100, 150, 20, DARKBLUE);

            // Desenha o ranking
            DrawText("Ranking:", 100, 200, 20, DARKBLUE);
            for (int i = 0; i < 5; i++) {
                DrawText(TextFormat("%d. %s - %.2f segundos", i + 1, ranking[i].nome, ranking[i].tempoTotal), 100, 230 + i * 30, 20, DARKBLUE);
            }
        }

        // Finaliza o desenho
        EndDrawing();
    }

    // Fecha a janela e limpa os recursos
    CloseWindow();

    return 0;
}
