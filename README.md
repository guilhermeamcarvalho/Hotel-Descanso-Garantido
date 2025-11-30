# Hotel-Descanso-Garantido
Sistema em C desenvolvido para gerenciar clientes, funcionários, quartos e estadias do Hotel Descanso Garantido. Permite cadastro, pesquisa, controle de reservas e cálculo de fidelidade, com dados armazenados em arquivos binários. Projeto interdisciplinar PUC Minas, usando metodologia Scrum e modularização.

# **DOCUMENTO DE CONTROLE DE IMPLEMENTAÇÃO**
**Sistema de Gerenciamento Hoteleiro - Hotel Descanso Garantido**  
**Equipe: - Guilherme Augusto Martins de Carvalho**
**- Rafael Galileu Thales de Oliveira**
**- Samuel Henrique Alvarenga e Lopoes**  

---

## **📋 SUMÁRIO DE ETAPAS**

| ETAPA | RESPONSÁVEL | STATUS |
|-------|-------------|--------|
| 1. Estrutura Básica e Menu | Guilherme | ⏳ Pendente |
| 2. Funções de Data e Arquivo | Rafael | ⏳ Pendente |
| 3. Cadastros Básicos | Samuel | ⏳ Pendente |
| 4. Busca e Quartos | Guilherme | ⏳ Pendente |
| 5. Datas e Listagens | Rafael | ⏳ Pendente |
| 6. Sistema de Estadias | Samuel | ⏳ Pendente |
| 7. Pesquisas Avançadas | Guilherme | ⏳ Pendente |
| 8. Fidelidade e Relatórios | Rafael | ⏳ Pendente |
| 9. Testes Finais | Samuel | ⏳ Pendente |
---
# **SPRINT 1 (2 a 3 dias)

## **📝 ETAPA 1: ESTRUTURA BÁSICA E MENU**

### **Responsável: GUILHERME**
### **Status:** ⏳ **PENDENTE**

### **✅ O QUE DEVE SER IMPLEMENTADO:**
```c
1. #includes (stdio.h, stdlib.h, string.h) ✅
2. Structs: Data, Cliente, Funcionario, Quarto, Estadia✅
3. Constantes dos arquivos: ARQ_CLIENTES, ARQ_FUNCIONARIOS, etc.
4. Função limparEntrada()
5. Função main() com menu esqueleto (13 opções)✅
```

### **📋 CHECKLIST DE VERIFICAÇÃO:**
- [ ] Structs definidas corretamente
- [ ] Constantes dos arquivos definidas
- [ ] `limparEntrada()` funciona
- [ ] Menu principal exibe todas as opções
- [ ] Programa compila sem erros: `gcc hotel.c -o hotel`
- [ ] Programa executa sem crash
- [ ] Menu navega entre opções (mesmo sem funcionalidades)
- [ ] Opção 0 (Sair) funciona

### **🧪 TESTES A REALIZAR:**
```bash
# Teste 1 - Compilação
gcc hotel.c -o hotel
echo $?  # Deve retornar 0

# Teste 2 - Execução básica
./hotel
# Deve exibir menu
# Digitar 0 deve encerrar o programa
```

### **📤 ENTREGAS ESPERADAS:**
- Commit message: `feat: estrutura básica e menu principal`
- Comentários: Estruturas definidas, menu funcional (esqueleto)

---

## **📝 ETAPA 2: FUNÇÕES DE DATA E ARQUIVO**

### **Responsável: RAFAEL**
### **Status:** ⏳ **PENDENTE**

### **✅ O QUE DEVE SER IMPLEMENTADO:**
```c
1. diasFevereiro(int ano)
2. diasNoMes(int mes, int ano)
3. validarData(Data d)
4. salvarClienteArquivo(Cliente c)
5. salvarFuncionarioArquivo(Funcionario f)
6. salvarQuartoArquivo(Quarto q)
7. salvarEstadiaArquivo(Estadia e)
```

### **📋 CHECKLIST DE VERIFICAÇÃO:**
- [ ] `diasFevereiro(2020)` retorna 29 (bissexto)
- [ ] `diasFevereiro(2021)` retorna 28 (não bissexto)
- [ ] `diasNoMes(2, 2020)` retorna 29
- [ ] `diasNoMes(2, 2021)` retorna 28
- [ ] `diasNoMes(4, 2024)` retorna 30
- [ ] `validarData({31, 12, 2024})` retorna 1 (válida)
- [ ] `validarData({31, 4, 2024})` retorna 0 (inválida)
- [ ] Funções de salvar não causam erros de compilação

