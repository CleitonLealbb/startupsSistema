#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sqlite3.h>
#include <unistd.h>

// Fun��es principais
void login();
void menu_inicial();
void cadastrar_industria();
void buscar_industria();
void atualizar_dados_mensais();
void gerar_relatorios();
void inicializar_banco();
void imprimir_linha(char caractere, int tamanho);
void salvar_relatorio_csv();
void salvar_relatorio_txt();
void limpar_tela();

// Vari�veis globais
sqlite3 *db; // Ponteiro para o banco de dados
char usuario_logado[50];

// Fun��o para imprimir uma linha decorativa
void imprimir_linha(char caractere, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
    {
        printf("%c", caractere);
    }
    printf("\n");
}

// Fun��o para inicializar o banco de dados
void inicializar_banco()
{
    int rc = sqlite3_open("industria.db", &db);
    if (rc)
    {
        fprintf(stderr, "Erro ao abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        exit(1);
    }

    // Cria��o das tabelas
    const char *sql_industrias =
        "CREATE TABLE IF NOT EXISTS industrias ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "nome TEXT NOT NULL, "
        "cnpj TEXT NOT NULL UNIQUE, "
        "endereco TEXT NOT NULL);";

    const char *sql_residuos =
        "CREATE TABLE IF NOT EXISTS residuos ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "industria_nome TEXT NOT NULL, "
        "tipo_residuo TEXT, "
        "residuos_tratados REAL NOT NULL, "
        "custo_tratamento REAL NOT NULL);";

    char *errmsg;

    // Tenta criar a tabela com a estrutura atualizada
    if (sqlite3_exec(db, sql_industrias, 0, 0, &errmsg) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao criar tabela industrias: %s\n", errmsg);
        sqlite3_free(errmsg);
        exit(1);
    }

    if (sqlite3_exec(db, sql_residuos, 0, 0, &errmsg) != SQLITE_OK)
    {
        // Se a tabela j� existir, tenta atualizar sua estrutura
        if (strstr(errmsg, "duplicate column name") == NULL)
        {
            const char *sql_alter_residuos =
                "ALTER TABLE residuos ADD COLUMN tipo_residuo TEXT;";
            if (sqlite3_exec(db, sql_alter_residuos, 0, 0, &errmsg) != SQLITE_OK)
            {
                fprintf(stderr, "Erro ao alterar tabela residuos: %s\n", errmsg);
                sqlite3_free(errmsg);
                exit(1);
            }
        }
        sqlite3_free(errmsg);
    }
}

// Fun��o de Login
void login()
{
    char usuario[50];
    char senha[50];

    imprimir_linha('=', 100);
    printf("   Startup Solu��es\n");
    printf("   Tela de Login\n");
    imprimir_linha('=', 100);

    printf("Usu�rio: ");
    scanf("%s", usuario);
    printf("Senha: ");
    scanf("%s", senha);

    if (strcmp(usuario, "admin") == 0 && strcmp(senha, "1234") == 0)
    {
        strcpy(usuario_logado, usuario);
        printf("\nLogin bem-sucedido!\n\n");
        limpar_tela();
        menu_inicial();

    }
    else
    {
        printf("Usu�rio ou senha incorretos. Tente novamente.\n\n");
        sleep(5);
        limpar_tela();
        login();
    }

}

// Fun��o para o Menu Inicial
void menu_inicial()
{
    int opcao;

    imprimir_linha('=', 100);
    printf("   Startup Solu��es\n");
    printf("   Menu Inicial\n");
    imprimir_linha('=', 100);

    printf("1. Cadastro de Ind�strias\n");
    printf("2. Buscar Ind�strias\n");
    printf("3. Atualiza��o Mensal de Res�duos\n");
    printf("4. Gera��o de Relat�rios\n");
    printf("5. Sair\n");
    printf("Escolha uma op��o: ");
    scanf("%d", &opcao);
    limpar_tela();

    switch (opcao)
    {
    case 1:
        cadastrar_industria();
        break;
    case 2:
        buscar_industria();
        break;
    case 3:
        atualizar_dados_mensais();
        break;
    case 4:
        gerar_relatorios();
        break;
    case 5:
        printf("Logar-se!\n");
        sqlite3_close(db); // Fecha o banco de dados
        sleep(3);
        limpar_tela();
        login();
    default:
        printf("Op��o inv�lida. Tente novamente.\n\n");
        menu_inicial();
    }
}

// Fun��o para cadastrar uma ind�stria
void cadastrar_industria()
{
    char nome[100], cnpj[20], endereco[200];
    sqlite3_stmt *stmt;

    printf("\nCadastro de Ind�stria\n");
    printf("Digite '0' a qualquer momento para cancelar o cadastro.\n");

    // Nome da empresa
    printf("Nome da Empresa: ");
    scanf(" %[^\n]", nome);
    if (strcmp(nome, "0") == 0)
    {
        printf("Cadastro cancelado. Retornando ao menu inicial...\n");
        sleep(3);
        limpar_tela();  // Chama a fun��o para limpar a tela
        menu_inicial();
        return;
    }

    // CNPJ
    printf("CNPJ: ");
    scanf(" %[^\n]", cnpj);
    if (strcmp(cnpj, "0") == 0)
    {
        printf("Cadastro cancelado. Retornando ao menu inicial...\n");
        sleep(3);
        limpar_tela();  // Chama a fun��o para limpar a tela
        menu_inicial();
        return;
    }

    // Endere�o
    printf("Endere�o (Rua, N�mero, Cidade, Estado): ");
    scanf(" %[^\n]", endereco);
    if (strcmp(endereco, "0") == 0)
    {
        printf("Cadastro cancelado. Retornando ao menu inicial...\n");
        sleep(3);
        limpar_tela();  // Chama a fun��o para limpar a tela
        menu_inicial();
        return;
    }

    // Inser��o no banco de dados
    const char *sql = "INSERT INTO industrias (nome, cnpj, endereco) VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, nome, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, cnpj, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, endereco, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Erro ao cadastrar ind�stria: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Ind�stria cadastrada com sucesso!\n");
    }

    sqlite3_finalize(stmt);
    limpar_tela();// Limpa a tela antes de chamar o menu
    sleep(2);
    menu_inicial();
}


