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