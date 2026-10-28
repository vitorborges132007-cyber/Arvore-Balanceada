#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <limits>
#include <string>
#include <random>

using namespace std;
using namespace std::chrono;

struct NoAVL {
    int valor;
    NoAVL* esquerda;
    NoAVL* direita;
    int altura;

    explicit NoAVL(int v) : valor(v), esquerda(nullptr), direita(nullptr), altura(1) {}
};

class ArvoreAVL {
private:
    NoAVL* raiz;
    unsigned long long rotacoes;

    int alturaNo(NoAVL* no) const {
        return no == nullptr ? 0 : no->altura;
    }

    int fator(NoAVL* no) const {
        return no == nullptr ? 0 : alturaNo(no->esquerda) - alturaNo(no->direita);
    }

    void atualizarAltura(NoAVL* no) {
        no->altura = 1 + max(alturaNo(no->esquerda), alturaNo(no->direita));
    }

    NoAVL* rotacaoDireita(NoAVL* y) {
        NoAVL* x = y->esquerda;
        NoAVL* t2 = x->direita;
        x->direita = y;
        y->esquerda = t2;
        atualizarAltura(y);
        atualizarAltura(x);
        ++rotacoes;
        return x;
    }

    NoAVL* rotacaoEsquerda(NoAVL* x) {
        NoAVL* y = x->direita;
        NoAVL* t2 = y->esquerda;
        y->esquerda = x;
        x->direita = t2;
        atualizarAltura(x);
        atualizarAltura(y);
        ++rotacoes;
        return y;
    }

    NoAVL* inserirNo(NoAVL* no, int valor, bool& inseriu) {
        if (no == nullptr) {
            inseriu = true;
            return new NoAVL(valor);
        }

        if (valor < no->valor) {
            no->esquerda = inserirNo(no->esquerda, valor, inseriu);
        } else if (valor > no->valor) {
            no->direita = inserirNo(no->direita, valor, inseriu);
        } else {
            return no;
        }

        atualizarAltura(no);
        int fb = fator(no);

        if (fb > 1 && valor < no->esquerda->valor) {
            return rotacaoDireita(no);
        }

        if (fb < -1 && valor > no->direita->valor) {
            return rotacaoEsquerda(no);
        }

        if (fb > 1 && valor > no->esquerda->valor) {
            no->esquerda = rotacaoEsquerda(no->esquerda);
            return rotacaoDireita(no);
        }

        if (fb < -1 && valor < no->direita->valor) {
            no->direita = rotacaoDireita(no->direita);
            return rotacaoEsquerda(no);
        }

        return no;
    }

    NoAVL* menorNo(NoAVL* no) const {
        while (no != nullptr && no->esquerda != nullptr) {
            no = no->esquerda;
        }
        return no;
    }

    NoAVL* removerNo(NoAVL* no, int valor, bool& removeu) {
        if (no == nullptr) {
            return nullptr;
        }

        if (valor < no->valor) {
            no->esquerda = removerNo(no->esquerda, valor, removeu);
        } else if (valor > no->valor) {
            no->direita = removerNo(no->direita, valor, removeu);
        } else {
            removeu = true;

            if (no->esquerda == nullptr || no->direita == nullptr) {
                NoAVL* filho = no->esquerda != nullptr ? no->esquerda : no->direita;
                delete no;
                return filho;
            }

            NoAVL* sucessor = menorNo(no->direita);
            no->valor = sucessor->valor;
            bool removeuSucessor = false;
            no->direita = removerNo(no->direita, sucessor->valor, removeuSucessor);
        }

        atualizarAltura(no);
        int fb = fator(no);

        if (fb > 1 && fator(no->esquerda) >= 0) {
            return rotacaoDireita(no);
        }

        if (fb > 1 && fator(no->esquerda) < 0) {
            no->esquerda = rotacaoEsquerda(no->esquerda);
            return rotacaoDireita(no);
        }

        if (fb < -1 && fator(no->direita) <= 0) {
            return rotacaoEsquerda(no);
        }

        if (fb < -1 && fator(no->direita) > 0) {
            no->direita = rotacaoDireita(no->direita);
            return rotacaoEsquerda(no);
        }

        return no;
    }