// Fun��o para buscar ind�strias
void buscar_industria()
{

    const char *sql = "SELECT id, nome, cnpj, endereco FROM industrias;";
    sqlite3_stmt *stmt;
    char opcao[3];  // Vari�vel para ler a op��o do usu�rio

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao buscar ind�strias: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\nLista de Ind�strias:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const char *nome = (const char *)sqlite3_column_text(stmt, 1);
        const char *cnpj = (const char *)sqlite3_column_text(stmt, 2);
        const char *endereco = (const char *)sqlite3_column_text(stmt, 3);

        printf("ID: %d | Nome: %s | CNPJ: %s | Endere�o: %s\n", id, nome, cnpj, endereco);
    }

    sqlite3_finalize(stmt);

    // Ap�s exibir todas as empresas, pergunta o que o usu�rio deseja fazer
    printf("\nDigite '0' para voltar ao menu inicial ou '1' para encerrar o programa.\n");
    scanf("%s", opcao);
    limpar_tela();
    if (strcmp(opcao, "0") == 0)
    {
        menu_inicial();  // Volta ao menu inicial
    }
    else if (strcmp(opcao, "1") == 0)
    {
        printf("Programa encerrado.\n");

        exit(0);  // Encerra o programa
    }
    else
    {
        printf("Op��o inv�lida. Encerrando o programa.\n");
        exit(1);  // Encerra o programa caso uma op��o inv�lida seja inserida
    }

}


// Fun��o atualizar dados mensais

