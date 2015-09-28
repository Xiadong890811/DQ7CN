#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QListView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public slots:

    void openFPTFolderTapped();
    void FPTListSelected(QModelIndex index);
    void FPTMessageListSelected(QModelIndex index);

    void ValidTranslate();
    void SaveTranslate();
    void SaveFPT();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStringListModel *folderModel;
    QStringListModel *messageModel;
    QStringListModel *messageLineModel;
    QString fptFolderPath;
    class FPTFile *fptFile;

private:
    bool _validTranslate();
    int ListViewSelectedRow(QListView *view);
};

#endif // MAINWINDOW_H
