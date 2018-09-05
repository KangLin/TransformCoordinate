#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "TransformCoordinate.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QCursor>
#include <QLocale>
#include "Widgets/DlgAbout/DlgAbout.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   
    m_Translator.load(":/translations/app_" + QLocale::system().name() + ".qm");
    qApp->installTranslator(&m_Translator);
    
    ui->setupUi(this);    
    
    QStringList lstCoor;
    lstCoor << "WGS84" << "GCJ02" << "BD09LL" << "BD09MC";
    ui->cbSrcCoor->addItems(lstCoor);
    ui->cbSrcCoor->setCurrentIndex(WGS84);
    ui->cbDstCoor->addItems(lstCoor);
    ui->cbDstCoor->setCurrentIndex(GCJ02);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbConversion_clicked()
{
    double x0, y0, x1, y1;
    x0 = ui->leSrcLong->text().toDouble();
    y0 = ui->leSrcLat->text().toDouble();
    
    int nRet = TransformCoordinate(x0, y0,
                          x1, y1, 
                          (_COORDINATE)ui->cbSrcCoor->currentIndex(),
                          (_COORDINATE)ui->cbDstCoor->currentIndex());
    if(nRet)
        return;
    
    ui->leDstLong->setText(QString::number(x1));
    ui->leDstLat->setText(QString::number(y1));
}

void MainWindow::on_pbBrowsSrcFile_clicked()
{
    QString szExt = tr("GPX file(*.gpx);;NMea file(*.nmea);;ACT file(*.act);;txt(*.txt);;All files(*.*)");
#ifdef BUILD_LIBKML
    szExt = tr("KML file(*.kml);;") + szExt;
#endif
    QString szFile = QFileDialog::getOpenFileName(this, tr("Open source file"), 
                                                  QString(),
                                                  szExt);
    ui->leSrcFile->setText(szFile);
}

void MainWindow::on_pbBrowsDstFile_clicked()
{
    QString szExt = tr("GPX file(*.gpx);;");
#ifdef BUILD_LIBKML
    szExt = tr("KML file(*.kml);;") + szExt;
#endif
    
    QString szFile = QFileDialog::getSaveFileName(this, tr("Open Destination file"),
                                                  QString(),
                                                  szExt);
    ui->leDstFile->setText(szFile);
}

void MainWindow::on_pbConversionFile_clicked()
{
#ifdef BUILD_GPXMODEL
    this->statusBar()->showMessage(tr("Start transform ......"));
    QCursor cursor = this->cursor();
    this->setCursor(Qt::WaitCursor);
    TransformCoordinateFiles(ui->leSrcFile->text().toStdString().c_str(),
                    ui->leDstFile->text().toStdString().c_str(),
                    (_COORDINATE)ui->cbSrcCoor->currentIndex(),
                    (_COORDINATE)ui->cbDstCoor->currentIndex());
    //QMessageBox::information(this, tr("End"), tr("Transform coordinate end"));
    this->setCursor(cursor);
    this->statusBar()->showMessage(tr("Ready"));
#else
    qDebug() << "Please set BUILD_GPXMODEL to ON";
#endif
}

void MainWindow::on_leSrcFile_textChanged(const QString &text)
{
    if(!ui->leDstFile->text().isEmpty())
        return;
    QString szFile;
    szFile = ui->leSrcFile->text();
    QFileInfo f(text);
    szFile = f.path() + QDir::separator() + f.completeBaseName() + "_TC." + f.suffix();
    ui->leDstFile->setText(szFile);
}

void MainWindow::on_pbSrcDir_clicked()
{  
    QFileDialog df(this, tr("Open source directory"));
    df.setFileMode(QFileDialog::DirectoryOnly);
    df.setOptions(QFileDialog::ShowDirsOnly);
    if(df.exec() == QDialog::Rejected)
        return;
    ui->leSrcDir->setText(df.directory().absolutePath());
}

void MainWindow::on_pbDstDir_clicked()
{
    QFileDialog df(this, tr("Open destination directory"));
    df.setFileMode(QFileDialog::DirectoryOnly);
    df.setOptions(QFileDialog::ShowDirsOnly);
    if(df.exec() == QDialog::Rejected)
        return;
    ui->leDstDir->setText(df.directory().absolutePath());
}

void MainWindow::on_leSrcDir_textChanged(const QString &text)
{
    if(!ui->leDstDir->text().isEmpty())
        return;
    
    ui->leDstDir->setText(text + "_TC");
}

void MainWindow::on_pbConversionDir_clicked()
{
    QString szDir = ui->leDstDir->text();
    QDir dir;
    if(!dir.exists(szDir))
        dir.mkpath(szDir);
    this->statusBar()->showMessage(tr("Start transform ......"));
    QCursor cursor = this->cursor();
    this->setCursor(Qt::WaitCursor);
    QDir d(ui->leSrcDir->text());
    int num = 1;
    foreach (QFileInfo f, d.entryInfoList()) {
        if(f.isFile())
        {
#ifdef BUILD_GPXMODEL
            this->statusBar()->showMessage(tr("Be transforming ") + QString::number(num++) + " ......");
            TransformCoordinateFiles(f.filePath().toStdString().c_str(),
                    (ui->leDstDir->text() + QDir::separator() + f.fileName()).toStdString().c_str(),
                    (_COORDINATE)ui->cbSrcCoor->currentIndex(),
                    (_COORDINATE)ui->cbDstCoor->currentIndex());
#else
            qDebug() << "Please set BUILD_GPXMODEL to ON";
#endif
        }
    }
    this->setCursor(cursor);
    this->statusBar()->showMessage(tr("Ready"));
    //QMessageBox::information(this, tr("End"), tr("Transform coordinate end"));
}

void MainWindow::on_actionAbout_A_triggered()
{
    CDlgAbout dlgAbout;
    dlgAbout.exec();
}