void atualizar_dados_mensais()
{

    char cnpj[20];
    char tipo_residuo[50];
    double residuos_tratados, custo_tratamento;

    imprimir_linha('=', 100);
    printf("Atualiza��o dos Dados Mensais\n");
    imprimir_linha('=', 100);

    // Coleta de informa��es do usu�rio
    printf("CNPJ da ind�stria: ");
    scanf("%s", cnpj);

    // Verifica se o CNPJ existe no banco de dados
    sqlite3_stmt *stmt_check;
    const char *sql_check = "SELECT COUNT(*) FROM industrias WHERE cnpj = ?;";
    if (sqlite3_prepare_v2(db, sql_check, -1, &stmt_check, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar consulta de verifica��o: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt_check, 1, cnpj, -1, SQLITE_STATIC);

    int count = 0;
    if (sqlite3_step(stmt_check) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt_check, 0);
    }
    sqlite3_finalize(stmt_check);

    if (count == 0)
    {
        printf("CNPJ n�o encontrado no banco de dados. Verifique e tente novamente.\n");
        sleep(3);
        limpar_tela();
        menu_inicial();  // Retorna ao menu inicial
        return;
    }

    printf("Tipo de res�duo: ");
    scanf(" %[^\n]", tipo_residuo);

    // Valida os dados de res�duos e custo
    printf("Quantidade de res�duos tratados (em toneladas): ");
    if (scanf("%lf", &residuos_tratados) != 1 || residuos_tratados < 0)
    {
        printf("Erro: A quantidade de res�duos deve ser um n�mero positivo.\n");

        menu_inicial();  // Retorna ao menu inicial em caso de erro
        return;
    }

    printf("Custo do tratamento: ");
    if (scanf("%lf", &custo_tratamento) != 1 || custo_tratamento < 0)
    {
        printf("Erro: O custo de tratamento deve ser um valor positivo.\n");

        menu_inicial();  // Retorna ao menu inicial em caso de erro
        return;
    }

    // Prepara a consulta para inserir os dados
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO residuos (industria_nome, tipo_residuo, residuos_tratados, custo_tratamento) "
                      "VALUES (?, ?, ?, ?);";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao preparar a consulta: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, cnpj, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, tipo_residuo, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, residuos_tratados);
    sqlite3_bind_double(stmt, 4, custo_tratamento);

    // Executa a consulta para inserir os dados
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Erro ao atualizar os dados de res�duos: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Dados atualizados com sucesso!\n");
    }

    sqlite3_finalize(stmt);

    // Pergunta ao usu�rio se ele deseja voltar ao menu ou encerrar o programa
    printf("\nDigite '0' para voltar ao menu inicial ou '1' para encerrar o programa.\n");
    char opcao[3];
    scanf("%s", opcao);

    if (strcmp(opcao, "0") == 0)
    {
        limpar_tela();  // Chama a fun��o para limpar a tela antes de voltar ao menu
        menu_inicial();  // Volta ao menu inicial
    }
    else if (strcmp(opcao, "1") == 0)
    {
        printf("Programa encerrado.\n");
        exit(0);  // Encerra o programa
    }
    else
    {
        printf("Op��o inv�lida. Encerrando o programa.\n");
        exit(1);  // Encerra o programa em caso de op��o inv�lida
    }
}



void gerar_relatorios()
{
    sqlite3_stmt *stmt;

    printf("\nGerando Relat�rio de Res�duos Tratados\n");

    // Consulta para obter os dados de res�duos
    const char *sql = "SELECT industria_nome, tipo_residuo, SUM(residuos_tratados), SUM(custo_tratamento) "
                      "FROM residuos GROUP BY industria_nome, tipo_residuo;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao gerar relat�rio: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\nRelat�rio de Res�duos Tratados por Ind�stria:\n");
    printf("%-30s %-30s %-20s %-20s\n", "Ind�stria", "Tipo de Res�duo", "Total Tratado (ton)", "Total Custo");
    imprimir_linha('-', 100);

    // Imprime os dados do relat�rio
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *nome_industria = (const char *)sqlite3_column_text(stmt, 0);
        const char *tipo_residuo = (const char *)sqlite3_column_text(stmt, 1);
        double total_tratado = sqlite3_column_double(stmt, 2);
        double total_custo = sqlite3_column_double(stmt, 3);

        printf("%-30s %-30s %-20.2f %-20.2f\n", nome_industria, tipo_residuo, total_tratado, total_custo);
    }

    sqlite3_finalize(stmt);

    // Op��es de a��o ap�s gerar o relat�rio
    printf("\nEscolha uma op��o:\n");
    printf("0 - Voltar ao menu inicial\n");
    printf("1 - Salvar relat�rio em arquivo .txt\n");
    printf("2 - Salvar relat�rio em arquivo .csv\n");

    int opcao;
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 0:
        limpar_tela();  // Limpa a tela antes de voltar ao menu
        menu_inicial(); // Retorna ao menu inicial
        break;
    case 1:
        salvar_relatorio_txt();  // Chama fun��o para salvar em .txt
        break;
    case 2:
        salvar_relatorio_csv(); // Chama fun��o para salvar em .csv
        break;
    default:
        printf("Op��o inv�lida. Voltando ao menu inicial.\n");
        limpar_tela();  // Limpa a tela
        menu_inicial(); // Retorna ao menu inicial
        break;
    }
}