    bool validarNo(NoAVL* no, long long minimo, long long maximo, int& alturaReal, int& quantidade) const {
        if (no == nullptr) {
            alturaReal = 0;
            return true;
        }

        if (no->valor <= minimo || no->valor >= maximo) {
            return false;
        }

        int ae = 0;
        int ad = 0;

        bool ve = validarNo(no->esquerda, minimo, no->valor, ae, quantidade);
        bool vd = validarNo(no->direita, no->valor, maximo, ad, quantidade);

        alturaReal = 1 + max(ae, ad);
        ++quantidade;

        return ve && vd && no->altura == alturaReal && abs(ae - ad) <= 1;
    }

    void imprimirEmOrdemNo(NoAVL* no) const {
        if (no == nullptr) {
            return;
        }

        imprimirEmOrdemNo(no->esquerda);
        cout << no->valor << ' ';
        imprimirEmOrdemNo(no->direita);
    }

    void imprimirEstruturaNo(NoAVL* no, int nivel) const {
        if (no == nullptr) {
            return;
        }

        imprimirEstruturaNo(no->direita, nivel + 1);
        cout << string(static_cast<size_t>(nivel) * 4, ' ')
             << no->valor << " (h=" << no->altura << ")\n";
        imprimirEstruturaNo(no->esquerda, nivel + 1);
    }

    void liberarNo(NoAVL* no) {
        if (no == nullptr) {
            return;
        }

        liberarNo(no->esquerda);
        liberarNo(no->direita);
        delete no;
    }

public:
    ArvoreAVL() : raiz(nullptr), rotacoes(0) {}

    ~ArvoreAVL() {
        liberarNo(raiz);
    }

    ArvoreAVL(const ArvoreAVL&) = delete;
    ArvoreAVL& operator=(const ArvoreAVL&) = delete;

    bool inserir(int valor) {
        bool inseriu = false;
        raiz = inserirNo(raiz, valor, inseriu);
        return inseriu;
    }

    bool buscar(int valor) const {
        NoAVL* atual = raiz;

        while (atual != nullptr) {
            if (valor == atual->valor) {
                return true;
            }

            atual = valor < atual->valor ? atual->esquerda : atual->direita;
        }

        return false;
    }

    bool remover(int valor) {
        bool removeu = false;
        raiz = removerNo(raiz, valor, removeu);
        return removeu;
    }

    bool vazia() const {
        return raiz == nullptr;
    }

    int altura() const {
        return alturaNo(raiz);
    }

    unsigned long long obterRotacoes() const {
        return rotacoes;
    }

    bool validar() const {
        int alturaReal = 0;
        int quantidade = 0;

        return validarNo(
            raiz,
            numeric_limits<long long>::min(),
            numeric_limits<long long>::max(),
            alturaReal,
            quantidade
        );
    }

    void imprimirEmOrdem() const {
        imprimirEmOrdemNo(raiz);
        cout << '\n';
    }

    void imprimirEstrutura() const {
        if (raiz == nullptr) {
            cout << "Arvore vazia.\n";
        } else {
            imprimirEstruturaNo(raiz, 0);
        }
    }
};

enum Cor {
    VERMELHO,
    PRETO
};

struct NoRN {
    int valor;
    Cor cor;
    NoRN* esquerda;
    NoRN* direita;
    NoRN* pai;

    explicit NoRN(int v)
        : valor(v), cor(VERMELHO), esquerda(nullptr), direita(nullptr), pai(nullptr) {}
};

class ArvoreRN {
private:
    NoRN* raiz;
    unsigned long long rotacoes;

    void rotacaoEsquerda(NoRN* x) {
        NoRN* y = x->direita;
        x->direita = y->esquerda;

        if (y->esquerda != nullptr) {
            y->esquerda->pai = x;
        }

        y->pai = x->pai;

        if (x->pai == nullptr) {
            raiz = y;
        } else if (x == x->pai->esquerda) {
            x->pai->esquerda = y;
        } else {
            x->pai->direita = y;
        }

        y->esquerda = x;
        x->pai = y;
        ++rotacoes;
    }

    void rotacaoDireita(NoRN* y) {
        NoRN* x = y->esquerda;
        y->esquerda = x->direita;

        if (x->direita != nullptr) {
            x->direita->pai = y;
        }

        x->pai = y->pai;

        if (y->pai == nullptr) {
            raiz = x;
        } else if (y == y->pai->esquerda) {
            y->pai->esquerda = x;
        } else {
            y->pai->direita = x;
        }

        x->direita = y;
        y->pai = x;
        ++rotacoes;
    }

