#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>

typedef struct Pergunta {
    char enunciado[256];
    char alternativas[3][100];  // 3 alternativas para a pergunta
    char resposta;       // Letra da resposta correta ('A', 'B', 'C')
    int tempo;
    struct Pergunta *prox;
}Pergunta;

Pergunta* perguntaAleatoria(Pergunta* head);

int main() {
    srand(time(NULL));  // Inicializa a semente do gerador de números aleatórios

    // perguntas fáceis(40 seg)
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

    // Perguntas Médias(30 seg)
    Pergunta *q3 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q3->enunciado, "3. Qual é o estilo arquitetônico da Igrejinha de Piedade?");
    strcpy(q3->alternativas[0], " a) Barroco");
    strcpy(q3->alternativas[1], " b) Neoclássico");
    strcpy(q3->alternativas[2], " c) Maneirista");
    q3->resposta = 'C';
    q3->tempo = 30;

    q2->prox = q3;

    Pergunta *q4 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q4->enunciado, "4. Qual foi a doação feita por Francisco Gomes Salgueiro relacionada à igreja?");
    strcpy(q4->alternativas[0], " a) Um terreno");
    strcpy(q4->alternativas[1], " b) A capela e propriedades");
    strcpy(q4->alternativas[2], " c) Um altar");
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

    //Perguntas Difíceis (30 seg)
    Pergunta *q6 = (Pergunta*)malloc(sizeof(Pergunta));
    strcpy(q6->enunciado, "6. Que medidas as autoridades têm tomado para lidar com os ataques de tubarão nas praias perto da Igrejinha de Piedade?");
    strcpy(q6->alternativas[0], " a) Limitar o acesso às praias");
    strcpy(q6->alternativas[1], " b) Reforçar a vigilância com drones");
    strcpy(q6->alternativas[2], " c) Fazer estudos sobre o comportamento dos tubarões");
    q6->resposta = 'C';
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
    
   // Tela Inicial
    int screen = 0;         // Variável para rastrear a tela atual
    float timer = 0.0f;     // Contador de tempo para a mudança de tela
    float changeTime = 5.0f; // Tempo (em segundos) para mudar de tela

    SetTargetFPS(60); // Define o FPS alvo para uma contagem de tempo mais precisa

    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib basic window"); //desenha a janela
    SetTargetFPS(60);

    while (!WindowShouldClose()) { // enquanto a janela estiver aberta
        // Define o tamanho dos botões
        int buttonWidth = 200;
        int buttonHeight = 50;
        int spacing = 20;  // Espaço entre os botões

        // Calcula a posição horizontal (x) para centralizar os botões
        int centerX = (GetScreenWidth() - buttonWidth) / 2;

        // Define as posições verticais (y) dos três botões
        Rectangle button1 = {centerX, 150, buttonWidth, buttonHeight};
        Rectangle button2 = {centerX, button1.y + buttonHeight + spacing, buttonWidth, buttonHeight};
        Rectangle button3 = {centerX, button2.y + buttonHeight + spacing, buttonWidth, buttonHeight};

        // Obtém a posição atual do mouse
        Vector2 mousePosition = GetMousePosition();

        timer += GetFrameTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (screen == 0) {
            DrawText("QuizSharck", 130, 220, 20, BLACK);

            // Verifica se o tempo necessário foi alcançado
            if (timer >= changeTime) {
                DrawText("Bem-vindo ao QuizShark! Pressione Enter para começar!", 130, 520, 20, BLACK);
                // Inicia o quiz ao pressionar Enter
                if (IsKeyPressed(KEY_ENTER)) {
                    screen = 1; 
                }
            }

        } else if (screen == 1) {

            Pergunta *perguntaAtual = perguntaAleatoria(head);

            DrawText(perguntaAtual->enunciado, 130, 220, 20, BLACK);

            // Desenha os três botões
            DrawRectangleRec(button1, LIGHTGRAY);
            DrawRectangleRec(button2, LIGHTGRAY);
            DrawRectangleRec(button3, LIGHTGRAY);

            // Adiciona texto dentro de cada botão
            // Calcula a posição do texto para centralizá-lo dentro do botão
            DrawText(perguntaAtual->alternativas[0], button1.x + (buttonWidth - MeasureText(perguntaAtual->alternativas[0], 20)) / 2, button1.y + 15, 20, DARKGRAY);
            DrawText(perguntaAtual->alternativas[1], button2.x + (buttonWidth - MeasureText(perguntaAtual->alternativas[1], 20)) / 2, button2.y + 15, 20, DARKGRAY);
            DrawText(perguntaAtual->alternativas[2], button3.x + (buttonWidth - MeasureText(perguntaAtual->alternativas[2], 20)) / 2, button3.y + 15, 20, DARKGRAY);

            // Verifica se um botão foi clicado e qual
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                char alternativaEscolhida;

                if(CheckCollisionPointRec(mousePosition, button1)) {
                    alternativaEscolhida = 'A';
                }else if(CheckCollisionPointRec(mousePosition, button2)) {
                    alternativaEscolhida = 'B';
                }else{
                    alternativaEscolhida = 'C';
                }

                if(alternativaEscolhida == perguntaAtual->resposta) {
                    screen = 1;
                }
            }else{
                screen = 2;
            }
        }else if(screen == 2) {
            DrawText("Game Over!", 130, 220, 20, BLACK);
            if (timer >= changeTime) {
                screen = 0;
                timer = 0.0f; // Reinicia o temporizador
            }
        }
        
        EndDrawing();

    }
    CloseWindow();
    return 0;
}

// Função para escolher uma pergunta aleatória
Pergunta* perguntaAleatoria(Pergunta *head) {
    int indiceAleatorio = rand() % 8; // rand() % 8 limita os números entre 0 e 7

    for (int i = 0; i < indiceAleatorio; i++) {
        head = head->prox;
    }
    return head;
}