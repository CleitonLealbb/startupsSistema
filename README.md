# startupsSistema
sistema em linguam C conhecimentos basicos adiquirido no curso linguagem e tecnica de progamação.

# Sistema de Cadastro e Gerenciamento de Indústrias

Este projeto tem como objetivo cadastrar e gerenciar dados de indústrias, realizar atualizações mensais de resíduos tratados e gerar relatórios detalhados. Desenvolvido em C, é compatível com Linux, macOS e Windows.

---

## Índice

1. [Requisitos de Sistema](#requisitos-de-sistema)  
2. [Instalação e Configuração](#instalação-e-configuração)  
3. [Compilação do Programa](#compilação-do-programa)  
4. [Execução do Programa](#execução-do-programa)  
5. [Testes Funcionais](#testes-funcionais)  
6. [Problemas Comuns e Soluções](#problemas-comuns-e-soluções)  

---

## Requisitos de Sistema

### Requisitos Mínimos

1. **Sistema Operacional**  
   Compatível com Linux, macOS e Windows.

2. **Compilador C**  
   - Linux/macOS: GCC (GNU Compiler Collection).  
   - Windows: MinGW, MSYS2 ou equivalente.

3. **Ferramentas Adicionais (Opcional)**  
   - Editor de Código: Visual Studio Code, Sublime Text, Atom, Code::Blocks, entre outros.

---

## Instalação e Configuração

### Pré-requisitos de Software

#### Instalação do Compilador C

##### Linux ou macOS
1. Verifique se o GCC está instalado:
   ```bash
   gcc --version
Caso não esteja, instale com o comando:
bash
Copiar código
sudo apt-get install build-essential  # Debian/Ubuntu
sudo yum groupinstall "Development Tools"  # Fedora/CentOS
Windows
Instale o MinGW ou MSYS2:
MinGW
MSYS2
Após a instalação, adicione o diretório do compilador ao PATH do sistema:
makefile
Copiar código
C:\MinGW\bin
Verifique a instalação:
bash
Copiar código
gcc --version
Banco de Dados
Criando o Banco de Dados SQLite
Crie o banco:
bash
Copiar código
sqlite3 industrias.db
Insira o script SQL:
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