    void corrigirInsercao(NoRN* z) {
        while (z != raiz && z->pai != nullptr && z->pai->cor == VERMELHO) {
            NoRN* pai = z->pai;
            NoRN* avo = pai->pai;

            if (pai == avo->esquerda) {
                NoRN* tio = avo->direita;

                if (tio != nullptr && tio->cor == VERMELHO) {
                    pai->cor = PRETO;
                    tio->cor = PRETO;
                    avo->cor = VERMELHO;
                    z = avo;
                } else {
                    if (z == pai->direita) {
                        z = pai;
                        rotacaoEsquerda(z);
                        pai = z->pai;
                        avo = pai->pai;
                    }

                    pai->cor = PRETO;
                    avo->cor = VERMELHO;
                    rotacaoDireita(avo);
                }
            } else {
                NoRN* tio = avo->esquerda;

                if (tio != nullptr && tio->cor == VERMELHO) {
                    pai->cor = PRETO;
                    tio->cor = PRETO;
                    avo->cor = VERMELHO;
                    z = avo;
                } else {
                    if (z == pai->esquerda) {
                        z = pai;
                        rotacaoDireita(z);
                        pai = z->pai;
                        avo = pai->pai;
                    }

                    pai->cor = PRETO;
                    avo->cor = VERMELHO;
                    rotacaoEsquerda(avo);
                }
            }
        }

        if (raiz != nullptr) {
            raiz->cor = PRETO;
        }
    }

    NoRN* minimo(NoRN* no) const {
        while (no != nullptr && no->esquerda != nullptr) {
            no = no->esquerda;
        }

        return no;
    }

    void transplantar(NoRN* antigo, NoRN* novo) {
        if (antigo->pai == nullptr) {
            raiz = novo;
        } else if (antigo == antigo->pai->esquerda) {
            antigo->pai->esquerda = novo;
        } else {
            antigo->pai->direita = novo;
        }

        if (novo != nullptr) {
            novo->pai = antigo->pai;
        }
    }

    Cor corDe(NoRN* no) const {
        return no == nullptr ? PRETO : no->cor;
    }
    
    void corrigirRemocao(NoRN* x, NoRN* paiX, bool xEraEsquerda) {
        while (x != raiz && corDe(x) == PRETO) {
            if (paiX == nullptr) {
                break;
            }

            bool estaEsquerda = x != nullptr ? x == paiX->esquerda : xEraEsquerda;

            if (estaEsquerda) {
                NoRN* irmao = paiX->direita;

                if (corDe(irmao) == VERMELHO) {
                    irmao->cor = PRETO;
                    paiX->cor = VERMELHO;
                    rotacaoEsquerda(paiX);
                    irmao = paiX->direita;
                }

                if (irmao == nullptr ||
                    (corDe(irmao->esquerda) == PRETO &&
                     corDe(irmao->direita) == PRETO)) {
                    if (irmao != nullptr) {
                        irmao->cor = VERMELHO;
                    }

                    x = paiX;
                    paiX = x->pai;

                    if (paiX != nullptr) {
                        xEraEsquerda = x == paiX->esquerda;
                    }
                } else {
                    if (corDe(irmao->direita) == PRETO) {
                        if (irmao->esquerda != nullptr) {
                            irmao->esquerda->cor = PRETO;
                        }

                        irmao->cor = VERMELHO;
                        rotacaoDireita(irmao);
                        irmao = paiX->direita;
                    }

                    if (irmao != nullptr) {
                        irmao->cor = paiX->cor;

                        if (irmao->direita != nullptr) {
                            irmao->direita->cor = PRETO;
                        }
                    }

                    paiX->cor = PRETO;
                    rotacaoEsquerda(paiX);
                    x = raiz;
                    paiX = nullptr;
                }
            } else {
                NoRN* irmao = paiX->esquerda;

                if (corDe(irmao) == VERMELHO) {
                    irmao->cor = PRETO;
                    paiX->cor = VERMELHO;
                    rotacaoDireita(paiX);
                    irmao = paiX->esquerda;
                }

                if (irmao == nullptr ||
                    (corDe(irmao->esquerda) == PRETO &&
                     corDe(irmao->direita) == PRETO)) {
                    if (irmao != nullptr) {
                        irmao->cor = VERMELHO;
                    }

                    x = paiX;
                    paiX = x->pai;

                    if (paiX != nullptr) {
                        xEraEsquerda = x == paiX->esquerda;
                    }
                } else {
                    if (corDe(irmao->esquerda) == PRETO) {
                        if (irmao->direita != nullptr) {
                            irmao->direita->cor = PRETO;
                        }

                        irmao->cor = VERMELHO;
                        rotacaoEsquerda(irmao);
                        irmao = paiX->esquerda;
                    }

                    if (irmao != nullptr) {
                        irmao->cor = paiX->cor;

                        if (irmao->esquerda != nullptr) {
                            irmao->esquerda->cor = PRETO;
                        }
                    }

                    paiX->cor = PRETO;
                    rotacaoDireita(paiX);
                    x = raiz;
                    paiX = nullptr;
                }
            }
        }

        if (x != nullptr) {
            x->cor = PRETO;
        }
    }