void salvar_relatorio_txt()
{
    FILE *arquivo = fopen("relatorio_residuos.txt", "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para salvar o relat�rio.\n");
        return;
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT industria_nome, tipo_residuo, SUM(residuos_tratados), SUM(custo_tratamento) "
                      "FROM residuos GROUP BY industria_nome, tipo_residuo;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao gerar relat�rio: %s\n", sqlite3_errmsg(db));
        fclose(arquivo);
        return;
    }

    // Escreve os dados no arquivo
    fprintf(arquivo, "%-30s %-30s %-20s %-20s\n", "Ind�stria", "Tipo de Res�duo", "Total Tratado (ton)", "Total Custo");
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *nome_industria = (const char *)sqlite3_column_text(stmt, 0);
        const char *tipo_residuo = (const char *)sqlite3_column_text(stmt, 1);
        double total_tratado = sqlite3_column_double(stmt, 2);
        double total_custo = sqlite3_column_double(stmt, 3);

        fprintf(arquivo, "%-30s %-30s %-20.2f %-20.2f\n", nome_industria, tipo_residuo, total_tratado, total_custo);
    }

    sqlite3_finalize(stmt);
    fclose(arquivo);
    // Mensagem confirmando o salvamento do arquivo txt
    printf("Relat�rio salvo com sucesso em 'relatorio_residuos.txt'.\n");
    sleep(3);
    limpar_tela();
    // Chama as op��es novamente
    gerar_relatorios();
}


void salvar_relatorio_csv()
{
    FILE *arquivo = fopen("relatorio_residuos.csv", "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para salvar o relat�rio.\n");
        return;
    }

    sqlite3_stmt *stmt;
    const char *sql = "SELECT industria_nome, tipo_residuo, SUM(residuos_tratados), SUM(custo_tratamento) "
                      "FROM residuos GROUP BY industria_nome, tipo_residuo;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Erro ao gerar relat�rio: %s\n", sqlite3_errmsg(db));
        fclose(arquivo);
        return;
    }

    // Escreve os dados no arquivo CSV
    fprintf(arquivo, "Ind�stria,Tipo de Res�duo,Total Tratado (ton),Total Custo\n");
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *nome_industria = (const char *)sqlite3_column_text(stmt, 0);
        const char *tipo_residuo = (const char *)sqlite3_column_text(stmt, 1);
        double total_tratado = sqlite3_column_double(stmt, 2);
        double total_custo = sqlite3_column_double(stmt, 3);

        fprintf(arquivo, "%s,%s,%.2f,%.2f\n", nome_industria, tipo_residuo, total_tratado, total_custo);
    }

    sqlite3_finalize(stmt);
    fclose(arquivo);
    // Mensagem de confirma��o de arquivo CSV salvo
    printf("Relat�rio salvo com sucesso em 'relatorio_residuos.csv'.\n");
    sleep(3);
    limpar_tela();
    // Chama as op��es novamente
    gerar_relatorios();
}



// Fun��o para limpar a tela (usando comando para sistemas Unix-like)
void limpar_tela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Fun��o principal
int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    inicializar_banco();
    login();
    return 0;
}
