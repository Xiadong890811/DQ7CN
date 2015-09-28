#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FPTFile.hpp"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    fptFile = nullptr;
    ui->setupUi(this);
    this->folderModel = new QStringListModel();
    this->messageModel = new QStringListModel();
    this->messageLineModel = new QStringListModel();

    connect(ui->openFPTFolder, SIGNAL(clicked()), this, SLOT(openFPTFolderTapped()));
    connect(ui->FPTList, SIGNAL(doubleClicked(QModelIndex)), this , SLOT(FPTListSelected(QModelIndex)));
    connect(ui->FPTMessageList, SIGNAL(clicked(QModelIndex)), this , SLOT(FPTMessageListSelected(QModelIndex)));
    connect(ui->ValidTranslate, SIGNAL(clicked()), this, SLOT(ValidTranslate()));
    connect(ui->SaveTranslate, SIGNAL(clicked()), this, SLOT(SaveTranslate()));
    connect(ui->SaveFPT, SIGNAL(clicked()), this, SLOT(SaveFPT()));

    ui->FPTList->setModel(this->folderModel);
    ui->FPTList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->FPTMessageList->setModel(this->messageModel);
    ui->FPTMessageList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->FPTMessageLineList->setModel(this->messageLineModel);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete folderModel;
    delete messageModel;
    delete messageLineModel;
}

void MainWindow::openFPTFolderTapped()
{
    QString selectedPath = QFileDialog::getExistingDirectory();

    QDir FPTScan(selectedPath);
    QStringList files = FPTScan.entryList();
    folderModel->removeRows(0,folderModel->rowCount());
    foreach(QString file , files){
        if(file.endsWith(".fpt")){
            int row = folderModel->rowCount();
            folderModel->insertRows(row, 1);
            QModelIndex index = folderModel->index(row);
            folderModel->setData(index, file);
        }
    }
    fptFolderPath = selectedPath;
}

void MainWindow::FPTListSelected(QModelIndex index){
    QString fpt = index.data().toString();
    QString fptPath = fptFolderPath + "/" + fpt;

    if(fptFile) delete fptFile;
    fptFile = new FPTFile;
    fptFile->ReadFile((const char *)fptPath.toUtf8().data());

    messageModel->removeRows(0,messageModel->rowCount());

    for(std::shared_ptr<TXTEntity> mess : fptFile->entities){
        int row = messageModel->rowCount();
        messageModel->insertRows(row, 1);
        QModelIndex index = messageModel->index(row);

        QString messageName(mess.get()->name);
        messageModel->setData(index, messageName);
    }
}

void MainWindow::FPTMessageListSelected(QModelIndex index){
    std::shared_ptr<TXTEntity> mess = fptFile->entities[index.row()];
    std::shared_ptr<unsigned char> str = fptFile->ReadString(mess.get());
    QString qStr((const char *)str.get());

    ui->messageContent->setPlainText(qStr);

    QStringList lines = qStr.split(QString("\r\n"));
    lines.removeLast();

    ui->messageInfo->setText(QString().sprintf("%s 行数：%d  长度:%d",((QString)lines.at(0)).toUtf8().data(),lines.count(),mess.get()->length));

    messageLineModel->removeRows(0,messageLineModel->rowCount());
    messageLineModel->insertRows(0 ,lines.count());

    for(int lineIdx = 0; lineIdx < lines.count(); lineIdx ++){
        QString lineContent = lines.at(lineIdx);
        QModelIndex index = messageModel->index(lineIdx);
        messageLineModel->setData(index,lineContent);
    }
}

void MainWindow::ValidTranslate(){
    if(_validTranslate()){
        ui->ErrorMsg->setText("OK");
    }
    else{
        ui->ErrorMsg->setText("翻译的文本太长了.");
    }
}

void MainWindow::SaveTranslate(){
    if(!_validTranslate()){
        ui->ErrorMsg->setText("翻译的文本太长了.");
    }

    QModelIndexList selectedIdxs = ui->FPTMessageList->selectionModel()->selectedIndexes();
    if(selectedIdxs.count() == 0) return;

    QStringList list;
    for(int idx = 0; idx < messageLineModel->rowCount(); idx ++){
        QModelIndex modelIndex = messageLineModel->index(idx);
        list.append(modelIndex.data().toString());
    }
    QString trans = list.join("\r\n") + "\r\n";
    QByteArray transByte = trans.toUtf8();



    QModelIndex selected = selectedIdxs.at(0);
    std::shared_ptr<TXTEntity> mess = fptFile->entities[selected.row()];

    unsigned int realStart =  fptFile->dataStart + 0x40 + mess.get()->location;
    memset(fptFile->fileBuf.get() + realStart, 0 , mess.get()->length);
    memcpy(fptFile->fileBuf.get() + realStart, transByte.data(), transByte.count() );
    ui->ErrorMsg->setText("保存完成了.");
}

bool MainWindow::_validTranslate(){
    QStringList list;

    for(int idx = 0; idx < messageLineModel->rowCount(); idx ++){
        QModelIndex modelIndex = messageLineModel->index(idx);
        list.append(modelIndex.data().toString());
    }
    QString trans = list.join("\r\n") + "\r\n";

    int row = ListViewSelectedRow(ui->FPTMessageList);
    if(row == -1) return false;

    std::shared_ptr<TXTEntity> mess = fptFile->entities[row];

    return trans.toUtf8().count() <= (int)mess.get()->length;

}

int MainWindow::ListViewSelectedRow(QListView *view){
    QModelIndexList selectedIdxs = ui->FPTMessageList->selectionModel()->selectedIndexes();
    if(selectedIdxs.count() == 0) return -1;
    QModelIndex selected = selectedIdxs.at(0);
    return selected.row();
}

void MainWindow::SaveFPT(){
    int ftpFileRow = ListViewSelectedRow(ui->FPTList);
    QString ftpFileName = this->folderModel->index(ftpFileRow).data().toString();

    QString fptPath = fptFolderPath + "/trans" + ftpFileName;
    FILE *fp = fopen(fptPath.toUtf8().data(),"w+");
    fwrite(this->fptFile->fileBuf.get(), 1, this->fptFile->fileLen, fp);
    fclose(fp);
    ui->ErrorMsg->setText("保存." + fptPath + " 完了.");
}
