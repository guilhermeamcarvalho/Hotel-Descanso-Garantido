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
// FUNÇÃO DE VALIDAÇÃO DE DATA
// ============================================================

/*
 * Função: validarData
 * Objetivo: Verificar se uma data é válida
 * Parâmetros: d - estrutura Data contendo dia, mês e ano
 * Retorno: int - 1 se válida, 0 se inválida
 */
int validarData(Data d) {
    // Validar ano (consideramos de 1900 a 2100 como válido)
    if (d.ano < 1900 || d.ano > 2100) {
        return 0;  // Ano fora do intervalo permitido
    }
    
    // Validar mês (deve estar entre 1 e 12)
    if (d.mes < 1 || d.mes > 12) {
        return 0;  // Mês inválido
    }
    
    // Validar dia baseado no mês e ano
    int diasNoMesValido = diasNoMes(d.mes, d.ano);  // Obtém dias do mês
    if (d.dia < 1 || d.dia > diasNoMesValido) {
        return 0;  // Dia inválido para o mês/ano
    }
    
    return 1;  // Data válida
}

// ============================================================
// FUNÇÕES PARA SALVAR DADOS EM ARQUIVOS
// ============================================================

/*
 * Função: salvarClienteArquivo
 * Objetivo: Salvar um cliente no arquivo binário
 * Parâmetros: c - estrutura Cliente a ser salva
 * Retorno: void
 */
void salvarClienteArquivo(Cliente c)
{
    FILE *arq = fopen(ARQ_CLIENTES, "ab");  // Abre para append binário
    fwrite(&c, sizeof(Cliente), 1, arq);    // Escreve estrutura no arquivo
    fclose(arq);                            // Fecha arquivo
}

/*
 * Função: salvarFuncionarioArquivo
 * Objetivo: Salvar um funcionário no arquivo binário
 * Parâmetros: arquivo - estrutura Funcionario a ser salva
 * Retorno: void
 */
void salvarFuncionarioArquivo(Funcionario arquivo)
{
    FILE *arq = fopen(ARQ_FUNCIONARIOS, "ab");
    fwrite(&arquivo, sizeof(Funcionario), 1, arq);
    fclose(arq);
}

/*
 * Função: salvarQuartoArquivo
 * Objetivo: Salvar um quarto no arquivo binário
 * Parâmetros: q - estrutura Quarto a ser salva
 * Retorno: void
 */
void salvarQuartoArquivo(Quarto q)
{
    FILE *arq = fopen(ARQ_QUARTOS, "ab");
    fwrite(&q, sizeof(Quarto), 1, arq);
    fclose(arq);
}

/*
 * Função: salvarEstadiaArquivo
 * Objetivo: Salvar uma estadia no arquivo binário
 * Parâmetros: e - estrutura Estadia a ser salva
 * Retorno: void
 */
void salvarEstadiaArquivo(Estadia e)
{
    FILE *arq = fopen(ARQ_ESTADIAS, "ab");
    fwrite(&e, sizeof(Estadia), 1, arq);
    fclose(arq);
}

// ============================================================
// FUNÇÕES DE BUSCA EM ARQUIVOS
// ============================================================

/*
 * Função: buscarCliente
 * Objetivo: Buscar um cliente pelo código
 * Parâmetros: codigo - código do cliente a buscar
 *             resultado - ponteiro para armazenar cliente encontrado
 * Retorno: int - 1 se encontrou, 0 se não encontrou
 */
int buscarCliente(int codigo, Cliente *resultado)
{
    FILE *arquivo = fopen(ARQ_CLIENTES, "rb");
    if (!arquivo)
        return 0;  // Arquivo não existe

    Cliente c;
    // Percorre arquivo procurando cliente com código especificado
    while (fread(&c, sizeof(Cliente), 1, arquivo))
    {
        if (c.codigoCliente == codigo)
        {
            *resultado = c;  // Copia cliente encontrado
            fclose(arquivo);
            return 1;  // Sucesso
        }
    }
    fclose(arquivo);
    return 0;  // Cliente não encontrado
}

