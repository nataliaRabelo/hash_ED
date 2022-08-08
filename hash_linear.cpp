/**
 * @file hash_linear.cpp
 * @author Natália Bruno
 * @brief Implementa uma hash simples com colisão linear para estudo da disciplina de ED.
 * @version 0.1
 * @date 2021-04-05
 * Compile este código com o seguinte comando no terminal:
 * g++ hash_linear.cpp -std=c++17 -o hash_linear
 */
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <array>

using namespace std;

///Tamanho máximo da hash. Aumentar caso os testes forem maiores.
constexpr unsigned int TAMANHO_MAX_HASH = 1'000'000; 
///Tamanho de uma chave da hash. Aumentar caso queira testar com strings maiores
constexpr unsigned int TAMANHO_CHAVE = 5;
///Quantidade de testes que deve ser feito com a hash
constexpr unsigned int TESTES = 1'000;
///Semente para o mecanismo de números pseudoaleatorios. Trocar este número para poder gerar outras strings
constexpr int SEED = 0;
///Mecanismo para geração de chaves aleatórias
mt19937 mecanismo(SEED);
///Range para as letrar maiúsculas: vai da letra 'A' até 'Z'.
uniform_int_distribution<int> letra('A', 'Z'); 

///Código para imprimir uma chave
ostream& operator<<(ostream& os, array<char, TAMANHO_CHAVE> chave){
    for(char x : chave) os << x;
    return os;
}

///Código para gerar uma chave aleatoria
array<char, TAMANHO_CHAVE> gera_chave(){
    //cerr << "Gerando uma chave aleatoriamente." << endl;
    array<char, TAMANHO_CHAVE> chave{'A', 'A', 'A', 'A', 'A'};
    for(int i = 0; i < TAMANHO_CHAVE; ++i)
        chave[i] = static_cast<char>(letra(mecanismo));
    //cerr << "Chave gerada: " << chave << endl;
    return chave;
}

/**
 * @brief Implementa uma hash simples com colisão linear através de um vector.
 * As chaves devem ser compostas por strings com letras maiúsculas do tamanho configurado acima. Não utilize strings maiores ou menores.
 * A string é alocada de acordo com a soma dos valores de cada letra. Exemplos para tamanho 5:
 * "AAAAA" deve ser alocado na posição 0 (0 + 0 + 0 + 0 + 0). 
 * "AAAAB" deve ser alocado na posição 1 (0 + 0 + 0 + 0 + 1). Assim como "BAAAA", "ABAAA", "AABAA" e "AAABA".
 * "AFAAB" deve ser alocado na posição 6 (0 + 5 + 0 + 0 + 1). 
 * As colisões são resolvidas com endereçamento aberto de esquema linear.
 * @warning Atenção com os parametros dados no começo do código! Usar somente chaves com letras maiúsculas e do tamanho correto!
 */
class Hash_Linear {
    vector< array<char, TAMANHO_CHAVE> > hash; ///Armazenamento da Hash
    vector<bool> usado; ///Sinaliza se a posição já possui uma chave ou não

public:
    /**
     * @brief Recebe uma chave como entrada e retorna em qual posição ela deve ficar dentro da hash.
     * A posição é calculada de forma muito simples: 
     * @return int Qual a posição em que esta chave deve ficar.
     */
    int Decodificar(const array<char, TAMANHO_CHAVE>& chave){
        // cerr << "Decodificando " << chave << endl;
        int soma = 0;
        for(char x : chave)
            soma += static_cast<int>(x) - 'A';
        // cerr << "Chave decodificada: " << soma << endl;
        return soma;
    };

    /**
     * @brief Constrói uma nova hash com o tamanho máximo dado.
     * @param tamanho Tamanho que deve ser alocado para a hash. Se não for passado, então o tamanho máximo configurado é utilizado.
     */
    explicit Hash_Linear(int tamanho = TAMANHO_MAX_HASH){
        //cerr << "Construindo uma nova hash de tamanho " << tamanho << endl;
        hash.resize(tamanho, array<char, TAMANHO_CHAVE>{'A', 'A', 'A', 'A', 'A'}); //aloca a quantidade de memória certa para a hash
        usado.resize(tamanho, false); //aloca a quantidade de memória certa para o controle da hash
        //cerr << "Hash construída" << endl;
    };

