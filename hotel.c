/* 
 * Sistema de Gerenciamento Hoteleiro - Hotel Descanso Garantido
 * Desenvolvido por: Guilherme, Rafael e Samuel
 * 
 * Descrição: Sistema completo para gerenciamento de um hotel,
 * incluindo cadastro de clientes, funcionários, quartos,
 * gerenciamento de estadias e sistema de fidelidade.
 */

#include <stdio.h>      // (printf, scanf, etc.)
#include <stdlib.h>     // (FILE, malloc, etc.)
#include <string.h>     // Para funções de manipulação de strings

// ============================================================
// DEFINIÇÃO DE STRUCTS (ESTRUTURAS DE DADOS)
// ============================================================

/*
 * Estrutura para armazenar datas
 * Armazena dia, mês e ano para uso em todo o sistema
 */
typedef struct
{
    int dia;    // Dia do mês (1-31)
    int mes;    // Mês do ano (1-12)
    int ano;    // Ano completo (ex: 2024)
} Data;

/*
 * Estrutura para armazenar informações dos clientes
 * Contém todos os dados necessários para identificar e contatar clientes
 */
typedef struct
{
    int codigoCliente;      // Código único do cliente (gerado automaticamente)
    char nome[50];          // Nome completo do cliente (máximo 49 caracteres + \0)
    char endereco[50];      // Endereço do cliente
    char telefone[20];      // Telefone para contato
} Cliente;

/*
 * Estrutura para armazenar informações dos funcionários
 * Registra dados dos colaboradores do hotel
 */
typedef struct
{
    int codigoFuncionario;  // Código único do funcionário
    char nome[50];          // Nome completo do funcionário
    char telefone[20];      // Telefone para contato
    char cargo[30];         // Cargo/função no hotel
    double salario;         // Salário mensal
} Funcionario;

/*
 * Estrutura para armazenar informações dos quartos
 * Define as características e status de cada quarto
 */
typedef struct
{
    int numeroQuarto;       // Número do quarto (deve ser único)
    int capacidade;         // Número máximo de hóspedes
    double valorDiaria;     // Preço por diária
    int estaOcupado;        // Status: 0 = livre, 1 = ocupado
} Quarto;

/*
 * Estrutura para armazenar informações das estadias
 * Registra todas as informações de uma hospedagem
 */
typedef struct
{
    int codigoEstadia;      // Código único da estadia
    int codigoCliente;      // Código do cliente hospedado
    int numeroQuarto;       // Número do quarto ocupado
    Data dataEntrada;       // Data de check-in
    Data dataSaida;         // Data de check-out
    int quantidadeDiarias;  // Número total de diárias
    int estadiaAtiva;       // Status: 1 = ativa, 0 = finalizada
} Estadia;

// ============================================================
// DEFINIÇÃO DOS NOMES DOS ARQUIVOS DE DADOS
// ============================================================

// Nomes dos arquivos binários onde os dados serão persistidos
const char *ARQ_CLIENTES = "clientes.bin";        // Arquivo de clientes
const char *ARQ_FUNCIONARIOS = "funcionarios.bin"; // Arquivo de funcionários
const char *ARQ_QUARTOS = "quartos.bin";          // Arquivo de quartos
const char *ARQ_ESTADIAS = "estadias.bin";        // Arquivo de estadias

// ============================================================
// FUNÇÃO AUXILIAR PARA LIMPAR BUFFER DE ENTRADA
// ============================================================

/*
 * Função: limparEntrada
 * Objetivo: Limpar o buffer de entrada do teclado para evitar problemas
 *           com funções como scanf e fgets
 * Parâmetros: -
 * Retorno: void
 */
void limparEntrada()
{
    int c;  // Variável para armazenar cada caractere
    // Lê e descarta todos os caracteres até encontrar \n ou EOF
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // Loop vazio - apenas consome os caracteres
    }
}

// ============================================================
// FUNÇÕES PARA CÁLCULO DE DIÁRIAS (VALIDAÇÃO DE DATAS)
// ============================================================

/*
 * Função: diasFevereiro
 * Objetivo: Determinar quantos dias tem fevereiro em um determinado ano
 *           Considera anos bissextos
 * Parâmetros: ano - ano a ser verificado
 * Retorno: int - 29 se for bissexto, 28 se não for
 */