/*
 * Função: buscarQuarto
 * Objetivo: Buscar um quarto pelo número
 * Parâmetros: numero - número do quarto a buscar
 *             resultado - ponteiro para armazenar quarto encontrado
 * Retorno: int - 1 se encontrou, 0 se não encontrou
 */
int buscarQuarto(int numero, Quarto *resultado)
{
    FILE *arquivo = fopen(ARQ_QUARTOS, "rb");
    if (!arquivo)
        return 0;

    Quarto q;
    while (fread(&q, sizeof(Quarto), 1, arquivo))
    {
        if (q.numeroQuarto == numero)
        {
            *resultado = q;
            fclose(arquivo);
            return 1;
        }
    }
    fclose(arquivo);
    return 0;
}

// ============================================================
// FUNÇÕES PARA GERAR CÓDIGOS AUTOMÁTICOS
// ============================================================

/*
 * Função: gerarCodigoCliente
 * Objetivo: Gerar um código único para novo cliente
 *           Lê o arquivo para encontrar o maior código e incrementa
 * Parâmetros: -
 * Retorno: int - próximo código disponível
 */
int gerarCodigoCliente()
{
    FILE *arquivo = fopen(ARQ_CLIENTES, "rb");  // Abre para leitura binária
    if (!arquivo)  // Se arquivo não existe
        return 1;  // Primeiro código será 1

    Cliente c;
    int maior = 0;  // Armazena o maior código encontrado

    // Percorre todos os clientes no arquivo
    while (fread(&c, sizeof(Cliente), 1, arquivo))
    {
        if (c.codigoCliente > maior)
            maior = c.codigoCliente;  // Atualiza maior código
    }
    fclose(arquivo);  // Fecha o arquivo
    return maior + 1;  // Retorna próximo código
}

/*
 * Função: gerarCodigoFuncionario
 * Objetivo: Gerar código único para novo funcionário
 * Parâmetros: -
 * Retorno: int - próximo código disponível
 */
int gerarCodigoFuncionario()
{
    FILE *arquivo = fopen(ARQ_FUNCIONARIOS, "rb");
    if (!arquivo)
        return 1;  // Primeiro código

    Funcionario func;
    int maior = 0;

    while (fread(&func, sizeof(Funcionario), 1, arquivo))
    {
        if (func.codigoFuncionario > maior)
            maior = func.codigoFuncionario;
    }
    fclose(arquivo);
    return maior + 1;
}

/*
 * Função: gerarCodigoEstadia
 * Objetivo: Gerar código único para nova estadia
 * Parâmetros: -
 * Retorno: int - próximo código disponível
 */
int gerarCodigoEstadia()
{
    FILE *arquivo = fopen(ARQ_ESTADIAS, "rb");
    if (!arquivo)
        return 1;  // Primeiro código

    Estadia e;
    int maior = 0;

    while (fread(&e, sizeof(Estadia), 1, arquivo))
    {
        if (e.codigoEstadia > maior)
            maior = e.codigoEstadia;
    }
    fclose(arquivo);
    return maior + 1;
}

// ============================================================
// FUNÇÕES DE CADASTRO
// ============================================================

/*
 * Função: cadastrarCliente
 * Objetivo: Cadastrar um novo cliente no sistema
 *           Coleta informações via console e salva em arquivo
 * Parâmetros: Nenhum
 * Retorno: void
 */
void cadastrarCliente()
{
    Cliente c;
    c.codigoCliente = gerarCodigoCliente();  // Gera código automaticamente

    // Limpa buffer e coleta dados do cliente
    limparEntrada();
    printf("\nNome completo: ");
    fgets(c.nome, 50, stdin);
    c.nome[strcspn(c.nome, "\n")] = 0;  // Remove \n do final

    printf("Endereco: ");
    fgets(c.endereco, 50, stdin);
    c.endereco[strcspn(c.endereco, "\n")] = 0;

    printf("Telefone: ");
    fgets(c.telefone, 20, stdin);
    c.telefone[strcspn(c.telefone, "\n")] = 0;

    // Salva cliente no arquivo
    salvarClienteArquivo(c);
    printf("\nCliente cadastrado com sucesso! (Codigo %d)\n", c.codigoCliente);
    
    // Pausa antes de voltar ao menu
    printf("\nPressione ENTER para voltar ao menu...");
    limparEntrada();
    getchar();
}