### **🧪 TESTES A REALIZAR:**
```c
// Teste de datas
Data data1 = {29, 2, 2020};  // Válida (bissexto)
Data data2 = {29, 2, 2021};  // Inválida
printf("Teste 1: %d (esperado: 1)\n", validarData(data1));
printf("Teste 2: %d (esperado: 0)\n", validarData(data2));
```

### **📤 ENTREGAS ESPERADAS:**
- Commit message: `feat: funções de data e salvamento em arquivo`
- Testes unitários realizados

---

## **📝 ETAPA 3: CADASTROS BÁSICOS**

### **Responsável: SAMUEL**
### **Status:** ⏳ **PENDENTE**

### **✅ O QUE DEVE SER IMPLEMENTADO:**
```c
1. gerarCodigoCliente()
2. gerarCodigoFuncionario()
3. gerarCodigoEstadia()
4. cadastrarCliente()
5. cadastrarFuncionario()
6. Atualizar main() para usar as funções (casos 1 e 2)
```

### **📋 CHECKLIST DE VERIFICAÇÃO:**
- [ ] `gerarCodigoCliente()` retorna 1 se arquivo não existe
- [ ] `gerarCodigoCliente()` incrementa corretamente
- [ ] `cadastrarCliente()` coleta todos os dados
- [ ] `cadastrarCliente()` salva no arquivo
- [ ] `cadastrarFuncionario()` valida salário positivo
- [ ] Menu opção 1 funciona completamente
- [ ] Menu opção 2 funciona completamente
- [ ] Arquivo `clientes.bin` é criado ao cadastrar
- [ ] Arquivo `funcionarios.bin` é criado ao cadastrar

### **🧪 TESTES A REALIZAR:**
```
Executar programa:
1. Escolher opção 1 (Cadastrar cliente)
2. Inserir dados: Nome, Endereço, Telefone
3. Verificar se mensagem de sucesso aparece
4. Verificar se arquivo clientes.bin foi criado
5. Repetir para opção 2 (Cadastrar funcionário)
```

### **📤 ENTREGAS ESPERADAS:**
- Commit message: `feat: sistema de códigos automáticos e cadastro de clientes/funcionários`
- **SISTEMA PARCIAL: Opções 1 e 2 do menu funcionando**

-----------------------------------
# **SPRINT 2 (2 a 3 dias)

## **📝 ETAPA 4: BUSCA E QUARTOS**

### **Responsável: GUILHERME**
### **Status:** ⏳ **PENDENTE**

### **✅ O QUE DEVE SER IMPLEMENTADO:**
```c
1. buscarCliente(int codigo, Cliente *resultado)
2. buscarQuarto(int numero, Quarto *resultado)
3. cadastrarQuarto()
4. mostrarClientes()
5. Atualizar main() para usar as funções (casos 3 e 6)
```

### **📋 CHECKLIST DE VERIFICAÇÃO:**
- [ ] `buscarCliente()` retorna 1 se encontra cliente
- [ ] `buscarCliente()` retorna 0 se não encontra
- [ ] `buscarQuarto()` funciona similarmente
- [ ] `cadastrarQuarto()` valida número único
- [ ] `cadastrarQuarto()` valida capacidade > 0
- [ ] `cadastrarQuarto()` valida valor diária > 0
- [ ] `mostrarClientes()` lista todos os clientes
- [ ] `mostrarClientes()` mostra contador
- [ ] Menu opção 3 funciona completamente
- [ ] Menu opção 6 funciona completamente

### **🧪 TESTES A REALIZAR:**
```
1. Cadastrar 2 clientes (opção 1)
2. Cadastrar 2 quartos (opção 3)
3. Ver listagem de clientes (opção 6) - deve mostrar 2
4. Tentar cadastrar quarto com número repetido - deve recusar
```

### **📤 ENTREGAS ESPERADAS:**
- Commit message: `feat: funções de busca, cadastro de quartos e listagem de clientes`
- **SISTEMA PARCIAL: Opções 1, 2, 3 e 6 funcionando**

---

## **📝 ETAPA 5: DATAS E LISTAGENS**

### **Responsável: RAFAEL**
### **Status:** ⏳ **PENDENTE**

### **✅ O QUE DEVE SER IMPLEMENTADO:**
```c
1. lerData(const char *mensagem, Data *data)
2. diasEntreDatas(Data entrada, Data saida)
3. mostrarFuncionarios()
4. mostrarQuartos()
5. mostrarEstadias()
6. Atualizar main() para usar as funções (casos 7, 8, 9)
```

