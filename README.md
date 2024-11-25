3.	MANUAL DE INSTALAÇÃO, COMPILAÇÃO E TESTES
Este documento apresenta o manual de instalação, compilação e testes do programa desenvolvido em linguagem C, que tem como objetivo realizar o cadastro e gerenciamento de dados de indústrias, atualização mensal de dados de resíduos e geração de relatórios. 
O programa foi desenvolvido com foco em praticidade, eficiência e compatibilidade multiplataforma (Linux, macOS e Windows). 
3.1.	Requisitos Mínimos  do Sistema 
3.1.1.	Sistema Operacional 
Compatível com Linux, macOS e Windows. 
3.1.2.	Compilador C 
Linux/macOS GCC (GNU Compiler Collection). 
Windows MinGW, MSYS2 ou equivalente. 
3.1.3.	Ferramentas Adicionais (Opcional) 
Editor de Código Visual Studio Code, Sublime Text, Atom, Code::Blocks, entre outros. 
3.2.	Pré-requisitos de Software 
3.2.1.	Instalação do Compilador C 
•	Linux ou macOS 
Verifique se o GCC está instalado: 
bash 
Copiar código 
gcc --version 
Caso não esteja, instale com o comando: 
bash 
Copiar código 
sudo apt-get install build-essential  # Debian/Ubuntu 
sudo yum groupinstall "Development Tools"  # Fedora/CentOS 
•	Windows 
Instale o MinGW ou MSYS2: 
MinGW: Baixe do site oficial OSDN MinGW. 
MSYS2: Baixe do site oficial MSYS2. 
Após a instalação, adicione o diretório do compilador ao PATH do sistema. Exemplo: 
text 
Copiar código 
C:\MinGW\bin 
Verifique a instalação com: 
bash 
Copiar código 
gcc --version 
3.3.	Instalação e Configuração do Banco de Dados 
3.3.1.	Criando o Banco de Dados SQLite 
No terminal ou prompt de comando, crie o banco de dados: 
bash 
Copiar código 
sqlite3 industrias.db 
Insira o seguinte script SQL para criar as tabelas: 
sql 
Copiar código 
CREATE TABLE industrias ( 
    id INTEGER PRIMARY KEY AUTOINCREMENT, 
    nome TEXT NOT NULL, 
    cnpj TEXT NOT NULL, 
    endereco TEXT NOT NULL 
); 
 
CREATE TABLE residuos ( 
    id INTEGER PRIMARY KEY AUTOINCREMENT, 
    industria_nome TEXT NOT NULL, 
    tipo_residuo TEXT NOT NULL, 
    residuos_tratados REAL NOT NULL, 
    custo_tratamento REAL NOT NULL 
); 
Salve as alterações e feche o banco. 








4.	COMPILANDO O PROGRAMA
4.1.	Obtendo o Código-Fonte 
Baixe ou clone o repositório do projeto: 
bash 
Copiar código 
git clone <URL-do-repositório> 
Caso tenha recebido um arquivo compactado, extraia-o em uma pasta local. 
4.2.	Compilando o Código 
Linux/macOS: 
bash 
Copiar código 
gcc -o startupSistema main.c -lsqlite3 
Windows: 
bash 
Copiar código 
gcc -o startupSistema.exe main.c -lsqlite3 
Se a compilação for bem-sucedida, será gerado um arquivo executável chamado startupSistema (ou startupSistema.exe no Windows). 
poderá inserir o nome de usuário e senha para acessar o menu principal.






5.	EXECUTANDO O PROGRAMA 
5.1.	No Linux/macOS 
Execute no terminal: 
bash 
Copiar código 
./startupSistema.exe 
5.2.	No Windows 
Execute no prompt de comando: 
bash 
Copiar código 
startupSistema.exe  
Ao executar, a tela de login será exibida. Use as credenciais padrão para acessar o menu inicial: 
Usuário: admin 
Senha: 1234 










6.	TESTES FUNCIONAIS 
6.1.	Teste de Cadastro de Indústrias 
Acesse o menu inicial e escolha a opção 1 - Cadastro de Indústrias. 
Insira os seguintes dados de exemplo: 
Nome: Indústria XYZ 
CNPJ: 12.345.678/0001-90 
Endereço: Rua A, 123, São Paulo, SP 
Verifique no banco de dados: 
bash 
Copiar código 
sqlite3 industrias.db "SELECT * FROM industrias;" 
6.2.	Teste de Atualização Mensal 
Escolha a opção 2 - Atualização Mensal de Resíduos no menu. 
Insira os seguintes dados: 
CNPJ: 12.345.678/0001-90 
Tipo de Resíduo: Plástico 
Quantidade: 10.5 
Custo Total: 5000.00 
Verifique os dados no banco: 
bash 
Copiar código 
sqlite3 industrias.db "SELECT * FROM residuos;" 
6.3.	Teste de Relatórios 
Escolha a opção 3 - Geração de Relatórios. 
Escolha salvar o relatório como .txt ou .csv. 
Verifique o arquivo gerado no diretório do programa. 