/*
 * Função: cadastrarFuncionario
 * Objetivo: Cadastrar um novo funcionário no sistema
 *           Inclui validação de salário (não pode ser negativo)
 * Parâmetros: Nenhum
 * Retorno: void
 */
void cadastrarFuncionario()
{
    Funcionario arquivo;
    arquivo.codigoFuncionario = gerarCodigoFuncionario();

    // Coleta dados básicos
    limparEntrada();
    printf("\nNome completo: ");
    fgets(arquivo.nome, 50, stdin);
    arquivo.nome[strcspn(arquivo.nome, "\n")] = 0;

    printf("Telefone: ");
    fgets(arquivo.telefone, 20, stdin);
    arquivo.telefone[strcspn(arquivo.telefone, "\n")] = 0;

    printf("Cargo: ");
    fgets(arquivo.cargo, 30, stdin);
    arquivo.cargo[strcspn(arquivo.cargo, "\n")] = 0;

    // Validação de salário (deve ser positivo)
    do
    {
        printf("Salario: ");
        scanf("%lf", &arquivo.salario);
        if (arquivo.salario < 0)
            printf("Valor invalido! O salario nao pode ser negativo.\n");
    } while (arquivo.salario < 0);

    // Salva funcionário
    salvarFuncionarioArquivo(arquivo);
    printf("\nFuncionario cadastrado! (Codigo %d)\n", arquivo.codigoFuncionario);

    printf("\nPressione ENTER para voltar ao menu...");
    limparEntrada();
    getchar();
}

/*
 * Função: cadastrarQuarto
 * Objetivo: Cadastrar um novo quarto no hotel
 *           Valida número único, capacidade positiva e valor positivo
 * Parâmetros: Nenhum
 * Retorno: void
 */
void cadastrarQuarto()
{
    Quarto q;

    // Valida número do quarto (deve ser positivo)
    do
    {
        printf("\nNumero do quarto: ");
        scanf("%d", &q.numeroQuarto);
        if (q.numeroQuarto <= 0)
            printf("Numero invalido! O numero do quarto deve ser positivo.\n");
    } while (q.numeroQuarto <= 0);

    // Verifica se quarto com esse número já existe
    Quarto existente;
    if (buscarQuarto(q.numeroQuarto, &existente))
    {
        printf("Ja existe um quarto com esse numero.\n");
        return;
    }

    // Valida capacidade (deve ser maior que zero)
    do
    {
        printf("Capacidade: ");
        scanf("%d", &q.capacidade);
        if (q.capacidade <= 0)
            printf("Capacidade invalida! Deve ser maior que zero.\n");
    } while (q.capacidade <= 0);

    // Valida valor da diária (deve ser positivo)
    do
    {
        printf("Valor da diaria: ");
        scanf("%lf", &q.valorDiaria);
        if (q.valorDiaria <= 0)
            printf("Valor invalido! A diaria deve ser positiva.\n");
    } while (q.valorDiaria <= 0);

    q.estaOcupado = 0;  // Novo quarto começa como livre

    // Salva quarto
    salvarQuartoArquivo(q);
    printf("\nQuarto cadastrado com sucesso!\n");

    printf("\nPressione ENTER para voltar ao menu...");
    limparEntrada();
    getchar();
}







// ============================================================
// FUNÇÃO PRINCIPAL - MENU DO SISTEMA
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
            cadastrarCliente();
            break;
        case 2:
            cadastrarFuncionario();
            break;
        case 3:
            cadastrarQuarto();
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