int diasFevereiro(int ano)
{
    int bissexto = 0;  // Flag para indicar se é ano bissexto

    // Verifica se é ano bissexto (divisível por 4)
    if (ano % 4 == 0)
    {
        // Exceção: se for divisível por 100, só é bissexto se for divisível por 400
        if (ano % 100 != 0 || ano % 400 == 0)
            bissexto = 1;  // É bissexto
    }
    
    // Retorna quantidade de dias conforme o ano
    if (bissexto == 1)
        return 29;  // Fevereiro em ano bissexto
    else
        return 28;  // Fevereiro em ano não bissexto
}

/*
 * Função: diasNoMes
 * Objetivo: Retornar o número de dias em um determinado mês/ano
 * Parâmetros: mes - mês (1-12)
 *             ano - ano para verificar fevereiro
 * Retorno: int - número de dias no mês
 */
int diasNoMes(int mes, int ano)
{
    // Switch para determinar dias conforme o mês
    switch (mes)
    {
    case 2:  // Fevereiro - depende do ano
        return diasFevereiro(ano);
    case 4:   // Abril
    case 6:   // Junho
    case 9:   // Setembro
    case 11:  // Novembro
        return 30;  // Meses com 30 dias
    default:  // Todos os outros meses
        return 31;  // Meses com 31 dias
    }
}













// ============================================================
// FUNÇÃO PRINCIPAL - MENU DO SISTEMA (ESQUELETO INICIAL)
// ============================================================

/*
 * Função: main
 * Objetivo: Exibe menu e gerencia navegação entre funcionalidades
 * Parâmetros:-
 * Retorno: int - 0 se execução bem sucedida
 */
int main()
{
    int menu;  // Armazena opção escolhida pelo usuário
    
    do
    {
        // Exibe menu com categorias organizadas
        printf("\n========== HOTEL DESCANSO GARANTIDO ==========\n");
        
        printf("\n=== CADASTROS ===\n");
        printf("1 - Cadastrar novo cliente\n");
        printf("2 - Cadastrar novo funcionario\n");
        printf("3 - Cadastrar novo quarto\n");
        
        printf("\n=== ESTADIAS ===\n");
        printf("4 - Registrar nova estadia\n");
        printf("5 - Encerrar estadia\n");
        
        printf("\n=== CONSULTAS ===\n");
        printf("6 - Mostrar todos os clientes\n");
        printf("7 - Mostrar todos os funcionarios\n");
        printf("8 - Mostrar todos os quartos\n");
        printf("9 - Mostrar todas as estadias\n");
        
        printf("\n=== PESQUISAS ===\n");
        printf("10 - Pesquisar cliente\n");
        printf("11 - Pesquisar funcionario\n");
        printf("12 - Mostrar estadias de um cliente\n");
        printf("13 - Calcular pontos de fidelidade\n");
        
        printf("\n=== SISTEMA ===\n");
        printf("0 - Sair\n");
        
        printf("\nOpcao: ");
        scanf("%d", &menu);
        limparEntrada();
        
        if (menu < 0)
        {
            printf("Opcao invalida! Digite um numero positivo.\n");
            continue;
        }

        // Switch para ir para cada função (ainda não implementadas)
        switch (menu)
        {
        case 1:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Cadastrar cliente\n");
            break;
        case 2:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Cadastrar funcionario\n");
            break;
        case 3:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Cadastrar quarto\n");
            break;
        case 4:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Nova estadia\n");
            break;
        case 5:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Encerrar estadia\n");
            break;
        case 6:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Mostrar clientes\n");
            break;
        case 7:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Mostrar funcionarios\n");
            break;
        case 8:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Mostrar quartos\n");
            break;
        case 9:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Mostrar estadias\n");
            break;
        case 10:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Pesquisar cliente\n");
            break;
        case 11:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Pesquisar funcionario\n");
            break;
        case 12:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Mostrar estadias de cliente\n");
            break;
        case 13:
            printf("\n[FUNCAO NAO IMPLEMENTADA] Calcular pontos de fidelidade\n");
            break;
        case 0:
            printf("\nSaindo do sistema...\n");
            printf("Obrigado por usar o sistema do Hotel Descanso Garantido!\n");
            break;
        default:
            // Opção inválida
            printf("Opcao invalida! Digite um numero entre 0 e 13.\n");
        }
        
        if (menu != 0) {
            printf("\nPressione ENTER para voltar ao menu...");
            getchar();
        }

    } while (menu != 0);  // Continua até usuário escolher sair (0)

    return 0;  // Retorno padrão indicando execução bem sucedida
}