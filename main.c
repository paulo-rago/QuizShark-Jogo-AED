#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>

typedef struct Pergunta {
    char enunciado[256];
    char alternativas[3][100];
    char resposta;
    int tempo;
    struct Pergunta *prox;
} Pergunta;

Pergunta* perguntaAleatoria(Pergunta** head);

int main() {
    srand(time(NULL));

    // Configuração das perguntas
    Pergunta *q1 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q1->enunciado, "1. Qual é a localização da Igrejinha de Piedade?");
    strcpy(q1->alternativas[0], " a) Recife");
    strcpy(q1->alternativas[1], " b) Jaboatão dos Guararapes");
    strcpy(q1->alternativas[2], " c) Olinda");
    q1->resposta = 'B';
    q1->tempo = 40;
    
    Pergunta *q2 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q2->enunciado, "2. Em que século foi fundada a Igrejinha de Piedade?");
    strcpy(q2->alternativas[0], " a) Século XVI");
    strcpy(q2->alternativas[1], " b) Século XVII");
    strcpy(q2->alternativas[2], " c) Século XVIII");
    q2->resposta = 'B';
    q2->tempo = 40;

    Pergunta *q3 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q3->enunciado, "3. Qual é o estilo arquitetônico da Igrejinha de Piedade?");
    strcpy(q3->alternativas[0], " a) Barroco");
    strcpy(q3->alternativas[1], " b) Neoclássico");
    strcpy(q3->alternativas[2], " c) Maneirista");
    q3->resposta = 'C';
    q3->tempo = 30;

    Pergunta *q4 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q4->enunciado, "4. Qual foi a doação feita por Francisco Gomes Salgueiro relacionada à igreja?");
    strcpy(q4->alternativas[0], " a) Um terreno");
    strcpy(q4->alternativas[1], " b) A capela e propriedades");
    strcpy(q4->alternativas[2], " c) Um altar");
    q4->resposta = 'B';
    q4->tempo = 30;

    Pergunta *q5 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q5->enunciado, "5. Por que a Igrejinha de Piedade é considerada um ponto crítico para incidentes com tubarões?");
    strcpy(q5->alternativas[0], " a) Pela poluição da água");
    strcpy(q5->alternativas[1], " b) Devido à abertura nos arrecifes");
    strcpy(q5->alternativas[2], " c) Por causa da pesca excessiva");
    q5->resposta = 'B';
    q5->tempo = 30;

    Pergunta *q6 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q6->enunciado, "6. Que medidas as autoridades têm tomado para lidar com os ataques de tubarão nas praias perto da Igrejinha de Piedade?");
    strcpy(q6->alternativas[0], " a) Limitar o acesso às praias");
    strcpy(q6->alternativas[1], " b) Reforçar a vigilância com drones");
    strcpy(q6->alternativas[2], " c) Fazer estudos sobre o comportamento dos tubarões");
    q6->resposta = 'C';
    q6->tempo = 30;

    Pergunta *q7 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q7->enunciado, "7. Qual é a relevância da Igrejinha de Piedade no contexto histórico e social de Pernambuco?");
    strcpy(q7->alternativas[0], " a) É um ponto de referência para turistas apenas");
    strcpy(q7->alternativas[1], " b) Está ligada ao ciclo da cana-de-açúcar e à história colonial");
    strcpy(q7->alternativas[2], " c) Não possui relevância histórica");
    q7->resposta = 'B';
    q7->tempo = 30;

    Pergunta *q8 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q8->enunciado, "8. Qual foi a ação recomendada por especialistas em resposta ao aumento de ataques de tubarão em Piedade?");
    strcpy(q8->alternativas[0], " a) Construir barreiras no mar");
    strcpy(q8->alternativas[1], " b) Implantar restrições de banho em áreas de risco");
    strcpy(q8->alternativas[2], " c) Organizar campanhas de conscientização");
    q8->resposta = 'B';
    q8->tempo = 30;

    // Conecta as perguntas na lista encadeada
    q1->prox = q2;
    q2->prox = q3;
    q3->prox = q4;
    q4->prox = q5;
    q5->prox = q6;
    q6->prox = q7;
    q7->prox = q8;
    q8->prox = NULL;

    Pergunta *head = q1;

    InitWindow(800, 600, "Quiz Shark");
    SetTargetFPS(60);

    int screen = 0;
    Pergunta *perguntaAtual = perguntaAleatoria(&head);
    double startTime = 0.0;
    double tempoRestante = perguntaAtual ? perguntaAtual->tempo : 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (screen == 0) {
            DrawText("QuizShark", 130, 220, 20, BLACK);
            DrawText("Bem-vindo ao QuizShark! Pressione Enter para começar!", 130, 520, 20, BLACK);

            if (IsKeyPressed(KEY_ENTER)) {
                screen = 1;
                startTime = GetTime();
                tempoRestante = perguntaAtual ? perguntaAtual->tempo : 0;
            }

        } else if (screen == 1) {
            if (!perguntaAtual) {
                screen = 3; // Tela de "Fim de Jogo" se não houver mais perguntas
            } else {
                tempoRestante = perguntaAtual->tempo - (GetTime() - startTime);

                if (tempoRestante <= 0.0) {
                    screen = 2; // Muda para "Game Over" se o tempo acabar
                }

                DrawText(perguntaAtual->enunciado, 130, 220, 20, BLACK);

                Rectangle button1 = { 300, 250, 200, 50 };
                Rectangle button2 = { 300, 310, 200, 50 };
                Rectangle button3 = { 300, 370, 200, 50 };

                DrawRectangleRec(button1, LIGHTGRAY);
                DrawRectangleRec(button2, LIGHTGRAY);
                DrawRectangleRec(button3, LIGHTGRAY);

                DrawText(perguntaAtual->alternativas[0], button1.x + 10, button1.y + 15, 20, DARKGRAY);
                DrawText(perguntaAtual->alternativas[1], button2.x + 10, button2.y + 15, 20, DARKGRAY);
                DrawText(perguntaAtual->alternativas[2], button3.x + 10, button3.y + 15, 20, DARKGRAY);

                Vector2 mousePosition = GetMousePosition();

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    char alternativaEscolhida = '\0';

                    if (CheckCollisionPointRec(mousePosition, button1)) {
                        alternativaEscolhida = 'A';
                    } else if (CheckCollisionPointRec(mousePosition, button2)) {
                        alternativaEscolhida = 'B';
                    } else if (CheckCollisionPointRec(mousePosition, button3)) {
                        alternativaEscolhida = 'C';
                    }

                    if (alternativaEscolhida == perguntaAtual->resposta) {
                        free(perguntaAtual); // Libera a pergunta respondida
                        perguntaAtual = perguntaAleatoria(&head);
                        startTime = GetTime();
                        tempoRestante = perguntaAtual ? perguntaAtual->tempo : 0;
                    } else {
                        screen = 2;
                    }
                }

                char tempoTexto[32];
                snprintf(tempoTexto, sizeof(tempoTexto), "Tempo: %.0f", tempoRestante);
                DrawText(tempoTexto, 650, 20, 20, RED);
            }

        } else if (screen == 2) {
            DrawText("Game Over!", 130, 220, 20, BLACK);

            if (IsKeyPressed(KEY_ENTER)) {
                screen = 0;
                perguntaAtual = perguntaAleatoria(&head);
            }
        } else if (screen == 3) {
            DrawText("Parabéns! Você respondeu todas as perguntas!", 130, 220, 20, BLACK);

            if (IsKeyPressed(KEY_ENTER)) {
                screen = 0;
                perguntaAtual = perguntaAleatoria(&head);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Função para escolher uma pergunta aleatória e removê-la da lista
Pergunta* perguntaAleatoria(Pergunta **head) {
    if (*head == NULL) return NULL; // Verifica se há perguntas restantes

    int count = 0;
    Pergunta *temp = *head;

    while (temp) { // Conta o número de perguntas restantes
        count++;
        temp = temp->prox;
    }

    int indiceAleatorio = rand() % count;
    Pergunta *prev = NULL;
    temp = *head;

    for (int i = 0; i < indiceAleatorio; i++) {
        prev = temp;
        temp = temp->prox;
    }

    if (prev == NULL) { // Se a pergunta é a primeira da lista
        *head = temp->prox;
    } else {
        prev->prox = temp->prox;
    }

    return temp;
}