    void imprimirEmOrdemNo(NoRN* no) const {
        if (no == nullptr) {
            return;
        }

        imprimirEmOrdemNo(no->esquerda);
        cout << no->valor << ' ';
        imprimirEmOrdemNo(no->direita);
    }

    void imprimirEstruturaNo(NoRN* no, int nivel) const {
        if (no == nullptr) {
            return;
        }

        imprimirEstruturaNo(no->direita, nivel + 1);

        cout << string(static_cast<size_t>(nivel) * 4, ' ')
             << no->valor << " ("
             << (no->cor == VERMELHO ? "VERMELHO" : "PRETO")
             << ")\n";

        imprimirEstruturaNo(no->esquerda, nivel + 1);
    }

    int alturaNo(NoRN* no) const {
        if (no == nullptr) {
            return 0;
        }

        return 1 + max(alturaNo(no->esquerda), alturaNo(no->direita));
    }

    int verificarRegras(
        NoRN* no,
        long long minimoValor,
        long long maximoValor,
        bool& valido,
        int& quantidade
    ) const {
        if (no == nullptr) {
            return 1;
        }

        ++quantidade;

        if (no->valor <= minimoValor || no->valor >= maximoValor) {
            valido = false;
        }

        if (no->esquerda != nullptr && no->esquerda->pai != no) {
            valido = false;
        }

        if (no->direita != nullptr && no->direita->pai != no) {
            valido = false;
        }

        if (no->cor == VERMELHO &&
            (corDe(no->esquerda) == VERMELHO ||
             corDe(no->direita) == VERMELHO)) {
            valido = false;
        }

        int esq = verificarRegras(
            no->esquerda,
            minimoValor,
            no->valor,
            valido,
            quantidade
        );

        int dir = verificarRegras(
            no->direita,
            no->valor,
            maximoValor,
            valido,
            quantidade
        );

        if (esq != dir) {
            valido = false;
        }

        return esq + (no->cor == PRETO ? 1 : 0);
    }

    void liberarNo(NoRN* no) {
        if (no == nullptr) {
            return;
        }

        liberarNo(no->esquerda);
        liberarNo(no->direita);
        delete no;
    }

public:
    ArvoreRN() : raiz(nullptr), rotacoes(0) {}

    ~ArvoreRN() {
        liberarNo(raiz);
    }

    ArvoreRN(const ArvoreRN&) = delete;
    ArvoreRN& operator=(const ArvoreRN&) = delete;

    bool inserir(int valor) {
        NoRN* pai = nullptr;
        NoRN* atual = raiz;

        while (atual != nullptr) {
            pai = atual;

            if (valor < atual->valor) {
                atual = atual->esquerda;
            } else if (valor > atual->valor) {
                atual = atual->direita;
            } else {
                return false;
            }
        }

        NoRN* novo = new NoRN(valor);
        novo->pai = pai;

        if (pai == nullptr) {
            raiz = novo;
        } else if (valor < pai->valor) {
            pai->esquerda = novo;
        } else {
            pai->direita = novo;
        }

        corrigirInsercao(novo);
        return true;
    }

    bool buscar(int valor) const {
        NoRN* atual = raiz;

        while (atual != nullptr) {
            if (valor == atual->valor) {
                return true;
            }

            atual = valor < atual->valor ? atual->esquerda : atual->direita;
        }

        return false;
    }

