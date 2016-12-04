#include "dialogocontrolelegendas.h"
#include "ui_dialogocontrolelegendas.h"
#include "dialogoeditarlegenda.h"
#include "dialogoadicionarlegenda.h"
#include <QMessageBox>

DialogoControleLegendas::DialogoControleLegendas(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DialogoControleLegendas)
{
    ui->setupUi(this);
    dados = new DadosLegenda[100](); // cria vetor de tamanho 100 para guardar dados

}

DialogoControleLegendas::~DialogoControleLegendas()
{
    delete ui;
    delete [] dados; // liberar memoria do vetor de objetos
}

void DialogoControleLegendas::on_botaoAddLegenda_clicked()
{

    if(posicao >= 99){ // caso esteja no numero maximo de legendas suportadas
        QMessageBox msgBox; // cria caixa de mensagem
        msgBox.setWindowTitle("Erro"); // seta titulo da caixa de menssagem
        msgBox.setText("Número máximo de legendas atingido."); // seta texto da caixa de mensagem
        msgBox.exec(); // exibe caixa de mensagem
    }
    else{ // caso ainda possa adicionar novas legendas
        DialogoAdicionarLegenda win; // cria dialogo para adição de legendas
        int re = win.exec(); // exibe dialogo
        // somente se o botao OK for pressionado e um item foi selecionado, faz mudanças
        if( !win.getNewItem().getCaminho().isEmpty() && re ){
            dados[posicao] = win.getNewItem(); // adiciona novo item ao array de legendas
            ui->listaDeLegendas->addItem(dados[posicao].getNome()); // adiciona o novo item a lista de legendas
            posicao++; // inclementa a posição no array
        }
    }
}

void DialogoControleLegendas::on_botaoOK_clicked()
{
    close();
}

void DialogoControleLegendas::on_botaoEditaLegenda_clicked()
{
    QListWidget *lista = ui->listaDeLegendas; // cria ponteiro para facilitar leitura do codigo
    // caso não tenha nenhum item selecionado não faz nada
    if(lista->currentRow() >= 0){

        DialogoEditarLegenda win; // cria dialogo para edição de legendas

        win.editItem(dados[lista->currentRow()]);

        int re = win.exec(); // exibe dialogo
        // somente se o botao OK for pressionado e um item foi selecionado, faz mudanças
        if( !win.getEditedItem().getCaminho().isEmpty() && re ){
            dados[lista->currentRow()] = win.getEditedItem(); // adiciona novo item ao array de legendas
            lista->item(lista->currentRow())->setText(dados[lista->currentRow()].getNome()); // atualiza texto da lista de legendas
        }
    }
}

void DialogoControleLegendas::on_botaoRemoveLegenda_clicked()
{
    QListWidget *lista = ui->listaDeLegendas; // cria ponteiro para facilitar leitura do codigo

    // caso não tenha nenhum item selecionado não faz nada
    if(lista->currentRow() >= 0){

        // percorre o vetor de itens a partir do selecionado
        for(int i = lista->currentRow(); i < posicao - 1; i++)
            dados[i] = dados[i+1];

        posicao--; // reduz em 1 o numero de posições ocupadas do vetor

        delete lista->takeItem(ui->listaDeLegendas->currentRow()); // remove item da visualização
    }
}

void DialogoControleLegendas::on_botaoSubirLegenda_clicked()
{
    QListWidget *lista = ui->listaDeLegendas; // cria ponteiro para facilitar leitura do codigo

    // caso nenhum ou o primeiro item esteja marcado, não faz nada
    if(lista->currentRow() >= 1){
        DadosLegenda temp = dados[lista->currentRow()]; // guarda dados temporariamente
        int linhaAtual = lista->currentRow(); // salva atual linha selecionada na lista
        dados[linhaAtual] = dados[linhaAtual - 1 ]; // substitui o conteudo da atual linha selecionada com o da linha acima
        dados[linhaAtual - 1 ] = temp; // substitui o conteudo da linha acima com o antigo conteudo da linha abaixo

        QString nome = lista->item(linhaAtual)->text(); // copia texto da atual linha selecionada
        lista->item(linhaAtual)->setText( lista->item(linhaAtual - 1)->text() ); // substitui texto da linha atual pelo da linha acima
        lista->item(linhaAtual - 1)->setText(nome); // substitui texto da linha acima pelo salvo anteriormente
        lista->setCurrentRow(linhaAtual - 1); // seta atual linha para a linha acima
        lista->item(linhaAtual - 1)->setSelected(true); // marca linha acima como selecionada
        lista->setFocus(); // foca lista
    }
}

void DialogoControleLegendas::on_botaoDescerLegenda_clicked()
{
    QListWidget *lista = ui->listaDeLegendas; // cria ponteiro para facilitar leitura do codigo

    /*
     * Para ocorrer a condição abaixo precisa estar dentro de 3 situações ao mesmo tempo:
     * 1 - Ter ao menos um item selecionado (checar situação 3)
     * 2 - O item selecionado não pode ser o último da lista
     * 3 - O item selecionado não pode ser o o único da lista
     */
    if(lista->currentRow() >= 0 && lista->currentRow() < lista->count() - 1 ){
        DadosLegenda temp = dados[lista->currentRow()]; // guarda dados temporariamente
        int linhaAtual = lista->currentRow(); // salva atual linha selecionada na lista
        dados[linhaAtual] = dados[linhaAtual + 1 ]; // substitui o conteudo da atual linha selecionada com o da linha abaixo
        dados[linhaAtual + 1 ] = temp; // substitui o conteudo da linha abaixo com o antigo conteudo da linha acima

        QString nome = lista->item(linhaAtual)->text(); // copia texto da atual linha selecionada
        lista->item(linhaAtual)->setText( lista->item(linhaAtual + 1)->text() ); // substitui texto da linha atual pelo da linha abaixo
        lista->item(linhaAtual + 1)->setText(nome); // substitui texto da linha abaixo pelo salvo anteriormente
        lista->setCurrentRow(linhaAtual + 1); // seta atual linha para a linha abaixo
        lista->item(linhaAtual + 1)->setSelected(true); // marca linha abaixo como selecionada
        lista->setFocus(); // foca lista
    }
}
