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
    struct Pergunta *prox;
} Pergunta;

Pergunta* perguntaAleatoria(Pergunta* head);

int main() {
    srand(time(NULL));  // Inicializa a semente do gerador de números aleatórios

    // Inicializa a janela
    InitWindow(800, 600, "QuizShark");
    SetTargetFPS(60);

    // Variáveis de controle
    int screen = 0;
    float timer = 0.0f;

    // Criação das perguntas
    Pergunta *q1 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q1->enunciado, "1. Qual é a localização da Igrejinha de Piedade?");
    strcpy(q1->alternativas[0], " a) Recife");
    strcpy(q1->alternativas[1], " b) Jaboatão dos Guararapes");
    strcpy(q1->alternativas[2], " c) Olinda");
    q1->resposta = 'B';
    q1->tempo = 40;
    
    Pergunta *head = q1;
    Pergunta *q2 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q2->enunciado, "2. Em que século foi fundada a Igrejinha de Piedade?");
    strcpy(q2->alternativas[0], " a) Século XVI");
    strcpy(q2->alternativas[1], " b) Século XVII");
    strcpy(q2->alternativas[2], " c) Século XVIII");
    q2->resposta = 'B';
    q2->tempo = 40;
    q1->prox = q2;

    Pergunta *q3 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q3->enunciado, "3. Qual é o estilo arquitetônico da Igrejinha de Piedade?");
    strcpy(q3->alternativas[0], " a) Barroco");
    strcpy(q3->alternativas[1], " b) Neoclássico");
    strcpy(q3->alternativas[2], " c) Maneirista");
    q3->resposta = 'C';
    q3->tempo = 30;
    q2->prox = q3;

    Pergunta *q4 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q4->enunciado, "4. Qual é a importância da Igrejinha de Piedade?");
    strcpy(q4->alternativas[0], " a) É um ponto turístico");
    strcpy(q4->alternativas[1], " b) É um local histórico");
    strcpy(q4->alternativas[2], " c) Não tem importância");
    q4->resposta = 'B';
    q4->tempo = 30;
    q3->prox = q4;

    Pergunta *q5 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q5->enunciado, "5. Por que a Igrejinha de Piedade é considerada um ponto crítico para incidentes com tubarões?");
    strcpy(q5->alternativas[0], " a) Pela poluição da água");
    strcpy(q5->alternativas[1], " b) Devido à abertura nos arrecifes");
    strcpy(q5->alternativas[2], " c) Por causa da pesca excessiva");
    q5->resposta = 'B';
    q5->tempo = 30;
    q4->prox = q5;

    Pergunta *q6 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q6->enunciado, "6. Que medidas as autoridades têm tomado para lidar com os ataques de tubarão nas praias perto da Igrejinha de Piedade?");
    strcpy(q6->alternativas[0], " a) Aumentar a fiscalização");
    strcpy(q6->alternativas[1], " b) Proibir o banho de mar");
    strcpy(q6->alternativas[2], " c) Construir barreiras");
    q6->resposta = 'A';
    q6->tempo = 30;
    q5->prox = q6;

    Pergunta *q7 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q7->enunciado, "7. Qual é a relevância da Igrejinha de Piedade no contexto histórico e social de Pernambuco?");
    strcpy(q7->alternativas[0], " a) É um ponto de referência para turistas apenas");
    strcpy(q7->alternativas[1], " b) Está ligada ao ciclo da cana-de-açúcar e à história colonial");
    strcpy(q7->alternativas[2], " c) Não possui relevância histórica");
    q7->resposta = 'B';
    q7->tempo = 30;
    q6->prox = q7;

    Pergunta *q8 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q8->enunciado, "8. Qual foi a ação recomendada por especialistas em resposta ao aumento de ataques de tubarão em Piedade?");
    strcpy(q8->alternativas[0], " a) Construir barreiras no mar");
    strcpy(q8->alternativas[1], " b) Implantar restrições de banho em áreas de risco");
    strcpy(q8->alternativas[2], " c) Organizar campanhas de conscientização");
    q8->resposta = 'B';
    q8->tempo = 30;
    q7->prox = q8;
    q8->prox = NULL;

    // Escolhe uma pergunta inicial aleatória
    Pergunta *perguntaAtual = perguntaAleatoria(head);

    // Loop principal do jogo
        while (!WindowShouldClose()) {
        // Define a posição e tamanho dos botões
        int buttonWidth = 300;
        int buttonHeight = 60;
        int spacing = 20;
        int centerX = (GetScreenWidth() - buttonWidth) / 2;

        Rectangle button1 = {centerX, 350, buttonWidth, buttonHeight};
        Rectangle button2 = {centerX, 350 + buttonHeight + spacing, buttonWidth, buttonHeight};
        Rectangle button3 = {centerX, 350 + 2 * (buttonHeight + spacing), buttonWidth, buttonHeight};

        Vector2 mousePosition = GetMousePosition();
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (screen == 0) {
            DrawText("QuizShark", 250, 100, 40, DARKBLUE);
            DrawText("Pressione Enter para começar!", 210, 450, 20, DARKGRAY);
            if (IsKeyPressed(KEY_ENTER)) {
                screen = 1;
                perguntaAtual = perguntaAleatoria(head);
                timer = 0.0f;
            }
        } else if (screen == 1) {
            // Caixa de fundo para a pergunta
            DrawRectangle(100, 100, 600, 100, LIGHTGRAY);
            DrawRectangleLines(100, 100, 600, 100, DARKGRAY);
            DrawText(perguntaAtual->enunciado, 120, 130, 20, DARKBLUE);

            // Botões com alternativas
            DrawRectangleRec(button1, SKYBLUE);
            DrawRectangleLinesEx(button1, 2, DARKGRAY);
            DrawText(perguntaAtual->alternativas[0], button1.x + 20, button1.y + 15, 20, BLACK);

            DrawRectangleRec(button2, SKYBLUE);
            DrawRectangleLinesEx(button2, 2, DARKGRAY);
            DrawText(perguntaAtual->alternativas[1], button2.x + 20, button2.y + 15, 20, BLACK);

            DrawRectangleRec(button3, SKYBLUE);
            DrawRectangleLinesEx(button3, 2, DARKGRAY);
            DrawText(perguntaAtual->alternativas[2], button3.x + 20, button3.y + 15, 20, BLACK);

            // Verifica a resposta
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                char alternativaEscolhida;
                if (CheckCollisionPointRec(mousePosition, button1)) {
                    alternativaEscolhida = 'A';
                } else if (CheckCollisionPointRec(mousePosition, button2)) {
                    alternativaEscolhida = 'B';
                } else if (CheckCollisionPointRec(mousePosition, button3)) {
                    alternativaEscolhida = 'C';
                }

                if (alternativaEscolhida == perguntaAtual->resposta) {
                    perguntaAtual = perguntaAleatoria(head); // Nova pergunta
                } else {
                    screen = 2;
                    timer = 0.0f;
                }
            }
        } else if (screen == 2) {
            DrawText("Game Over!", 300, 250, 40, RED);
            DrawText("Pressione Enter para tentar novamente.", 180, 320, 20, DARKGRAY);
            timer += GetFrameTime();
            if (IsKeyPressed(KEY_ENTER)) {
                screen = 0;
                timer = 0.0f;
            }
        }

        EndDrawing();
    }

    // Libera a memória das perguntas
    Pergunta *temp;
    while (head != NULL) {
        temp = head;
        head = head->prox;
        free(temp);
    }

    CloseWindow();
    return 0;
}

// Função para escolher uma pergunta aleatória
Pergunta* perguntaAleatoria(Pergunta *head) {
    int indiceAleatorio = rand() % 8; // Gera um número entre 0 e 7 para escolher a pergunta
    Pergunta *current = head;
    for (int i = 0; i < indiceAleatorio && current->prox != NULL; i++) {
        current = current->prox;
    }
    return current;
}