### **📋 CHECKLIST DE VERIFICAÇÃO:**
- [ ] `lerData()` valida formato correto (DD MM AAAA)
- [ ] `lerData()` permite até 3 tentativas
- [ ] `diasEntreDatas()` calcula corretamente datas no mesmo mês
- [ ] `diasEntreDatas()` calcula corretamente datas em meses diferentes
- [ ] `mostrarFuncionarios()` lista com formatação
- [ ] `mostrarQuartos()` mostra status (OCUPADO/LIVRE)
- [ ] `mostrarQuartos()` mostra estatísticas
- [ ] `mostrarEstadias()` mostra datas formatadas
- [ ] Menu opções 7, 8, 9 funcionam

### **🧪 TESTES A REALIZAR:**
```c
// Teste de cálculo de datas
Data entrada = {1, 1, 2024};
Data saida = {10, 1, 2024};
printf("Dias entre 01/01 e 10/01: %d (esperado: 9)\n", 
       diasEntreDatas(entrada, saida));
```

### **📤 ENTREGAS ESPERADAS:**
- Commit message: `feat: sistema completo de datas e funções de listagem`
- **SISTEMA PARCIAL: Opções 1-3, 6-9 funcionando**

---

## **📝 ETAPA 6: SISTEMA DE ESTADIAS**

### **Responsável: SAMUEL**
### **Status:** ⏳ **PENDENTE**

### **✅ O QUE DEVE SER IMPLEMENTADO:**
```c
1. quartoOcupado(int numero, int ocupado)
2. novaEstadia()
3. encerrarEstadia()
4. Atualizar main() para usar as funções (casos 4 e 5)
```

### **📋 CHECKLIST DE VERIFICAÇÃO:**
- [ ] `quartoOcupado()` atualiza status corretamente
- [ ] `novaEstadia()` valida cliente existente
- [ ] `novaEstadia()` mostra apenas quartos disponíveis
- [ ] `novaEstadia()` calcula diárias automaticamente
- [ ] `novaEstadia()` marca quarto como ocupado
- [ ] `encerrarEstadia()` calcula valor total
- [ ] `encerrarEstadia()` marca estadia como finalizada
- [ ] `encerrarEstadia()` libera quarto
- [ ] Menu opção 4 funciona completamente
- [ ] Menu opção 5 funciona completamente

### **🧪 TESTES A REALIZAR:**
```
1. Cadastrar cliente e quarto
2. Fazer nova estadia (opção 4)
3. Verificar se quarto aparece como OCUPADO (opção 8)
4. Encerrar estadia (opção 5)
5. Verificar se quarto aparece como LIVRE (opção 8)
```

### **📤 ENTREGAS ESPERADAS:**
- Commit message: `feat: sistema completo de estadias (check-in e check-out)`
- **SISTEMA PARCIAL: Opções 1-9 funcionando (faltam 10-13)**

-------------------------
# **SPRINT 3 (2 a 3 dias)

## **📝 ETAPA 7: PESQUISAS AVANÇADAS**

### **Responsável: GUILHERME**
### **Status:** ⏳ **PENDENTE**

### **✅ O QUE DEVE SER IMPLEMENTADO:**
```c
1. pesquisarCliente()
2. pesquisarFuncionario()
3. Atualizar main() para usar as funções (casos 10 e 11)
```

### **📋 CHECKLIST DE VERIFICAÇÃO:**
- [ ] `pesquisarCliente()` busca por código
- [ ] `pesquisarCliente()` busca por nome (parcial)
- [ ] `pesquisarCliente()` mostra mensagem se não encontrar
- [ ] `pesquisarFuncionario()` busca por código
- [ ] `pesquisarFuncionario()` busca por nome (parcial)
- [ ] Ambas funções mostram resultados formatados
- [ ] Menu opção 10 funciona completamente
- [ ] Menu opção 11 funciona completamente

### **🧪 TESTES A REALIZAR:**
```
1. Cadastrar cliente "João Silva"
2. Pesquisar por "João" (opção 10, busca por nome)
3. Deve encontrar João Silva
4. Pesquisar por "Silva" (busca parcial)
5. Também deve encontrar
6. Pesquisar por código específico
```

### **📤 ENTREGAS ESPERADAS:**
- Commit message: `feat: sistema de pesquisas avançadas`
- **SISTEMA PARCIAL: Opções 1-11 funcionando (faltam 12-13)**

---

## **📝 ETAPA 8: FIDELIDADE E RELATÓRIOS**

### **Responsável: RAFAEL**
### **Status:** ⏳ **PENDENTE**

