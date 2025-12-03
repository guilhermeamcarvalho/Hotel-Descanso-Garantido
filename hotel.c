/*
 * Sistema de Gerenciamento Hoteleiro - Hotel Descanso Garantido
 * Desenvolvido por: Guilherme, Rafael e Samuel
 *
 * Descrição: Sistema completo para gerenciamento de um hotel,
 * incluindo cadastro de clientes, funcionários, quartos,
 * gerenciamento de estadias e sistema de fidelidade.
 */

#include <stdio.h>  // (printf, scanf, etc.)
#include <stdlib.h> // (FILE, malloc, etc.)
#include <string.h> // Para funções de manipulação de strings

// ============================================================
// DEFINIÇÃO DE STRUCTS (ESTRUTURAS DE DADOS)
// ============================================================

/*
 * Estrutura para armazenar datas
 * Armazena dia, mês e ano para uso em todo o sistema
 */
typedef struct
{
    int dia; // Dia do mês (1-31)
    int mes; // Mês do ano (1-12)
    int ano; // Ano completo (ex: 2024)
} Data;

/*
 * Estrutura para armazenar informações dos clientes
 * Contém todos os dados necessários para identificar e contatar clientes
 */
typedef struct
{
    int codigoCliente; // Código único do cliente (gerado automaticamente)
    char nome[50];     // Nome completo do cliente (máximo 49 caracteres + \0)
    char endereco[50]; // Endereço do cliente
    char telefone[20]; // Telefone para contato
} Cliente;

/*
 * Estrutura para armazenar informações dos funcionários
 * Registra dados dos colaboradores do hotel
 */
typedef struct
{
    int codigoFuncionario; // Código único do funcionário
    char nome[50];         // Nome completo do funcionário
    char telefone[20];     // Telefone para contato
    char cargo[30];        // Cargo/função no hotel
    double salario;        // Salário mensal
} Funcionario;

/*
 * Estrutura para armazenar informações dos quartos
 * Define as características e status de cada quarto
 */
typedef struct
{
    int numeroQuarto;   // Número do quarto (deve ser único)
    int capacidade;     // Número máximo de hóspedes
    double valorDiaria; // Preço por diária
    int estaOcupado;    // Status: 0 = livre, 1 = ocupado
} Quarto;

/*
 * Estrutura para armazenar informações das estadias
 * Registra todas as informações de uma hospedagem
 */
typedef struct
{
    int codigoEstadia;     // Código único da estadia
    int codigoCliente;     // Código do cliente hospedado
    int numeroQuarto;      // Número do quarto ocupado
    Data dataEntrada;      // Data de check-in
    Data dataSaida;        // Data de check-out
    int quantidadeDiarias; // Número total de diárias
    int estadiaAtiva;      // Status: 1 = ativa, 0 = finalizada
} Estadia;

// ============================================================
// DEFINIÇÃO DOS NOMES DOS ARQUIVOS DE DADOS
// ============================================================