    bool remover(int valor) {
        NoRN* z = raiz;

        while (z != nullptr && z->valor != valor) {
            z = valor < z->valor ? z->esquerda : z->direita;
        }

        if (z == nullptr) {
            return false;
        }

        NoRN* y = z;
        Cor corOriginal = y->cor;
        NoRN* x = nullptr;
        NoRN* paiX = nullptr;
        bool xEraEsquerda = false;

        if (z->esquerda == nullptr) {
            x = z->direita;
            paiX = z->pai;

            if (paiX != nullptr) {
                xEraEsquerda = z == paiX->esquerda;
            }

            transplantar(z, z->direita);

            if (x != nullptr) {
                paiX = x->pai;
            }
        } else if (z->direita == nullptr) {
            x = z->esquerda;
            paiX = z->pai;

            if (paiX != nullptr) {
                xEraEsquerda = z == paiX->esquerda;
            }

            transplantar(z, z->esquerda);

            if (x != nullptr) {
                paiX = x->pai;
            }
        } else {
            y = minimo(z->direita);
            corOriginal = y->cor;
            x = y->direita;

            if (y->pai == z) {
                paiX = y;
                xEraEsquerda = false;

                if (x != nullptr) {
                    x->pai = y;
                }
            } else {
                paiX = y->pai;
                xEraEsquerda = y == paiX->esquerda;

                transplantar(y, y->direita);
                y->direita = z->direita;
                y->direita->pai = y;
            }

            transplantar(z, y);
            y->esquerda = z->esquerda;
            y->esquerda->pai = y;
            y->cor = z->cor;
        }

        delete z;

        if (corOriginal == PRETO) {
            corrigirRemocao(x, paiX, xEraEsquerda);
        }

        if (raiz != nullptr) {
            raiz->cor = PRETO;
        }

        return true;
    }

    bool vazia() const {
        return raiz == nullptr;
    }

    int altura() const {
        return alturaNo(raiz);
    }

    unsigned long long obterRotacoes() const {
        return rotacoes;
    }

    bool validar() const {
        if (raiz == nullptr) {
            return true;
        }

        if (raiz->cor != PRETO || raiz->pai != nullptr) {
            return false;
        }

        bool valido = true;
        int quantidade = 0;

        verificarRegras(
            raiz,
            numeric_limits<long long>::min(),
            numeric_limits<long long>::max(),
            valido,
            quantidade
        );

        return valido;
    }

    void imprimirEmOrdem() const {
        imprimirEmOrdemNo(raiz);
        cout << '\n';
    }

    void imprimirEstrutura() const {
        if (raiz == nullptr) {
            cout << "Arvore vazia.\n";
        } else {
            imprimirEstruturaNo(raiz, 0);
        }
    }
};

template <typename Arvore>
void executarMenuArvore(Arvore& arvore, const string& nome) {
    int opcao;

    do {
        cout << "\n===== ARVORE " << nome << " =====\n";
        cout << "1. Inserir\n";
        cout << "2. Buscar\n";
        cout << "3. Remover\n";
        cout << "4. Percurso em ordem\n";
        cout << "5. Imprimir estrutura\n";
        cout << "6. Validar balanceamento\n";
        cout << "7. Mostrar altura e rotacoes\n";
        cout << "0. Voltar\n";
        cout << "Opcao: ";

        if (!(cin >> opcao)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida.\n";
            continue;
        }

        int valor;

        switch (opcao) {
            case 1:
                cout << "Valor para inserir: ";

                if (cin >> valor) {
                    cout << (arvore.inserir(valor)
                        ? "Valor inserido.\n"
                        : "Valor repetido; nenhuma alteracao realizada.\n");
                } else {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Valor invalido.\n";
                }
                break;

            case 2:
                cout << "Valor para buscar: ";

                if (cin >> valor) {
                    cout << (arvore.buscar(valor)
                        ? "Valor encontrado.\n"
                        : "Valor nao encontrado.\n");
                } else {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Valor invalido.\n";
                }
                break;

            case 3:
                cout << "Valor para remover: ";

                if (cin >> valor) {
                    cout << (arvore.remover(valor)
                        ? "Valor removido.\n"
                        : "Valor nao encontrado; nenhuma alteracao realizada.\n");
                } else {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Valor invalido.\n";
                }
                break;

            case 4:
                cout << "Valores em ordem crescente: ";
                arvore.imprimirEmOrdem();
                break;

            case 5:
                arvore.imprimirEstrutura();
                break;

            case 6:
                cout << (arvore.validar()
                    ? "Estrutura valida.\n"
                    : "ERRO: propriedades da arvore violadas.\n");
                break;

            case 7:
                cout << "Altura: " << arvore.altura()
                     << "\nRotacoes acumuladas: "
                     << arvore.obterRotacoes() << '\n';
                break;

            case 0:
                break;

            default:
                cout << "Opcao invalida.\n";
        }
    } while (opcao != 0);
}