### **✅ O QUE DEVE SER IMPLEMENTADO:**
```c
1. mostrarEstadiasCliente()
2. calcularPontosFidelidade()
3. Atualizar main() para usar as funções (casos 12 e 13)
```

### **📋 CHECKLIST DE VERIFICAÇÃO:**
- [ ] `mostrarEstadiasCliente()` busca por código do cliente
- [ ] `mostrarEstadiasCliente()` busca por nome do cliente
- [ ] `mostrarEstadiasCliente()` mostra histórico completo
- [ ] `mostrarEstadiasCliente()` calcula total de diárias
- [ ] `calcularPontosFidelidade()` calcula 10 pontos/diária
- [ ] `calcularPontosFidelidade()` mostra benefícios
- [ ] Ambas funções mostram estatísticas
- [ ] Menu opção 12 funciona completamente
- [ ] Menu opção 13 funciona completamente

### **🧪 TESTES A REALIZAR:**
```
1. Criar cliente com múltiplas estadias
2. Usar opção 12 para ver histórico
3. Verificar se mostra todas estadias
4. Usar opção 13 para calcular pontos
5. Verificar se cálculo está correto (diárias × 10)
```

### **📤 ENTREGAS ESPERADAS:**
- Arquivo: `hotel.c`
- Commit message: `feat: sistema completo de fidelidade e relatórios`
- **SISTEMA COMPLETO: Todas 13 opções funcionando!**

---

## **📝 ETAPA 9: TESTES FINAIS**

### **Responsável: SAMUEL**
### **Status:** ⏳ **PENDENTE**
### **Prazo:** Semana 3 - Quarta-feira

### **✅ O QUE DEVE SER FEITO:**
1. Teste integrado completo
2. Correção de bugs
3. Validação de todos os requisitos
4. Documentação final

### **📋 CHECKLIST DE TESTES FINAIS:**
- [ ] **Teste de fluxo completo:**
  - Cadastrar cliente, funcionário, quarto
  - Fazer estadia
  - Encerrar estadia
  - Ver histórico
  - Calcular pontos
- [ ] **Teste de dados inválidos:**
  - Datas inválidas
  - Códigos inexistentes
  - Valores negativos
- [ ] **Teste de arquivos:**
  - Arquivos são criados corretamente
  - Dados persistem entre execuções
  - Não corrompe arquivos existentes
- [ ] **Teste de usabilidade:**
  - Menu é intuitivo
  - Mensagens de erro são claras
  - Formatação é consistente


### **📤 ENTREGAS ESPERADAS:**
- Arquivo final: `hotel.c`
- Relatório de testes
- Documentação do sistema

---

## **📊 CONTROLE DE PROGRESSO**

### **Progresso Geral:** 0% (0/9 etapas concluídas)

### **Progresso por Integrante:**
- **Guilherme:** 0/3 tarefas (0%)
  - ETAPA 1: ⏳ Pendente
  - ETAPA 4: ⏳ Pendente  
  - ETAPA 7: ⏳ Pendente

- **Rafael:** 0/3 tarefas (0%)
  - ETAPA 2: ⏳ Pendente
  - ETAPA 5: ⏳ Pendente
  - ETAPA 8: ⏳ Pendente

- **Samuel:** 0/3 tarefas (0%)
  - ETAPA 3: ⏳ Pendente
  - ETAPA 6: ⏳ Pendente
  - ETAPA 9: ⏳ Pendente

---

## **✅ CRITÉRIOS DE ACEITAÇÃO FINAL**

### **O sistema está pronto quando:**
1. [ ] Todas as 13 opções do menu funcionam
2. [ ] Não há erros de compilação
3. [ ] Não há crashes durante uso normal
4. [ ] Dados são persistidos corretamente
5. [ ] Validações de entrada funcionam
6. [ ] Cálculos de datas estão corretos
7. [ ] Sistema de fidelidade calcula pontos corretamente
8. [ ] Interface é amigável e informativa

---

## **📋 CHECKLIST DE ENTREGA FINAL**

### **Arquivos a serem entregues:**
- [ ] `hotel.c` (código completo)
- [ ] `README.md` (documentação)
- [ ] `clientes.bin`, `funcionarios.bin`, etc. (dados de exemplo)
- [ ] Relatório de testes

### **Documentação a ser preenchida:**
- [ ] Nomes completos dos integrantes
- [ ] Data de início e conclusão
- [ ] Descrição de funcionalidades implementadas
- [ ] Instruções de compilação e execução
- [ ] Exemplos de uso