// Nomes dos arquivos binários onde os dados serão persistidos
const char *ARQ_CLIENTES = "clientes.bin";         // Arquivo de clientes
const char *ARQ_FUNCIONARIOS = "funcionarios.bin"; // Arquivo de funcionários
const char *ARQ_QUARTOS = "quartos.bin";           // Arquivo de quartos
const char *ARQ_ESTADIAS = "estadias.bin";         // Arquivo de estadias

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
    int c; // Variável para armazenar cada caractere
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
    int bissexto = 0; // Flag para indicar se é ano bissexto

    // Verifica se é ano bissexto (divisível por 4)
    if (ano % 4 == 0)
    {
        // Exceção: se for divisível por 100, só é bissexto se for divisível por 400
        if (ano % 100 != 0 || ano % 400 == 0)
            bissexto = 1; // É bissexto
    }

    // Retorna quantidade de dias conforme o ano
    if (bissexto == 1)
        return 29; // Fevereiro em ano bissexto
    else
        return 28; // Fevereiro em ano não bissexto
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
    case 2: // Fevereiro - depende do ano
        return diasFevereiro(ano);
    case 4:        // Abril
    case 6:        // Junho
    case 9:        // Setembro
    case 11:       // Novembro
        return 30; // Meses com 30 dias
    default:       // Todos os outros meses
        return 31; // Meses com 31 dias
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
int validarData(Data d)
{
    // Validar ano (consideramos de 1900 a 2100 como válido)
    if (d.ano < 1900 || d.ano > 2100)
    {
        return 0; // Ano fora do intervalo permitido
    }

    // Validar mês (deve estar entre 1 e 12)
    if (d.mes < 1 || d.mes > 12)
    {
        return 0; // Mês inválido
    }

    // Validar dia baseado no mês e ano
    int diasNoMesValido = diasNoMes(d.mes, d.ano); // Obtém dias do mês
    if (d.dia < 1 || d.dia > diasNoMesValido)
    {
        return 0; // Dia inválido para o mês/ano
    }

    return 1; // Data válida
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
    FILE *arq = fopen(ARQ_CLIENTES, "ab"); // Abre para append binário
    fwrite(&c, sizeof(Cliente), 1, arq);   // Escreve estrutura no arquivo
    fclose(arq);                           // Fecha arquivo
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
        return 0; // Arquivo não existe

    Cliente c;
    // Percorre arquivo procurando cliente com código especificado
    while (fread(&c, sizeof(Cliente), 1, arquivo))
    {
        if (c.codigoCliente == codigo)
        {
            *resultado = c; // Copia cliente encontrado
            fclose(arquivo);
            return 1; // Sucesso
        }
    }
    fclose(arquivo);
    return 0; // Cliente não encontrado
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

// FUNÇÃO PARA MARCAR QUARTO COMO OCUPADO OU LIVRE

// ============================================================

/*

* Função: quartoOcupado

* Objetivo: Alterar status de ocupação de um quarto

* Parâmetros: numero - número do quarto

*             ocupado - novo status (1 = ocupado, 0 = livre)

* Retorno: void

*

* Nota: Cria arquivo temporário para atualização, depois substitui o original

*/

void quartoOcupado(int numero, int ocupado)

{

    // Abre arquivo original para leitura e temporário para escrita

    FILE \*in = fopen(ARQ\_QUARTOS, "rb");

    FILE \*out = fopen("temp.dat", "wb");

    Quarto q;

    // Lê todos os quartos do arquivo original

    while (fread(\& q, sizeof(Quarto), 1, in))

    {

        // Se é o quarto procurado, atualiza status

        if (q.numeroQuarto == numero)

            q.estaOcupado = ocupado;

        // Escreve no arquivo temporário (atualizado ou não)

        fwrite(\& q, sizeof(Quarto), 1, out);
    }

    // Fecha arquivos

    fclose(in);

    fclose(out);

    // Substitui arquivo original pelo temporário

    remove(ARQ\_QUARTOS);

    rename("temp.dat", ARQ\_QUARTOS);
}
// ============================================================
// FUNÇÕES PARA PESQUISA DE CLIENTES
// ============================================================

/*
 * Função: pesquisarCliente
 * Objetivo: Pesquisar cliente por código ou nome
 *           Permite busca exata por código ou parcial por nome
 * Parâmetros: -
 * Retorno: void
 */
void pesquisarCliente()
{
    int opcao;  // Opção de busca (1=código, 2=nome)
    printf("\n=== PESQUISAR CLIENTE ===\n");
    printf("1 - Pesquisar por codigo\n");
    printf("2 - Pesquisar por nome\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparEntrada();  // Limpa buffer após scanf
    
    // Abre arquivo de clientes
    FILE *arquivo = fopen(ARQ_CLIENTES, "rb");
    if (!arquivo)
    {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }
    
    Cliente c;
    int encontrou = 0;  // Flag para indicar se encontrou resultados
    
    // Busca por código
    if (opcao == 1)
    {
        int codigo;
        printf("Digite o codigo do cliente: ");
        scanf("%d", &codigo);
        limparEntrada();
        
        // Percorre arquivo procurando cliente com código especificado
        while (fread(&c, sizeof(Cliente), 1, arquivo))
        {
            if (c.codigoCliente == codigo)
            {
                printf("\n=== CLIENTE ENCONTRADO ===\n");
                printf("Codigo: %d\n", c.codigoCliente);
                printf("Nome: %s\n", c.nome);
                printf("Endereco: %s\n", c.endereco);
                printf("Telefone: %s\n", c.telefone);
                encontrou = 1;
                break;  // Encontrou, pode parar busca
            }
        }
    }
    // Busca por nome (parcial)
    else if (opcao == 2)
    {
        char nomeBusca[50];
        printf("Digite o nome (ou parte do nome) do cliente: ");
        fgets(nomeBusca, 50, stdin);
        nomeBusca[strcspn(nomeBusca, "\n")] = 0;  // Remove \n do final
        
        printf("\n=== RESULTADOS DA PESQUISA ===\n");
        // Percorre todos os clientes
        while (fread(&c, sizeof(Cliente), 1, arquivo))
        {
            // Verifica se o nome buscado está contido no nome do cliente
            // strstr retorna ponteiro se encontrar substring, NULL se não
            if (strstr(c.nome, nomeBusca) != NULL)
            {
                printf("\nCodigo: %d\n", c.codigoCliente);
                printf("Nome: %s\n", c.nome);
                printf("Endereco: %s\n", c.endereco);
                printf("Telefone: %s\n", c.telefone);
                printf("-------------------\n");
                encontrou = 1;
            }
        }
    }
    else
    {
        printf("Opcao invalida!\n");
        fclose(arquivo);
        return;
    }
    
    fclose(arquivo);
    
    // Mensagem se não encontrou - cliente
    if (!encontrou)
        printf("Cliente nao encontrado.\n");
    
    // Pausa para usuário ver resultados
    printf("\nPressione ENTER para voltar ao menu...");
    getchar();
}

/*
 * Função: pesquisarFuncionario
 * Objetivo: Pesquisar funcionário por código ou nome
 *           Funcionamento similar a pesquisarCliente
 * Parâmetros: -
 * Retorno: void
 */
void pesquisarFuncionario()
{
    int opcao;
    printf("\n=== PESQUISAR FUNCIONARIO ===\n");
    printf("1 - Pesquisar por codigo\n");
    printf("2 - Pesquisar por nome\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparEntrada();
    
    FILE *arquivo = fopen(ARQ_FUNCIONARIOS, "rb");
    if (!arquivo)
    {
        printf("Nenhum funcionario cadastrado.\n");
        return;
    }
    
    Funcionario func;
    int encontrou = 0;
    
    if (opcao == 1)
    {
        int codigo;
        printf("Digite o codigo do funcionario: ");
        scanf("%d", &codigo);
        limparEntrada();
        
        while (fread(&func, sizeof(Funcionario), 1, arquivo))
        {
            if (func.codigoFuncionario == codigo)
            {
                printf("\n=== FUNCIONARIO ENCONTRADO ===\n");
                printf("Codigo: %d\n", func.codigoFuncionario);
                printf("Nome: %s\n", func.nome);
                printf("Telefone: %s\n", func.telefone);
                printf("Cargo: %s\n", func.cargo);
                printf("Salario: R$ %.2f\n", func.salario);
                encontrou = 1;
                break;
            }
        }
    }
    else if (opcao == 2)
    {
        char nomeBusca[50];
        printf("Digite o nome (ou parte do nome) do funcionario: ");
        fgets(nomeBusca, 50, stdin);
        nomeBusca[strcspn(nomeBusca, "\n")] = 0;
        
        printf("\n=== RESULTADOS DA PESQUISA ===\n");
        while (fread(&func, sizeof(Funcionario), 1, arquivo))
        {
            if (strstr(func.nome, nomeBusca) != NULL)
            {
                printf("\nCodigo: %d\n", func.codigoFuncionario);
                printf("Nome: %s\n", func.nome);
                printf("Telefone: %s\n", func.telefone);
                printf("Cargo: %s\n", func.cargo);
                printf("Salario: R$ %.2f\n", func.salario);
                printf("-------------------\n");
                encontrou = 1;
            }
        }
    }
    else
    {
        printf("Opcao invalida!\n");
        fclose(arquivo);
        return;
    }
    
    fclose(arquivo);
    
    if (!encontrou)
        printf("Funcionario nao encontrado.\n");
    
    printf("\nPressione ENTER para voltar ao menu...");
    getchar();
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
    FILE *arquivo = fopen(ARQ_CLIENTES, "rb"); // Abre para leitura binária
    if (!arquivo)                              // Se arquivo não existe
        return 1;                              // Primeiro código será 1

    Cliente c;
    int maior = 0; // Armazena o maior código encontrado

    // Percorre todos os clientes no arquivo
    while (fread(&c, sizeof(Cliente), 1, arquivo))
    {
        if (c.codigoCliente > maior)
            maior = c.codigoCliente; // Atualiza maior código
    }
    fclose(arquivo);  // Fecha o arquivo
    return maior + 1; // Retorna próximo código
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
        return 1; // Primeiro código

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
        return 1; // Primeiro código

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
    c.codigoCliente = gerarCodigoCliente(); // Gera código automaticamente

    // Limpa buffer e coleta dados do cliente
    limparEntrada();
    printf("\nNome completo: ");
    fgets(c.nome, 50, stdin);
    c.nome[strcspn(c.nome, "\n")] = 0; // Remove \n do final

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

    q.estaOcupado = 0; // Novo quarto começa como livre

    // Salva quarto
    salvarQuartoArquivo(q);
    printf("\nQuarto cadastrado com sucesso!\n");

    printf("\nPressione ENTER para voltar ao menu...");
    limparEntrada();
    getchar();
}
// ============================================================
// FUNÇÃO PARA REGISTRAR NOVA ESTADIA
// ============================================================

/*
 * Função: novaEstadia
 * Objetivo: Registrar uma nova estadia/hospedagem
 *           Processo completo: valida cliente, mostra quartos disponíveis,
 *           valida datas, calcula diárias e atualiza status
 * Parâmetros: Nenhum
 * Retorno: void
 */
void novaEstadia()
{
    Estadia e;
    e.codigoEstadia = gerarCodigoEstadia(); // Gera código automático

    // Valida código do cliente
    do
    {
        printf("Codigo do cliente: ");
        scanf("%d", &e.codigoCliente);
        if (e.codigoCliente <= 0)
            printf("Codigo invalido! Deve ser positivo.\n");
    } while (e.codigoCliente <= 0);

    // Verifica se cliente existe
    Cliente ctmp;
    if (!buscarCliente(e.codigoCliente, &ctmp))
    {
        printf("Cliente nao encontrado!\n");
        return;
    }

    // Valida quantidade de hóspedes
    int hospedes;
    do
    {
        printf("Quantidade de hospedes: ");
        scanf("%d", &hospedes);
        if (hospedes <= 0)
            printf("Quantidade invalida! Deve ser maior que zero.\n");
    } while (hospedes <= 0);

    // Mostra quartos disponíveis compatíveis com a capacidade
    FILE *arquivo = fopen(ARQ_QUARTOS, "rb");
    Quarto q;
    int encontrou = 0;

    printf("\nQuartos disponiveis para %d hospede(s):\n", hospedes);

    while (fread(&q, sizeof(Quarto), 1, arquivo))
    {
        if (!q.estaOcupado && q.capacidade >= hospedes)
        {
            printf("Quarto %d | Capacidade %d | Diaria: R$ %.2f\n",
                   q.numeroQuarto, q.capacidade, q.valorDiaria);
            encontrou = 1;
        }
    }
    fclose(arquivo);

    if (!encontrou)
    {
        printf("\nNenhum quarto disponivel com essa capacidade.\n");
        return;
    }

    // Solicita número do quarto desejado
    do
    {
        printf("\nDigite o numero do quarto desejado: ");
        scanf("%d", &e.numeroQuarto);
        if (e.numeroQuarto <= 0)
            printf("Numero invalido! Deve ser positivo.\n");
    } while (e.numeroQuarto <= 0);

    // Valida escolha do quarto
    Quarto escolhido;
    if (!buscarQuarto(e.numeroQuarto, &escolhido) ||
        escolhido.estaOcupado ||
        escolhido.capacidade < hospedes)
    {
        printf("Quarto invalido ou indisponivel!\n");
        return;
    }

    // Lê data de entrada
    if (!lerData("Data de entrada", &e.dataEntrada))
    {
        return;
    }

    // Lê data de saída
    if (!lerData("Data de saida", &e.dataSaida))
    {
        return;
    }

    // Calcula quantidade de diárias
    e.quantidadeDiarias = diasEntreDatas(e.dataEntrada, e.dataSaida);
    if (e.quantidadeDiarias <= 0)
    {
        printf("Datas invalidas! A data de saida deve ser posterior a data de entrada.\n");
        return;
    }

    e.estadiaAtiva = 1;

    // Salva estadia e marca quarto como ocupado
    salvarEstadiaArquivo(e);
    quartoOcupado(e.numeroQuarto, 1);

    // Resumo
    printf("\nEstadia registrada com sucesso!\n");
    printf("Codigo da estadia: %d\n", e.codigoEstadia);
    printf("Total de diarias: %d\n", e.quantidadeDiarias);

    double valorTotal = e.quantidadeDiarias * escolhido.valorDiaria;
    printf("Valor total estimado: R$ %.2f\n", valorTotal);

    printf("\nPressione ENTER para voltar ao menu...");
    limparEntrada();
    getchar();
}

// ============================================================
// FUNÇÃO PARA ENCERRAR ESTADIA
// ============================================================

/*
 * Função: encerrarEstadia
 * Objetivo: Encerrar uma estadia ativa
 *           Calcula valor total, marca estadia como finalizada
 *           e libera o quarto para nova ocupação
 * Parâmetros: Nenhum
 * Retorno: void
 */
void encerrarEstadia()
{
    int codigo;

    // Valida código da estadia
    do
    {
        printf("Codigo da estadia: ");
        scanf("%d", &codigo);
        if (codigo <= 0)
            printf("Codigo invalido! Deve ser positivo.\n");
    } while (codigo <= 0);

    // Abre arquivos
    FILE *in = fopen(ARQ_ESTADIAS, "rb");
    FILE *out = fopen("temp.dat", "wb");

    Estadia e;
    int achou = 0;

    // Processa todas as estadias
    while (fread(&e, sizeof(Estadia), 1, in))
    {
        if (e.codigoEstadia == codigo && e.estadiaAtiva)
        {
            // Obtém informações do quarto
            Quarto q;
            buscarQuarto(e.numeroQuarto, &q);

            double total = e.quantidadeDiarias * q.valorDiaria;

            printf("\nValor total da estadia: R$ %.2f\n", total);

            e.estadiaAtiva = 0;               // Finaliza estadia
            quartoOcupado(e.numeroQuarto, 0); // Libera o quarto
            achou = 1;
        }

        fwrite(&e, sizeof(Estadia), 1, out); // Grava no arquivo temporário
    }

    // Fecha e substitui arquivo original
    fclose(in);
    fclose(out);
    remove(ARQ_ESTADIAS);
    rename("temp.dat", ARQ_ESTADIAS);

    // Feedback
    if (!achou)
        printf("Estadia nao encontrada.\n");
    else
        printf("Estadia encerrada!\n");

    printf("\nPressione ENTER para voltar ao menu...");
    limparEntrada();
    getchar();
}

// ============================================================
// FUNÇÕES PARA MOSTRAR DADOS (LISTAGENS)
// ============================================================

/*
 * Função: mostrarClientes
 * Objetivo: Listar todos os clientes cadastrados no sistema
 *           Inclui contador total
 * Parâmetros: Nenhum
 * Retorno: void
 */
void mostrarClientes()
{
    FILE *arquivo = fopen(ARQ_CLIENTES, "rb");
    if (!arquivo)
    {
        printf("Nenhum cliente cadastrado.\n");
        printf("\nPressione ENTER para voltar ao menu...");
        getchar();
        return;
    }

    Cliente c;
    int count = 0; // Contador de clientes

    printf("\n=== LISTA DE CLIENTES ===\n");
    printf("========================================\n");

    // Lê e exibe cada cliente
    while (fread(&c, sizeof(Cliente), 1, arquivo))
    {
        printf("\nCodigo: %d\n", c.codigoCliente);
        printf("Nome: %s\n", c.nome);
        printf("Endereco: %s\n", c.endereco);
        printf("Telefone: %s\n", c.telefone);
        printf("-------------------\n");
        count++;
    }
    fclose(arquivo);

    printf("Total de clientes: %d\n", count);
    printf("\nPressione ENTER para voltar ao menu...");
    limparEntrada();
    getchar();
}

// ============================================================
// FUNÇÃO AUXILIAR PARA LER DATA COM VALIDAÇÃO
// ============================================================

/*
 * Função: lerData
 * Objetivo: Ler uma data do usuário com validação e mensagens de erro
 * Parâmetros: mensagem - texto a ser exibido para o usuário
 *             data - ponteiro para estrutura Data onde armazenar o resultado
 * Retorno: int - 1 se leitura bem sucedida, 0 se falhou após 3 tentativas
 */
int lerData(const char *mensagem, Data *data)
{
    int tentativas = 0; // Contador de tentativas falhas

    // Permite até 3 tentativas
    while (tentativas < 3)
    {
        printf("%s (DD MM AAAA): ", mensagem);

        // Ler os três valores (dia, mês, ano)
        if (scanf("%d %d %d", &data->dia, &data->mes, &data->ano) != 3)
        {
            printf("Formato invalido! Digite dia, mes e ano separados por espacos.\n");
            limparEntrada(); // Limpa buffer para próxima tentativa
            tentativas++;
            continue;
        }

        // Validar a data usando a função validarData
        if (!validarData(*data))
        {
            printf("Data invalida! Verifique se o dia, mes e ano estao corretos.\n");
            printf("Exemplo: 25 12 2024 para 25 de dezembro de 2024.\n");
            tentativas++;
            continue;
        }

        return 1; // Data válida lida com sucesso
    }

    // Se chegou aqui, falhou após 3 tentativas
    printf("Muitas tentativas invalidas. Voltando ao menu.\n");
    return 0; // Falha na leitura
}

// ============================================================
// FUNÇÃO PARA CÁLCULO DE DIAS ENTRE DATAS
// ============================================================

/*
 * Função: diasEntreDatas
 * Objetivo: Calcular o número de dias entre duas datas
 *           Usado para calcular diárias de estadia
 * Parâmetros: e - data de entrada
 *             s - data de saída
 * Retorno: int - número de dias, ou -1 se datas inválidas
 */
int diasEntreDatas(Data e, Data s)
{
    // Primeiro validar ambas as datas
    if (!validarData(e) || !validarData(s))
    {
        return -1; // Datas inválidas
    }

    // Verificar se a data de saída é posterior à data de entrada
    if (s.ano < e.ano)
        return -1; // Saída em ano anterior à entrada
    if (s.ano == e.ano && s.mes < e.mes)
        return -1; // Mesmo ano, mas mês de saída anterior
    if (s.ano == e.ano && s.mes == e.mes && s.dia <= e.dia)
        return -1; // Mesmo mês/ano, mas dia de saída não posterior

    int dias = 0; // Acumulador de dias

    // Caso simples: mesmo mês e ano
    if (e.ano == s.ano && e.mes == s.mes)
        return s.dia - e.dia; // Apenas subtrai os dias

    // Dias restantes no mês de entrada
    dias += diasNoMes(e.mes, e.ano) - e.dia;

    // Meses intermediários entre entrada e saída
    int mesAtual = e.mes + 1;
    int anoAtual = e.ano;

    // Percorre meses entre entrada e saída (excluindo mês de entrada e saída)
    while (anoAtual < s.ano || (anoAtual == s.ano && mesAtual < s.mes))
    {
        dias += diasNoMes(mesAtual, anoAtual); // Adiciona dias do mês
        mesAtual++;

        // Ajusta ano se passar de dezembro
        if (mesAtual > 12)
        {
            mesAtual = 1;
            anoAtual++;
        }
    }

    // Dias no mês de saída
    dias += s.dia;

    return dias; // Retorna total de dias
}

// ============================================================
// FUNÇÕES PARA MOSTRAR DADOS (CONTINUAÇÃO)
// ============================================================

/*
 * Função: mostrarFuncionarios
 * Objetivo: Listar todos os funcionários cadastrados
 * Parâmetros: Nenhum
 * Retorno: void
 */
void mostrarFuncionarios()
{
    FILE *arquivo = fopen(ARQ_FUNCIONARIOS, "rb");
    if (!arquivo)
    {
        printf("Nenhum funcionario cadastrado.\n");
        printf("\nPressione ENTER para voltar ao menu...");
        getchar();
        return;
    }

    Funcionario func;
    int count = 0;

    printf("\n=== LISTA DE FUNCIONaRIOS ===\n");
    printf("========================================\n");
    while (fread(&func, sizeof(Funcionario), 1, arquivo))
    {
        printf("\nCodigo: %d\n", func.codigoFuncionario);
        printf("Nome: %s\n", func.nome);
        printf("Telefone: %s\n", func.telefone);
        printf("Cargo: %s\n", func.cargo);
        printf("Salario: R$ %.2f\n", func.salario);
        printf("-------------------\n");
        count++;
    }
    fclose(arquivo);

    printf("Total de funcionarios: %d\n", count);
    printf("\nPressione ENTER para voltar ao menu...");
    limparEntrada();
    getchar();
}

/*
 * Função: mostrarQuartos
 * Objetivo: Listar todos os quartos com status de ocupação
 *           Inclui estatísticas: total, ocupados e livres
 * Parâmetros: Nenhum
 * Retorno: void
 */
void mostrarQuartos()
{
    FILE *arquivo = fopen(ARQ_QUARTOS, "rb");
    if (!arquivo)
    {
        printf("Nenhum quarto cadastrado.\n");
        printf("\nPressione ENTER para voltar ao menu...");
        getchar();
        return;
    }

    Quarto q;
    int count = 0;    // Total de quartos
    int ocupados = 0; // Quartos ocupados
    int livres = 0;   // Quartos livres

    printf("\n=== LISTA DE QUARTOS ===\n");
    printf("========================================\n");
    while (fread(&q, sizeof(Quarto), 1, arquivo))
    {
        printf("\nNumero: %d\n", q.numeroQuarto);
        printf("Capacidade: %d hospedes\n", q.capacidade);
        printf("Valor da diaria: R$ %.2f\n", q.valorDiaria);
        printf("Status: %s\n", q.estaOcupado ? "OCUPADO" : "LIVRE");
        printf("-------------------\n");

        count++;
        if (q.estaOcupado)
            ocupados++;
        else
            livres++;
    }
    fclose(arquivo);

    // Mostra estatísticas
    printf("RESUMO:\n");
    printf("Total de quartos: %d\n", count);
    printf("Quartos ocupados: %d\n", ocupados);
    printf("Quartos livres: %d\n", livres);
    printf("\nPressione ENTER para voltar ao menu...");
    limparEntrada();
    getchar();
}

/*
 * Função: mostrarEstadias
 * Objetivo: Listar todas as estadias registradas
 *           Inclui status (ativa/finalizada) e estatísticas
 * Parâmetros: Nenhum
 * Retorno: void
 */
void mostrarEstadias()
{
    FILE *arquivo = fopen(ARQ_ESTADIAS, "rb");
    if (!arquivo)
    {
        printf("Nenhuma estadia registrada.\n");
        printf("\nPressione ENTER para voltar ao menu...");
        getchar();
        return;
    }

    Estadia e;
    int count = 0;       // Total de estadias
    int ativas = 0;      // Estadias ativas
    int finalizadas = 0; // Estadias finalizadas

    printf("\n=== LISTA DE ESTADIAS ===\n");
    printf("========================================\n");
    while (fread(&e, sizeof(Estadia), 1, arquivo))
    {
        printf("\nCodigo da estadia: %d\n", e.codigoEstadia);
        printf("Codigo do cliente: %d\n", e.codigoCliente);
        printf("Numero do quarto: %d\n", e.numeroQuarto);
        printf("Data de entrada: %02d/%02d/%04d\n", e.dataEntrada.dia, e.dataEntrada.mes, e.dataEntrada.ano);
        printf("Data de saida: %02d/%02d/%04d\n", e.dataSaida.dia, e.dataSaida.mes, e.dataSaida.ano);
        printf("Quantidade de diarias: %d\n", e.quantidadeDiarias);
        printf("Status: %s\n", e.estadiaAtiva ? "ATIVA" : "FINALIZADA");
        printf("-------------------\n");

        count++;
        if (e.estadiaAtiva)
            ativas++;
        else
            finalizadas++;
    }
    fclose(arquivo);

    // Mostra estatísticas
    printf("RESUMO:\n");
    printf("Total de estadias: %d\n", count);
    printf("Estadias ativas: %d\n", ativas);
    printf("Estadias finalizadas: %d\n", finalizadas);
    printf("\nPressione ENTER para voltar ao menu...");
    limparEntrada();
    getchar();
}

// ============================================================
// FUNÇÃO PARA MOSTRAR ESTADIAS DE UM CLIENTE
// ============================================================

/*
 * Função: mostrarEstadiasCliente
 * Objetivo: Mostrar todas as estadias de um cliente específico
 *           Permite busca por código ou nome do cliente
 * Parâmetros: -
 * Retorno: void
 */
void mostrarEstadiasCliente()
{
    int opcao;
    printf("\n=== PESQUISAR ESTADIAS POR CLIENTE ===\n");
    printf("1 - Pesquisar por codigo do cliente\n");
    printf("2 - Pesquisar por nome do cliente\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparEntrada();
    
    int codigoCliente = -1;      // Código do cliente encontrado
    char nomeCliente[50] = "";   // Nome do cliente
    
    // Busca por código
    if (opcao == 1)
    {
        printf("Digite o codigo do cliente: ");
        scanf("%d", &codigoCliente);
        limparEntrada();
        
        // Verificar se o cliente existe
        Cliente c;
        if (!buscarCliente(codigoCliente, &c))
        {
            printf("Cliente nao encontrado.\n");
            printf("\nPressione ENTER para voltar ao menu...");
            getchar();
            return;
        }
        strcpy(nomeCliente, c.nome);  // Armazena nome do cliente
    }
    // Busca por nome
    else if (opcao == 2)
    {
        char nomeBusca[50];
        printf("Digite o nome (ou parte do nome) do cliente: ");
        fgets(nomeBusca, 50, stdin);
        nomeBusca[strcspn(nomeBusca, "\n")] = 0;
        
        // Buscar cliente pelo nome no arquivo
        FILE *arquivoClientes = fopen(ARQ_CLIENTES, "rb");
        if (!arquivoClientes)
        {
            printf("Nenhum cliente cadastrado.\n");
            printf("\nPressione ENTER para voltar ao menu...");
            getchar();
            return;
        }
        
        Cliente c;
        int encontrados = 0;
        printf("\n=== CLIENTES ENCONTRADOS ===\n");
        
        // Lista todos os clientes que correspondem à busca
        while (fread(&c, sizeof(Cliente), 1, arquivoClientes))
        {
            if (strstr(c.nome, nomeBusca) != NULL)
            {
                printf("%d - %s\n", c.codigoCliente, c.nome);
                encontrados++;
            }
        }
        fclose(arquivoClientes);
        
        // Se não encontrou nenhum cliente
        if (encontrados == 0)
        {
            printf("Nenhum cliente encontrado com esse nome.\n");
            printf("\nPressione ENTER para voltar ao menu...");
            getchar();
            return;
        }
        
        // Se encontrou mais de um cliente, pede para escolher
        if (encontrados > 1)
        {
            printf("\nDigite o codigo do cliente desejado: ");
            scanf("%d", &codigoCliente);
            limparEntrada();
            
            // Obter nome do cliente selecionado
            Cliente clienteSelecionado;
            if (buscarCliente(codigoCliente, &clienteSelecionado))
            {
                strcpy(nomeCliente, clienteSelecionado.nome);
            }
        }
        else  // Se encontrou apenas um cliente
        {
            // Reabre arquivo para pegar dados do único cliente encontrado
            arquivoClientes = fopen(ARQ_CLIENTES, "rb");
            while (fread(&c, sizeof(Cliente), 1, arquivoClientes))
            {
                if (strstr(c.nome, nomeBusca) != NULL)
                {
                    codigoCliente = c.codigoCliente;
                    strcpy(nomeCliente, c.nome);
                    break;
                }
            }
            fclose(arquivoClientes);
        }
    }
    else
    {
        printf("Opcao invalida!\n");
        printf("\nPressione ENTER para voltar ao menu...");
        getchar();
        return;
    }
    
    // Agora mostrar as estadias deste cliente
    FILE *arquivoEstadias = fopen(ARQ_ESTADIAS, "rb");
    if (!arquivoEstadias)
    {
        printf("Nenhuma estadia registrada.\n");
        printf("\nPressione ENTER para voltar ao menu...");
        getchar();
        return;
    }
    
    Estadia e;
    int totalEstadias = 0;   // Contador de estadias
    int totalDiarias = 0;    // Acumulador de diárias
    
    printf("\n=== ESTADIAS DO CLIENTE: %s (Codigo: %d) ===\n", nomeCliente, codigoCliente);
    printf("==============================================\n");
    
    // Percorre todas as estadias procurando as do cliente
    while (fread(&e, sizeof(Estadia), 1, arquivoEstadias))
    {
        if (e.codigoCliente == codigoCliente)
        {
            printf("\nEstadia: %d\n", e.codigoEstadia);
            printf("Quarto: %d\n", e.numeroQuarto);
            printf("Entrada: %02d/%02d/%04d\n", e.dataEntrada.dia, e.dataEntrada.mes, e.dataEntrada.ano);
            printf("Saida: %02d/%02d/%04d\n", e.dataSaida.dia, e.dataSaida.mes, e.dataSaida.ano);
            printf("Diarias: %d\n", e.quantidadeDiarias);
            printf("Status: %s\n", e.estadiaAtiva ? "ATIVA" : "FINALIZADA");
            printf("-------------------\n");
            
            totalEstadias++;
            totalDiarias += e.quantidadeDiarias;
        }
    }
    
    fclose(arquivoEstadias);
    
    // Mostra resumo
    if (totalEstadias == 0)
    {
        printf("Nenhuma estadia encontrada para este cliente.\n");
    }
    else
    {
        printf("\nRESUMO:\n");
        printf("Total de estadias: %d\n", totalEstadias);
        printf("Total de diarias: %d\n", totalDiarias);
        
        // Calcular pontos de fidelidade (10 pontos por diária)
        int pontos = totalDiarias * 10;
        printf("Pontos de fidelidade acumulados: %d pontos\n", pontos);
    }
    
    printf("\nPressione ENTER para voltar ao menu...");
    getchar();
}

// ============================================================
// FUNÇÃO PARA CALCULAR PONTOS DE FIDELIDADE
// ============================================================

/*
 * Função: calcularPontosFidelidade
 * Objetivo: Calcular pontos de fidelidade de um cliente
 *           Sistema: 10 pontos por diária hospedada
 * Parâmetros: Nenhum
 * Retorno: void
 */
void calcularPontosFidelidade()
{
    int opcao;
    printf("\n=== CALCULAR PONTOS DE FIDELIDADE ===\n");
    printf("1 - Calcular por codigo do cliente\n");
    printf("2 - Calcular por nome do cliente\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparEntrada();
    
    int codigoCliente = -1;
    char nomeCliente[50] = "";
    
    // Processamento similar à função mostrarEstadiasCliente
    if (opcao == 1)
    {
        printf("Digite o codigo do cliente: ");
        scanf("%d", &codigoCliente);
        limparEntrada();
        
        Cliente c;
        if (!buscarCliente(codigoCliente, &c))
        {
            printf("Cliente nao encontrado.\n");
            printf("\nPressione ENTER para voltar ao menu...");
            getchar();
            return;
        }
        strcpy(nomeCliente, c.nome);
    }
    else if (opcao == 2)
    {
        char nomeBusca[50];
        printf("Digite o nome (ou parte do nome) do cliente: ");
        fgets(nomeBusca, 50, stdin);
        nomeBusca[strcspn(nomeBusca, "\n")] = 0;
        
        FILE *arquivoClientes = fopen(ARQ_CLIENTES, "rb");
        if (!arquivoClientes)
        {
            printf("Nenhum cliente cadastrado.\n");
            printf("\nPressione ENTER para voltar ao menu...");
            getchar();
            return;
        }
        
        Cliente c;
        int encontrados = 0;
        printf("\n=== CLIENTES ENCONTRADOS ===\n");
        
        while (fread(&c, sizeof(Cliente), 1, arquivoClientes))
        {
            if (strstr(c.nome, nomeBusca) != NULL)
            {
                printf("%d - %s\n", c.codigoCliente, c.nome);
                encontrados++;
            }
        }
        fclose(arquivoClientes);
        
        if (encontrados == 0)
        {
            printf("Nenhum cliente encontrado com esse nome.\n");
            printf("\nPressione ENTER para voltar ao menu...");
            getchar();
            return;
        }
        
        if (encontrados > 1)
        {
            printf("\nDigite o codigo do cliente desejado: ");
            scanf("%d", &codigoCliente);
            limparEntrada();
            
            Cliente clienteSelecionado;
            if (buscarCliente(codigoCliente, &clienteSelecionado))
            {
                strcpy(nomeCliente, clienteSelecionado.nome);
            }
        }
        else
        {
            arquivoClientes = fopen(ARQ_CLIENTES, "rb");
            while (fread(&c, sizeof(Cliente), 1, arquivoClientes))
            {
                if (strstr(c.nome, nomeBusca) != NULL)
                {
                    codigoCliente = c.codigoCliente;
                    strcpy(nomeCliente, c.nome);
                    break;
                }
            }
            fclose(arquivoClientes);
        }
    }
    else
    {
        printf("Opcao invalida!\n");
        printf("\nPressione ENTER para voltar ao menu...");
        getchar();
        return;
    }
    
    // Calcular pontos de fidelidade
    FILE *arquivoEstadias = fopen(ARQ_ESTADIAS, "rb");
    if (!arquivoEstadias)
    {
        printf("Nenhuma estadia registrada.\n");
        printf("\nPressione ENTER para voltar ao menu...");
        getchar();
        return;
    }
    
    Estadia e;
    int totalDiarias = 0;    // Acumula total de diárias
    int totalEstadias = 0;   // Conta total de estadias
    
    // Soma diárias de todas as estadias do cliente
    while (fread(&e, sizeof(Estadia), 1, arquivoEstadias))
    {
        if (e.codigoCliente == codigoCliente)
        {
            totalDiarias += e.quantidadeDiarias;
            totalEstadias++;
        }
    }
    
    fclose(arquivoEstadias);
    
    // Calcula pontos: 10 pontos por diária
    int pontos = totalDiarias * 10;
    
    // Exibe resultados
    printf("\n=== PONTOS DE FIDELIDADE ===\n");
    printf("Cliente: %s (Codigo: %d)\n", nomeCliente, codigoCliente);
    printf("Total de estadias: %d\n", totalEstadias);
    printf("Total de diarias: %d\n", totalDiarias);
    printf("Pontos de fidelidade: %d pontos\n", pontos);
    printf("\n(10 pontos por diaria hospedada)\n");
    
    // Mostra benefícios disponíveis baseados nos pontos
    if (pontos > 0)
    {
        printf("\nVANTAGENS:\n");
        if (pontos >= 100)
            printf("- %d pontos: Direito a 1 diaria gratis!\n", (pontos/100)*100);
        if (pontos >= 50)
            printf("- %d pontos: Upgrade de quarto disponivel\n", (pontos/50)*50);
        printf("- Proximo beneficio: %d pontos\n", ((pontos/100)+1)*100);
    }
    
    printf("\nPressione ENTER para voltar ao menu...");
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
    int menu; // Armazena opção escolhida pelo usuário

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
            mostrarClientes();
            break;
        case 7:
            mostrarFuncionarios();
            break;
        case 8:
            mostrarQuartos();
            break;
        case 9:
            mostrarEstadias();
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

        if (menu != 0)
        {
            printf("\nPressione ENTER para voltar ao menu...");
            getchar();
        }

    } while (menu != 0); // Continua até usuário escolher sair (0)

    return 0; // Retorno padrão indicando execução bem sucedida
}