struct ResultadoTeste {
    string nome;
    bool sucesso;
    long long tempoInsercao;
    long long tempoBusca;
    long long tempoRemocao;
    int altura;
    unsigned long long rotacoes;
};

template <typename Arvore>
ResultadoTeste executarCenario(
    const string& nome,
    const vector<int>& insercoes,
    const vector<int>& buscas,
    const vector<int>& remocoes,
    const vector<int>& esperadosPresentes,
    const vector<int>& esperadosAusentes
) {
    Arvore arvore;
    bool sucesso = arvore.validar();
    long long tempoInsercao = 0;

    for (int valor : insercoes) {
        auto inicioOperacao = steady_clock::now();
        bool resultado = arvore.inserir(valor);
        auto fimOperacao = steady_clock::now();

        tempoInsercao += duration_cast<nanoseconds>(
            fimOperacao - inicioOperacao
        ).count();

        if (!resultado) {
            sucesso = false;
        }

        if (!arvore.validar()) {
            sucesso = false;
        }
    }

    for (int valor : insercoes) {
        if (arvore.inserir(valor)) {
            sucesso = false;
        }

        if (!arvore.validar()) {
            sucesso = false;
        }
    }

    auto inicioBusca = steady_clock::now();

    for (int valor : buscas) {
        bool esperado = find(
            insercoes.begin(),
            insercoes.end(),
            valor
        ) != insercoes.end();

        if (arvore.buscar(valor) != esperado) {
            sucesso = false;
        }
    }

    auto fimBusca = steady_clock::now();

    long long tempoBusca = duration_cast<nanoseconds>(
        fimBusca - inicioBusca
    ).count();

    for (int valor : esperadosPresentes) {
        if (!arvore.buscar(valor)) {
            sucesso = false;
        }
    }

    for (int valor : esperadosAusentes) {
        if (arvore.buscar(valor)) {
            sucesso = false;
        }
    }

    long long tempoRemocao = 0;

    for (int valor : remocoes) {
        bool esperado = find(
            insercoes.begin(),
            insercoes.end(),
            valor
        ) != insercoes.end();

        auto inicioOperacao = steady_clock::now();
        bool resultado = arvore.remover(valor);
        auto fimOperacao = steady_clock::now();

        tempoRemocao += duration_cast<nanoseconds>(
            fimOperacao - inicioOperacao
        ).count();

        if (resultado != esperado) {
            sucesso = false;
        }

        if (!arvore.validar()) {
            sucesso = false;
        }
    }

    for (int valor : remocoes) {
        if (arvore.buscar(valor)) {
            sucesso = false;
        }
    }

    return {
        nome,
        sucesso,
        tempoInsercao,
        tempoBusca,
        tempoRemocao,
        arvore.altura(),
        arvore.obterRotacoes()
    };
}

