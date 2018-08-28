#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pbConversion_clicked();
    void on_pbBrowsSrcFile_clicked();
    void on_pbBrowsDstFile_clicked();
    void on_pbConversionFile_clicked();   
    void on_leSrcFile_textChanged(const QString &text);
    
    void on_pbSrcDir_clicked();
    
    void on_pbDstDir_clicked();
    
    void on_pbConversionDir_clicked();
    
    void on_leSrcDir_textChanged(const QString &text);
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