    /**
     * @brief Insere uma chave na hash.
     * Primeiro faz a decodificação.
     * Depois itera na hash começando na posição correta até achar uma vazia.
     * Se achar uma vazia, faz a inserção. Caso contrário termina pq a hash está cheia.
     * @param chave Chave que vai ser inserida.
     */
    void Insert(const array<char, TAMANHO_CHAVE>& chave){
        //cerr << "Inserindo a chave " << chave << " na hash" << endl;
        int posicao = Decodificar(chave);
        while(usado[posicao] && posicao < hash.size()){
           //cerr << "Posição " << posicao << " ocupada! Tentando a próxima." << endl;
            ++posicao;
        }
        if(posicao == hash.size()){
            //cerr << "Nao da para inserir a chave! A hash esta cheia!" << endl;        
            return;
        }
        
        //cerr << "Inserindo na pos: " << posicao << endl;
        hash[posicao] = chave;
        usado[posicao] = true;    
    };

    /**
     * @brief Procura uma chave na hash, retornando verdadeiro caso ache.
     * Primeiro começa decodificando a chave para ir para a posição correta.
     * Depois itera na hash até achar a chave ou uma posição vazia.
     * Se achar a chave, retorna sua posição. Caso contrário retorna '-1'.
     * Se chegar no final da hash, então ela está cheia e não encontramos a chave, retornando '-1'.
     * @param chave Chave que está sendo buscada.
     * @return Posição em que a chave está ou '-1' caso não seja encontrada.
     */
    int Search(const array<char, TAMANHO_CHAVE>& chave){
        //cerr << "Procurando a chave " << chave << endl;
        int posicao = Decodificar(chave);
        while(posicao < hash.size()){
            if(usado[posicao] == false){
                //cerr << "Nao encontrei a chave!" << endl;
                return -1;
            }
            if(hash[posicao] == chave) //encontrei a chave!
                break;
            ++posicao;
        }
        if(posicao == hash.size()){
            //cerr << "Cheguei no final da hash e nao encontrei a chave!" << endl;        
            return -1;
        }
        
        //cerr << "Encontrei a chave na pos: " << posicao << endl;
        return posicao;
    };

    /**
     * @brief Deleta uma chave da hash caso exista.
     * Primeiro procuramos a chave. Se ela não existir, então não fazemos nada.
     * Caso contrário, estaremos com a posição correta 
     */
    void Delete(const array<char, TAMANHO_CHAVE>& chave){
        //cerr << "Tentando deletar a chave " << chave << endl;
        int posicao = Search(chave);
        if(posicao == -1){
            //cerr << "Não encontrei a chave na hash" << endl;
            return;
        }

        //cerr << "Encontrei a chave na posicao: " << posicao << endl;
        usado[posicao] = false;
    };
};

int main(){
    Hash_Linear hash; //Constrói a hash com tamanho máximo

    cout << "Contando o tempo de insercao de " << TESTES << " elementos aleatorios." << endl;
    chrono::time_point<chrono::steady_clock> comeco = chrono::steady_clock::now();
    for(int i = 0; i < TESTES; ++i){
        array<char, TAMANHO_CHAVE> nova_chave = gera_chave();
        hash.Insert(nova_chave);
    }
    chrono::time_point<chrono::steady_clock> fim = chrono::steady_clock::now();
    int tempo_total_ms = chrono::duration_cast<chrono::milliseconds>(fim - comeco).count();
    cout << "Tempo total de insercao: " << tempo_total_ms << " (ms)" << endl;

    cout << "Contando o tempo de busca de " << TESTES << " elementos aleatorios." << endl;
    comeco = chrono::steady_clock::now();
    for(int i = 0; i < TESTES; ++i){
        array<char, TAMANHO_CHAVE> nova_chave = gera_chave();
        hash.Search(nova_chave);
    }
    fim = chrono::steady_clock::now();
    tempo_total_ms = chrono::duration_cast<chrono::milliseconds>(fim - comeco).count();
    cout << "Tempo total de busca: " << tempo_total_ms << " (ms)" << endl;

    cout << "Contando o tempo de delecao de " << TESTES << " elementos aleatorios." << endl;
    comeco = chrono::steady_clock::now();
    for(int i = 0; i < TESTES; ++i){
        array<char, TAMANHO_CHAVE> nova_chave = gera_chave();
        hash.Delete(nova_chave);
    }
    fim = chrono::steady_clock::now();
    tempo_total_ms = chrono::duration_cast<chrono::milliseconds>(fim - comeco).count();
    cout << "Tempo total de delecao: " << tempo_total_ms << " (ms)" << endl;

    return 0;
}