void executarTestes() {
    vector<vector<int>> cenarios = {
        {},
        {42},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        {10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
        {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 90}
    };

    vector<string> nomes = {
        "vazia",
        "um elemento",
        "crescente",
        "decrescente",
        "nao ordenada"
    };

    cout << "\n========== TESTES AUTOMATICOS ==========\n";
    cout << left << setw(18) << "Cenario"
         << setw(12) << "AVL"
         << setw(12) << "Rubro-Negra" << '\n';

    bool todosPassaram = true;

    for (size_t i = 0; i < cenarios.size(); ++i) {
        vector<int> dados = cenarios[i];
        vector<int> buscas = dados;
        buscas.push_back(999999);

        vector<int> remocoes;

        if (!dados.empty()) {
            remocoes.push_back(dados.front());

            if (dados.size() > 2) {
                remocoes.push_back(dados[dados.size() / 2]);
            }

            if (dados.size() > 1) {
                remocoes.push_back(dados.back());
            }
        }

        remocoes.push_back(999999);

        vector<int> presentes;
        vector<int> ausentes = {999999};

        ResultadoTeste a = executarCenario<ArvoreAVL>(
            nomes[i],
            dados,
            buscas,
            remocoes,
            presentes,
            ausentes
        );

        ResultadoTeste r = executarCenario<ArvoreRN>(
            nomes[i],
            dados,
            buscas,
            remocoes,
            presentes,
            ausentes
        );

        cout << left << setw(18) << nomes[i]
             << setw(12) << (a.sucesso ? "PASSOU" : "FALHOU")
             << setw(12) << (r.sucesso ? "PASSOU" : "FALHOU")
             << '\n';

        todosPassaram = todosPassaram && a.sucesso && r.sucesso;
    }

    vector<int> aleatorios;

    for (int i = 1; i <= 1000; ++i) {
        aleatorios.push_back(i);
    }

    mt19937 gerador(20261027);
    shuffle(aleatorios.begin(), aleatorios.end(), gerador);

    vector<int> buscas = {1, 250, 500, 750, 1000, -1, 1001};
    vector<int> remocoes;

    for (int i = 1; i <= 1000; i += 2) {
        remocoes.push_back(i);
    }

    remocoes.push_back(2000);

    ResultadoTeste a = executarCenario<ArvoreAVL>(
        "aleatoria 1000",
        aleatorios,
        buscas,
        remocoes,
        {},
        {2000, -1, 1001}
    );

    ResultadoTeste r = executarCenario<ArvoreRN>(
        "aleatoria 1000",
        aleatorios,
        buscas,
        remocoes,
        {},
        {2000, -1, 1001}
    );

    cout << left << setw(18) << "aleatoria 1000"
         << setw(12) << (a.sucesso ? "PASSOU" : "FALHOU")
         << setw(12) << (r.sucesso ? "PASSOU" : "FALHOU")
         << '\n';

    todosPassaram = todosPassaram && a.sucesso && r.sucesso;

    cout << "\nResultado geral: "
         << (todosPassaram
             ? "TODOS OS TESTES PASSARAM"
             : "HA TESTES COM FALHA")
         << '\n';

    cout << "\nComparacao do cenario aleatorio com 1000 valores\n";
    cout << left << setw(22) << "Metrica"
         << setw(18) << "AVL"
         << setw(18) << "Rubro-Negra" << '\n';

    cout << left << setw(22) << "Insercao (ns)"
         << setw(18) << a.tempoInsercao
         << setw(18) << r.tempoInsercao << '\n';

    cout << left << setw(22) << "Busca (ns)"
         << setw(18) << a.tempoBusca
         << setw(18) << r.tempoBusca << '\n';

    cout << left << setw(22) << "Remocao (ns)"
         << setw(18) << a.tempoRemocao
         << setw(18) << r.tempoRemocao << '\n';

    cout << left << setw(22) << "Altura final"
         << setw(18) << a.altura
         << setw(18) << r.altura << '\n';

    cout << left << setw(22) << "Rotacoes acumuladas"
         << setw(18) << a.rotacoes
         << setw(18) << r.rotacoes << '\n';

    cout << "Os tempos variam entre execucoes e devem ser interpretados "
         << "como evidencias experimentais, nao como prova isolada "
         << "de superioridade.\n";
}

int main() {
    int opcao;

    do {
        cout << "\n========== ARVORES BALANCEADAS ==========\n";
        cout << "1. Utilizar arvore AVL\n";
        cout << "2. Utilizar arvore Rubro-Negra\n";
        cout << "3. Executar testes e comparacao\n";
        cout << "0. Encerrar\n";
        cout << "Opcao: ";

        if (!(cin >> opcao)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida.\n";
            continue;
        }

        if (opcao == 1) {
            ArvoreAVL arvore;
            executarMenuArvore(arvore, "AVL");
        } else if (opcao == 2) {
            ArvoreRN arvore;
            executarMenuArvore(arvore, "RUBRO-NEGRA");
        } else if (opcao == 3) {
            executarTestes();
        } else if (opcao != 0) {
            cout << "Opcao invalida.\n";
        }

    } while (opcao != 0);

    cout << "Programa encerrado.\n";
    return 0;
}